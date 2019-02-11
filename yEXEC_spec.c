/*============================[[    beg-code    ]]============================*/
#include   "yEXEC.h"
#include   "yEXEC_priv.h"



char         /*--> verify a user name ----------------------------------------*/
yEXEC_user              (char *a_user, int *a_uid, int *a_gid, char *a_dir)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         x_len       =    0;
   int         i           =    0;
   char       *x_valid     = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
   tPASSWD    *x_pass      = NULL;
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   if (a_uid != NULL)  *a_uid = -1;
   if (a_gid != NULL)  *a_gid = -1;
   strcpy (a_dir, "");
   /*---(defense)------------------------*/
   DEBUG_YEXEC  yLOG_point   ("a_user"    , a_user);
   --rce;  if (a_user == NULL) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_info    ("a_user"    , a_user);
   /*---(check length)-------------------*/
   x_len = strlen (a_user);
   DEBUG_YEXEC  yLOG_value   ("x_len"     , x_len);
   DEBUG_YEXEC  yLOG_value   ("min"       , 3);
   --rce;  if (x_len <=  3) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_value   ("max"       , LEN_USER);
   --rce;  if (x_len >= LEN_USER) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check letters)------------------*/
   --rce;  for (i = 0; i < x_len; ++i) {
      if (strchr (x_valid, a_user [i]) != NULL)  continue;
      DEBUG_YEXEC  yLOG_value   ("position"  , i);
      DEBUG_YEXEC  yLOG_char    ("illegal"   , a_user [i]);
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check validity)-----------------*/
   x_pass    = getpwnam (a_user);
   DEBUG_YEXEC  yLOG_point   ("x_pass"    , x_pass);
   --rce;  if (x_pass == NULL) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save)---------------------------*/
   DEBUG_YEXEC  yLOG_value   ("uid"       , x_pass->pw_uid);
   if (a_uid != NULL)  *a_uid = x_pass->pw_uid;
   DEBUG_YEXEC  yLOG_value   ("gid"       , x_pass->pw_gid);
   if (a_gid != NULL)  *a_gid = x_pass->pw_gid;
   DEBUG_YEXEC  yLOG_info    ("dir"       , x_pass->pw_dir);
   strcpy (a_dir, x_pass->pw_dir);
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yEXEC_whoami            (int *a_pid, int *a_ppid, int *a_uid, char *a_root, char *a_user, char a_wheel)
{
   /*---(locals)-------------------------*/
   char        rce         =  -10;
   int         rc          =    0;
   int         x_uid       =    0;
   tPASSWD    *x_pass      = NULL;
   int         x_len       =    0;
   char        x_root      =  'n';
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_enter   (__FUNCTION__);
   /*---(get real uid)-------------------*/
   x_uid    = getuid ();
   DEBUG_YEXEC  yLOG_value   ("x_uid"     , x_uid);
   /*---(pull user name)-----------------*/
   x_pass    = getpwuid (x_uid);
   DEBUG_YEXEC  yLOG_point   ("x_pass"    , x_pass);
   --rce;  if (x_pass == NULL) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_info    ("->name"    , x_pass->pw_name);
   /*---(check user name)----------------*/
   x_len      = strlen (x_pass->pw_name);
   DEBUG_YEXEC  yLOG_value   ("x_len"     , x_len);
   --rce;  if (x_len < 1) {
      DEBUG_YEXEC  yLOG_note    ("user name is empty");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (x_len > 20) {
      DEBUG_YEXEC  yLOG_note    ("user name is too long (> 20 chars)");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save user info)-----------------*/
   DEBUG_YEXEC  yLOG_note    ("save uid and user name");
   if (a_uid  != NULL)   *a_uid  = x_uid;
   strcpy (a_user, x_pass->pw_name);
   /*---(check for root user)--------------*/
   if (x_uid == 0)   x_root = 'y';
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
         DEBUG_YEXEC  yLOG_note    ("successfully gained root");
         x_root = 'y';
      }
   }
   /*---(save user info)-----------------*/
   if (a_root != NULL)   *a_root = x_root;
   /*---(log pid info)---------------------*/
   if (a_pid  != NULL) {
      *a_pid  = getpid();
      DEBUG_YEXEC  yLOG_value   ("pid"       , *a_pid);
   }
   if (a_ppid != NULL) {
      *a_ppid = getppid();
      DEBUG_YEXEC  yLOG_value   ("ppid"      , *a_ppid);
   }
   /*---(complete)-----------------------*/
   DEBUG_YEXEC  yLOG_exit    (__FUNCTION__);
   return 0;
}

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
      DEBUG_YEXEC  yLOG_note    ("can not for process one, are you crazy?");
      DEBUG_YEXEC  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(create a fork)------------------*/
   while (1) {
      /*---(fork)------------------------*/
      DEBUG_YEXEC  yLOG_value  ("x_tries"   , x_tries);
      if (x_tries >= 5)  break;
      /*---(fork)------------------------*/
      x_rpid = fork();
      DEBUG_YEXEC  yLOG_value  ("x_rpid"    , x_rpid);
      /*---(try again on fail)-----------*/
      if (x_rpid < 0) {
         DEBUG_YEXEC  yLOG_info   ("FAILED"    , "creation of child FAILED, try again");
         continue;
      }
      /*---(exit if parent)--------------*/
      if (x_rpid > 0) {         /* parent process      */
         DEBUG_YEXEC  yLOG_info   ("PARENT"    , "in parent, exiting parent");
         exit (0);
      }
      /*---(wait for parent to die)------*/
      rc = wait4 (x_rpid, &x_status, 0, NULL);
      DEBUG_YEXEC  yLOG_value   ("wait4_rc"  , rc);
      if        (rc <  0) {
         DEBUG_YEXEC  yLOG_note    ("wait4 returned an error");
      } else if (rc == 0) {
         DEBUG_YEXEC  yLOG_note    ("wait4 returned an zero");
      } else if (rc == x_rpid) {
         DEBUG_YEXEC  yLOG_note    ("wait4 found parent has changed status");
      } else {
         DEBUG_YEXEC  yLOG_note    ("wait4 returned unknown error");
      }
      /*---(check for success)-----------*/
      x_ppid  = getppid();
      DEBUG_YEXEC  yLOG_value  ("x_ppid"    , x_ppid);
      if (x_ppid == 1) {
         DEBUG_YEXEC  yLOG_info  ("ppid"       , "owned by init, success");
         break;
      }
      DEBUG_YEXEC  yLOG_info  ("FAILED"     , "not owned by init, failure");
      /*---(prepare for next)------------*/
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
   DEBUG_YEXEC  yLOG_info   ("fds"       , "close all inherited file descriptors");
   for (i = 0; i < 256; ++i) {
      if (i == a_logger)          continue;
      if (x_rpid == 1 && i == 1)  continue; /* semi-daemons need stdout     */
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
   dup2(fd, 0);
   dup2(fd, 2);
   if (x_rpid != 1)  dup2(fd, 1);
   /*---(obtain a new process group)--*/
   if (x_rpid > 1) {
      DEBUG_YEXEC  yLOG_info   ("session"   , "create a new process/session");
      x_sid = setsid();
      DEBUG_YEXEC  yLOG_value   ("x_sid"      , x_sid);
      --rce;  if (x_sid < 0) {
         DEBUG_YEXEC  yLOG_info   ("sid"       , "creation FAILED");
         DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_YEXEC  yLOG_value ("new sid",  x_sid);
   }
   /*---(save rpid)----------------------*/
   if (a_rpid != NULL)  *a_rpid = x_rpid;
   /*---(complete)-----------------------*/
   DEBUG_YEXEC  yLOG_exit  (__FUNCTION__);
   return 0;
}

