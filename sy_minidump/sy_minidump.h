#ifndef SY_MINIDUMP_H
#define SY_MINIDUMP_H

#ifdef WIN32
#include <windows.h>
#include <dbghelp.h>
#include <qt_windows.h>
#endif


#ifdef WIN32
LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException);
#else
int setup_sigsegv();
#endif

#endif // sy_MINIDUMP_H
