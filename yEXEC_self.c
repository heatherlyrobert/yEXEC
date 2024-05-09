/*============================[[    beg-code    ]]============================*/
#include   "yEXEC.h"
#include   "yEXEC_priv.h"



/*====================------------------------------------====================*/
/*===----                        self knowledge                        ----===*/
/*====================------------------------------------====================*/
static void      o___MYSELF_____________o (void) {;}

char
yEXEC_whoami            (int *r_pid, int *r_ppid, int *r_uid, int *r_euid, char *r_root, char r_user [LEN_USER], char a_wheel, int *r_gid, int *r_egid, char r_group [LEN_USER])
{
   /*---(locals)-------------------------*/
   char        rce         =  -10;
   int         rc          =    0;
   int         x_euid      =    0;
   tPASSWD    *x_pass      = NULL;
   char        x_root      =  '-';
   int         x_egid      =    0;
   tGROUP     *x_group     = NULL;
   int         x_len       =    0;
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_enter   (__FUNCTION__);
   /*---(log pid info)---------------------*/
   if (r_pid  != NULL) {
      *r_pid  = getpid();
      DEBUG_YEXEC  yLOG_value   ("r_pid"     , *r_pid);
   }
   if (r_ppid != NULL) {
      *r_ppid = getppid();
      DEBUG_YEXEC  yLOG_value   ("r_ppid"    , *r_ppid);
   }
   /*---(get real uid)-------------------*/
   x_euid   = geteuid ();
   DEBUG_YEXEC  yLOG_value   ("x_euid"    , x_euid);
   /*---(pull user name)-----------------*/
   x_pass    = getpwuid (x_euid);
   DEBUG_YEXEC  yLOG_point   ("x_pass"    , x_pass);
   --rce;  if (x_pass == NULL) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_info    ("->name"    , x_pass->pw_name);
   /*---(save user info)-----------------*/
   DEBUG_YEXEC  yLOG_note    ("save uid and user name");
   if (r_uid   != NULL)   *r_uid   = getuid ();
   if (r_euid  != NULL)   *r_euid  = x_euid;
   if (r_user  != NULL) {
      x_len = strlen (x_pass->pw_name);
      strlcpy (r_user, x_pass->pw_name, LEN_USER);
      if (x_len >= LEN_USER)  r_user [LEN_USER - 1] = '>';
   }
   /*---(check for root user)--------------*/
   if (x_euid == 0)   x_root = 'y';
   DEBUG_YEXEC  yLOG_char    ("x_root"    , x_root);
   /*---(change uid/permissions)-----------*/
   DEBUG_YEXEC  yLOG_char    ("a_wheel"   , a_wheel);
   --rce;  if (x_root != 'y' && a_wheel == 'y') {
      DEBUG_YEXEC  yLOG_note    ("attempt to gain root (legally;)");
      rc = setuid (0);
      DEBUG_YEXEC  yLOG_value   ("setuid"    , rc);
      if (rc != 0) {
         DEBUG_YEXEC  yLOG_note    ("could not gain root access");
      } else {
         DEBUG_YEXEC  yLOG_note    ("successfully gained root (and put back to euid)");
         x_root = 'y';
      }
   }
   /*---(save user info)-----------------*/
   if (r_root != NULL)   *r_root = x_root;
   /*---(log group info)-------------------*/
   x_egid  = getegid ();
   DEBUG_YEXEC  yLOG_value   ("x_egid"    , x_egid);
   x_group = getgrgid (x_egid);
   DEBUG_YEXEC  yLOG_point   ("x_group"   , x_group);
   --rce;  if (x_group == NULL) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_info    ("->name"    , x_group->gr_name);
   if (r_gid   != NULL)   *r_gid  = getgid ();
   if (r_egid  != NULL)   *r_egid = x_egid;
   if (r_group != NULL) {
      x_len = strlen (x_group->gr_name);
      strlcpy (r_group, x_group->gr_name, LEN_USER);
      if (x_len >= LEN_USER)  r_group [LEN_USER - 1] = '>';
   }
   /*---(complete)-----------------------*/
   DEBUG_YEXEC  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         daemonizing                          ----===*/
/*====================------------------------------------====================*/
static void      o___DAEMON________o (void) {;}

int
yexec__foad        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   int         x_tries     =    0;
   int         x_rpid      =    0;
   int         x_ppid      =    0;
   int         x_status    =    0;
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   x_rpid  = getpid();
   DEBUG_YEXEC  yLOG_value   ("x_rpid"    , x_rpid);
   if (x_rpid <= 1) {
      DEBUG_YEXEC  yLOG_note    ("can not fork process one, are you crazy?");
      DEBUG_YEXEC  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(create a fork)------------------*/
   while (1) {
      /*---(check status)----------------*/
      x_ppid  = getppid ();
      DEBUG_YEXEC  yLOG_value   ("x_ppid"    , x_ppid);
      if (x_ppid == 1) {
         DEBUG_YEXEC  yLOG_info    ("ppid"       , "owned by init, success");
         break;
      }
      DEBUG_YEXEC  yLOG_note    ("not owned by init, need to fork");
      /*---(fork)------------------------*/
      DEBUG_YEXEC  yLOG_value  ("x_tries"   , x_tries);
      if (x_tries >= 10)  break;
      /*---(fork)------------------------*/
      rc = fork();
      DEBUG_YEXEC  yLOG_value  ("fork"      , rc);
      /*---(try again on fail)-----------*/
      if (rc < 0) {
         DEBUG_YEXEC  yLOG_info   ("FAILED"    , "creation of child FAILED, try again");
         continue;
      }
      /*---(exit if parent)--------------*/
      if (rc > 0) {         /* parent process      */
         DEBUG_YEXEC  yLOG_info   ("PARENT"    , "in parent, exiting parent");
         _exit (0);
      } else {
         DEBUG_YEXEC  yLOG_info   ("child"     , "quick wait to allow parent to die");
         /*> sleep (0.25);                                                            <*/
         sleep (1);
      }
      /*---(prepare for next)------------*/
      x_rpid  = getpid();
      DEBUG_YEXEC  yLOG_value   ("x_rpid"    , x_rpid);
      ++x_tries;
      /*---(done)------------------------*/
   }
   /*---(check for failure)--------------*/
   --rce;  if (x_tries >= 5) {
      DEBUG_YEXEC  yLOG_note    ("tried five times, could not do it");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YEXEC  yLOG_exit    (__FUNCTION__);
   return 0;
}

char             /* [------] daemonize the program ---------------------------*/
yEXEC_daemon       (int a_logger, int *a_rpid)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   int         x_rpid      =    0;
   int         x_ppid      =    0;
   int         i           =    0;
   int         fd          =    0;
   int         x_sid       =    0;
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_enter   (__FUNCTION__);
   /*---(check current pid)--------------*/
   x_rpid  = getpid();
   DEBUG_YEXEC  yLOG_value   ("x_rpid"    , x_rpid);
   /*---(fork off and die)---------------*/
   rc = yexec__foad ();
   DEBUG_YEXEC  yLOG_value   ("foad"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(record process info)---------*/
   x_rpid  = getpid();
   DEBUG_YEXEC  yLOG_value   ("new pid"    , x_rpid);
   x_ppid  = getppid();
   DEBUG_YEXEC  yLOG_value   ("new ppid"   , x_ppid);
   if (x_rpid == 1)   DEBUG_YEXEC  yLOG_note    ("semi-daemon (process one)");
   else               DEBUG_YEXEC  yLOG_note    ("full on daemon");
   /*---(fix the umask)---------------*/
   DEBUG_YEXEC  yLOG_info    ("umask"     , "reset the default file permissions");
   umask (0);
   /*---(close off all descriptors)---*/
   DEBUG_YEXEC  yLOG_info    ("fds"       , "close all inherited file descriptors");
   DEBUG_YEXEC  yLOG_value   ("a_logger"  , a_logger);
   for (i = 3; i < 256; ++i) {
      if (i == a_logger)          continue;
      close (i);
   }
   /*---(std fds to the bitbucket)---*/
   DEBUG_YEXEC  yLOG_info   ("std fds"   , "redirect stdin, stdout, and stderr to /dev/null");
   fd = open ("/dev/null", O_RDWR);
   DEBUG_YEXEC  yLOG_value   ("fd"         , fd);
   --rce;  if (fd < 0) {
      DEBUG_YEXEC  yLOG_info    ("fds"       , "creation of safe fd FAILED");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   dup2 (fd, 0);
   dup2 (fd, 1);
   dup2 (fd, 2);
   close (fd);
   /*---(obtain a new process group)--*/
   /*> if (x_rpid > 1) {                                                              <*/
   DEBUG_YEXEC  yLOG_info   ("session"   , "create a new process/session");
   x_sid = setsid();
   DEBUG_YEXEC  yLOG_value   ("x_sid"      , x_sid);
   /*> --rce;  if (x_sid < 0) {                                                       <* 
    *>    DEBUG_YEXEC  yLOG_info   ("sid"       , "creation FAILED");                 <* 
    *>    DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);                              <* 
    *>    return rce;                                                                 <* 
    *> }                                                                              <*/
   /*> }                                                                              <*/
   /*---(save rpid)----------------------*/
   if (a_rpid != NULL)  *a_rpid = x_rpid;
   /*---(complete)-----------------------*/
   DEBUG_YEXEC  yLOG_exit  (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                   process name change                        ----===*/
/*====================------------------------------------====================*/
static void      o___NAMING________o (void) {;}

char
yEXEC_maxname           (int a_argc, char *a_argv [], int *a_max)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   int         x_max       =    0;
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_senter  (__FUNCTION__);
   /*---(get max)------------------------*/
   for (i = 0; i < a_argc; ++i) {
      DEBUG_YEXEC  yLOG_sint    (i);
      DEBUG_YEXEC  yLOG_snote   (a_argv [i]);
      DEBUG_YEXEC  yLOG_sint    (strlen (a_argv [i]));
      x_max += strlen (a_argv [i]) + 1;
      DEBUG_YEXEC  yLOG_sint    (x_max);
   }
   DEBUG_YEXEC  yLOG_sint    (x_max);
   if (a_max != NULL)  *a_max = x_max;
   /*---(complete)-----------------------*/
   DEBUG_YEXEC  yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
yEXEC_rename            (char *a_mem, char *a_name, int a_max)
{
   /*---(locals)-----------+-----+-----+-*/
   int         x_len       =    0;
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YEXEC  yLOG_spoint  (a_name);
   if (a_name == NULL) {
      DEBUG_YEXEC  yLOG_sexit   (__FUNCTION__);
   }
   DEBUG_YEXEC  yLOG_snote   (a_name);
   x_len = strlen (a_name);
   DEBUG_YEXEC  yLOG_sint    (x_len);
   /*---(save new)-----------------------*/
   memset (a_mem, 0, a_max);
   ystrlcpy (a_mem, a_name, a_max - 1);
   /*---(complete)-----------------------*/
   DEBUG_YEXEC  yLOG_sexit   (__FUNCTION__);
   return 0;
}


