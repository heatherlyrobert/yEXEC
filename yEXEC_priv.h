/*============================----beg-of-source---============================*/


/*===[[ VERSION ]]========================================*/
/* rapidly evolving version number to aid with visual change confirmation     */
#define   YEXEC_VER_NUM      "1.1c"
#define   YEXEC_VER_TXT      "split into files, many changes and better logging in proc functions"


#include <yLOG.h>              /* heatherly logging                           */
#include <yURG.h>              /* heatherly debugging framework               */

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


#define     LEN_RECD    2000
#define     LEN_PATH    300
#define     LEN_TITLE   30
#define     LEN_USER    12


/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
typedef struct passwd    tPASSWD;
typedef struct tm        tTIME;
typedef struct dirent    tDIRENT;
typedef struct sigaction tSIGACT;
typedef struct stat      tSTAT;



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


char         /*--> break string into argv structure ------[ leaf-- [ ------ ]-*/
yexec__parse       (const char *a_cstring);

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


char*       yexec_proc__unit        (char *a_question, int a_num);
char        yexec_proc__unit_read   (void);

/*============================----end-of-source---============================*/
