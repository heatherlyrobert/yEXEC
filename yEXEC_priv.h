/*============================----beg-of-source---============================*/
/*===[[ HEADER GUARD ]]=======================================================*/
#ifndef YEXEC_pguard
#define YEXEC_pguard loaded

/*===[[ HEADER ]]=============================================================*/
/*                      ´·········1·········2·········3·········4·········5·········6·········7*/
/*--------- 12345678901 ´123456789-123456789-123456789-123456789-123456789-123456789-123456789-*/
/*········· ··········· ´·····························´········································*/
#define     P_NAME      "yEXEC"
#define     P_FOCUS     "PS (programming support)"
#define     P_NICHE     "ex (process execution)"
#define     P_SUBJECT   "process control"
#define     P_PURPOSE   "clear, clean, program launching and tracking"
/*········· ··········· ´·····························´········································*/
#define     P_NAMESAKE  "brontes-akmonides (son of the anvil)"
#define     P_PRONOUNCE "brahn·tees ahk·moh·nee·dais"
#define     P_HERITAGE  "thunderer, one of the original three cyclops, blacksmith to gods"
#define     P_BRIEFLY   "blacksmith to the gods"
#define     P_IMAGERY   "massive, brutish, immortal giant with a single round eye"
#define     P_REASON    "brontes is a blacksmith, launching and catching like an anvil"
/*········· ··········· ´·····························´········································*/
#define     P_ONELINE   P_NAMESAKE " " P_SUBJECT
/*········· ··········· ´·····························´········································*/
#define     P_HOMEDIR   "/home/system/yEXEC.program_launching"
#define     P_BASENAME  "yEXEC"
#define     P_FULLNAME  "/usr/local/lib64/libyEXEC"
#define     P_SUFFIX    "···"
#define     P_CONTENT   "···"
/*········· ··········· ´·····························´········································*/
#define     P_SYSTEM    "gnu/linux   (powerful, ubiquitous, technical, and hackable)"
#define     P_LANGUAGE  "ansi-c      (wicked, limitless, universal, and everlasting)"
#define     P_COMPILER  "gcc 5.3.0"
#define     P_CODESIZE  "small       (appoximately 1,000 slocl)"
#define     P_DEPENDS   "ySTR"
/*········· ··········· ´·····························´········································*/
#define     P_AUTHOR    "heatherlyrobert"
#define     P_CREATED   "2011-05"
/*········· ··········· ´·····························´········································*/
#define     P_VERMAJOR  "1.--, improvements for next generation tools"
#define     P_VERMINOR  "1.2-, integrating into khronos and eos"
#define     P_VERNUM    "1.2n"
#define     P_VERTXT    "updated to yURG_by_name after yURG change"
/*········· ··········· ´·····························´········································*/
#define     P_PRIORITY  "direct, simple, brief, vigorous, and lucid (h.w. fowler)"
#define     P_PRINCIPAL "[grow a set] and build your wings on the way down (r. bradbury)"
#define     P_REMINDER  "there are many better options, but i *own* every byte of this one"
/*········· ··········· ´·····························´········································*/
/*--------- 12345678901 ´123456789-123456789-123456789-123456789-123456789-123456789-123456789-*/





#include    <yLOG.h>         /* CUSTOM : heatherly process logging            */
#include    <yURG.h>         /* CUSTOM : heatherly debugging framework        */
#include    <ySTR.h>         /* CUSTOM : heatherly string handling            */
#include    <yCOLOR_solo.h>  /* CUSTOM : heatherly color constants            */
#include    <yDLST_solo.h>   /* CUSTOM : heatherly list constants             */
#include    <yREGEX.h>       /* CUSTOM : heatherly regular expressions        */

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
typedef struct rusage    tRUSE;


#define     SIMPLIFIER     /**/
#define     FACTCHECK      /**/

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



extern char   g_runas;
extern char   g_runmode;
extern char   g_runfile     [LEN_PATH];
extern char   g_norun;
extern char   s_terse     [LEN_HUND];
extern char   s_fancy     [LEN_RECD];

typedef struct cEXEC tEXEC;
typedef struct cPROC tPROC;
typedef struct cTIE  tTIE;
typedef struct cLIB  tLIB;


void             /* [------] receive signals ---------------------------------*/
yEXEC__comm        (
      int         a_signal,       /* signal number                            */
      siginfo_t  *a_info,         /* signal detail structure                  */
      void       *a_nada);        /* ignored                                  */


/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
extern      char        unit_answer [LEN_RECD];

/*> char        yexec_act__checkdir     (cchar *a_dir, int a_perms);                                                                                                     <* 
 *> char        yexec_act__filter       (cchar *a_name, cchar *a_prefix, int a_muid);                                                                                    <* 
 *> char        yexec_act__prepare      (cchar a_pre, cchar a_act, cchar *a_oneline, cchar *a_muser, cchar *a_regex, char *a_prefix, char *a_dir, void *a_assimilate);   <* 
 *> char        yexec_act__assim        (cchar a_runas, cchar a_loc, cchar *a_name, char *r_user, char *r_desc);                                                         <* 
 *> char        yexec_act__mkdir        (void);                                                                                                                          <* 
 *> char        yexec_act__rmdir        (void);                                                                                                                          <*/

/*> char        yexec_args__empty       (void);                                              <* 
 *> char        yexec_args__init        (char a_runas, char a_runmode, char *a_runfile);     <* 
 *> char        yexec_args__single      (char *a_levels, char n);                            <* 
 *> char        yexec_args__clearmode   (char *a_runas, char *a_runmode, char *a_runfile);   <* 
 *> char*       yexec_args__unit        (char *a_question);                                  <*/

/*> char        yexec__naming           (uchar *a_name);                                                                                                                                <* 
 *> char        yexec__location         (cchar a_pre, cchar a_loc, cchar *a_home, cchar *a_root, uchar *a_name, cchar *a_muser, int a_muid, char *r_fuser, int *r_fuid, char *r_dir);   <* 
 *> char        yexec__stats            (cchar a_loc, cchar *a_dir, uchar *a_name, cchar *a_muser, int a_muid, uchar *a_user, int a_uid);                                               <*/
char*       yexec_base__unit        (char *a_question);
char        yexec__unit_quiet       (void);
char        yexec__unit_loud        (void);
char        yexec__unit_end         (void);

char        yexec_min_in_msec       (int a_dur, char a_min, int a_floor , int *a_mindur);
char        yexec_max_in_msec       (int a_dur, char a_max, int a_mindur, int *a_maxdur);

char*       yexec_spec__unit        (char *a_question, char *a_text);

char        yexec__command          (void);
char        yexec__setpath          (char a_path);
char        yexec__onpath           (void);
char*       yexec_proc__unit        (char *a_question, int a_num);
char        yexec_proc__unit_read   (char a_type, char *a_info);

/*---(support)--------------*/
char*       yexec_mon__ememory      (tEXEC *a_cur);
char        yexec_mon__ewipe        (tEXEC *a_new, char a_type);
char        yexec_mon__pwipe        (tPROC *a_new, char a_type);
char*       yexec_mon__pmemory      (tPROC *a_cur);
char        yexec_mon__lwipe        (tLIB  *a_new, char a_type);
char*       yexec_mon__lmemory      (tLIB  *a_cur);
/*---(malloc)---------------*/
char        yexec_mon__shared_new   (char a_type, void **a_new, char a_force);
char        yexec_mon__enew         (void **a_new);
char        yexec_mon__eforce       (void **a_new);
char        yexec_mon__pnew         (void **a_new);
char        yexec_mon__pforce       (void **a_new);
char        yexec_mon__tnew         (void **a_new);
char        yexec_mon__tforce       (void **a_new);
char        yexec_mon__lnew         (void **a_new);
char        yexec_mon__lforce       (void **a_new);
char        yexec_mon__efree        (void **a_old);
char        yexec_mon__pfree        (void **a_old);
char        yexec_mon__tfree        (void **a_old);
char        yexec_mon__lfree        (void **a_old);
/*---(mass)-----------------*/
char        yexec_mon__epurge       (void);
char        yexec_mon__ppurge       (void);
char        yexec_mon__tpurge       (void);
char        yexec_mon__lpurge       (void);
/*---(cursoring)------------*/
char        yexec_mon__by_cursor    (char a_type, char a_move, void **a_curr);
char        yexec_mon__by_rpid      (int a_rpid, tPROC **a_curr);
char        yexec_mon__by_inode     (int a_inode, void **a_curr);
/*---(helpers)--------------*/
char        yexec_mon__hook_exec    (tPROC *a_proc, char *a_name);
char        yexec_mon__unhook_exec  (tPROC *a_proc);
char        yexec_mon__hook_proc    (tPROC **a_proc, int a_rpid);
char        yexec_mon__unhook_proc  (tPROC **a_proc);
char        yexec_mon__hook_lib     (tPROC *a_proc, char *a_name);
char        yexec_mon__unhook_lib   (tPROC *a_proc);
/*---(monitor)--------------*/
char        yexec_mon__cpu_detail   (int a_rpid, char *a_name, int *a_ppid, char *a_land, char *a_state, long *a_utime, long *a_stime, char *a_snice);
char        yexec_mon__cpu          (tPROC *a_proc);
char        yexec_mon__mem_detail   (int a_rpid, long *a_max, long *a_base, long *a_min, long *a_text, long *a_data, long *a_heap, long *a_stack, long *a_kern, long *a_libs, long *a_empty);
char        yexec_mon__mem          (tPROC *a_proc);
char        yexec_mon__review       (void);
/*---(unit test)------------*/
char*       yexec_mon__unit         (char *a_question, int n);

char*       yexec_tty__unit         (char *a_question);

char        yexec_unpoint           (char *a_ptr []);
char        yexec_arg               (char *a_src);
char        yexec_env               (char *a_src);



char        yexec_data__pubname     (short a_rpid, char a_unit, char *r_public);
char        yexec_data__cmdline     (short a_rpid, char a_unit, char *r_cmdline);
char        yexec_data__ppid        (short a_rpid, char a_unit, char *r_state, int *r_ppid);


char        yexec_fifo__verify      (char a_force, cchar a_name [LEN_PATH]);
char        yEXEC_fifo_create       (cchar a_name [LEN_PATH]);
char        yexec_fifo__open        (cchar a_name [LEN_PATH]);
char        yexec_fifo__close       (void);
char        yEXEC_fifo_nolisten     (void);
char        yexec_fifo__listener    (cchar a_recd [LEN_RECD]);
char        yEXEC_fifo_listen       (char a_persist, cchar a_name [LEN_PATH], void *f_callback);
char        yEXEC_fifo_destroy      (cchar a_name [LEN_PATH]);
char        yEXEC_fifo_speak        (cchar a_name [LEN_PATH], cchar a_recd [LEN_RECD]);



#endif
/*============================----end-of-source---============================*/
