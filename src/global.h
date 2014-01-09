#ifndef DAEMON_H
#define DAEMON_H

#define _GNU_SOURCE
#define _XOPEN_SOURCE 700
#ifndef __x86_64__
#define _FILE_OFFSET_BITS 64
#endif

#define _BSD_SOURCE
#include "tui.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>

#include <errno.h>
#include <pthread.h>
#include <assert.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ifaddrs.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdint.h>

#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/time.h>
#include <errno.h>
#include <assert.h>
//#include <curl/curl.h>

cursed *hosts_window;
cursed *if_window;

#endif
