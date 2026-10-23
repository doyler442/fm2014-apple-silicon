#!/bin/bash
# SPDX-License-Identifier: LGPL-2.1-or-later
# FM2014 compatibility build helper, 24 October 2026.
set -euo pipefail

project_dir=$(cd -- "$(dirname -- "$0")" && pwd -P)
upstream_dir="$project_dir/build/AArchX"
revision=9838c317d2c346b4694ca94e7f772106be8a56d0
patch_file="$project_dir/patches/aarchx-fm2014.patch"

if [ "$(uname -s)" != Darwin ] || [ "$(uname -m)" != arm64 ]; then
    echo 'Build this project on an Apple silicon Mac.' >&2
    exit 1
fi
xcrun --find clang >/dev/null
mkdir -p "$project_dir/build"

if [ ! -e "$upstream_dir" ]; then
    git init -q "$upstream_dir"
    git -C "$upstream_dir" remote add origin https://github.com/mont127/AArchX.git
    git -C "$upstream_dir" fetch --depth 1 origin "$revision"
    git -C "$upstream_dir" checkout --detach FETCH_HEAD
fi
if [ "$(git -C "$upstream_dir" rev-parse HEAD)" != "$revision" ]; then
    echo 'Unexpected upstream revision. Preserve your build folder and use a fresh checkout.' >&2
    exit 1
fi
if git -C "$upstream_dir" apply --reverse --check "$patch_file" 2>/dev/null; then
    echo 'Compatibility patch already applied.'
else
    git -C "$upstream_dir" apply --check "$patch_file"
    git -C "$upstream_dir" apply "$patch_file"
fi

cd "$upstream_dir"
make -j6 ocerz
make apis32
make steam32
bash tools/fetch_guest32_cxx.sh

cd "$project_dir"
clang -arch arm64 -O2 -Wall -Wextra fm2014-launch.c -o build/fm2014-launch
ln -sfn AArchX/ocerz build/ocerz
ln -sfn AArchX/runtime build/runtime
printf '\nBuild complete. Steam Launch Options:\n"%s/build/fm2014-launch" %%command%%\n' "$project_dir"
