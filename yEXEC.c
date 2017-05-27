/*============================[[    beg-code    ]]============================*/
#include   "yEXEC.h"
#include   "yEXEC_priv.h"


/*---(globals)----------+-----------+----*/
char        s_cmd       [2000];
char       *s_argv      [20];
int         s_argc      = 0;
char        bulletproof = 'n';
char        interactive = 'n';
char        children    = 'n';
char        local       = 'n';


char        yEXEC_ver [200] = "";



/*====================------------------------------------====================*/
/*===----                           utility                            ----===*/
/*====================------------------------------------====================*/
static void      o___UTILITY_________________o (void) {;}

char*        /*--> return library versioning info --------[ leaf-- [ ------ ]-*/
yEXEC_version      (void)
{
   char        t [20] = "";
#if    __TINYC__ > 0
   strncpy (t, "[tcc built  ]", 15);
#elif  __GNUC__  > 0
   strncpy (t, "[gnu gcc    ]", 15);
#elif  __HEPH__  > 0
   strncpy (t, "[hephaestus ]", 18);
#else
   strncpy (t, "[unknown    ]", 15);
#endif
   snprintf (yEXEC_ver, 100, "%s   %s : %s", t, YEXEC_VER_NUM, YEXEC_VER_TXT);
   return yEXEC_ver;
}



/*====================------------------------------------====================*/
/*===----                            basics                            ----===*/
/*====================------------------------------------====================*/
static void      o___BASICS__________________o (void) {;}

char         /*--> break string into argv structure ------[ leaf-- [ ------ ]-*/
yEXEC__parse       (const char *a_cstring)
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
   for (i = 0; i < 20; ++i) s_argv[i] = NULL;
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
   /*---(complete)-------------------------*/
   return 0;
}

int              /* [------] execute a specific command ----------------------*/
yEXEC_run          (
      /*---(params)-----------+---+---------*/
      char       *a_output    ,   /* file to capture execution entry          */
      char       *a_title     ,   /* short title                              */
      char       *a_user      ,   /* user name                                */
      char       *a_cmd       ,   /* command to execute                       */
      char       *a_shell     ,   /* shell to run under                       */
      char       *a_path      ,   /* path to use                              */
      char        a_fork      )   /* fork new or use existing process space   */
{
   /*---(begin)--------------------------*/
   yLOG_senter ("RUN");
   /*---(locals)-----------+-----------+-*/
   int         rc          = 0;             /* simple return code             */
   int         rpid        = 0;             /* child pid for execution        */
   int         ppid        = 0;             /* parent pid                     */
   FILE       *f           = NULL;          /* file for logging exec details  */
   char        msg         [200];           /* output string for messages     */
   long        now;                         /* present datetime               */
   tTIME      *curr_time   = NULL;
   tPASSWD    *pass;                        /* retrieving password            */
   char        envp        [10][200];
   int         i           = 0;
   /*---(header)--------------------------------*/
   yLOG_snote (a_title);
   yLOG_snote ("fork?");
   yLOG_schar (a_fork);
   /*---(fork off to execute)-------------------*/
   if (a_fork == yEXEC_FORK || a_fork == yEXEC_DETACH) {
      yLOG_snote ("foad");
      rpid = fork    ();
      ppid = getppid ();
      if (rpid < 0) {                        /* error                            */
         yLOG_svalue ("pid", rpid);
         yLOG_snote  ("FAILURE");
         yLOG_sexit  ("RUN");
         return  -1;
      }
      if (rpid > 0) {
         yLOG_svalue ("pid", rpid);
         yLOG_snote  ("SUCCESS");
         yLOG_snote  ("parent closing");
         yLOG_sexit  ("RUN");
         return rpid;
      }
   } else {
      yLOG_snote  ("REPLACE");
      yLOG_snote  ("closing logger");
      yLOG_end    ();   /* stop logging so the next can take over */
   }
   /*---(display header for debugging)----------*/
   f      = fopen (a_output, "a");
   fprintf(f, "========================================================================begin===\n");
   /*---(get the date)-----------------------*/
   now       = time      (NULL);
   curr_time = localtime (&now);
   strftime (msg, 50, "%Ss %Mm %Hh %dd %mm  %ww", curr_time);
   fprintf  (f, "start     : %s\n",   msg);
   /*---(get user information)------------------*/
   pass = getpwnam (a_user);
   if (pass == NULL) {
      fprintf  (f, "could not open password file");
      fclose (f);
      exit   (-2);
   }
   /*---(set execution environment)-------------*/
   rc = setenv ("USER",                 pass->pw_name, 1);
   snprintf (envp[0], 200, "USER=%s",   pass->pw_name);
   fprintf  (f, "USER set  : %s\n", pass->pw_name);
   if (rc <  0) {
      fclose (f);
      exit   (-3);
   }
   rc = setenv ("HOME",                 pass->pw_dir,  1);
   snprintf (envp[1], 200, "HOME=%s",   pass->pw_dir);
   fprintf  (f, "HOME set  : %s\n", pass->pw_dir);
   if (rc <  0) {
      fclose (f);
      exit   (-3);
   }
   rc = setenv ("SHELL",                a_shell, 1);
   snprintf(envp[2], 200, "SHELL=%s",  a_shell);
   fprintf(f, "SHELL set : %s\n", a_shell);
   if (rc <  0) {
      fclose (f);
      exit (-3);
   }
   rc = setenv ("PATH",                 a_path, 1);
   snprintf (envp[3], 200, "PATH=%s",   a_path);
   fprintf  (f, "PATH set  : %s\n", a_path);
   if (rc <  0) {
      fclose (f);
      exit   (-3);
   }
   /*---(set permissions)-----------------------*/
   rc = initgroups (a_user, pass->pw_gid);
   if (rc <  0) {
      fclose (f);
      exit (-3);
   }
   rc = setregid(pass->pw_gid,         pass->pw_gid);
   snprintf  (envp[4], 200, "GID=%d",    pass->pw_gid);
   fprintf   (f, "GID set   : %d\n", pass->pw_gid);
   if (rc <  0) {
      fclose (f);
      return (-3);
   }
   rc = setreuid(pass->pw_uid, pass->pw_uid);
   snprintf  (envp[5], 200, "UID=%d",    pass->pw_uid);
   fprintf   (f, "UID set   : %d\n", pass->pw_uid);
   if (rc <  0) {
      fclose (f);
      return (-3);
   }
   envp[6][0] = '\0';
   /*---(set current dir)-----------------------*/
   rc = chdir(pass->pw_dir);
   fprintf   (f, "chdir     : %s\n", pass->pw_dir);
   if (rc <  0) {
      fclose (f);
      return (-3);
   }
   /*---(try direct execution)------------------*/

   /*> sprintf (run, "%s > /dev/null", a_proc->run);                                  <*/
   /*> sprintf (run, "%s", a_proc->run);                                                        <* 
    *> fprintf(output, "execvp    : %.100s\n", run);                                            <* 
    *> fprintf(output, "===-----------------------------===\n");                                <* 
    *> fflush (output);                                                                         <* 
    *> fclose (output);                                                                         <* 
    *> char    backup2[LEN_CMD];                                                                <* 
    *> strncpy(backup2, run, LEN_CMD);                                                          <* 
    *> /+> str_parse (backup2);                                                           <+/   <* 
    *> strlargs   (backup2, LEN_CMD, 20, &my.argc, my.argv);                                    <* 
    *> rc = execvp (*my.argv, my.argv);                                                         <*/


   fprintf   (f, "execvp    : %.50s\n", a_cmd);
   yEXEC__parse (a_cmd);
   for (i = 0; i < 20; ++i) {
      if (s_argv [i] == NULL) break;
      fprintf   (f, "arg %02d    : %.50s\n", i, s_argv [i]);
   }
   fprintf   (f, "execvp    : %.50s\n", a_cmd);
   fprintf   (f, "==========================================================================end===\n");
   fflush    (f);
   fclose    (f);
   rc = execvp (*s_argv, s_argv);
   /*---(close log)-----------------------------*/
   f = fopen (a_output, "a");
   fprintf   (f, "FAILED execvp, fallback...\n");
   fprintf   (f, "execl     : %.50s\n", a_cmd);
   fprintf   (f, "==========================================================================end===\n");
   fflush    (f);
   fclose    (f);
   rc = execl(a_shell, a_shell, "-c", a_cmd, NULL, NULL);
   /*---(this should never come back)-----------*/
   f = fopen (a_output, "a");
   fprintf   (f, "FAILED execl, just won't run\n");
   fprintf   (f, "==========================================================================end===\n");
   fflush    (f);
   fclose    (f);
   _exit (-3);    /* must use _exit to get out properly                       */
}

char         /*--> verify status of a running job --------[ ------ [ ------ ]-*/
yEXEC_check        (char *a_title, int a_rpid)
{
   yLOG_senter  ("CHK");
   /*---(locals)-------------------------*/
   int       rc        = 0;                       /* return code              */
   int       x_status  = 0;                       /* the line's job status    */
   int       x_return  = 0;                       /* the line's return code   */
   /*---(output header)-----------------*/
   yLOG_snote   (a_title);
   yLOG_svalue  ("pid"       , a_rpid);
   /*---(check status)------------------*/
   /*> rc = wait4 (a_rpid, &x_status, WNOHANG, NULL);                                 <*/
   rc = waitpid (a_rpid, &x_status, WNOHANG);
   yLOG_svalue  ("waitpid"   , rc);
   /*---(handle running)----------------*/
   if (rc ==  0) {
      yLOG_snote   ("still running");
      yLOG_sexit   ("CHK");
      return 1;
   }
   /*---(handle terminated)-------------*/
   if (!WIFEXITED (x_status)) {
      yLOG_snote   ("TERMINATED");
      yLOG_sexit   ("CHK");
      return 0;
   }
   /*---(gather return code)------------*/
   yLOG_snote   ("exited");
   x_return = WEXITSTATUS (x_status);
   yLOG_svalue  ("status"    , x_return);
   /*---(handle completed)--------------*/
   if (x_return == 0) {
      yLOG_snote   ("normal");
      yLOG_sexit   ("CHK");
      return 0;
   }
   /*---(handle warning)----------------*/
   if (x_return >  0) {
      yLOG_snote   ("warning");
      yLOG_sexit   ("CHK");
      return 0;
   }
   /*---(handle failure)----------------*/
   yLOG_snote   ("FAILURE");
   yLOG_sexit   ("CHK");
   return -1;
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
/*===----                           signals                            ----===*/
/*====================------------------------------------====================*/
static void      o___SIGNALS_________________o (void) {;}

char       /*lf--: PURPOSE : exit on termintation/signal ---------------------*/
yEXEC_term         (const char *a_func, const int a_exit)
{
   /*---(log)----------------------------*//*===fat=beg===*/
   if (strncmp(a_func, "", 1) != 0) yLOG_exit  (a_func);
   yLOG_end   ();
   /*---(check for harsh exit)-----------*//*===fat=end===*/
   if (a_exit > 0) exit(a_exit);
   /*---(complete)-----------------------*/
   return 0;
}

void             /* [------] receive signals ---------------------------------*/
yEXEC__comm        (int a_signal, siginfo_t *a_info, void *a_nada)
{
   /*---(locals)-----------+-----------+-*/
   int         status      = 0;
   int         xlink       = 0;
   int         rc          = 0;
   int         errsave     = 0;
   char        rchar       = 0;
   switch (a_signal) {
   case  SIGCHLD:
      yLOG_senter("sigchld");
      yLOG_spoint((void *) a_info);
      yLOG_svalue("pid"     , (int) a_info->si_pid);
      rchar = a_info->si_status;
      yLOG_svalue("rc"      , (int) rchar);
      yLOG_snote ("wait");
      rc = waitpid (a_info->si_pid, &status, WNOHANG);
      yLOG_svalue("wait_rc" , rc);
      yLOG_snote ("done");
      yLOG_sexit ("sigchld");
      break;
   case  SIGHUP:
      yLOG_info  ("SIGNAL", "SIGHUP MEANS REFRESH");
      break;
   case  SIGUSR1:
      yLOG_info  ("SIGNAL", "SIGUSR1 means user message");
      break;
   case  SIGUSR2:
      yLOG_info  ("SIGNAL", "SIGUSR2 means user message");
      break;
   case  SIGTERM:
      if (bulletproof != 'y') {
         yLOG_info  ("SIGNAL", "SIGTERM means terminate daemon");
         yLOG_end   ();
         if (local == 'y')  PROG_signal (a_signal, a_info, a_nada);
         exit(-1);
      }
      yLOG_info  ("SIGNAL", "SIGTERM means terminate daemon, but this one must not");
      break;
   case  SIGSEGV:
      if (bulletproof != 'y') {
         yLOG_info  ("SIGNAL", "SIGSEGV means daemon blew up");
         yLOG_end   ();
         if (local == 'y')  PROG_signal (a_signal, a_info, a_nada);
         exit(-1);
      }
      yLOG_info  ("SIGNAL", "SIGTERM means terminate daemon, but this one must not");
      break;
   default:
      yLOG_info  ("SIGNAL", "UNKNOWN, signal received, but not handled");
      break;
   }
   if (local == 'y')  PROG_signal (a_signal, a_info, a_nada);
   /*---(complete)------------------------------*/
   return;
}

char             /* [------] setup signal handling ---------------------------*/
yEXEC_signal       (
      char a_bulletproof,    /* Y/N ignores terminate and segfault            */
      char a_interactive,    /* Y/N reacts to terminal keyboard and jobs      */
      char a_children,       /* Y/N hear about children terminating           */
      char a_local)          /* Y/N program has its own handler as well       */
{
   /*---(locals)-----------+-----------+-*/
   tSIGACT     sa;
   int         rc          = 0;
   /*---(begin)--------------------------*/
   yLOG_enter  (__FUNCTION__);
   bulletproof  = a_bulletproof;
   yLOG_char   ("bullet"    , bulletproof);
   interactive  = a_interactive;
   yLOG_char   ("interact"  , interactive);
   children     = a_children;
   yLOG_char   ("children"  , children);
   local        = a_local;
   yLOG_char   ("local"     , local);
   /*---(start with ignores)-------------*/
   sa.sa_handler   = SIG_IGN;
   sa.sa_sigaction = NULL;
   sa.sa_flags     = SA_RESTART;
   sigfillset(&sa.sa_mask);
   sa.sa_restorer  = NULL;
   if (interactive != 'y') {
      yLOG_info  ("keyboard"  , "ignore INT, QUIT, CONT, and STOP");
      rc  = 0;
      rc += sigaction(SIGINT ,  &sa      , NULL);       /* keyboard interrupt          */
      rc += sigaction(SIGQUIT,  &sa      , NULL);       /* keyboard quit               */
      rc += sigaction(SIGCONT,  &sa      , NULL);       /* keyboard continue           */
      rc += sigaction(SIGTSTP,  &sa      , NULL);       /* keyboard stop               */
      if (rc != 0) {
         yLOG_info  ("warning"   , "at least one sigaction returned error");
         yLOG_value ("returned"  , rc);
      }
   }
   if (interactive != 'y') {
      yLOG_info  ("terminal"  , "ignore TIN and TOUT");
      rc  = 0;
      rc += sigaction(SIGTTIN,  &sa      , NULL);       /* tty input for background    */
      rc += sigaction(SIGTTOU,  &sa      , NULL);       /* tty output for background   */
      if (rc != 0) {
         yLOG_info  ("warning"   , "at least one sigaction returned error");
         yLOG_value ("returned"  , rc);
      }
   }
   if (children != 'y' && children != 'a') {
      yLOG_info  ("children"  , "ignore children, CHLD");
      rc  = 0;
      rc += sigaction(SIGCHLD,  &sa      , NULL);    /* ignore children             */
      if (rc != 0) {
         yLOG_info  ("warning"   , "at least one sigaction returned error");
         yLOG_value ("returned"  , rc);
      }
   }
   /*---(then to defaults)---------------*/
   sa.sa_handler   = SIG_DFL;
   sa.sa_sigaction = NULL;
   sa.sa_flags     = SA_RESTART;
   if (interactive == 'y') {
      yLOG_info  ("keyboard"  , "default INT, QUIT, CONT, and STOP");
      rc  = 0;
      rc += sigaction(SIGINT ,  &sa      , NULL);       /* keyboard interrupt          */
      rc += sigaction(SIGQUIT,  &sa      , NULL);       /* keyboard quit               */
      rc += sigaction(SIGCONT,  &sa      , NULL);       /* keyboard continue           */
      rc += sigaction(SIGTSTP,  &sa      , NULL);       /* keyboard stop               */
      if (rc != 0) {
         yLOG_info  ("warning"   , "at least one sigaction returned error");
         yLOG_value ("returned"  , rc);
      }
   }
   if (interactive == 'y') {
      yLOG_info  ("terminal"  , "default TIN and TOUT");
      rc  = 0;
      rc += sigaction(SIGTTIN,  &sa      , NULL);       /* tty input for background    */
      rc += sigaction(SIGTTOU,  &sa      , NULL);       /* tty output for background   */
      if (rc != 0) {
         yLOG_info  ("warning"   , "at least one sigaction returned error");
         yLOG_value ("returned"  , rc);
      }
   }
   /*---(then to handles)----------------*/
   sa.sa_handler   = NULL;
   sa.sa_sigaction = yEXEC__comm;
   sa.sa_flags     = SA_RESTART | SA_SIGINFO;
   sigfillset(&sa.sa_mask);
   sa.sa_restorer  = NULL;
   if (children == 'y' || children == 'a') {
      yLOG_info  ("children", "pay attention to children, CHLD");
      rc += sigaction(SIGCHLD,  &sa      , NULL);       /* watch for children          */
   }
   yLOG_info  ("program",  "look for HUP, USR1, USR2, and ALRM");
   rc  = 0;
   rc += sigaction(SIGHUP ,  &sa      , NULL);       /* hangup means refresh        */
   rc += sigaction(SIGUSR1,  &sa      , NULL);       /* custom (minor)              */
   rc += sigaction(SIGUSR2,  &sa      , NULL);       /* custom (major)              */
   rc += sigaction(SIGALRM,  &sa      , NULL);       /* alarm                       */
   if (rc != 0) {
      yLOG_info  ("warning"   , "at least one sigaction returned error");
      yLOG_value ("returned"  , rc);
   }
   yLOG_info  ("baddies",  "handle SEGV, TERM, and ABRT");
   rc  = 0;
   rc += sigaction(SIGTERM,  &sa      , NULL);       /* catch a kill                */
   rc += sigaction(SIGSEGV,  &sa      , NULL);       /* catch a segfault            */
   rc += sigaction(SIGABRT,  &sa      , NULL);       /* catch an abort              */
   if (rc != 0) {
      yLOG_info  ("warning"   , "at least one sigaction returned error");
      yLOG_value ("returned"  , rc);
   }
   /*---(complete)-----------------------*/
   yLOG_exit  (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                           special                            ----===*/
/*====================------------------------------------====================*/
static void      o___SPECIAL_________________o (void) {;}

char             /* [------] daemonize the program ---------------------------*/
yEXEC_daemon       (int a_logger, int *a_rpid)
{
   yLOG_enter (__FUNCTION__);
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
      yLOG_info   ("foad"      , "fork off and die");
      yLOG_value  ("attempt"   , count);
      rpid = fork();
      if (rpid < 0) {         /* error               */
         yLOG_info   ("fork"      , "creation of child FAILED");
         yEXEC_term  (__FUNCTION__, 1);
      }
      if (rpid > 0) {         /* parent process      */
         yLOG_info   ("PARENT"    , "exiting parent");
         exit(0);
      }
      /*---(wait for parent to die)------*/
      rc = wait4 (rpid, &status, 0, NULL);
      yLOG_value   ("wait4_rc"  , rc);
      if        (rc <  0) {
         yLOG_note    ("wait4 returned an error");
      } else if (rc == 0) {
         yLOG_note    ("wait4 returned an zero");
      } else if (rc == rpid) {
         yLOG_note    ("wait4 found parent has changed status");
      } else {
         yLOG_note    ("wait4 returned unknown error");
      }
      /*---(fix the umask)---------------*/
      yLOG_info    ("umask"     , "reset the default file permissions");
      umask(0);
      /*---(close off all descriptors)---*/
      yLOG_info   ("fds"       , "close all inherited file descriptors");
      for (i = 0; i < 256; ++i) {
         if (i == a_logger) continue;
         close(i);
      }
      /*---(std fds to the bitbucket)---*/
      yLOG_info   ("std fds"   , "redirect stdin, stdout, and stderr to /dev/null");
      fd = open("/dev/null", O_RDWR);
      if (fd < 0) {
         yLOG_info   ("fds"       , "creation of safe fd FAILED");
         yEXEC_term  (__FUNCTION__, 2);
      }
      dup2(fd, 0);
      dup2(fd, 1);
      dup2(fd, 2);
      /*---(obtain a new process group)--*/
      yLOG_info   ("session"   , "create a new process/session");
      sid = setsid();
      if (sid < 0) {
         yLOG_info   ("sid"       , "creation FAILED");
         yEXEC_term  (__FUNCTION__, 3);
      }
      yLOG_value ("new sid",  sid);
      /*---(record process info)---------*/
      rpid  = getpid();
      ppid  = getppid();
      yLOG_value ("new pid"    , rpid);
      yLOG_value ("new ppid"   , ppid);
      /*---(check for success)-----------*/
      if (ppid == 1) {
         yLOG_info  ("ppid"       , "owned by init, success");
         break;
      }
      yLOG_info  ("FAIL"       , "not owned by init, failure");
      /*---(done)------------------------*/
   }
   /*---(final message)------------------*/
   if (ppid != 1) {
      yLOG_info  ("FAIL"       , "too many attempts");
   }
   /*---(complete)-----------------------*/
   if (a_rpid != NULL)  *a_rpid = rpid;
   yLOG_exit  (__FUNCTION__);
   return 0;
}

/*> int              /+ [------] execute a specific command and detach -----------+/    <* 
 *> yEXEC_detach       (                                                                <* 
 *>       char       *a_output,       /+ file to capture execution entry          +/    <* 
 *>       char       *a_title,        /+ short title                              +/    <* 
 *>       char       *a_user,         /+ user name                                +/    <* 
 *>       char       *a_cmd,          /+ command to execute                       +/    <* 
 *>       char       *a_shell,        /+ shell to run under                       +/    <* 
 *>       char       *a_path)         /+ path to use                              +/    <* 
 *> {                                                                                   <* 
 *>    yLOG_enter  (__FUNCTION__);                                                      <* 
 *>    /+---(locals)-----------+-----------+-+/                                         <* 
 *>    int         i           = 0;             /+ loop iterator                  +/    <* 
 *>    int         fd          = 0;             /+ file descriptor                +/    <* 
 *>    int         status      = 0;                                                     <* 
 *>    int         rpid        = 0;                                                     <* 
 *>    int         ppid        = 0;                                                     <* 
 *>    int         sid         = 0;                                                     <* 
 *>    int         tries       = 0;                                                     <* 
 *>    /+---(existing information)-----------+/                                         <* 
 *>    rpid  = getpid();                                                                <* 
 *>    ppid  = getppid();                                                               <* 
 *>    yLOG_value  ("curr pid"  , rpid);                                                <* 
 *>    yLOG_value  ("curr ppid" , ppid);                                                <* 
 *>    /+---(fork off and die)---------------+/                                         <* 
 *>    yLOG_info   ("foad"      , "fork off and die");                                  <* 
 *>    rpid = fork();                                                                   <* 
 *>    if (rpid < 0) {         /+ error               +/                                <* 
 *>       yLOG_info   ("fork"      ,   "creation of child FAILED");                     <* 
 *>       yEXEC_term  (__FUNCTION__, 1);                                                <* 
 *>    }                                                                                <* 
 *>    if (rpid > 0) {         /+ parent process      +/                                <* 
 *>       yLOG_info   ("PARENT"    , "exiting parent");                                 <* 
 *>       exit(0);                                                                      <* 
 *>    }                                                                                <* 
 *>    wait4 (rpid, &status, 0, NULL);                                                  <* 
 *>    /+---(obtain a new process group)------------+/                                  <* 
 *>    yLOG_info   ("session"   , "create a new process/session");                      <* 
 *>    sid = setsid();                                                                  <* 
 *>    if (sid < 0) {                                                                   <* 
 *>       yLOG_info   ("sid"       , "creation FAILED");                                <* 
 *>       yEXEC_term  (__FUNCTION__, 3);                                                <* 
 *>    }                                                                                <* 
 *>    yLOG_value  ("new sid",  sid);                                                   <* 
 *>    /+---(record process info)-------------------+/                                  <* 
 *>    rpid  = getpid();                                                                <* 
 *>    ppid  = getppid();                                                               <* 
 *>    yLOG_value  ("new pid"   , rpid);                                                <* 
 *>    yLOG_value  ("new ppid"  , ppid);                                                <* 
 *>    /+---(check for owner)-----------------------+/                                  <* 
 *>    while (ppid != 1) {                                                              <* 
 *>       /+---(fork off and die)----------------------+/                               <* 
 *>       yLOG_info("foad"      , "fork off and die again");                            <* 
 *>       rpid = fork();                                                                <* 
 *>       if (rpid < 0) {         /+ error               +/                             <* 
 *>          yLOG_info   ("fork"      , "creation of child FAILED");                    <* 
 *>          yEXEC_term  (__FUNCTION__, 1);                                             <* 
 *>       }                                                                             <* 
 *>       if (rpid > 0) {         /+ parent process      +/                             <* 
 *>          yLOG_info   ("PARENT"    , "exiting parent");                              <* 
 *>          exit(0);                                                                   <* 
 *>       }                                                                             <* 
 *>       wait4 (rpid, &status, 0, NULL);                                               <* 
 *>       rpid  = getpid();                                                             <* 
 *>       ppid  = getppid();                                                            <* 
 *>       yLOG_value ("and pid"   , rpid);                                              <* 
 *>       yLOG_value ("and ppid"  , ppid);                                              <* 
 *>       ++tries;                                                                      <* 
 *>       if (tries >= 5) break;                                                        <* 
 *>    }                                                                                <* 
 *>    if (ppid != 1) {                                                                 <* 
 *>       yLOG_info   ("ppid",  "not owned by init, FAILED");                           <* 
*>       yEXEC_term  (__FUNCTION__, 5);                                                <* 
*>    }                                                                                <* 
*>    yLOG_info   ("ppid",  "owned by init, success");                                 <* 
*>    /+---(exec)----------------------------------+/                                  <* 
*>    yEXEC_run   (a_output, a_title, a_user, a_cmd, a_shell, a_path, yEXEC_NOFORK);   <* 
*>    /+---(complete)------------------------------+/                                  <* 
*>    yLOG_exit   (__FUNCTION__);                                                      <* 
*>    return 0;                                                                        <* 
*> }                                                                                   <*/


/*============================[[    end-code    ]]============================*/
