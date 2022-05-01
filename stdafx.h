#pragma once
#include <assert.h>

#define KG_PROCESS_ERROR(x) if (!(x)) goto Exit0;
#define KGLOG_PROCESS_ERROR(x) if (!(x)) { printf("KGLOG_PROCESS_ERROR(%s) at %s in %s : %d\n", #x, __FUNCTION__, __FILE__, __LINE__); goto Exit0; };

# if defined(_MSC_VER)
# ifndef _CRT_SECURE_NO_DEPRECATE
# define _CRT_SECURE_NO_DEPRECATE (1)
# endif
# pragma warning(disable : 4996)
# endif

#include <time.h>

#ifdef _WIN32

#ifndef timezone
#define timezone _timezone
#endif

#endif