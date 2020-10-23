#include "sy_minidump.h"


#include <QString>
#include <QTime>
#include <QtDebug>


#ifdef WIN32
LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException)
{
    //程式异常捕获
    //创建 Dump 文件
    QString file_name = QString("%2.%1.dmp").arg(QTime::currentTime().toString("HHmmss.zzz")).arg(SY_APP_TYPE);
    HANDLE hDumpFile = CreateFile((LPCWSTR)(file_name.utf16()), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if( hDumpFile != INVALID_HANDLE_VALUE){
        //Dump信息
        MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
        dumpInfo.ExceptionPointers = pException;
        dumpInfo.ThreadId = GetCurrentThreadId();
        dumpInfo.ClientPointers = TRUE;
        //写入Dump文件内容
        MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
    }
    return EXCEPTION_EXECUTE_HANDLER;
}
#else
static inline void see_back_trace(void)
{
//    void *bt[20];
//    char **strings;
//    size_t sz;
//    size_t i;

//    qDebug() << ( "E00070:\tbacktrace():\n");
//    sz = backtrace(bt, 20);
//    strings = backtrace_symbols(bt, sz);

//    for(i = 0; i < sz; ++i)
//            qDebug() <<  "E20030:\t%s\n" << strings[i];
}

static void
signal_segv(int signum, siginfo_t * info, void * ptr) {

//    static const char *si_codes[3] = {"", "SEGV_MAPERR", "SEGV_ACCERR"};
//    ucontext_t *ucontext = (ucontext_t*)ptr;


//    printf( "===[ CRASH REPORT ]======\n");
//    printf( "  si_signo = %d (%s)\n", signum, strsignal(signum));
//    printf( "  si_errno = %d\n", info->si_errno);
//    printf( "  si_code  = %d (%s)\n", info->si_code, si_codes[info->si_code]);
//    printf( "  si_addr  = %p\n\n", info->si_addr);
//    see_back_trace();
//    printf( "===[ END OF CRASH REPORT ]======\n");
}

static void debug_enable_coredump()
{
//    struct rlimit rl = {RLIM_INFINITY, RLIM_INFINITY};
//    int rv;

//    rv = setrlimit(RLIMIT_CORE, &rl);
//    if (rv != 0) {
//        perror("E20070:\tsetrlimit");
//        exit(1);
//    }
}
int setup_sigsegv() {
//    struct sigaction action;
//    memset(&action, 0, sizeof(action));
//    action.sa_sigaction = signal_segv;
//    action.sa_flags = SA_SIGINFO;

//    debug_enable_coredump();

//    if (sigaction(SIGFPE, &action, NULL) < 0) {
//            perror("E20070:\tsigaction");
//            return 0;
//    }
//    if (sigaction(SIGILL, &action, NULL) < 0) {
//            perror("E20070:\tsigaction");
//            return 0;
//    }
//    if (sigaction(SIGSEGV, &action, NULL) < 0) {
//            perror("E20070:\tsigaction");
//            return 0;
//    }
//    if (sigaction(SIGBUS, &action, NULL) < 0) {
//            perror("E20070:\tsigaction");
//            return 0;
//    }

//    if (sigaction(SIGABRT, &action, NULL) < 0) {
//            perror("E20070:\tsigaction");
//            return 0;
//    }
    return 1;
}

#endif
