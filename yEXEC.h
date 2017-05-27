/*============================----beg-of-source---============================*/

/*===[[ HEADER ]]=============================================================*

 *   focus         : (PS) programming support
 *   niche         : (ex) program_execution
 *   application   : yEXEC
 *   purpose       : clear, clean, program launching and tracking
 *
 *   base_system   : gnu/linux   (powerful, ubiquitous, technical, and hackable)
 *   lang_name     : ansi-c      (righteous, limitless, universal, and forever)
 *   dependencies  : yLOG
 *   size goal     : small       (less than 1,000 lines)
 *
 *   author        : the_heatherlys
 *   created       : 2011-05
 *   priorities    : direct, simple, brief, vigorous, and lucid (h.w. fowler)
 *   end goal      : loosely coupled, strict interface, maintainable, portable
 * 
 */
/*===[[ SUMMARY ]]============================================================*

 *   yEXEC is a simple, clean, and clear library for launching and monitoring
 *   jobs for use in programs such as khronos, eos, kharon, haides, and nyx.
 *
 */
/*===[[ PURPOSE ]]============================================================*

 *   yEXEC is a set of shared functions providing reliable, consisten, and clean
 *   access to program launching and monitoring services.  its purpose is to
 *   take a complex and foundational capability and provide a shared service
 *   that can be tested, maintained, and documented once to avoid migrations.
 *
 *   keeping with the refactoring motto of "DRY: don't repeat yourself", we will
 *   build this code into a single set of shared functions so that we have a
 *   single, unambiguous, authoritative version of the functions.
 *
 *   yEXEC will focus on...
 *      - very solid logging on all functions (noisy and traceable)
 *      - provide execution output on launches
 *      - simple, defensive job launching
 *      - checking for program completion
 *      - finding jobs by name
 *      - daemonizing programs
 *      - normal signal handling
 *
 *   as always, there are many, stable, accepted, existing programs and
 *   utilities built by better programmers that are likely superior in speed,
 *   size, capability, and reliability; BUT, i would not have learned nearly as
 *   much using them, so i follow the adage...
 *
 *   TO TRULY LEARN> do not seek to follow in the footsteps of the men of old;
 *   seek what they sought ~ Matsuo Basho
 *
 *   the underlying idea is to use this build process to learn new programming
 *   capabilities and put additional pressure on me to improve my standards,
 *   maintenance, and coding environment.  just using programs and ideas does
 *   not lead to understanding -- so get messy ;)
 *
 *   any one who wishes to become a good writer [programmer] should endeavour,
 *   before he allows himself to be tempted by the more showy qualities, to be
 *   direct, simple, brief, vigourous, and lucid -- henry watson fowler (1908)
 *
 *   simplicity is prerequisite for reliability and security
 *
 */
/*============================================================================*/



/*===[[ HEADER GUARD ]]=======================================================*/
#ifndef YEXEC_hguard
#define YEXEC_hguard loaded


#include <yLOG.h>              /* heatherly logging                           */

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





typedef long   long      llong;
typedef const  int       cint;
typedef const  long      clong;
typedef const  char      cchar;

typedef struct passwd    tPASSWD;
typedef struct tm        tTIME;
typedef struct dirent    tDIRENT;
typedef struct sigaction tSIGACT;


#define     yEXEC_NONE     ""

#define     yEXEC_SOFT     'n'
#define     yEXEC_HARD     'y'

#define     yEXEC_TNO      'n'
#define     yEXEC_TYES     'y'

#define     yEXEC_CNO      'n'
#define     yEXEC_CYES     'y'
#define     yEXEC_CALL     'a'

#define     yEXEC_NOLOCAL  'n'
#define     yEXEC_LOCAL    'y'

#define     yEXEC_BASH     "/bin/bash"
#define     yEXEC_DASH     "/bin/dash"

#define     yEXEC_SYSM     "/sbin:/bin:/usr/sbin:/usr/bin"
#define     yEXEC_NORM     "/sbin:/bin:/usr/sbin:/usr/bin:/opt/sbin:/opt/bin:/usr/local/sbin:/usr/local/bin"

#define     yEXEC_FORK     'y'
#define     yEXEC_DETACH   'd'
#define     yEXEC_NOFORK   '-'


char*        /*--> return library versioning info --------[ leaf-- [ ------ ]-*/
yEXEC_version      (void);

int          /*--> execute a specific command ------------[ ------ [ ------ ]-*/
yEXEC_run          (
      char       *a_output,       /* file to capture execution entry          */
      char       *a_title,        /* short title                              */
      char       *a_user,         /* user name                                */
      char       *a_cmd,          /* command to execute                       */
      char       *a_shell,        /* shell to run under                       */
      char       *a_path,         /* path to use                              */
      char        a_fork);        /* fork new or use existing process space   */

char         /*--> verify status of a running job --------[ ------ [ ------ ]-*/
yEXEC_check        (
      char       *a_title,        /* program description                      */
      int         a_rpid);        /* pid of program to locate                 */

char             /* [------] find a running job by name ----------------------*/
yEXEC_find         (
      char       *a_name,         /* name to search for                       */
      int        *a_rpid);        /* pid of program found                     */

char             /* [------] exit on termintation/signal ---------------------*/
yEXEC_term         (
      const char *a_func,         /* descriptive function name                */
      const int   a_exit);        /* exit code                                */

char             /* [------] setup signal handling ---------------------------*/
yEXEC_signal       (
      char        a_bulletproof,  /* fail on SEGV and TERM (y/n)              */
      char        a_interactive,  /* allow terminal signals                   */
      char        a_children,     /* track children        (y/n)              */
      char        a_callback);    /* call local handler    (y/n)              */

void
PROG_signal        (int a_signal, siginfo_t *a_info, void *a_nada);

char             /* [------] daemonize the current program -------------------*/
yEXEC_daemon       (
      int         a_logger,       /* file id of logger                        */
      int        *a_rpid);        /* resulting pid of program                 */

/*> int              /+ [------] execute a specific command ----------------------+/   <* 
 *> yEXEC_detach       (                                                               <* 
 *>       char       *a_output,       /+ file to capture execution entry          +/   <* 
 *>       char       *a_title,        /+ short title                              +/   <* 
 *>       char       *a_user,         /+ user name                                +/   <* 
 *>       char       *a_cmd,          /+ command to execute                       +/   <* 
 *>       char       *a_shell,        /+ shell to run under                       +/   <* 
 *>       char       *a_path);        /+ path to use                              +/   <*/



#endif
/*============================----end-of-source---============================*/
