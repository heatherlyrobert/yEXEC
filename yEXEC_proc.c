/*============================[[    beg-code    ]]============================*/
#include   "yEXEC.h"
#include   "yEXEC_priv.h"



/*---(globals)----------+-----------+----*/
char        s_cmd       [2000];
char       *s_argv      [20];
int         s_argc      = 0;
char        s_envp      [10][200];

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



/*====================------------------------------------====================*/
/*===----                            basics                            ----===*/
/*====================------------------------------------====================*/
static void      o___BASICS__________________o (void) {;}

char         /*--> break string into argv structure ------[ leaf-- [ ------ ]-*/
yexec__parse       (const char *a_cstring)
{
   /*---(locals)-----------+-----------+-*/
   int         i, k;
   int         x_len       = 0;
   int         pos         = 0;
   /*---(defenses)-----------------------*/
   if (a_cstring == NULL) return -1;
   /*---(prepare)------------------------*/
   strncpy (s_cmd, a_cstring, 2000);
   s_cmd [1999] = '\0';  /* null terminate */
   x_len = strlen (s_cmd);
   for (i = 0; i < 20; ++i) s_argv[i] = (char *) 0;
   s_argv[0] = s_cmd + 0;
   s_argc    = 0;
   i         = 1;
   pos       = 0;
   /*---(run through string)-------------*/
   while (pos < x_len) {
      /*---(get though argument)---------*/
      for (k = pos; k <= x_len; ++k) {
         if (  s_cmd[k] != '\0' &&
               s_cmd[k] != ' ' )       continue;
         pos = k;
         break;
      }
      /*---(check for the end)-----------*/
      if (s_cmd [pos] == '\0')         break;
      /*> if (pos + 1 >= x_len)            break;                                     <*/
      /*---(get though whitespace)-------*/
      for (k = pos; k < x_len; ++k) {
         if (s_cmd[k] == ' ') {
            s_cmd[k]  =  '\0';
            continue;
         }
         pos = k;
         break;
      }
      /*---(check for the end)-----------*/
      /*> if (s_cmd [pos] == '\0')         break;                                     <*/
      /*> if (pos + 1 >= x_len)            break;                                     <*/
      /*---(assign next argument)--------*/
      s_argv[i] = s_cmd + pos;
      /*---(prepare for next loop)-------*/
      ++i;
      if (i >= 20) break;
   }
   /*---(complete)-----------------------*/
   return 0;
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
   /*---(defense)------------------------*/
   DEBUG_YEXEC  yLOG_point   ("*s_argv"   , *s_argv);
   --rce;  if (*s_argv == NULL) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_info    ("*s_argv"   , *s_argv);
   /*---(check existance)----------------*/
   rc = stat (*s_argv, &s);
   DEBUG_YEXEC  yLOG_value   ("stat"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check file type)----------------*/
   --rce;  if (S_ISDIR (s.st_mode))  {
      DEBUG_YEXEC  yLOG_note    ("can not use a directory");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_note    ("not a directory");
   --rce;  if (S_ISLNK (s.st_mode))  {
      DEBUG_YEXEC  yLOG_note    ("can not use a symlink");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_note    ("not a symlink");
   --rce;  if (!S_ISREG (s.st_mode))  {
      DEBUG_YEXEC  yLOG_note    ("can only use regular files");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_note    ("confirmed as regular file");
   /*---(check executable)---------------*/
   --rce;  if (!(s.st_mode & S_IEXEC))  {
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
   /*---(initial path marker)------------*/
   strcpy  (x_cmd , *s_argv);
   DEBUG_YEXEC  yLOG_char    ("x_cmd[0]"  , x_cmd[0]);
   --rce;  if (x_cmd[0] != '/') {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check last path marker)---------*/
   p = strrchr (x_cmd, '/');
   x_pos = p - x_cmd;
   DEBUG_YEXEC  yLOG_value   ("x_pos"     , x_pos);
   --rce;  if (x_pos < 4) {
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
   /*---(check title)--------------------*/
   DEBUG_YEXEC  yLOG_point   ("a_title"   , a_title);
   --rce;  if (a_title == NULL) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get user information)-----------*/
   rc = yEXEC_user (a_user, &s_uid, &s_gid, s_home);
   DEBUG_YEXEC  yLOG_value   ("user"      , rc);
   --rce;  if (rc < 0) {
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
   case YEXEC_TIGHT :
      DEBUG_YEXEC  yLOG_note    ("tight path");
      s_path  = s_tight;
      break;
   case YEXEC_FULL  :
      DEBUG_YEXEC  yLOG_note    ("full path");
      s_path  = s_full;
      break;
   default          :
      DEBUG_YEXEC  yLOG_note    ("unknown path, set to tight");
      s_path  = s_tight;
      break;
   }
   /*---(check command)------------------*/
   DEBUG_YEXEC  yLOG_point   ("a_cmd"     , a_cmd);
   --rce;  if (a_cmd == NULL) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_len = strlen (a_cmd);
   DEBUG_YEXEC  yLOG_value   ("x_len"     , x_len);
   --rce;  if (x_len <= 0) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(parse argv)----------------------------*/
   rc = yexec__parse (a_cmd);
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
   if (a_output == NULL)   strcpy (s_output, "/dev/null");
   else                    strcpy (s_output, a_output);
   f = fopen (s_output, "a");
   if (f == NULL)  {
      strcpy (s_output, "/dev/null");
   }
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
   int         fd          =    0;
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
      DEBUG_YEXEC  yLOG_end    ();   /* stop logging so the next can take over */
   }
   /*---(close off all descriptors)---*/
   for (i = 0; i < 256; ++i)   close(i);
   /*---(std fds to the bitbucket)---*/
   fd = open ("/dev/null", O_RDWR);
   dup2(fd, 0);
   dup2(fd, 1);
   dup2(fd, 2);
   /*---(complete)-----------------------*/
   return 0;
}

char
yexec__environ          (FILE *a_file, char *a_user)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   /*---(set user)-----------------------*/
   rc = setenv ("USER",  a_user, 1);
   --rce;  if (rc <  0) {
      fprintf  (a_file, "USER set  : FATAL, setting failed\n");
      return rce;
   }
   fprintf  (a_file, "USER set  : %s\n", a_user);
   snprintf (s_envp [0], 200, "USER=%s",   a_user);
   /*---(set home)-----------------------*/
   rc = setenv ("HOME",  s_home,  1);
   --rce;  if (rc <  0) {
      fprintf  (a_file, "HOME set  : FATAL, setting failed\n");
      return rce;
   }
   fprintf  (a_file, "HOME set  : %s\n", s_home);
   snprintf (s_envp [1], 200, "HOME=%s",   s_home);
   /*---(set shell)----------------------*/
   rc = setenv ("SHELL", s_shell, 1);
   --rce;  if (rc <  0) {
      fprintf  (a_file, "SHELL set : FATAL, setting failed\n");
      return rce;
   }
   fprintf  (a_file, "SHELL set : %s\n", s_shell);
   snprintf (s_envp [2], 200, "SHELL=%s",  s_shell);
   /*---(set path)-----------------------*/
   rc = setenv ("PATH",  s_path, 1);
   --rce;  if (rc <  0) {
      fprintf  (a_file, "PATH set  : FATAL, setting failed\n");
      return rce;
   }
   fprintf  (a_file, "PATH set  : %s\n", s_path);
   snprintf (s_envp [3], 200, "PATH=%s", s_path);
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
   snprintf  (s_envp [4], 200, "GID=%d", s_gid);
   fprintf   (a_file, "GID set   : %d\n", s_gid);
   /*---(set uid)------------------------*/
   rc = setreuid (s_uid, s_uid);
   --rce;  if (rc <  0) {
      fprintf  (a_file, "FATAL, initgroups failed\n");
      return rce;
   }
   snprintf  (s_envp[5], 200, "UID=%d", s_uid);
   fprintf   (a_file, "UID set   : %d\n", s_uid);
   /*---(finish enviroment)--------------*/
   s_envp [6][0] = NULL;
   /*---(set current dir)----------------*/
   rc = chdir (s_home);
   --rce;  if (rc <  0) {
      fprintf  (a_file, "chdir     : FATAL, changing dir failed\n");
      return rce;
   }
   fprintf  (a_file, "chdir     : %s\n", s_home);
   /*---(complete)-----------------------*/
   return 0;
}

char         /*--> early validation of key fields ----------------------------*/
yEXEC_runable      (char *a_title, char *a_user, char  *a_cmd, char a_path)
{
   return yexec__validate (a_title, a_user, a_cmd, YEXEC_DASH, a_path, NULL);
}

int              /* [------] execute a specific command ----------------------*/
yEXEC_run          (char *a_title, char *a_user, char  *a_cmd, char a_shell, char a_path, char a_fork, char *a_output)
{
   /*---(locals)-----------+-----------+-*/
   int         rce         =  -10;
   char        rc          =    0;
   int         x_rpid      =   -1;
   FILE       *f           = NULL;
   char        msg         [200];
   long        now         =    0;
   tTIME      *curr_time   = NULL;
   int         i           =    0;
   /*---(validate)-----------------------*/
   rc = yexec__validate (a_title, a_user, a_cmd, a_shell, a_path, a_output);
   if (rc < 0)        return rc;
   /*---(fork)---------------------------*/
   x_rpid = yexec__fork (a_title, a_fork);
   if (x_rpid != 0)   return x_rpid;
   /*---(output)-------------------------*/
   f = fopen (s_output, "a");
   fprintf(f, "========================================================================begin===\n");
   /*---(get the date)----------------*/
   now       = time      (NULL);
   curr_time = localtime (&now);
   strftime (msg, 50, "%Ss %Mm %Hh %dd %mm  %ww", curr_time);
   fprintf  (f, "start     : %s\n",   msg);
   /*---(set up environment)-------------*/
   rc = yexec__environ (f, a_user);
   --rce;  if (rc < 0) {
      fclose (f);
      _exit (rce);    /* must use _exit to get out properly                       */
   }
   /*---(try execve)----------------------------*/
   fprintf   (f, "execve    : %.50s\n", a_cmd);
   fprintf   (f, "==========================================================================end===\n");
   fflush    (f);
   fclose    (f);
   /*> rc = execve (*s_argv, s_argv, s_envp);                                         <*/
   rc = execve (*s_argv, s_argv, NULL  );
   /*---(try execvp)----------------------------*/
   f = fopen (s_output, "a");
   fprintf   (f, "FAILED    : %s\n", strerror (errno));
   fprintf   (f, "execvp    : %.50s\n", a_cmd);
   fprintf   (f, "==========================================================================end===\n");
   fflush    (f);
   fclose    (f);
   rc = execvp (*s_argv, s_argv);
   /*---(try execl)-----------------------------*/
   f = fopen (s_output, "a");
   fprintf   (f, "FAILED execvp, fallback...\n");
   fprintf   (f, "execl     : %.50s\n", a_cmd);
   fprintf   (f, "==========================================================================end===\n");
   fflush    (f);
   fclose    (f);
   rc = execl (s_shell, s_shell, "-c", a_cmd, NULL, NULL);
   /*---(failure)-------------------------------*/
   f = fopen (s_output, "a");
   fprintf   (f, "FAILED execl, just won't run\n");
   fprintf   (f, "==========================================================================end===\n");
   fflush    (f);
   fclose    (f);
   _exit (-3);    /* must use _exit to get out properly                       */
}

char         /*--> verify status of a running job --------[ ------ [ ------ ]-*/
yEXEC_check        (char *a_title, int a_rpid, int *a_rc)
{
   /*---(locals)-------------------------*/
   int       rc        =    0;
   int       x_status  =    0;
   int       x_return  =    0;
   int       x_signal  =    0;
   /*---(output header)-----------------*/
   DEBUG_YEXEC  yLOG_senter  ("CHK");
   DEBUG_YEXEC  yLOG_snote   (a_title);
   DEBUG_YEXEC  yLOG_svalue  ("pid"       , a_rpid);
   if (a_rc != NULL)  *a_rc = 0;
   /*---(check status)------------------*/
   /*> rc = wait4 (a_rpid, &x_status, WNOHANG, NULL);                                 <*/
   rc = waitpid (a_rpid, &x_status, WNOHANG);
   DEBUG_YEXEC  yLOG_svalue  ("waitpid"   , rc);
   /*---(handle running)----------------*/
   if (rc ==  0) {
      DEBUG_YEXEC  yLOG_snote   ("still running");
      DEBUG_YEXEC  yLOG_sexit   ("CHK");
      return YEXEC_RUNNING;
   }
   /*---(handle terminated)-------------*/
   if (!WIFEXITED (x_status)) {
      if (WIFSIGNALED (x_status)) {
         x_signal = WTERMSIG (x_status);
         if (a_rc != NULL)  *a_rc = x_signal;
         switch (x_signal) {
         case SIGTERM    : case SIGKILL    :
            DEBUG_YEXEC  yLOG_snote   ("TERMINATED");
            DEBUG_YEXEC  yLOG_sexit   ("CHK");
            return YEXEC_KILLED;
            break;
         default   :
            DEBUG_YEXEC  yLOG_snote   ("DIED");
            DEBUG_YEXEC  yLOG_sexit   ("CHK");
            return YEXEC_DIED;
         }
      }
      if (a_rc != NULL)  *a_rc = -1;
      DEBUG_YEXEC  yLOG_snote   ("ERROR");
      DEBUG_YEXEC  yLOG_sexit   ("CHK");
      return YEXEC_ERROR;
   }
   /*---(gather return code)------------*/
   DEBUG_YEXEC  yLOG_snote   ("exited");
   x_return = WEXITSTATUS (x_status);
   DEBUG_YEXEC  yLOG_svalue  ("status"    , x_return);
   if (a_rc != NULL)  *a_rc = x_return;
   /*---(handle launch failures)--------*/
   if (x_return == 127) {  /* command not found from linux */
      DEBUG_YEXEC  yLOG_snote   ("boom");
      DEBUG_YEXEC  yLOG_sexit   ("CHK");
      return YEXEC_ERROR;
   }
   /*---(handle completed)--------------*/
   if (x_return == 0) {
      DEBUG_YEXEC  yLOG_snote   ("normal");
      DEBUG_YEXEC  yLOG_sexit   ("CHK");
      return YEXEC_NORMAL;
   }
   /*---(handle errors)-----------------*/
   if (x_return >  127) {  /* negative returns from linux uchar return */
      DEBUG_YEXEC  yLOG_snote   ("FAILURE");
      if (a_rc != NULL)  *a_rc = x_return - 256;
      DEBUG_YEXEC  yLOG_sexit   ("CHK");
      return YEXEC_FAILURE;
   }
   /*---(handle warnings)---------------*/
   DEBUG_YEXEC  yLOG_snote   ("warning");
   DEBUG_YEXEC  yLOG_sexit   ("CHK");
   return YEXEC_WARNING;
}


char             /* [------] find a running job by name ----------------------*/
yEXEC_find         (char *a_name, int *a_rpid)
{
   /*---(locals)-----------+-----------+-*/
   tDIRENT    *den;
   DIR        *dir;
   FILE       *f;
   char        name        [100];
   char        title       [1000];
   char        line        [1000];
   int         rc;
   char       *p;
   int         status;
   int         rpid        = -1;
   int       x_status  = 0;                       /* the line's job status    */
   char        count       = 0;
   /*---(open the proc system)-----------*/
   dir = opendir("/proc");
   if (a_rpid != NULL)  *a_rpid = 9999;
   if (dir == NULL) return -1;
   /*---(cycle through the entries)------*/
   while ((den = readdir(dir)) != NULL) {
      /*---(filter non-processes)--------*/
      if (atoi(den->d_name)     == 0)  continue;
      /*---(open the process)------------*/
      sprintf(name, "/proc/%s/stat", den->d_name);
      f = fopen (name, "r");
      if (f == NULL) continue;
      /*---(read the entry)--------------*/
      fgets (line, 1000, f);
      p = strtok (line, "(");
      p = strtok (NULL, ")");
      strcpy (title, p);
      /*> printf ("title = <<%s>>, name = <<%s>> \n", title, a_name);                 <*/
      fclose (f);
      /*---(verify)----------------------*/
      if (strcmp (title, a_name) != 0)  continue;
      ++count;
      rpid =  atoi (den->d_name);
   }
   closedir(dir);
   if (a_rpid != NULL)  *a_rpid = rpid;
   return count;
}



/*====================------------------------------------====================*/
/*===----                      unit test accessor                      ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

static char   u_time     [LEN_RECD];
static char   u_user     [LEN_RECD];
static char   u_home     [LEN_RECD];
static char   u_shell    [LEN_RECD];
static char   u_path     [LEN_RECD];
static char   u_gid      [LEN_RECD];
static char   u_uid      [LEN_RECD];
static char   u_chdir    [LEN_RECD];

char*            /*--> unit test accessor ------------------------------*/
yexec_proc__unit        (char *a_question, int a_num)
{
   /*---(locals)-----------+-----+-----+-*/
   char        t           [LEN_RECD]  = "[]";
   int         c           =    0;
   /*---(prepare)------------------------*/
   strncpy  (unit_answer, "FILE             : question not understood", LEN_RECD);
   /*---(crontab name)-------------------*/
   if      (strcmp (a_question, "time"    )        == 0) {
      sprintf (t, "[%s]", u_time);
      snprintf (unit_answer, LEN_RECD, "PROC time        : %2d%s", strlen (u_time), t);
   }
   else if (strcmp (a_question, "user"    )        == 0) {
      sprintf (t, "[%s]", u_user);
      snprintf (unit_answer, LEN_RECD, "PROC user        : %2d%s", strlen (u_user), t);
   }
   else if (strcmp (a_question, "home"    )        == 0) {
      sprintf (t, "[%s]", u_home);
      snprintf (unit_answer, LEN_RECD, "PROC home        : %2d%s", strlen (u_home), t);
   }
   else if (strcmp (a_question, "shell"   )        == 0) {
      sprintf (t, "[%s]", u_shell);
      snprintf (unit_answer, LEN_RECD, "PROC shell       : %2d%s", strlen (u_shell), t);
   }
   else if (strcmp (a_question, "path"    )        == 0) {
      if (strlen (u_path) < 40)  sprintf (t, "[%s]", u_path);
      else                       sprintf (t, "[%-40.40s>", u_path);
      snprintf (unit_answer, LEN_RECD, "PROC path        : %2d%s", strlen (u_path), t);
   }
   else if (strcmp (a_question, "uid"     )        == 0) {
      sprintf (t, "[%s]", u_uid);
      snprintf (unit_answer, LEN_RECD, "PROC uid         : %2d%s", strlen (u_uid), t);
   }
   else if (strcmp (a_question, "gid"     )        == 0) {
      sprintf (t, "[%s]", u_gid);
      snprintf (unit_answer, LEN_RECD, "PROC gid         : %2d%s", strlen (u_gid), t);
   }
   else if (strcmp (a_question, "chdir"   )        == 0) {
      sprintf (t, "[%s]", u_chdir);
      snprintf (unit_answer, LEN_RECD, "PROC chdir       : %2d%s", strlen (u_chdir), t);
   }
   else if (strcmp (a_question, "exec"    )        == 0) {
      sprintf (t, "[%s]", *s_argv);
      snprintf (unit_answer, LEN_RECD, "PROC exec        : %2d%s", strlen (*s_argv), t);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}

char
yexec_proc__unit_read         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   FILE       *f           = NULL;
   char        c           =    0;
   char        x_recd      [LEN_RECD];
   int         x_len       =    0;
   char        x_cmd       [LEN_RECD];
   /*---(chmod)--------------------------*/
   sprintf (x_cmd, "chmod 0777 %s", YEXEC_UNIT);
   system  (x_cmd);
   /*---(open)---------------------------*/
   f = fopen (YEXEC_UNIT, "rt");
   --rce; if (f == NULL)  return rce;
   /*---(prepare)------------------------*/
   strncpy (u_time  , "", LEN_RECD);
   strncpy (u_user  , "", LEN_RECD);
   strncpy (u_home  , "", LEN_RECD);
   strncpy (u_shell , "", LEN_RECD);
   strncpy (u_path  , "", LEN_RECD);
   strncpy (u_gid   , "", LEN_RECD);
   strncpy (u_uid   , "", LEN_RECD);
   strncpy (u_chdir , "", LEN_RECD);
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
      }
      ++c;
   }
   /*---(close)--------------------------*/
   fclose (f);
   /*---(erase)--------------------------*/
   sprintf (x_cmd, "rm -f %s", YEXEC_UNIT);
   system  (x_cmd);
   /*---(complete)-----------------------*/
   return 0;
}




