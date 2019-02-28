/*============================[[    beg-code    ]]============================*/
#include   "yEXEC.h"
#include   "yEXEC_priv.h"



/*====================------------------------------------====================*/
/*===----                       informational                          ----===*/
/*====================------------------------------------====================*/
static void      o___INFO____________________o (void) {;}

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
   if (a_user != NULL)   strcpy (a_user, x_pass->pw_name);
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



/*====================------------------------------------====================*/
/*===----                         daemonizing                          ----===*/
/*====================------------------------------------====================*/
static void      o___DAEMON__________________o (void) {;}

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
         sleep (0.25);
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
   for (i = 0; i < 256; ++i) {
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
/*===----                      consoles and ttys                       ----===*/
/*====================------------------------------------====================*/
static void      o___TTYS____________________o (void) {;}

char
yEXEC_tty_close         (int *a_fd)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YEXEC  yLOG_point   ("a_fd"      , a_fd);
   --rce;  if (a_fd == NULL) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_value   ("*a_fd"     , *a_fd);
   --rce;  if (*a_fd < 0) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(close term)---------------------*/
   rc = close (*a_fd);
   DEBUG_YEXEC  yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(ground pointer)-----------------*/
   *a_fd = -1;
   /*---(complete)-----------------------*/
   DEBUG_YEXEC  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yEXEC_tty_open          (char *a_dev, int *a_fd, char a_std, char a_keep)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tTERMIOS    x_tty;
   int         x_fd        =   -1;
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YEXEC  yLOG_point   ("a_dev"     , a_dev);
   --rce;  if (a_dev == NULL) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_info    ("a_dev"     , a_dev);
   /*---(environment)--------------------*/
   rc = putenv ("TERM=Eterm");
   DEBUG_YEXEC  yLOG_value   ("putenv"    , rc);
   --rce;  if (rc != 0) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(open term)----------------------*/
   x_fd = open (a_dev, O_RDWR | O_NOCTTY | O_NONBLOCK);
   DEBUG_YEXEC  yLOG_value   ("open"      , x_fd);
   --rce;  if (x_fd < 0) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get attributes)-----------------*/
   rc = tcgetattr (x_fd, &x_tty);
   DEBUG_YEXEC  yLOG_value   ("getattr"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(change attributes)--------------*/
   x_tty.c_cflag        &= CBAUD|CBAUDEX|CSIZE|CSTOPB|PARENB|PARODD;
   x_tty.c_cflag        |= HUPCL|CLOCAL|CREAD;
   x_tty.c_cc[VINTR]	    = CINTR;
   x_tty.c_cc[VQUIT]	    = CQUIT;
   x_tty.c_cc[VERASE]    = CERASE; /* ASCII DEL (0177) */
   x_tty.c_cc[VKILL]	    = CKILL;
   x_tty.c_cc[VEOF]	    = CEOF;
   x_tty.c_cc[VTIME]	    = 0;
   x_tty.c_cc[VMIN]	    = 1;
   x_tty.c_cc[VSWTC]	    = _POSIX_VDISABLE;
   x_tty.c_cc[VSTART]    = CSTART;
   x_tty.c_cc[VSTOP]	    = CSTOP;
   x_tty.c_cc[VSUSP]	    = CSUSP;
   x_tty.c_cc[VEOL]	    = _POSIX_VDISABLE;
   x_tty.c_cc[VREPRINT]  = CREPRINT;
   x_tty.c_cc[VDISCARD]  = CDISCARD;
   x_tty.c_cc[VWERASE]   = CWERASE;
   x_tty.c_cc[VLNEXT]    = CLNEXT;
   x_tty.c_cc[VEOL2]	    = _POSIX_VDISABLE;
   x_tty.c_iflag         = IGNPAR|ICRNL|IXON|IXANY;
   x_tty.c_oflag         = OPOST|ONLCR;
   x_tty.c_lflag         = ISIG|ICANON|ECHO|ECHOCTL|ECHOPRT|ECHOKE;
   x_tty.c_iflag        |=  IGNBRK;
   x_tty.c_iflag        &= ~(BRKINT|INLCR|IGNCR|IXON);
   x_tty.c_oflag        &= ~(OCRNL|ONLRET);
   /*---(set attributes)-----------------*/
   rc = tcsetattr (x_fd, TCSANOW, &x_tty);
   DEBUG_YEXEC  yLOG_value   ("setattr"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(flush data)---------------------*/
   rc = tcflush (x_fd, TCIOFLUSH);
   DEBUG_YEXEC  yLOG_value   ("flush"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(duplicate)----------------------*/
   DEBUG_YEXEC  yLOG_char    ("a_std"     , a_std);
   switch (a_std) {
   case YEXEC_STDALL :
      DEBUG_YEXEC  yLOG_note    ("connecting stdin, stdout, and stderr");
      dup2 (x_fd, 0);
      dup2 (x_fd, 1);
      dup2 (x_fd, 2);
      break;
   case YEXEC_STDOUT :
      DEBUG_YEXEC  yLOG_note    ("connecting stdout only");
      dup2 (x_fd, 1);
      break;
   }
   /*---(close term)---------------------*/
   if (a_keep != YEXEC_YES) {
      rc = yEXEC_tty_close (&x_fd);
      DEBUG_YEXEC  yLOG_value   ("close"     , rc);
      --rce;  if (x_fd < 0) {
         DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(save)---------------------------*/
   if (a_fd != NULL)  *a_fd = x_fd;
   /*---(complete)-----------------------*/
   DEBUG_YEXEC  yLOG_exit    (__FUNCTION__);
   return 0;
}



