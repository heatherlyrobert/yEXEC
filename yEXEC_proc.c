/*============================[[    beg-code    ]]============================*/
#include   "yEXEC.h"
#include   "yEXEC_priv.h"


/*
 *  metis § wn8-- § permission and log issues written to console/syslog
 *  metis § wn1-- § do not fail if exec feedback can not be written
 *  metis § dw2-- § change importance to match metis codes
 *  metis § ww1-- § add metis duration code to duration values
 *
 *
 *
 */

#define     MAX_ARGV    20


/*---(arguments)-------------------------*/
char        s_arg       [LEN_RECD];
char        s_argw      [LEN_RECD];
char        s_argf      [LEN_RECD];
char       *s_argv      [MAX_ARGV];
int         s_argc      = 0;
/*---(environment)-----------------------*/
char        s_env       [LEN_RECD];
char        s_envw      [LEN_RECD];
char        s_envf      [LEN_RECD];
char       *s_envp      [MAX_ARGV];
int         s_envc      = 0;

int         s_uid       =    0;
int         s_gid       =    0;
char        s_home      [LEN_PATH];
char        s_output    [LEN_PATH];

char       *s_dash      = "/bin/dash";
char       *s_bash      = "/bin/bash";
char       *s_shell     = NULL;

char       *s_tight     = "/sbin:/bin:/usr/sbin:/usr/bin";
char       *s_full      = "/sbin:/bin:/usr/sbin:/usr/bin:/opt/sbin:/opt/bin:/usr/local/sbin:/usr/local/bin";
char       *s_path      = NULL;

char        s_direct    = '-';
char        s_symlink   = '-';



/*====================------------------------------------====================*/
/*===----                             parsing                          ----===*/
/*====================------------------------------------====================*/
static void      o___BASICS__________________o (void) {;}

char
yexec_unpoint           (char *a_ptr [])
{
   /*---(locals)-------------------------*/
   char        rce         =  -10;
   int         i           =    0;
   int         j           =    0;
   char       *x_cur       = NULL;
   for (i = 0; i < LEN_LABEL; ++i) {
      x_cur = a_ptr [i];
      printf ("%02d) %-10.10p   ", i, a_ptr [i]);
      if (a_ptr [i] == NULL)  {
         printf ("--[]\n");
         continue;
      }
      printf ("%02d[%s]\n", strlen (x_cur), x_cur);
      /*> j = 0;                                                                      <* 
       *> while (x_cur + j++ != '\0') {                                               <* 
       *>    printf ("%c", x_cur + j);                                                <* 
       *> }                                                                           <* 
       *> printf ("¤");                                                               <*/
   }
   printf ("\n");
   return 0;
}

char
yexec_arg               (char *a_src)
{
   char        rc          =    0;
   yURG_err_clear ();
   rc = strlparse (a_src, s_argw, s_argf, MAX_ARGV, &s_argc, s_argv, LEN_RECD);
   if (rc < 0)  yURG_err ('f', "command line string was unparsable");
   return rc;
}

char
yexec_env               (char *a_src)
{
   char        rc          =    0;
   yURG_err_clear ();
   rc = strlparse (a_src, s_envw, s_envf, MAX_ARGV, &s_envc, s_envp, LEN_RECD);
   if (rc < 0)  yURG_err ('f', "environment description string was unparsable");
   return rc;
}

char       /*--> verify file existance ---------------------------------------*/
yexec__command          (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   tSTAT       s;
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_enter   (__FUNCTION__);
   /*---(defaults)-----------------------*/
   yURG_err_clear ();
   s_direct  = '-';
   s_symlink = '-';
   /*---(defense)------------------------*/
   DEBUG_YEXEC  yLOG_point   ("*s_argv"   , *s_argv);
   --rce;  if (*s_argv == NULL) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_info    ("*s_argv"   , *s_argv);
   /*---(check for direct path)----------*/
   if (*s_argv [0] == '!') {
      DEBUG_YEXEC  yLOG_note    ("set to path outside normal (exceptional)");
      ++s_argv [0];
      s_direct = 'y';
   }
   if (*s_argv [0] == '@') {
      DEBUG_YEXEC  yLOG_note    ("allow running a symlink (exceptional)");
      ++s_argv [0];
      s_symlink = 'y';
   }
   /*---(check existance)----------------*/
   rc = lstat (*s_argv, &s);
   DEBUG_YEXEC  yLOG_value   ("stat"      , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "executable reference can not be found on filesystem");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check file type)----------------*/
   --rce;  if (S_ISDIR (s.st_mode))  {
      yURG_err ('f', "executable references a directory, not a regular file");
      DEBUG_YEXEC  yLOG_note    ("can not use a directory");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_note    ("not a directory");
   if (s_symlink != 'y') {
      --rce;  if (S_ISLNK (s.st_mode))  {
         yURG_err ('f', "executable references a symbolic link (security risk)");
         DEBUG_YEXEC  yLOG_note    ("can not use a symlink");
         DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_YEXEC  yLOG_note    ("not a symlink");
      --rce;  if (!S_ISREG (s.st_mode))  {
         yURG_err ('f', "executable references a non-regular file (not allowed)");
         DEBUG_YEXEC  yLOG_note    ("can only use regular files");
         DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_YEXEC  yLOG_note    ("confirmed as regular file");
   } else {
      --rce;  if (S_ISREG (s.st_mode))  {
         yURG_err ('f', "requested symlink, but pointing to a regular file");
         DEBUG_YEXEC  yLOG_note    ("requested symlink, but not a symlink");
         DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      --rce;  if (!S_ISLNK (s.st_mode))  {
         yURG_err ('f', "requested symlink, but file is not a symlink");
         DEBUG_YEXEC  yLOG_note    ("requested symlink, but not a symlink");
         DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_YEXEC  yLOG_note    ("confirmed as symlink file");
   }
   /*---(check executable)---------------*/
   --rce;  if (!(s.st_mode & S_IEXEC))  {
      yURG_err ('f', "executable references a file without execute permissions");
      DEBUG_YEXEC  yLOG_note    ("not set as executable");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_note    ("permissions show executable");
   /*---(complete)-----------------------*/
   DEBUG_YEXEC  yLOG_exit    (__FUNCTION__);
   return 0;
}

char       /*--> verify file existance ---------------------------------------*/
yexec__setpath          (char a_path)
{
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_senter  (__FUNCTION__);
   /*---(set path)-----------------------*/
   DEBUG_YEXEC  yLOG_schar   (a_path);
   switch (a_path) {
   case YEXEC_TIGHT :
      DEBUG_YEXEC  yLOG_snote   ("tight path");
      s_path  = s_tight;
      break;
   case YEXEC_FULL  :
      DEBUG_YEXEC  yLOG_snote   ("full path");
      s_path  = s_full;
      break;
   default          :
      DEBUG_YEXEC  yLOG_snote   ("unknown path, set to tight");
      s_path  = s_tight;
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_YEXEC  yLOG_sexit   (__FUNCTION__);
   return 0;
}

char       /*--> verify file existance ---------------------------------------*/
yexec__onpath           (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   char        x_path      [LEN_RECD];
   char        x_cmd       [LEN_RECD];
   char        x_dir       [LEN_RECD];
   char       *p           = NULL;
   int         x_pos       =    0;
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_enter   (__FUNCTION__);
   /*---(check for absolute)-------------*/
   yURG_err_clear ();
   if (s_direct == 'y') {
      DEBUG_YEXEC  yLOG_note    ("path is direct/exceptional reference");
      DEBUG_YEXEC  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(initial path marker)------------*/
   strcpy  (x_cmd , *s_argv);
   DEBUG_YEXEC  yLOG_char    ("x_cmd[0]"  , x_cmd[0]);
   --rce;  if (x_cmd[0] != '/') {
      yURG_err ('f', "executable does not include an absolute path (security risk)");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check last path marker)---------*/
   p = strrchr (x_cmd, '/');
   x_pos = p - x_cmd;
   DEBUG_YEXEC  yLOG_value   ("x_pos"     , x_pos);
   --rce;  if (x_pos < 4) {
      yURG_err ('f', "executable path is too short to be real");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare path)-------------------*/
   x_cmd [x_pos] = '\0';
   sprintf (x_dir , ":%s:", x_cmd);
   DEBUG_YEXEC  yLOG_info    ("x_dir"     , x_dir);
   /*---(check on path)------------------*/
   sprintf (x_path, ":%s:", s_path);
   DEBUG_YEXEC  yLOG_info    ("x_path"    , x_path);
   --rce;  if (strstr (x_path, x_dir) == NULL) {
      yURG_err ('f', "executable path not within requested environment path (security risk)");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YEXEC  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yexec__validate         (char *a_title, char *a_user, char *a_cmd, char a_shell, char a_path, char *a_output)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   int         x_len       =    0;
   FILE       *f           = NULL;
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_enter   (__FUNCTION__);
   /*---(defaulting)---------------------*/
   yURG_err_clear ();
   s_direct  = '-';
   s_symlink = '-';
   /*---(check title)--------------------*/
   DEBUG_YEXEC  yLOG_point   ("a_title"   , a_title);
   --rce;  if (a_title == NULL || strlen (a_title) <= 0) {
      yURG_err ('f', "run-time title provided is null/empty (not allowed)");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get user information)-----------*/
   rc = yEXEC_userdata (a_user, &s_uid, &s_gid, s_home, NULL);
   DEBUG_YEXEC  yLOG_value   ("user"      , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "run-time user %s is not registered (not allowed)", (a_user != NULL) ? a_user : "null/empty");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_value   ("s_uid"     , s_uid);
   /*---(check shell)--------------------*/
   DEBUG_YEXEC  yLOG_char    ("a_shell"   , a_shell);
   --rce;  switch (a_shell) {
   case YEXEC_DASH  :
      DEBUG_YEXEC  yLOG_note    ("dash (debian ash)");
      s_shell = s_dash;
      break;
   case YEXEC_BASH  :
      DEBUG_YEXEC  yLOG_note    ("bash (bourne again)");
      s_shell = s_bash;
      break;
   default          :
      DEBUG_YEXEC  yLOG_note    ("unknown shell, set to dash");
      s_shell = s_dash;
      break;
   }
   /*---(check path)---------------------*/
   DEBUG_YEXEC  yLOG_char    ("a_path"    , a_path);
   --rce;  switch (a_path) {
   case YEXEC_TIGHT  :
      DEBUG_YEXEC  yLOG_note    ("tight path");
      s_path  = s_tight;
      break;
   case YEXEC_FULL   :
      DEBUG_YEXEC  yLOG_note    ("full path");
      s_path  = s_full;
      break;
   default           :
      DEBUG_YEXEC  yLOG_note    ("unknown path, set to tight");
      s_path  = s_tight;
      break;
   }
   /*---(check command)------------------*/
   DEBUG_YEXEC  yLOG_point   ("a_cmd"     , a_cmd);
   --rce;  if (a_cmd == NULL || strlen (a_cmd) <= 0) {
      yURG_err ('f', "command string provided is null/empty (not allowed)");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(parse argv)----------------------------*/
   rc = yexec_arg (a_cmd);
   DEBUG_YEXEC  yLOG_value   ("parse"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check command)-------------------------*/
   rc = yexec__command ();
   DEBUG_YEXEC  yLOG_value   ("command"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check on path)-------------------------*/
   rc = yexec__onpath ();
   DEBUG_YEXEC  yLOG_value   ("onpath"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check output)--------------------------*/
   if      (a_output == NULL)         strcpy (s_output, YEXEC_UNIT);
   else if (strlen (a_output) <= 0)   strcpy (s_output, YEXEC_UNIT);
   else                               strcpy (s_output, a_output);
   f = fopen (s_output, "at");
   if (f == NULL)                     strcpy (s_output, YEXEC_UNIT);
   fclose (f);
   DEBUG_YEXEC  yLOG_info    ("s_output"  , s_output);
   /*---(complete)-----------------------*/
   DEBUG_YEXEC  yLOG_exit    (__FUNCTION__);
   return 0;
}

int
yexec__fork             (char *a_title, char a_fork)
{
   /*---(locals)-----------+-----------+-*/
   int         rc          =    0;
   int         x_rpid      =    0;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_senter ("RUN");
   DEBUG_YEXEC  yLOG_snote (a_title);
   DEBUG_YEXEC  yLOG_snote ("n/a");
   DEBUG_YEXEC  yLOG_snote ("fork?");
   DEBUG_YEXEC  yLOG_schar (a_fork);
   /*---(fork off to execute)------------*/
   if (a_fork == YEXEC_FORK) {
      DEBUG_YEXEC  yLOG_snote ("foad");
      x_rpid = fork    ();
      DEBUG_YEXEC  yLOG_svalue ("pid", x_rpid);
      if (x_rpid < 0) {                        /* error                            */
         DEBUG_YEXEC  yLOG_snote  ("FAILURE");
         DEBUG_YEXEC  yLOG_sexit  ("RUN");
         return  -1;
      }
      if (x_rpid > 0) {
         DEBUG_YEXEC  yLOG_snote  ("SUCCESS");
         DEBUG_YEXEC  yLOG_sexit  ("RUN");
         return x_rpid;
      }
   } else {
      DEBUG_YEXEC  yLOG_snote  ("REPLACE");
      DEBUG_YEXEC  yLOG_snote  ("closing logger");
      DEBUG_YEXEC  yLOGS_end   ();   /* stop logging so the next can take over */
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
yexec__environ          (FILE *a_file, char *a_user, char a_fork, FILE *a_out)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   int         i           =    0;
   int         fd          =    0;
   char        t           [LEN_RECD]  = "";
   /*---(set user)-----------------------*/
   rc = setenv ("USER",  a_user, 1);
   --rce;  if (rc <  0) {
      fprintf  (a_file, "USER set  : FATAL, setting failed\n");
      return rce;
   }
   fprintf  (a_file, "USER set  : %s\n", a_user);
   snprintf (t, LEN_LONG, "USER=%s",   a_user);
   strlcat (s_env, t, LEN_RECD);
   /*> s_envp [0] = s_envi [0][0];                                                    <*/
   /*---(set home)-----------------------*/
   rc = setenv ("HOME",  s_home,  1);
   --rce;  if (rc <  0) {
      fprintf  (a_file, "HOME set  : FATAL, setting failed\n");
      return rce;
   }
   fprintf  (a_file, "HOME set  : %s\n", s_home);
   snprintf (t, LEN_LONG, "HOME=%s",   s_home);
   strlcat (s_env, t, LEN_RECD);
   /*> s_envp [1] = s_envi [1][0];                                                    <*/
   /*---(set shell)----------------------*/
   rc = setenv ("SHELL", s_shell, 1);
   --rce;  if (rc <  0) {
      fprintf  (a_file, "SHELL set : FATAL, setting failed\n");
      return rce;
   }
   fprintf  (a_file, "SHELL set : %s\n", s_shell);
   snprintf (t, LEN_LONG, "SHELL=%s",  s_shell);
   strlcat (s_env, t, LEN_RECD);
   /*> s_envp [2] = s_envi [2][0];                                                    <*/
   /*---(set path)-----------------------*/
   rc = setenv ("PATH",  s_path, 1);
   --rce;  if (rc <  0) {
      fprintf  (a_file, "PATH set  : FATAL, setting failed\n");
      return rce;
   }
   fprintf  (a_file, "PATH set  : %s\n", s_path);
   snprintf (t, LEN_LONG, "PATH=%s", s_path);
   strlcat (s_env, t, LEN_RECD);
   /*> s_envp [3] = s_envi [3][0];                                                    <*/
   /*---(set group)----------------------*/
   rc = initgroups (a_user, s_gid);
   --rce;  if (rc <  0) {
      fprintf  (a_file, "FATAL, initgroups failed\n");
      return rce;
   }
   rc = setregid (s_gid, s_gid);
   --rce;  if (rc <  0) {
      fprintf  (a_file, "FATAL, initgroups failed\n");
      return rce;
   }
   fprintf   (a_file, "GID set   : %d\n", s_gid);
   snprintf  (t, LEN_LONG, "GID=%d", s_gid);
   strlcat (s_env, t, LEN_RECD);
   /*> s_envp [4] = s_envi [4][0];                                                    <*/
   /*---(set uid)------------------------*/
   rc = setreuid (s_uid, s_uid);
   --rce;  if (rc <  0) {
      fprintf  (a_file, "FATAL, setreuid failed\n");
      return rce;
   }
   fprintf   (a_file, "UID set   : %d\n", s_uid);
   snprintf  (t, LEN_LONG, "UID=%d", s_uid);
   strlcat (s_env, t, LEN_RECD);
   /*> s_envp [5] = s_envi [5][0];                                                    <*/
   /*---(parse)--------------------------*/
   yexec_env (s_env);
   /*---(set current dir)----------------*/
   rc = chdir (s_home);
   --rce;  if (rc <  0) {
      fprintf  (a_file, "chdir     : FATAL, changing dir failed\n");
      return rce;
   }
   fprintf  (a_file, "chdir     : %s\n", s_home);
   /*---(close off all descriptors)---*/
   fflush   (a_out);
   fclose   (a_out);
   if (a_fork == YEXEC_FORK) {
      for (i = 0; i < 256; ++i)   close (i);
      fd = open ("/dev/null", O_RDWR);
      dup2 (fd, 0);
      dup2 (fd, 1);
      dup2 (fd, 2);
      close (fd);
   } else {
      for (i = 3; i < 256; ++i)   close (i);
   }
   /*---(complete)-----------------------*/
   return 0;
}

char         /*--> early validation of key fields ----------------------------*/
yEXEC_runable      (char *a_title, char *a_user, char  *a_cmd, char a_path)
{
   return yexec__validate (a_title, a_user, a_cmd, YEXEC_DASH, a_path, NULL);
}

int              /* [------] execute a specific command ----------------------*/
yEXEC_full         (char *a_title, char *a_user, char  *a_cmd, char a_shell, char a_path, char a_fork, char *a_output)
{
   /*---(locals)-----------+-----------+-*/
   int         rce         =  -10;
   char        rc          =    0;
   int         x_rpid      =   -1;
   FILE       *f           = NULL;
   char        msg         [200];
   int         i           =    0;
   /*> char       *x_arg       [LEN_LABEL] = { "/bin/sleep" , "2", NULL };            <*/
   /*> char       *x_argv      [LEN_LABEL] = { "/bin/sleep", "1", "2", NULL };        <*/
   char       *x_env       [LEN_LABEL] = { NULL , NULL };
   /*---(validate)-----------------------*/
   rc = yexec__validate (a_title, a_user, a_cmd, a_shell, a_path, a_output);
   if (rc < 0)        return rc;
   /*---(fork)---------------------------*/
   /*> printf ("\n%-20.20s %-20.20s %s\n", a_title, a_user, a_cmd);                   <*/
   /*> yexec_unpoint (x_argv);                                                        <*/
   /*> yexec_unpoint (s_argv);                                                        <*/
   x_rpid = yexec__fork (a_title, a_fork);
   if (x_rpid != 0)   return x_rpid;
   /*---(output)-------------------------*/
   rc = chmod (s_output, 0666);
   if (rc < 0)  _exit (-YEXEC_NOCHMOD);
   if (strcmp (s_output, YEXEC_UNIT) == 0)   f = fopen (s_output, "wt");
   else                                      f = fopen (s_output, "at");
   if (f == NULL)  _exit (-YEXEC_BADLOG);
   fprintf(f, "========================================================================begin===\n");
   /*---(get the date)----------------*/
   yEXEC_heartbeat (getpid (), NULL, a_cmd, NULL, msg);
   fprintf  (f, "start     : %s\n",   msg);
   /*---(set up environment)-------------*/
   rc = yexec__environ (f, a_user, a_fork, f);
   --rce;  if (rc < 0)   _exit (rce);    /* must use _exit to get out properly                       */
   /*---(try execve)----------------------------*/
   f = fopen (s_output, "at");
   if (f == NULL)  _exit (-YEXEC_NOPERM);
   fprintf   (f, "execve    : %s\n", a_cmd);
   fprintf   (f, "==========================================================================end===\n");
   fflush    (f);
   fclose    (f);
   /*> rc = execve (*s_argv, s_argv, s_envp);                                         <*/
   /*> rc = execve (s_argv [0], s_argv, s_envi);                                      <*/
   /*> rc = execve (x_argv [0], x_argv, x_env);                                       <*/
   /*> rc = execve (s_argv [0], s_argv, x_env);                                       <*/
   rc = execve (s_argv [0], s_argv, s_envp);
   /*---(try execvp)----------------------------*/
   f = fopen (s_output, "at");
   if (f == NULL)  _exit (-YEXEC_NOPERM);
   fprintf   (f, "FAILED execve (%d) %3d, %s\n", rc, errno, strerror (errno));
   fprintf   (f, "execvp    : %s\n", a_cmd);
   fprintf   (f, "==========================================================================end===\n");
   fflush    (f);
   fclose    (f);
   rc = execvp (*s_argv, s_argv);
   /*---(try execl)-----------------------------*/
   f = fopen (s_output, "at");
   if (f == NULL)  _exit (-YEXEC_NOPERM);
   fprintf   (f, "FAILED execvp, fallback (%d)...\n", rc);
   fprintf   (f, "execl     : %s\n", a_cmd);
   fprintf   (f, "==========================================================================end===\n");
   fflush    (f);
   fclose    (f);
   rc = execl (s_shell, s_shell, "-c", a_cmd, NULL, NULL);
   /*---(failure)-------------------------------*/
   f = fopen (s_output, "at");
   if (f == NULL)  _exit (-YEXEC_NOPERM);
   fprintf   (f, "FAILED execl, just won't run (%d)\n", rc);
   fprintf   (f, "==========================================================================end===\n");
   fflush    (f);
   fclose    (f);
   _exit (-YEXEC_NOTEXEC);    /* must use _exit to get out properly                       */
}

int
yEXEC_run          (char *a_title, char *a_user, char *a_cmd)
{
   return yEXEC_full (a_title, a_user, a_cmd, YEXEC_DASH, YEXEC_FULL, YEXEC_FORK, YEXEC_UNIT);
}

int
yEXEC_tight        (char *a_title, char *a_user, char *a_cmd)
{
   return yEXEC_full (a_title, a_user, a_cmd, YEXEC_DASH, YEXEC_TIGHT, YEXEC_FORK, YEXEC_UNIT);
}

int
yEXEC_quick        (char *a_cmd)
{
   char        x_user      [LEN_LABEL] = "";
   yEXEC_whoami (NULL, NULL, NULL, NULL, x_user, NULL);
   return yEXEC_full ("quick", x_user, a_cmd, YEXEC_DASH, YEXEC_FULL, YEXEC_FORK, YEXEC_UNIT);
}

char         /*--> verify status of a running job --------[ ------ [ ------ ]-*/
yEXEC_verify       (char *a_title, int a_rpid, int *a_rc2, float *a_csec)
{
   /*---(locals)-------------------------*/
   int       rc        =    0;
   int       x_status  =    0;
   int       x_signal  =    0;
   char      x_rc      =    0;
   tRUSE     x_rusage;
   long      a, b;
   /*---(output header)-----------------*/
   DEBUG_YEXEC  yLOG_senter  ("CHK");
   DEBUG_YEXEC  yLOG_snote   (a_title);
   DEBUG_YEXEC  yLOG_svalue  ("pid"       , a_rpid);
   if (a_rc2 != NULL)  *a_rc2 = 0;
   /*---(check status)------------------*/
   /*> rc = wait4 (a_rpid, &x_status, WNOHANG, NULL);                                 <*/
   /*> rc = waitpid (a_rpid, &x_status, WNOHANG);                                     <*/
   rc = wait4 (a_rpid, &x_status, WNOHANG, &x_rusage);
   DEBUG_YEXEC  yLOG_svalue  ("waitpid"   , rc);
   a = (x_rusage.ru_stime.tv_sec  + x_rusage.ru_utime.tv_sec ) * 1000000;
   b = (x_rusage.ru_stime.tv_usec + x_rusage.ru_utime.tv_usec);
   if (a_csec != NULL)  *a_csec = a + b;
   /*---(handle no such child)----------*/
   if (rc == -1) {
      DEBUG_YEXEC  yLOG_snote   ("daemonized and gone");
      DEBUG_YEXEC  yLOG_sexit   ("CHK");
      return YEXEC_NOSUCH;
   }
   /*---(handle running)----------------*/
   if (rc == 0) {
      DEBUG_YEXEC  yLOG_snote   ("still running");
      DEBUG_YEXEC  yLOG_sexit   ("CHK");
      return YEXEC_RUNNING;
   }
   /*---(handle signal termed)----------*/
   if (!WIFEXITED (x_status)) {
      if (WIFSIGNALED (x_status)) {
         x_signal = WTERMSIG (x_status);
         if (a_rc2 != NULL)  *a_rc2 = x_signal;
         switch (x_signal) {
         case SIGTERM : case SIGKILL : case SIGQUIT : case SIGABRT :
            DEBUG_YEXEC  yLOG_snote   ("TERMINATED");
            DEBUG_YEXEC  yLOG_sexit   ("CHK");
            return YEXEC_KILLED;
            break;
         case SIGSEGV : case SIGFPE  : case SIGILL  : case SIGBUS  :
         case SIGPIPE : case SIGSYS  :
            DEBUG_YEXEC  yLOG_snote   ("BLEW-UP");
            DEBUG_YEXEC  yLOG_sexit   ("CHK");
            return YEXEC_SEGV;
            break;
         case SIGHUP  : case SIGALRM : case SIGUSR1 : case SIGUSR2 :
         case SIGSTOP : case SIGTSTP : case SIGTTIN : case SIGTTOU : case SIGURG  :
            DEBUG_YEXEC  yLOG_snote   ("IPC/USER");
            DEBUG_YEXEC  yLOG_sexit   ("CHK");
            return YEXEC_USER;
            break;
         case SIGXCPU : case SIGXFSZ :
            DEBUG_YEXEC  yLOG_snote   ("LIMITS");
            DEBUG_YEXEC  yLOG_sexit   ("CHK");
            return YEXEC_LIMIT;
            break;
         default   :
            DEBUG_YEXEC  yLOG_snote   ("OTHER/DIED");
            DEBUG_YEXEC  yLOG_sexit   ("CHK");
            return YEXEC_DIED;
         }
      }
      if (a_rc2 != NULL)  *a_rc2 = -1;
      DEBUG_YEXEC  yLOG_snote   ("ERROR");
      DEBUG_YEXEC  yLOG_sexit   ("CHK");
      return YEXEC_ERROR;
   }
   /*---(gather return code)------------*/
   DEBUG_YEXEC  yLOG_snote   ("exited");
   x_rc = WEXITSTATUS (x_status);
   DEBUG_YEXEC  yLOG_svalue  ("x_rc"      , x_rc);
   if (a_rc2 != NULL)  *a_rc2 = x_rc;
   /*---(handle launch failures)--------*/
   if (x_rc == 127) {  /* command not found from linux */
      DEBUG_YEXEC  yLOG_snote   ("boom");
      DEBUG_YEXEC  yLOG_sexit   ("CHK");
      return YEXEC_NOTREAL;
   }
   /*---(handle completed)--------------*/
   if (x_rc == 0) {
      DEBUG_YEXEC  yLOG_snote   ("normal");
      DEBUG_YEXEC  yLOG_sexit   ("CHK");
      return YEXEC_NORMAL;
   }
   /*---(handle errors)-----------------*/
   if (x_rc == -YEXEC_NOPERM) {
      DEBUG_YEXEC  yLOG_snote   ("no permission on files");
      if (a_rc2 != NULL)  *a_rc2 = x_rc;
      DEBUG_YEXEC  yLOG_sexit   ("CHK");
      return YEXEC_NOPERM;
   }
   if (x_rc == -YEXEC_NOTEXEC) {
      DEBUG_YEXEC  yLOG_snote   ("could not exec process");
      if (a_rc2 != NULL)  *a_rc2 = x_rc;
      DEBUG_YEXEC  yLOG_sexit   ("CHK");
      return YEXEC_NOTEXEC;
   }
   if (x_rc == -YEXEC_NOCHMOD) {
      DEBUG_YEXEC  yLOG_snote   ("could not chmod output");
      if (a_rc2 != NULL)  *a_rc2 = x_rc;
      DEBUG_YEXEC  yLOG_sexit   ("CHK");
      return YEXEC_NOCHMOD;
   }
   if (x_rc == SIGTERM) {
      DEBUG_YEXEC  yLOG_snote   ("graceful shutdown handled properly");
      if (a_rc2 != NULL)  *a_rc2 = x_rc;
      DEBUG_YEXEC  yLOG_snote   ("TERMINATED");
      DEBUG_YEXEC  yLOG_sexit   ("CHK");
      return YEXEC_KILLED;
   }
   if (x_rc <    0) {  /* negative returns from linux uchar return */
      DEBUG_YEXEC  yLOG_snote   ("FAILURE");
      if (a_rc2 != NULL)  *a_rc2 = x_rc;
      DEBUG_YEXEC  yLOG_sexit   ("CHK");
      return YEXEC_FAILURE;
   }
   /*---(handle warnings)---------------*/
   DEBUG_YEXEC  yLOG_snote   ("warning");
   DEBUG_YEXEC  yLOG_sexit   ("CHK");
   return YEXEC_WARNING;
}

char
yEXEC_check             (int a_rpid)
{
   return yEXEC_verify ("quick", a_rpid, NULL, NULL);
}

char
yEXEC_timing            (int a_rpid, char a_strict, int a_max, int a_dur, int a_grace, int a_peers)
{
   if (a_strict == 0) return '-';
   DEBUG_YEXEC  yLOG_complex ("timing"    , "%5d, %c, %5dm, %5dd, %5dg", a_rpid, a_strict, a_max, a_dur, a_grace);
   switch (a_strict) {
   case YEXEC_GRACEFUL :
      if (a_dur >= a_max + a_grace) {
         DEBUG_YEXEC  yLOG_note    ("violent termination, since graceful ignored");
         kill (a_rpid, SIGKILL);
         return YEXEC_VIOLENT;
      } else if (a_dur >= a_max) {
         DEBUG_YEXEC  yLOG_note    ("graceful termination request");
         kill (a_rpid, SIGTERM);
         return YEXEC_GRACEFUL;
      } else {
         DEBUG_YEXEC  yLOG_note    ("too early for graceful termination");
      }
      break;
   case YEXEC_VIOLENT  :
      if (a_dur >= a_max) {
         DEBUG_YEXEC  yLOG_note    ("violent termination");
         kill (a_rpid, SIGKILL);
         return YEXEC_VIOLENT;
      } else {
         DEBUG_YEXEC  yLOG_note    ("too early for violent termination");
      }
      break;
   case YEXEC_TILLEND  :
      if (a_peers == 0) {
         if (a_dur >= a_max) {
            DEBUG_YEXEC  yLOG_note    ("violent, end-of-peers, termination");
            kill (a_rpid, SIGKILL);
            return YEXEC_VIOLENT;
         }
      }
      break;
   default  :
      DEBUG_YEXEC  yLOG_note    ("nothing to do");
      break;
   }
   return '-';
}

char
yEXEC_detail            (char a_rc, int a_rc2, char *a_desc)
{
   if (a_desc == NULL)  return -1;
   switch (a_rc) {
   case YEXEC_RUNNING :
      strlcpy (a_desc, "still running, nothing to report"   , LEN_DESC);
      break;
   case YEXEC_NORMAL  :
      strlcpy (a_desc, "normal completion and return (0)"   , LEN_DESC);
      break;
   case YEXEC_FAILURE :
      sprintf (a_desc, "process ended with failure (%d)"    , a_rc2);
      break;
   case YEXEC_NOSUCH  :
      strlcpy (a_desc, "daemonized and gone, untouchable"   , LEN_DESC);
      break;
   case YEXEC_KILLED  :
      sprintf (a_desc, "terminated with (%2d) "             , a_rc2);
      switch (a_rc2) {
      case SIGTERM :  strlcat (a_desc, "SIGTERM", LEN_DESC);  break;
      case SIGKILL :  strlcat (a_desc, "SIGKILL", LEN_DESC);  break;
      case SIGQUIT :  strlcat (a_desc, "SIGQUIT", LEN_DESC);  break;
      case SIGABRT :  strlcat (a_desc, "SIGABRT", LEN_DESC);  break;
      default      :  strlcat (a_desc, "unknown", LEN_DESC);  break;
      }
      break;
   case YEXEC_SEGV    :
      sprintf (a_desc, "blew itself up with (%2d) "         , a_rc2);
      switch (a_rc2) {
      case SIGSEGV :  strlcat (a_desc, "SIGSEGV", LEN_DESC);  break;
      case SIGFPE  :  strlcat (a_desc, "SIGFPE" , LEN_DESC);  break;
      case SIGILL  :  strlcat (a_desc, "SIGILL" , LEN_DESC);  break;
      case SIGBUS  :  strlcat (a_desc, "SIGBUS" , LEN_DESC);  break;
      case SIGPIPE :  strlcat (a_desc, "SIGPIPE", LEN_DESC);  break;
      case SIGSYS  :  strlcat (a_desc, "SIGSYS" , LEN_DESC);  break;
      default      :  strlcat (a_desc, "unknown", LEN_DESC);  break;
      }
      break;
   case YEXEC_USER    :
      sprintf (a_desc, "ipc/user communication (%2d) "      , a_rc2);
      switch (a_rc2) {
      case SIGHUP  :  strlcat (a_desc, "SIGHUP" , LEN_DESC);  break;
      case SIGALRM :  strlcat (a_desc, "SIGALRM", LEN_DESC);  break;
      case SIGUSR1 :  strlcat (a_desc, "SIGUSR1", LEN_DESC);  break;
      case SIGUSR2 :  strlcat (a_desc, "SIGUSR2", LEN_DESC);  break;
      case SIGSTOP :  strlcat (a_desc, "SIGSTOP", LEN_DESC);  break;
      case SIGTSTP :  strlcat (a_desc, "SIGTSTP", LEN_DESC);  break;
      case SIGTTIN :  strlcat (a_desc, "SIGTTIN", LEN_DESC);  break;
      case SIGTTOU :  strlcat (a_desc, "SIGTTOU", LEN_DESC);  break;
      case SIGURG  :  strlcat (a_desc, "SIGURG" , LEN_DESC);  break;
      default      :  strlcat (a_desc, "unknown", LEN_DESC);  break;
      }
      break;
   case YEXEC_LIMIT   :
      sprintf (a_desc, "security limits reached (%2d) "     , a_rc2);
      switch (a_rc2) {
      case SIGXCPU :  strlcat (a_desc, "SIGXCPU", LEN_DESC);  break;
      case SIGXFSZ :  strlcat (a_desc, "SIGXFSZ", LEN_DESC);  break;
      default      :  strlcat (a_desc, "unknown", LEN_DESC);  break;
      }
      break;
   case YEXEC_DIED    :
      sprintf (a_desc, "unknown/uncategorized signal (%2d)" , a_rc2);
      break;
   case YEXEC_NOTREAL :
      sprintf (a_desc, "command not found by shell (%d)"    , a_rc2);
      break;
   case YEXEC_NOPERM  :
      sprintf (a_desc, "no permission on yEXEC log (%d)"    , a_rc2);
      break;
   case YEXEC_NOCHMOD :
      sprintf (a_desc, "could not chmod yEXEC log (%d)"     , a_rc2);
      break;
   case YEXEC_NOTEXEC :
      sprintf (a_desc, "could not execute file (%d)"        , a_rc2);
      break;
   default            :
      sprintf (a_desc, "UNKNOWN, unanticipated (%d)"        , a_rc2);
      break;
   }
   return 0;
}

char             /* [------] find a running job by name ----------------------*/
yEXEC_find         (char *a_name, int *a_rpid)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   DIR        *x_dir;
   tDIRENT    *x_den;
   FILE       *f;
   char        x_name      [1000];
   char        x_title     [1000];
   char        x_recd      [1000];
   char        x_save      [1000];
   char        t           [50];  
   char       *p;
   char       *q;
   int         x_rpid      =   -1;
   int         c           =    0;
   int         x_len       =    0;
   int         l           =    0;
   char        x_status    =  '-';
   /*---(output header)-----------------*/
   DEBUG_YEXEC  yLOG_senter  (__FUNCTION__);
   /*---(defense)-----------------------*/
   DEBUG_YEXEC  yLOG_spoint  (a_name);
   --rce;  if (a_name == NULL) {
      DEBUG_YEXEC  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_snote   (a_name);
   x_len = strlen (a_name);
   DEBUG_YEXEC  yLOG_sint    (x_len);
   /*---(prepare)------------------------*/
   if (a_rpid != NULL)  *a_rpid = 99999;
   /*---(open the proc system)-----------*/
   x_dir = opendir("/proc");
   DEBUG_YEXEC  yLOG_spoint  (x_dir);
   --rce;  if (x_dir == NULL) {
      DEBUG_YEXEC  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(cycle through the entries)------*/
   while ((x_den = readdir (x_dir)) != NULL) {
      /*---(filter non-processes)--------*/
      if (atoi(x_den->d_name)     == 0)  continue;
      /*---(open and get record)---------*/
      sprintf(x_name, "/proc/%s/stat", x_den->d_name);
      f = fopen (x_name, "r");
      if (f == NULL) continue;
      fgets (x_recd, 1000, f);
      fclose (f);
      /*---(read the entry)--------------*/
      strcpy (x_save, x_recd);
      p = strtok (x_recd, "(");
      p = strtok (NULL, ")");
      strcpy (x_title, p);
      p = strtok (NULL, ")");
      x_status = p [1];
      switch (x_status) {
      case 'Z' : continue;      break;
      }
      /*---(verify)----------------------*/
      q = strchr (x_title, ' ');
      if (q != NULL)  q [0] = '\0';
      if (strlen (x_title) != x_len)      continue;
      if (strcmp (x_title, a_name) != 0)  continue;
      ++c;
      l = strlen (x_save);
      if (x_save [l - 1] == '\n')  x_save [--l] = '\0';
      snprintf (t, 50, "%03d[%-40.40s]", l, x_save);
      DEBUG_YEXEC  yLOG_snote   (t);
      DEBUG_YEXEC  yLOG_schar   (x_status);
      DEBUG_YEXEC  yLOG_sint    (c);
      DEBUG_YEXEC  yLOG_snote   ("FOUND");
      if (x_rpid < 0) {
         x_rpid =  atoi (x_den->d_name);
         DEBUG_YEXEC  yLOG_sint    (x_rpid);
      }
      /*---(done)------------------------*/
   }
   closedir (x_dir);
   if (x_rpid <= 1)      x_rpid = 99999;
   if (a_rpid != NULL)  *a_rpid = x_rpid;
   DEBUG_YEXEC  yLOG_sint    (c);
   DEBUG_YEXEC  yLOG_sexit   (__FUNCTION__);
   return c;
}



/*====================------------------------------------====================*/
/*===----                        measuring usage                       ----===*/
/*====================------------------------------------====================*/
static void      o___USAGE___________________o (void) {;}

/*
 *   /proc/stat        cpu usage
 *   /proc/cpuinfo     cpu details, including MHz
 *   /proc/meminfo     memory avail, alloc, etc.
 *
 *   /proc/pid/stat    lots of stuff
 *   /proc/pid/statm   ???
 *   /proc/pid/io      bytes read, bytes written
 *   /proc/pid/status  lots, but has sleeping, running, etc
 *   /proc/pid/limits  getrlimit (), getslimit ()
 *
 *   /proc/pid/smaps   shared library coolness
 *
 *
 *   MUST CONVERT FROM JIFFIES to cpu msec
 *
 *   HOW TO SCALE from cpu to cpu to block procs into needed categories
 *
 */

char
yEXEC_cpu_main          (long *a_utime, long *a_stime, long *a_idle)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   FILE       *f;
   char        x_recd      [LEN_RECD]  = "";
   char       *p           = NULL;
   char       *r           = NULL;
   char        c           =    0;
   /*---(header)------------------------*/
   DEBUG_YEXEC  yLOG_senter  (__FUNCTION__);
   /*---(open stat file)-----------------*/
   f = fopen ("/proc/stat", "rt");
   DEBUG_YEXEC  yLOG_spoint  (f);
   --rce;  if (f == NULL) {
      DEBUG_YEXEC  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(read line)---------------------*/
   fgets (x_recd, LEN_RECD, f);
   p = strtok_r (x_recd, " ", &r);
   while (p != NULL && c <  5) {
      switch (c) {
      case  1 :
         if (a_utime != NULL)  *a_utime = atol (p);
         break;
      case  3 :
         if (a_stime != NULL)  *a_stime = atol (p);
         break;
      case  4 :
         if (a_idle  != NULL)  *a_idle  = atol (p);
         break;
      }
      ++c;
      p = strtok_r (NULL  , " ", &r);
   }
   /*---(close file)--------------------*/
   rc = fclose (f);
   DEBUG_YEXEC  yLOG_sint    (rc);
   --rce;  if (f <  0) {
      DEBUG_YEXEC  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)----------------------*/
   DEBUG_YEXEC  yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
yEXEC_mem_main          (void)
{
}

char
yEXEC_net_main          (void)
{
}


char
yEXEC_mem_proc_OLD      (int a_rpid, long *a_total, long *a_text, long *a_data, long *a_stack, long *a_heap, long *a_libs)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   FILE       *f;
   char        x_name      [LEN_HUND]  = "";
   char        x_recd      [LEN_RECD]  = "";
   int         i           =    0;
   char       *p           = NULL;
   char       *r           = NULL;
   char        x_perm      [LEN_TERSE] = "";
   /*---(header)------------------------*/
   DEBUG_YEXEC  yLOG_senter  (__FUNCTION__);
   /*---(defaults)----------------------*/
   if (a_total != NULL)  *a_total = 0;
   if (a_text  != NULL)  *a_text  = 0;
   if (a_data  != NULL)  *a_data  = 0;
   if (a_stack != NULL)  *a_heap  = 0;
   if (a_heap  != NULL)  *a_heap  = 0;
   if (a_libs  != NULL)  *a_libs  = 0;
   /*---(open proc)----------------------*/
   /*> sprintf (x_name, "/proc/%d/smaps", a_rpid);                                    <*/
   sprintf (x_name, "/proc/%d/status", a_rpid);
   f = fopen (x_name, "rt");
   DEBUG_YEXEC  yLOG_spoint  (f);
   --rce;  if (f == NULL) {
      DEBUG_YEXEC  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(read stanza)-----------------*/
   for (i = 0; i < 25; ++i) {
      fgets (x_recd, LEN_RECD, f);
      /*> printf ("%2d, %s\n", i, x_recd);                                            <*/
      p = strtok_r (x_recd, " \t", &r);
      /*> printf ("    %p\n", p);                                                     <*/
      if (p == NULL) break;
      p = strtok_r (NULL  , " \t", &r);
      /*> printf ("    %p\n", p);                                                     <*/
      if (p == NULL) break;
      switch (i) {
      case 12 : /* VmSize  */
         /*> printf ("%2d, %5ld, %s\n", i, atol (p), p);                              <*/
         if (a_total != NULL)  *a_total += atol (p);
         break;
      case 17 : /* VmData  */
         /*> printf ("%2d, %5ld, %s\n", i, atol (p), p);                              <*/
         if (a_data  != NULL)  *a_data  += atol (p);
         break;
      case 18 :  /* VmStk   */
         /*> printf ("%2d, %5ld, %s\n", i, atol (p), p);                              <*/
         if (a_stack != NULL)  *a_stack += atol (p);
         break;
      case 19 : /* VmExe   */
         /*> printf ("%2d, %5ld, %s\n", i, atol (p), p);                              <*/
         if (a_text  != NULL)  *a_text  += atol (p);
         break;
      }
   }
   /*---(close file)--------------------*/
   rc = fclose (f);
   DEBUG_YEXEC  yLOG_sint    (rc);
   --rce;  if (f <  0) {
      DEBUG_YEXEC  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)----------------------*/
   DEBUG_YEXEC  yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
yEXEC_net_proc          (void)
{
}

char
yEXEC_tracking          (FILE *a_file, int a_rpid, char a_track)
{
}



/*====================------------------------------------====================*/
/*===----                      unit test accessor                      ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

static char    x_info      [LEN_HUND] = "";  

char*            /*--> unit test accessor ------------------------------*/
yexec_proc__unit        (char *a_question, int a_num)
{
   /*---(locals)-----------+-----+-----+-*/
   char        s           [LEN_RECD]  = "";
   char        t           [LEN_RECD]  = "";
   int         c           =    0;
   /*---(prepare)------------------------*/
   strncpy  (unit_answer, "PROC             : question not understood", LEN_RECD);
   /*---(crontab name)-------------------*/
   if      (strcmp (a_question, "time"    )        == 0) {
      yexec_proc__unit_read ('t', t);
      snprintf (unit_answer, LEN_RECD, "PROC time        : %s", t);
   }
   else if (strcmp (a_question, "user"    )        == 0) {
      yexec_proc__unit_read ('u', t);
      snprintf (unit_answer, LEN_RECD, "PROC user        : %s", t);
   }
   else if (strcmp (a_question, "home"    )        == 0) {
      yexec_proc__unit_read ('l', t);
      snprintf (unit_answer, LEN_RECD, "PROC home        : %s", t);
   }
   else if (strcmp (a_question, "detail"  )        == 0) {
      yexec_proc__unit_read ('d', t);
      snprintf (unit_answer, LEN_RECD, "PROC detail      : %s", t);
   }
   else if (strcmp (a_question, "exec"    )        == 0) {
      yexec_proc__unit_read ('x', t);
      snprintf (unit_answer, LEN_RECD, "PROC exec        : %s  %2d[%s]", t, strlen (*s_argv), *s_argv);
   }
   else if (strcmp (a_question, "parse"   )        == 0) {
      yexec_proc__unit_read ('x', t);
      snprintf (unit_answer, LEN_RECD, "PROC parse       : %2d[%s]", strlen (s_arg), s_arg);
   }
   else if (strcmp (a_question, "whoami"  )        == 0) {
      strlcpy (s, "/tmp/unit_whoami.txt", LEN_HUND);
      snprintf (unit_answer, LEN_RECD, "PROC whoami      : main %-5d", getpid ());
      sprintf  (t, ", pid  %-5.5s"  , strlrecd (s, 0, NULL, NULL, LEN_RECD) + 7);
      strcat   (unit_answer, t);
      sprintf  (t, ", ppid %-5.5s"  , strlrecd (s, 1, NULL, NULL, LEN_RECD) + 7);
      strcat   (unit_answer, t);
      sprintf  (t, ", uid  %-5.5s"  , strlrecd (s, 2, NULL, NULL, LEN_RECD) + 7);
      strcat   (unit_answer, t);
      sprintf  (t, ", who  (%-1.1s)", strlrecd (s, 3, NULL, NULL, LEN_RECD) + 7);
      strcat   (unit_answer, t);
      sprintf  (t, " %s"            , strlrecd (s, 4, NULL, NULL, LEN_RECD) + 7);
      strcat   (unit_answer, t);
   }
   else if (strcmp (a_question, "whereami")        == 0) {
      strlcpy (s, "/tmp/unit_whoami.txt", LEN_HUND);
      snprintf (unit_answer, LEN_RECD, "PROC whereami    : ");
      sprintf  (t, "%-12.12s"  , strlrecd (s, 6, NULL, NULL, LEN_RECD) + 7);
      strcat   (unit_answer, t);
      sprintf  (t, "  %s"  , strlrecd (s, 5, NULL, NULL, LEN_RECD) + 7);
      strcat   (unit_answer, t);
   }
   /*---(argument testing)---------------*/
   else if (strcmp (a_question, "argc"    )        == 0) {
      snprintf (unit_answer, LEN_RECD, "PROC argc        : %d", s_argc);
   }
   else if (strcmp (a_question, "argv"    )        == 0) {
      if (a_num < 20 && a_num < s_argc && s_argv [a_num] != NULL)  {
         snprintf (unit_answer, LEN_RECD, "PROC argv   (%2d) : %2d[%-.40s]", a_num, strlen (s_argv [a_num]), s_argv [a_num]);
      } else {
         snprintf (unit_answer, LEN_RECD, "PROC argv   (%2d) :  0[]", a_num);
      }
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}

char
yexec_proc__unit_read         (char a_type, char *a_info)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   FILE       *f           = NULL;
   char        c           =    0;
   char        x_recd      [LEN_RECD];
   int         x_len       =    0;
   char        x_cmd       [LEN_RECD];
   char        u_time      [LEN_RECD] = "";
   char        u_user      [LEN_RECD] = "";
   char        u_home      [LEN_RECD] = "";
   char        u_shell     [LEN_RECD] = "";
   char        u_path      [LEN_RECD] = "";
   char        u_gid       [LEN_RECD] = "";
   char        u_uid       [LEN_RECD] = "";
   char        u_chdir     [LEN_RECD] = "";
   char        u_exec      [LEN_RECD] = "";
   char        t           [LEN_RECD] = "";
   char        s           [LEN_RECD] = "";
   /*---(chmod)--------------------------*/
   sprintf (x_cmd, "chmod 0777 %s  2> /dev/null", YEXEC_UNIT);
   system  (x_cmd);
   /*---(open)---------------------------*/
   f = fopen (YEXEC_UNIT, "rt");
   --rce; if (f == NULL)  return rce;
   /*---(read)---------------------------*/
   while (1) {
      fgets (x_recd, 2000, f);
      if (feof (f))  break;
      x_len = strlen (x_recd);
      x_recd [--x_len] = '\0';
      switch (c) {
      case  1 : strncpy (u_time  , x_recd + 12, LEN_RECD);  break;
      case  2 : strncpy (u_user  , x_recd + 12, LEN_RECD);  break;
      case  3 : strncpy (u_home  , x_recd + 12, LEN_RECD);  break;
      case  4 : strncpy (u_shell , x_recd + 12, LEN_RECD);  break;
      case  5 : strncpy (u_path  , x_recd + 12, LEN_RECD);  break;
      case  6 : strncpy (u_gid   , x_recd + 12, LEN_RECD);  break;
      case  7 : strncpy (u_uid   , x_recd + 12, LEN_RECD);  break;
      case  8 : strncpy (u_chdir , x_recd + 12, LEN_RECD);  break;
      case  9 : strncpy (u_exec  , x_recd + 12, LEN_RECD);  break;
      }
      ++c;
   }
   /*---(close)--------------------------*/
   fclose (f);
   /*---(write back)---------------------*/
   switch (a_type) {
   case 't' :
      sprintf (a_info, "%2d[%-.40s]", strlen (u_time), u_time);
      break;
   case 'u' :
      sprintf (t, "%2d[%-.20s]", strlen (u_user), u_user);
      sprintf (a_info, "%-24.24s  %4d  %4d", t, atoi (u_uid), atoi (u_gid));
      break;
   case 'd' :
      sprintf (t, "%2d[%-.20s]", strlen (u_shell), u_shell);
      sprintf (s, "%2d[%-.40s]", strlen (u_path ), u_path );
      sprintf (a_info, "%-24.24s  %s", t, s);
      break;
   case 'l' :
      sprintf (t, "%2d[%-.40s]", strlen (u_home ), u_home );
      sprintf (a_info, "%-40.40s  %c", t, (strcmp (u_home, u_chdir) == 0) ? 'y' : 'F');
      break;
   case 'x' :
      sprintf (t, "%2d[%-.40s]", strlen (u_exec ), u_exec );
      sprintf (a_info, "%-40.40s", t);
      break;
   }
   /*---(complete)-----------------------*/
   return 0;
}



