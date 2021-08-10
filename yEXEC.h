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

#define     YEXEC_UNIT     "/tmp/yEXEC_unit.execution_feedback"

#define     YEXEC_STDALL   'a'
#define     YEXEC_STDOUT   'o'
#define     YEXEC_BLIND    '-'


#define     YEXEC_CENTRAL  'C'
#define     YEXEC_LOCAL    'L'




/*---(run-as)-----------------------------------*/
#define     IAM_EOS          'e'
#define     IAM_ASTRAIOS     'a'
#define     IAM_HYPNOS       'y'
#define     IAM_HERACLES     'h'
#define     IAM_KHRONOS      'k'
/*---(run-as/unit testing)----------------------*/
#define     IAM_UEOS         'E'
#define     IAM_UASTRAIOS    'A'
#define     IAM_UHYPNOS      'Y'
#define     IAM_UHERACLES    'H'
#define     IAM_UKHRONOS     'K'
/*---(run-as/combination)-----------------------*/
#define     IAM_VALID        "eayhkEAYHK"
#define     IAM_UNIT         "EAYHK"
#define     IAM_ETC          "eayEAY"
/*---(run-as/done)------------------------------*/



/*===[[ ACTIONS ]]=============================*/
/*---(placeholder)----------*/
#define     ACT_NONE        '-'
/*---(simple)---------------*/
#define     ACT_VERSION     '1'
#define     ACT_HELP        '2'
#define     IF_VERSION      if (yEXEC_ifversion () == 1)
#define     IF_HELP         if (yEXEC_ifhelp    () == 1)
/*---(local)----------------*/
#define     ACT_VERIFY      'v'
#define     ACT_VVERIFY     'V'
#define     ACT_CVERIFY     'ÿ'
#define     IF_VERIFY       if (yEXEC_ifverify  () == 1)
#define     CASE_VERIFY     'v' : case 'V' : case 'ÿ'
/*---(incomming)------------*/
#define     ACT_INSTALL     'i'
#define     ACT_VINSTALL    'I'
#define     ACT_CINSTALL    'ð'
#define     IF_INSTALL      if (yEXEC_ifinstall () == 1)
#define     CASE_INSTALL    'i' : case 'I' : case 'ð'
/*---(central/inventory)----*/
#define     ACT_COUNT       'l'
#define     ACT_LIST        'L'
#define     IF_LIST         if (yEXEC_iflist    () == 1)
#define     CASE_LIST       'l' : case 'L'
/*---(central/installed)----*/
#define     ACT_CHECK       'c'
#define     ACT_VCHECK      'C'
#define     ACT_CCHECK      'ý'
#define     IF_CHECK        if (yEXEC_ifcheck   () == 1)
#define     CASE_CHECK      'c' : case 'C' : case 'ý'
/*---(central/security)-----*/
#define     ACT_AUDIT       'a'
#define     ACT_VAUDIT      'A'
#define     ACT_CAUDIT      'è'
#define     IF_AUDIT        if (yEXEC_ifaudit   () == 1)
#define     CASE_AUDIT      'a' : case 'A' : case 'è'
/*---(central/fix)----------*/
#define     ACT_FIX         'f'
#define     ACT_VFIX        'F'
#define     ACT_CFIX        'ü'
#define     IF_FIX          if (yEXEC_iffix     () == 1)
#define     CASE_FIX        'f' : case 'F' : case 'ü'
/*---(outgoing)-------------*/
#define     ACT_REMOVE      'r'
#define     ACT_VREMOVE     'R'
#define     ACT_CREMOVE     'ø'
#define     IF_REMOVE       if (yEXEC_ifremove  () == 1)
#define     CASE_REMOVE     'r' : case 'R' : case 'ø'
/*---(daemon)---------------*/
#define     ACT_RELOAD      'H'
#define     ACT_DAEMON      'd'
#define     ACT_VDAEMON     'D'
#define     ACT_CDAEMON     'ë'
#define     IF_DAEMON       if (yEXEC_ifdaemon  () == 1)
#define     CASE_DAEMON     'd' : case 'D' : case 'ë'
/*---(prickly daemon)-------*/
#define     ACT_PRICKLY     'p'
#define     ACT_VPRICKLY    'P'
#define     ACT_CPRICKLY    '÷'
#define     IF_PRICKLY      if (yEXEC_ifprickly () == 1)
#define     CASE_PRICKLY    'p' : case 'P' : case '÷'
/*---(normal)---------------*/
#define     ACT_NORMAL      'n'
#define     ACT_VNORMAL     'N'
#define     ACT_CNORMAL     'ô'
#define     IF_NORMAL       if (yEXEC_ifnormal  () == 1)
#define     CASE_NORMAL     'n' : case 'N' : case 'ô'
/*---(combination)----------*/
#define     IF_SILENT       if (yEXEC_ifsilent  () == 1)
#define     IF_VERBOSE      if (yEXEC_ifverbose () == 1)
#define     IF_CONFIRM      if (yEXEC_ifconfirm () == 1)
/*---(combination)----------*/
#define     IF_CCHECK       if (a_act == ACT_CCHECK)
#define     IF_VCHECK       if (a_act == ACT_VCHECK)
#define     IF_CREVIEW      if (a_act == ACT_CAUDIT  || a_act == ACT_CDAEMON || a_act == ACT_CPRICKLY || a_act == ACT_CNORMAL)
#define     IF_VREVIEW      if (a_act == ACT_VAUDIT  || a_act == ACT_VDAEMON || a_act == ACT_VPRICKLY || a_act == ACT_VNORMAL)
#define     IF_CCENTRAL     if (a_act == ACT_CCHECK  || a_act == ACT_CAUDIT  || a_act == ACT_CDAEMON  || a_act == ACT_CPRICKLY || a_act == ACT_CNORMAL)
#define     IF_VCENTRAL     if (a_act == ACT_VCHECK  || a_act == ACT_VAUDIT  || a_act == ACT_VDAEMON  || a_act == ACT_VPRICKLY || a_act == ACT_VNORMAL)
/*---(unit testing)---------*/
#define     IF_NORUN        if (yEXEC_ifnorun   () == 1)






/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char*       yEXEC_version           (void);
char        yEXEC_iam               (char a_iam , char *a_print);
char        yEXEC_mode              (char a_mode, char *a_print);


char        yEXEC_acceptable_full   (cchar a_runas, cchar *a_home, cchar *a_root, cchar *a_name, cchar *a_muser, int a_muid, char *r_fuser, int *r_fuid, char *r_fdesc, char *r_dir);
char        yEXEC_acceptable        (cchar a_runas, cchar *a_name, char *r_fuser, int *r_fuid, char *r_fdesc, char *r_dir);
char        yEXEC_central_dir       (cchar a_runas, cchar *a_name, char *a_dir, char *a_user, char *a_file);
char        yEXEC_central_full      (cchar a_runas, cchar *a_central, cchar *a_name, cchar *a_muser, int a_muid, char *r_fuser, int *r_fuid, char *r_fdesc);
char        yEXEC_central           (cchar a_runas, cchar *a_name, char *r_fuser, int *r_fuid, char *r_fdesc, char *r_dir);
char        yEXEC_act_verify        (cchar a_runas, cchar a_act, cchar *a_oneline, cchar *a_name, void *a_assimilate);
char        yEXEC_act_install       (cchar a_runas, cchar a_act, cchar *a_oneline, cchar *a_name, void *a_assimilate);
char        yEXEC_act_check         (cchar a_runas, cchar a_act, cchar *a_oneline, cchar *a_name, void *a_assimilate);
char        yEXEC_act_remove        (cchar a_runas, cchar a_act, cchar *a_oneline, cchar *a_name);
char        yEXEC_act_security      (cchar a_runas, cchar a_act, cchar *a_oneline);
char        yEXEC_act_fix           (cchar a_runas, cchar a_act, cchar *a_oneline);
char        yEXEC_act_review        (cchar a_runas, cchar a_act, cchar *a_oneline, cchar *a_muser, int a_muid, cchar *a_regex, void *a_assimilate);


/*---(arguments)-------------------------*/
char        yEXEC_args_handle       (char *a_runas, char *a_runmode, char *a_runfile, int *i, char *a_arg, char *a_next);
char        yEXEC_final             (int a_uid);
char        yEXEC_ifsilent          (void);
char        yEXEC_ifverbose         (void);
char        yEXEC_ifconfirm         (void);
char        yEXEC_ifverify          (void);
char        yEXEC_ifinstall         (void);
char        yEXEC_iflist            (void);
char        yEXEC_ifcheck           (void);
char        yEXEC_ifremove          (void);
char        yEXEC_ifaudit           (void);
char        yEXEC_iffix             (void);
char        yEXEC_ifdaemon          (void);
char        yEXEC_ifprickly         (void);
char        yEXEC_ifnormal          (void);
char        yEXEC_ifnorun           (void);


char        yEXEC_runable           (char *a_title, char *a_user, char  *a_cmd, char a_path);
int         yEXEC_quick             (char *a_cmd);
int         yEXEC_run               (char *a_title, char *a_user, char *a_cmd);
int         yEXEC_tight             (char *a_title, char *a_user, char *a_cmd);
int         yEXEC_full              (char *a_title, char *a_user, char *a_cmd, char a_shell, char a_path, char a_fork, char *a_output);
char        yEXEC_verify            (char *a_title, int a_rpid, int *a_rc2);
char        yEXEC_check             (int a_rpid);
char        yEXEC_timing            (int a_rpid, char a_strict, int a_max, int a_dur, int a_grace, int a_peers);
char        yEXEC_detail            (char a_rc, int a_rc2, char *a_desc);
char        yEXEC_find              (char *a_name, int *a_rpid);
char        yEXEC_maxname           (int a_argc, char *a_argv [], int *a_max);
char        yEXEC_rename            (char *a_mem, char *a_name, int a_max);

char        yEXEC_cpu_main          (long *a_utime, long *a_stime, long *a_idle);
char        yEXEC_cpu_proc          (int a_rpid, char a_cpu, char *a_state, long *a_utime, long *a_stime, char *a_snice);
char        yEXEC_mem_proc          (int a_rpid, char a_mem, long *a_total, long *a_true, long *a_text, long *a_data, long *a_heap, long *a_stack, long *a_kern, long *a_libs, long *a_empty, char *a_flag);
char        yEXEC_mem_proc_OLD      (int a_rpid, long *a_total, long *a_text, long *a_data, long *a_stack, long *a_heap, long *a_libs);

char        yEXEC_dur_in_sec        (char *a_text, int *a_dur);
char        yEXEC_flags             (int a_dur, int a_floor, char *a_flags, char *a_value, char *a_track, char *a_handoff, char *a_strict, char *a_min, int *a_mindur, char *a_max, int *a_maxdur, char *a_remedy);
char        yEXEC_flags_more        (int a_dur, int a_floor, char *a_flags, char *a_value, char *a_track, char *a_handoff, char *a_strict, char *a_min, int *a_mindur, char *a_max, int *a_maxdur, char *a_remedy, char *a_flex, char *a_throttle, char *a_cpu, char *a_disk, char *a_net);
char        yEXEC_flags_feedback    (char *a_terse, char *a_fancy);
char        yEXEC_controls          (void);


char        yEXEC_term              (const char *a_func, const int a_exit);

char        yEXEC_signal            (char a_tough, char a_inter, char a_child, void *a_signaler, char *a_output);
char        yEXEC_sighard           (void);
char        yEXEC_sigsoft           (void);
int         yEXEC_file_verify       (char *a_name, int n, char *a_recd);

char        yEXEC_heartbeat         (int a_rpid, long a_now, char *a_suffix, char *a_file, char *a_heartbeat);
char        yEXEC_heartbeat_check   (char *a_file, char *a_heartbeat);
char        yEXEC_userdata          (char *a_user, int *a_uid, int *a_gid, char *a_dir, char *a_shell);
char        yEXEC_whoami            (int *a_pid, int *a_ppid, int *a_uid, char *a_root, char *a_user, char a_wheel);
char        yEXEC_daemon            (int a_logger, int *a_rpid);





#endif
/*============================----end-of-source---============================*/
