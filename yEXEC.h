/*============================----beg-of-source---============================*/
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



#include <ySTR_solo.h>


typedef long   long      llong;
typedef const  int       cint;
typedef const  long      clong;
typedef const  char      cchar;

typedef struct passwd    tPASSWD;
typedef struct spwd      tSHADOW;


#define     YEXEC_NONE     ""

#define     YEXEC_SOFT     'n'
#define     YEXEC_HARD     'y'

#define     YEXEC_NO       'n'
#define     YEXEC_WAIT     'w'
#define     YEXEC_YES      'y'

#define     YEXEC_IGNORED  'i'
#define     YEXEC_DEFAULT  'd'
#define     YEXEC_TRAPPED  't'

#define     YEXEC_BASH     'b'
#define     YEXEC_DASH     'd'

#define     YEXEC_TIGHT    't'
#define     YEXEC_FULL     'f'

#define     YEXEC_FORK     'y'
#define     YEXEC_REPLACE  '-'

#define     YEXEC_ALREADY  'A'
#define     YEXEC_NOSUCH   '#'
#define     YEXEC_RUNNING  'r'
#define     YEXEC_KILLED   'T'
#define     YEXEC_SEGV     'M'
#define     YEXEC_USER     'H'
#define     YEXEC_LIMIT    'L'
#define     YEXEC_NOTREAL  '?'
#define     YEXEC_DIED     'd'
#define     YEXEC_ERROR    'e'
#define     YEXEC_NORMAL   'n'
#define     YEXEC_FAILURE  'F'
#define     YEXEC_WARNING  'w'

#define     YEXEC_NOCHMOD  '{'
#define     YEXEC_BADLOG   '|'
#define     YEXEC_NOTEXEC  '}'
#define     YEXEC_NOPERM   '~'

#define     YEXEC_OVER     'O'
#define     YEXEC_CPU      'C'
#define     YEXEC_DISK     'D'
#define     YEXEC_NET      'N'

#define     YEXEC_BADDS    "#?{|}~"
#define     YEXEC_BOOMS    "MHLde"
#define     YEXEC_KILLS    "T"
#define     YEXEC_FAILS    "Fw"

#define     YEXEC_GRACEFUL 'g'
#define     YEXEC_VIOLENT  'k'
#define     YEXEC_TILLEND  ']'

#define     YEXEC_UNIT     "/tmp/yEXEC_unit.execution_feedback"

#define     YEXEC_STDALL   'a'
#define     YEXEC_STDOUT   'o'
#define     YEXEC_BLIND    '-'



#define     YEXEC_2USEC    1000000






/*===[[ yEXEC_data.c ]]=======================================================*/
/*········· ´······················ ´·········································*/
/*---(process)--------------*/
char        yEXEC_proc_data         (short a_rpid, char *r_public, char *r_cmdline, char *r_state, int *r_ppid);
/*---(eterm)----------------*/
char        yEXEC_proc_eterm        (short a_rpid, int *r_uid, char *r_home, char *r_pwd, char *r_cabbr, char *r_cname, char *r_stype, char *r_shex, char *r_rows, char *r_cols);
char        yEXEC_find_my_eterm     (short a_rpid, int *a_eterm);
char        yEXEC_find_eterm_use    (short a_rpid, char *a_lvl, int *a_lowest, char a_pubname [LEN_LABEL], char a_cmdline [LEN_RECD]);
char        yEXEC_data_filter       (short a_rpid, char *a_pubname, short a_ppid, void *f_callback);
char        yEXEC_data_windows      (void *f_callback);
/*---(done)-----------------*/



/*===[[ yEXEC_fifo.c ]]=======================================================*/
/*········· ´······················ ´·········································*/
/*---(duration)-------------*/
char        yEXEC_fifo_create       (cchar a_name [LEN_PATH]);
int         yEXEC_fifo_listen       (char a_name [LEN_HUND], char r_recd [LEN_RECD]);
char        yEXEC_fifo_nolisten     (void);
char        yEXEC_fifo_destroy      (cchar a_name [LEN_PATH]);
char        yEXEC_fifo_speak        (cchar a_name [LEN_PATH], cchar a_recd [LEN_RECD]);
/*---(done)-----------------*/



/*===[[ yEXEC_flags.c ]]======================================================*/
/*········· ´······················ ´·········································*/
/*---(duration)-------------*/
char        yEXEC_dur_in_sec        (char *a_text, int *a_dur);
/*---(flags)----------------*/
char        yEXEC_flags             (int a_dur, int a_floor, char *a_flags, char *a_value, char *a_track, char *a_rolling, char *a_strict, char *a_min, int *a_mindur, char *a_max, int *a_maxdur, char *a_remedy);
char        yEXEC_flags_more        (int a_dur, int a_floor, char *a_flags, char *a_value, char *a_track, char *a_rolling, char *a_strict, char *a_min, int *a_mindur, char *a_max, int *a_maxdur, char *a_remedy, char *a_flex, char *a_throttle, char *a_cpu, char *a_disk, char *a_net);
char        yEXEC_flags_feedback    (char a_terse [LEN_HUND], char a_fancy [LEN_FULL]);
/*---(usage)----------------*/
char        yEXEC_controls          (void);
/*---(done)-----------------*/




char*       yEXEC_version           (void);



char        yEXEC_runable           (char *a_title, char *a_user, char  *a_cmd, char a_path);
int         yEXEC_quick             (char *a_cmd);
int         yEXEC_run               (char *a_title, char *a_user, char *a_cmd);
int         yEXEC_tight             (char *a_title, char *a_user, char *a_cmd);
int         yEXEC_full              (char *a_title, char *a_user, char *a_cmd, char a_shell, char a_path, char a_fork, char *a_output);
char        yEXEC_verify            (char *a_title, int a_rpid, int *a_rc2, float *a_csec);
char        yEXEC_check             (int a_rpid);
char        yEXEC_timing            (int a_rpid, char a_strict, int a_max, int a_dur, int a_grace, int a_peers);
char        yEXEC_detail            (char a_rc, int a_rc2, char *a_desc);
char        yEXEC_find              (char a_name [LEN_LABEL], int *r_rpid);
char        yEXEC_duplicate         (char a_name [LEN_LABEL], int a_mypid, int *r_rpid);
char        yEXEC_maxname           (int a_argc, char *a_argv [], int *a_max);
char        yEXEC_rename            (char *a_mem, char *a_name, int a_max);



char        yEXEC_cpu_main          (long *a_utime, long *a_stime, long *a_idle);
char        yEXEC_cpu_proc          (int a_rpid, char a_cpu, char *a_state, long *a_utime, long *a_stime, char *a_snice);
char        yEXEC_mem_proc          (int a_rpid, char a_mem, long *a_total, long *a_true, long *a_text, long *a_data, long *a_heap, long *a_stack, long *a_kern, long *a_libs, long *a_empty, char *a_flag);
char        yEXEC_mem_proc_OLD      (int a_rpid, long *a_total, long *a_text, long *a_data, long *a_stack, long *a_heap, long *a_libs);









/*===[[ yEXEC_sign.c ]]=======================================================*/
/*········· ´······················ ´·········································*/
/*---(terminate)------------*/
char        yEXEC_term              (const char *a_func, const int a_exit);
/*---(setting)--------------*/
char        yEXEC_signal            (char a_tough, char a_inter, char a_child, void *a_signaler, char *a_output);
char        yEXEC_sighard           (void);
char        yEXEC_sigsoft           (void);
/*---(done)-----------------*/











/*===[[ yEXEC_self.c ]]=======================================================*/
/*········· ´······················ ´·········································*/
/*---(myself)---------------*/
char        yEXEC_whoami            (int *r_pid, int *r_ppid, int *r_uid, int *r_euid, char *r_root, char r_user [LEN_USER], char a_wheel, int *r_gid, int *r_egid, char r_group [LEN_USER]);
/*---(heartbeat)------------*/
char        yEXEC_heartbeat         (int a_rpid, long a_now, char *a_suffix, char *a_file, char *a_heartbeat);
char        yEXEC_heartquiet        (int a_rpid, long a_now, char *a_suffix, char *a_file, char *a_heartbeat);
char        yEXEC_heartlong         (int a_rpid, long a_now, short a_update, char *a_suffix, char *a_file, char *a_heartbeat);
char        yEXEC_heartbeat_check   (char *a_file, char *a_heartbeat);
/*---(daemon)---------------*/
char        yEXEC_daemon            (int a_logger, int *a_rpid);
/*---(DEPRICATED)-----------*/
char        yEXEC_userdata          (char *a_user, int *a_uid, int *a_gid, char *a_dir, char *a_shell);
/*---(done)-----------------*/


#endif
/*============================----end-of-source---============================*/
