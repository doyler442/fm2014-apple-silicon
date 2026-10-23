/* SPDX-License-Identifier: LGPL-2.1-or-later
 * FM2014 compatibility launcher, 23–24 October 2026.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <mach-o/dyld.h>
int main(int argc, char **argv) {
    char self[PATH_MAX], engine[PATH_MAX], log[PATH_MAX];
    uint32_t size = sizeof self;
    if (_NSGetExecutablePath(self, &size) || !strrchr(self, '/')) return 1;
    *strrchr(self, '/') = 0;
    if (snprintf(engine, sizeof engine, "%s/ocerz", self) >= (int)sizeof engine ||
        snprintf(log, sizeof log, "%s/fm2014.log", self) >= (int)sizeof log) return 1;
    int fd=open(log,O_WRONLY|O_CREAT|O_TRUNC,0600);
    if(fd<0) return 1;
    dup2(fd,STDOUT_FILENO);dup2(fd,STDERR_FILENO);if(fd>2)close(fd);
    setenv("OCERZ_M32_COCOA_EVENT_LOOP","1",1);
    setenv("OCERZ_M32_GL_LOCK_HANDOFF","1",1);
    setenv("OCERZ_M32_ACTIVATE","1",1);
    setenv("OCERZ_M32_WINDOW_LOG","1",1);
    setenv("OCERZ_M32_MAIN_WAIT_PUMP","1",1);
    setenv("OCERZ_TCACHE","off",1);setenv("OCERZ_M32LOG","steam",1);
    fprintf(stderr,"FM2014 native launcher started; arguments=%d\n",argc-1);
    if(argc<2) {fprintf(stderr,"Run this launcher from Steam with %%command%%.\n");return 2;}
    char **args=calloc((size_t)argc+2,sizeof(char *));if(!args)return 1;
    args[0]=(char*)engine;args[1]="-native";
    for(int i=1;i<argc;i++)args[i+1]=argv[i];
    execv(engine,args);
    fprintf(stderr,"Could not start translator: %s\n",strerror(errno));return 1;
}
