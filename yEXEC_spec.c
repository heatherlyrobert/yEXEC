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

char             /* [------] daemonize the program ---------------------------*/
yEXEC_daemon       (int a_logger, int *a_rpid)
{
   DEBUG_YEXEC  yLOG_enter (__FUNCTION__);
   /*---(locals)-----------+-----------+-*/
   int         i           = 0;             /* loop iterator                  */
   int         fd          = 0;             /* file descriptor                */
   int         status      = 0;
   int         rpid        = 0;
   int         ppid        = 0;
   int         sid         = 0;
   int         count       = 0;             /* count of attempts to FOAD      */
   int         rc          = 0;
   /*---(fork off and die)---------------*/
   while (count < 5) {
      DEBUG_YEXEC  yLOG_info   ("foad"      , "fork off and die");
      DEBUG_YEXEC  yLOG_value  ("attempt"   , count);
      rpid = fork();
      if (rpid < 0) {         /* error               */
         DEBUG_YEXEC  yLOG_info   ("fork"      , "creation of child FAILED");
         yEXEC_term  (__FUNCTION__, 1);
      }
      if (rpid > 0) {         /* parent process      */
         DEBUG_YEXEC  yLOG_info   ("PARENT"    , "exiting parent");
         exit(0);
      }
      /*---(wait for parent to die)------*/
      rc = wait4 (rpid, &status, 0, NULL);
      DEBUG_YEXEC  yLOG_value   ("wait4_rc"  , rc);
      if        (rc <  0) {
         DEBUG_YEXEC  yLOG_note    ("wait4 returned an error");
      } else if (rc == 0) {
         DEBUG_YEXEC  yLOG_note    ("wait4 returned an zero");
      } else if (rc == rpid) {
         DEBUG_YEXEC  yLOG_note    ("wait4 found parent has changed status");
      } else {
         DEBUG_YEXEC  yLOG_note    ("wait4 returned unknown error");
      }
      /*---(fix the umask)---------------*/
      DEBUG_YEXEC  yLOG_info    ("umask"     , "reset the default file permissions");
      umask(0);
      /*---(close off all descriptors)---*/
      DEBUG_YEXEC  yLOG_info   ("fds"       , "close all inherited file descriptors");
      for (i = 0; i < 256; ++i) {
         if (i == a_logger) continue;
         close(i);
      }
      /*---(std fds to the bitbucket)---*/
      DEBUG_YEXEC  yLOG_info   ("std fds"   , "redirect stdin, stdout, and stderr to /dev/null");
      fd = open("/dev/null", O_RDWR);
      if (fd < 0) {
         DEBUG_YEXEC  yLOG_info   ("fds"       , "creation of safe fd FAILED");
         yEXEC_term  (__FUNCTION__, 2);
      }
      dup2(fd, 0);
      dup2(fd, 1);
      dup2(fd, 2);
      /*---(obtain a new process group)--*/
      DEBUG_YEXEC  yLOG_info   ("session"   , "create a new process/session");
      sid = setsid();
      if (sid < 0) {
         DEBUG_YEXEC  yLOG_info   ("sid"       , "creation FAILED");
         yEXEC_term  (__FUNCTION__, 3);
      }
      DEBUG_YEXEC  yLOG_value ("new sid",  sid);
      /*---(record process info)---------*/
      rpid  = getpid();
      ppid  = getppid();
      DEBUG_YEXEC  yLOG_value ("new pid"    , rpid);
      DEBUG_YEXEC  yLOG_value ("new ppid"   , ppid);
      /*---(check for success)-----------*/
      if (ppid == 1) {
         DEBUG_YEXEC  yLOG_info  ("ppid"       , "owned by init, success");
         break;
      }
      DEBUG_YEXEC  yLOG_info  ("FAIL"       , "not owned by init, failure");
      /*---(done)------------------------*/
   }
   /*---(final message)------------------*/
   if (ppid != 1) {
      DEBUG_YEXEC  yLOG_info  ("FAIL"       , "too many attempts");
   }
   /*---(complete)-----------------------*/
   if (a_rpid != NULL)  *a_rpid = rpid;
   DEBUG_YEXEC  yLOG_exit  (__FUNCTION__);
   return 0;
}

