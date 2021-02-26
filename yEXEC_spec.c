/*============================[[    beg-code    ]]============================*/
#include   "yEXEC.h"
#include   "yEXEC_priv.h"



/*====================------------------------------------====================*/
/*===----                       informational                          ----===*/
/*====================------------------------------------====================*/
static void      o___HEARTBEAT_______________o (void) {;}

char        s_heartbeat      [LEN_HUND];

char         /*--> write the heartbeat file ----------------------------------*/
yEXEC_heartbeat         (int a_rpid, long a_now, char *a_suffix, char *a_file, char *a_heartbeat)
{  /*---(design)-------------------------*/
   /*
    * strftime    %u   dow   1 (mo) - 7 (su)
    *                           ISO starts on sun and at 0
    *
    *             %W   woy   full weeks are 1-52, partial 0 and 53 (YES!!!)
    *                           ISO makes first partial part of last year ;(
    *
    *             both these match yLOG and ySCHED, and everywhere
    */
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   long        x_now       =    0;
   tTIME      *x_broke     = NULL;
   char        t           [LEN_TITLE] = "";
   FILE       *f           = NULL;
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_enter   (__FUNCTION__);
   /*---(set time)-----------------------*/
   if (a_now > 0)  x_now  = a_now;
   else            x_now  = time (NULL);
   DEBUG_YEXEC  yLOG_value   ("x_now"     , x_now);
   /*---(break it down)------------------*/
   x_broke   = localtime (&x_now);
   /*---(heartbeat)----------------------*/
   strftime (t, LEN_TITLE, "%y.%m.%d.%H.%M.%S.%u.%W.%j", x_broke);
   DEBUG_YEXEC  yLOG_info    ("t"         , t);
   sprintf  (s_heartbeat, "%-26.26s  %-10d  %6d", t, x_now, a_rpid);
   if (a_suffix != NULL) {
      strlcat (s_heartbeat, "  "    , LEN_HUND);
      strlcat (s_heartbeat, a_suffix, LEN_HUND);
   }
   DEBUG_YEXEC  yLOG_info    ("heartbeat" , s_heartbeat);
   if (a_heartbeat != NULL)  strlcpy (a_heartbeat, s_heartbeat, LEN_HUND);
   DEBUG_YEXEC  yLOG_point   ("a_file"    , a_file);
   /*---(write file)---------------------*/
   if (a_file != NULL) {
      /*---(open)------------------------*/
      DEBUG_YEXEC  yLOG_info    ("a_file"    , a_file);
      f = fopen (a_file, "wt");
      DEBUG_YEXEC  yLOG_point   ("f"         , f);
      --rce;  if (rc < 0) {
         DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      /*---(write)-----------------------*/
      fputs (s_heartbeat, f);
      /*---(close)-----------------------*/
      rc = fclose (f);
      DEBUG_YEXEC  yLOG_value   ("close"     , rc);
      --rce;  if (rc < 0) {
         DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(complete------------------------*/
   DEBUG_YEXEC  yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> read the last heartbeat -----------------------------------*/
yexec__unit_heartbeat   (char *a_file, char *a_heartbeat)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   FILE       *f           = NULL;
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_enter   (__FUNCTION__);
   /*---(open)---------------------------*/
   DEBUG_YEXEC  yLOG_info    ("a_file"    , a_file);
   f = fopen (a_file, "rt");
   DEBUG_YEXEC  yLOG_point   ("f"         , f);
   --rce;  if (rc < 0) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write)--------------------------*/
   fgets (a_heartbeat, LEN_HUND, f);
   DEBUG_YEXEC  yLOG_info    ("heartbeat" , a_heartbeat);
   /*---(close)--------------------------*/
   rc = fclose (f);
   DEBUG_YEXEC  yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YEXEC  yLOG_exit    (__FUNCTION__);
   return 0;
}




/*====================------------------------------------====================*/
/*===----                       informational                          ----===*/
/*====================------------------------------------====================*/
static void      o___INFO____________________o (void) {;}

char         /*--> verify a user name ----------------------------------------*/
yEXEC_userdata          (char *a_user, int *a_uid, int *a_gid, char *a_dir, char *a_shell)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         x_len       =    0;
   int         i           =    0;
   char       *x_valid     = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
   tPASSWD    *x_pass      = NULL;
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_enter   (__FUNCTION__);
   /*---(default save backs)-------------*/
   if (a_uid   != NULL)  *a_uid = -1;
   if (a_gid   != NULL)  *a_gid = -1;
   if (a_dir   != NULL)  strcpy (a_dir, "");
   if (a_shell != NULL)  strcpy (a_shell, "");
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
   if (a_uid   != NULL)  *a_uid = x_pass->pw_uid;
   DEBUG_YEXEC  yLOG_value   ("gid"       , x_pass->pw_gid);
   if (a_gid   != NULL)  *a_gid = x_pass->pw_gid;
   DEBUG_YEXEC  yLOG_info    ("dir"       , x_pass->pw_dir);
   if (a_dir   != NULL)  strcpy (a_dir, x_pass->pw_dir);
   DEBUG_YEXEC  yLOG_info    ("shell"     , x_pass->pw_shell);
   if (a_shell != NULL)  strcpy (a_shell, x_pass->pw_shell);
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
   char        x_root      =  '-';
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
   strlcpy (a_mem, a_name, a_max - 1);
   /*---(complete)-----------------------*/
   DEBUG_YEXEC  yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      unit test accessor                      ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char*            /*--> unit test accessor ------------------------------*/
yexec_spec__unit        (char *a_question, char *a_text)
{ 
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   char        x_heartbeat [LEN_HUND];
   /*---(prepare)------------------------*/
   strlcpy  (unit_answer, "SPEC             : question not understood", LEN_RECD);
   /*---(crontab name)-------------------*/
   if      (strcmp (a_question, "heartbeat"     )  == 0) {
      snprintf (unit_answer, LEN_RECD, "SPEC heartbeat   : %2d[%s]", strlen (s_heartbeat), s_heartbeat);
   }
   else if (strcmp (a_question, "lastbeat"      )  == 0) {
      yexec__unit_heartbeat (a_text, x_heartbeat);
      snprintf (unit_answer, LEN_RECD, "SPEC lastbeat    : %2d[%s]", strlen (x_heartbeat), x_heartbeat);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}



