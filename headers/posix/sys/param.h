#ifndef _SYS_PARAM_H
#define _SYS_PARAM_H

#include <limits.h>
#include <sys/types.h>

#define MAXPATHLEN      PATH_MAX
#define NOFILE          OPEN_MAX

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

/* maximum possible length of this machine's hostname */
#define MAXHOSTNAMELEN 256


#endif
