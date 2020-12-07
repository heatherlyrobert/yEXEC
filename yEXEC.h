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
#define     YEXEC_YES      'y'
#define     YEXEC_ALL      'a'

#define     YEXEC_BASH     'b'
#define     YEXEC_DASH     'd'

#define     YEXEC_TIGHT    't'
#define     YEXEC_FULL     'f'

#define     YEXEC_FORK     'y'
#define     YEXEC_NOFORK   '-'

#define     YEXEC_ALREADY  'A'
#define     YEXEC_NOSUCH   '#'
#define     YEXEC_RUNNING  'r'
#define     YEXEC_KILLED   'T'
#define     YEXEC_SEGV     'M'
#define     YEXEC_QUIT     'Q'
#define     YEXEC_USER     'H'
#define     YEXEC_LIMIT    'L'
#define     YEXEC_NOTREAL  '?'
#define     YEXEC_DIED     'd'
#define     YEXEC_ERROR    'e'
#define     YEXEC_NORMAL   'n'
#define     YEXEC_FAILURE  'F'
#define     YEXEC_WARNING  'w'

#define     YEXEC_UNIT     "/tmp/yEXEC_unit.execution_feedback"

#define     YEXEC_STDALL   'a'
#define     YEXEC_STDOUT   'o'
#define     YEXEC_BLIND    '-'


/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char*       yEXEC_version           (void);

int         yEXEC_args              (char *a_src);
char        yEXEC_runable           (char *a_title, char *a_user, char  *a_cmd, char a_path);
int         yEXEC_run               (char *a_title, char *a_user, char *a_cmd, char a_shell, char a_path, char a_fork, char *a_output);
char        yEXEC_check             (char *a_title, int a_rpid, int *rc);
char        yEXEC_find              (char *a_name, int *a_rpid);
char        yEXEC_maxname           (int a_argc, char *a_argv [], int *a_max);
char        yEXEC_rename            (char *a_mem, char *a_name, int a_max);

char        yEXEC_term              (const char *a_func, const int a_exit);

char        yEXEC_signal            (char a_bulletproof, char a_interactive, char a_children, void *a_signaler, char *a_output);
int         yEXEC_file_verify       (char *a_name, int n, char *a_recd);

char        yEXEC_heartbeat         (int a_rpid, long a_now, char *a_suffix, char *a_file, char *a_heartbeat);
char        yEXEC_user              (char *a_user, int *a_uid, int *a_gid, char *a_dir);
char        yEXEC_whoami            (int *a_pid, int *a_ppid, int *a_uid, char *a_root, char *a_user, char a_wheel);
char        yEXEC_daemon            (int a_logger, int *a_rpid);


char        yEXEC_tty_valid         (char *a_dev);
char        yEXEC_tty_open          (char *a_dev, int *a_fd, char a_std, char a_keep);
char        yEXEC_tty_close         (int *a_fd);
char        yEXEC_tty_noecho        (int a_fd);
char        yEXEC_tty_clear         (int a_fd);
char        yEXEC_tty_user          (char *a_user, char a_rotate, char *a_true);
char        yEXEC_tty_password      (char *a_user, char *a_pass);

char*       yEXEC_challenge         (int a_seed, char *a_challenge);
char        yEXEC_response          (char *a_response, char *a_phase, char *a_judgement, char *a_position, char *a_user);



#endif
/*============================----end-of-source---============================*/
