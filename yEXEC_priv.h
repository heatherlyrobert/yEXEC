/*============================----beg-of-source---============================*/
/*===[[ HEADER GUARD ]]=======================================================*/
#ifndef YEXEC_pguard
#define YEXEC_pguard loaded

/*===[[ HEADER ]]=============================================================*/
/*345678901-12345678901-123456789-123456789-123456789-123456789-123456789-123456789-123456789-*/

#define     P_FOCUS     "PS (programming support)"
#define     P_NICHE     "ex (process execution)"
#define     P_SUBJECT   "process control"
#define     P_PURPOSE   "clear, clean, program launching and tracking"

#define     P_NAMESAKE  "brontes-akmonides (son of the anvil)"
#define     P_HERITAGE  "thunder, one of the original three cyclops, blacksmith to gods"
#define     P_IMAGERY   "massive, brutish, immortal giant with a single round eye"
#define     P_REASON    ""

#define     P_ONELINE   P_NAMESAKE " " P_SUBJECT

#define     P_BASENAME  ""
#define     P_FULLPATH  ""
#define     P_SUFFIX    ""
#define     P_CONTENT   ""

#define     P_SYSTEM    "gnu/linux   (powerful, ubiquitous, technical, and hackable)"
#define     P_LANGUAGE  "ansi-c      (wicked, limitless, universal, and everlasting)"
#define     P_CODESIZE  "small       (appoximately 1,000 slocl)"
#define     P_DEPENDS   "ySTR"

#define     P_AUTHOR    "heatherlyrobert"
#define     P_CREATED   "2011-05"

#define     P_VERMAJOR  "1.--, improvements for next generation tools"
#define     P_VERMINOR  "1.1-, prepping for eos, nyx, hypnos, and hannibal"
#define     P_VERNUM    "1.1m"
#define     P_VERTXT    "updated and fully unit tested after fixing job control format"

#define     P_PRIORITY  "direct, simple, brief, vigorous, and lucid (h.w. fowler)"
#define     P_PRINCIPAL "[grow a set] and build your wings on the way down (r. bradbury)"
#define     P_REMINDER  "there are many better options, but i *own* every byte of this one"

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
#include <math.h>              /* round                                       */

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
#include <unistd.h>                 /* stat(),crpyt()                      */
#include <pwd.h>               /* getpwuid, getpwnam                          */
#include <grp.h>               /* initgroups                                  */
#include <shadow.h>                 /* LINIX     password shadow access    */

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

char        yexec__command          (void);
char        yexec__setpath          (char a_path);
char        yexec__onpath           (void);
char*       yexec_proc__unit        (char *a_question, int a_num);
char        yexec_proc__unit_read   (char a_type, char *a_info);

char*       yexec_tty__unit         (char *a_question);

char        yexec_unpoint           (char *a_ptr []);
char        yexec_arg               (char *a_src);
char        yexec_env               (char *a_src);


#endif
/*============================----end-of-source---============================*/
