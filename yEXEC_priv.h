/*============================----beg-of-source---============================*/
/*===[[ HEADER GUARD ]]=======================================================*/
#ifndef YEXEC_pguard
#define YEXEC_pguard loaded

/*===[[ HEADER ]]=============================================================*/
/*345678901-12345678901-123456789-123456789-123456789-123456789-123456789-123456789-123456789-*/

#define     P_FOCUS     "PS (programming support)"
#define     P_NICHE     "ex (process execution)"
#define     P_PURPOSE   "clear, clean, program launching and tracking"

#define     P_NAMESAKE  "brontes-akmonides (son of the anvil)"
#define     P_HERITAGE  "thunder, one of the original three cyclops, blacksmith to gods"
#define     P_IMAGERY   "massive, brutish, immortal giant with a single round eye"

#define     P_SYSTEM    "gnu/linux   (powerful, ubiquitous, technical, and hackable)"
#define     P_LANGUAGE  "ansi-c      (wicked, limitless, universal, and everlasting)"
#define     P_CODESIZE  "small       (appoximately 1,000 slocl)"

#define     P_AUTHOR    "heatherlyrobert"
#define     P_CREATED   "2011-05"
#define     P_DEPENDS   "ySTR"

#define     P_VERNUM    "1.1g"
#define     P_VERTXT    "fixed for changes to yLOG during updates"
#define     P_PRIORITY  "direct, simple, brief, vigorous, and lucid (h.w. fowler)"

/*345678901-12345678901-123456789-123456789-123456789-123456789-123456789-123456789-123456789-*/




#include <yLOG.h>              /* heatherly logging                           */
#include <yURG.h>              /* heatherly debugging framework               */
#include <ySTR.h>              /* heatherly string handling                   */

/*===[[ PUBLIC HEADERS ]]=====================================================*/
/*---(big standards)------------*/
#include <stdio.h>             /* printf, scanf                               */
#include <stdlib.h>            /* exit, system, ...                           */
#include <sys/unistd.h>        /* fork, execl, getpid, getuid                 */
#include <string.h>            /* strcat, strcpy, strlen, ...                 */

/*---(process)------------------*/
#include <sys/wait.h>          /* sigaction, waitpid, wait4                   */

/*---(timekeeping)--------------*/
#include <time.h>              /* time, localtime, strftime, ...              */
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/time.h>          /* gettimeofday                                */

/*---(filesystems)--------------*/
#include <sys/mount.h>         /* mount                                       */
#include <fcntl.h>             /* open                                        */
#include <sys/stat.h>          /* fstat, umask                                */
#include <dirent.h>

/*---(users)--------------------*/
#include <pwd.h>               /* getpwuid, getpwnam                          */
#include <grp.h>               /* initgroups                                  */

#include <errno.h>             /* standard errors                             */

/*---(tty)----------------------*/
#include    <termios.h>           /* clibc standard terminal control          */


/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
typedef struct passwd    tPASSWD;
typedef struct tm        tTIME;
typedef struct dirent    tDIRENT;
typedef struct sigaction tSIGACT;
typedef struct stat      tSTAT;
typedef struct termios   tTERMIOS;



#define          SUCCESS          0
#define          NOT_READY       -1
#define          FAILED          -2
#define          SKIPPING        -3

typedef struct cLOCAL tLOCAL;
struct cLOCAL {
   /*---(overall)-----------*/
   char      status;
   char      full  [LEN_RECD];
   /*---(parsing)-----------*/
   char      recd  [LEN_RECD];
};
extern  tLOCAL its;


void             /* [------] receive signals ---------------------------------*/
yEXEC__comm        (
      int         a_signal,       /* signal number                            */
      siginfo_t  *a_info,         /* signal detail structure                  */
      void       *a_nada);        /* ignored                                  */


/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
extern      char        unit_answer [LEN_RECD];

char        yexec__unit_quiet       (void);
char        yexec__unit_loud        (void);
char        yexec__unit_end         (void);

char        yexec__unit_heartbeat   (char *a_file, char *a_heartbeat);
char*       yexec_spec__unit        (char *a_question, char *a_text);

char*       yexec_proc__unit        (char *a_question, int a_num);
char        yexec_proc__unit_read   (void);



#endif
/*============================----end-of-source---============================*/
