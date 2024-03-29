#pragma once

#if !defined(_WIN32) && !defined(__APPLE__)
#include <features.h>
#endif

#define HAVE_STRRSTR 1
#define HAVE_STRCASECMP 1
#define HAVE_STRCASESTR 1
#define HAVE_STRDUP 1

#if (!defined(__GLIBC__) || (__GLIBC__ > 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ >= 38))) \
  && !defined(_WIN32)
#define HAVE_STRLCAT 1
#define HAVE_STRLCPY 1
#endif

#define HAVE_STRLEN 1
#define HAVE_STRNCASECMP 1
#define HAVE_VSNPRINTF 1

#define HAVE_LRINTF 1
#define HAVE_LRINT
#define HAVE_SYNC_FETCH_AND_ADD 1
