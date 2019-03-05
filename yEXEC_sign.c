/*============================[[    beg-code    ]]============================*/
#include   "yEXEC.h"
#include   "yEXEC_priv.h"



static char     s_bulletproof = 'n';
static char     s_interactive = 'n';
static char     s_children    = 'n';
static void   (*s_signaler)   (int a_signal, siginfo_t *a_info, void *a_nada);



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
      DEBUG_YEXEC  yLOG_senter("sigchld");
      DEBUG_YEXEC  yLOG_spoint((void *) a_info);
      DEBUG_YEXEC  yLOG_svalue("pid"     , (int) a_info->si_pid);
      rchar = a_info->si_status;
      DEBUG_YEXEC  yLOG_svalue("rc"      , (int) rchar);
      DEBUG_YEXEC  yLOG_snote ("wait");
      rc = waitpid (a_info->si_pid, &status, WNOHANG);
      DEBUG_YEXEC  yLOG_svalue("wait_rc" , rc);
      DEBUG_YEXEC  yLOG_snote ("done");
      DEBUG_YEXEC  yLOG_sexit ("sigchld");
      break;
   case  SIGHUP:
      DEBUG_YEXEC  yLOG_info  ("SIGNAL", "SIGHUP MEANS REFRESH");
      break;
   case  SIGUSR1:
      DEBUG_YEXEC  yLOG_info  ("SIGNAL", "SIGUSR1 means user message");
      break;
   case  SIGUSR2:
      DEBUG_YEXEC  yLOG_info  ("SIGNAL", "SIGUSR2 means user message");
      break;
   case  SIGTERM:
      if (s_bulletproof != 'y') {
         DEBUG_YEXEC  yLOG_info  ("SIGNAL", "SIGTERM means terminate daemon");
         DEBUG_YEXEC  yLOGS_end   ();
         if (s_signaler != NULL)  s_signaler (a_signal, a_info, a_nada);
         exit(-1);
      }
      DEBUG_YEXEC  yLOG_info  ("SIGNAL", "SIGTERM means terminate daemon, but this one must not");
      break;
   case  SIGSEGV:
      if (s_bulletproof != 'y') {
         DEBUG_YEXEC  yLOG_info  ("SIGNAL", "SIGSEGV means daemon blew up");
         DEBUG_YEXEC  yLOGS_end   ();
         if (s_signaler != NULL)  s_signaler (a_signal, a_info, a_nada);
         exit(-1);
      }
      DEBUG_YEXEC  yLOG_info  ("SIGNAL", "SIGTERM means terminate daemon, but this one must not");
      break;
   default:
      DEBUG_YEXEC  yLOG_info  ("SIGNAL", "UNKNOWN, signal received, but not handled");
      break;
   }
   if (s_signaler != NULL)  s_signaler (a_signal, a_info, a_nada);
   /*---(complete)------------------------------*/
   return;
}

char             /* [------] setup signal handling ---------------------------*/
yEXEC_signal       (
      char a_bulletproof,    /* Y/N ignores terminate and segfault            */
      char a_interactive,    /* Y/N reacts to terminal keyboard and jobs      */
      char a_children,       /* Y/N hear about children terminating           */
      void *a_signaler)      /* local callback for signal handling            */
{
   /*---(locals)-----------+-----------+-*/
   tSIGACT     sa;
   int         rc          = 0;
   /*---(begin)--------------------------*/
   DEBUG_YEXEC  yLOG_enter  (__FUNCTION__);
   s_bulletproof  = a_bulletproof;
   DEBUG_YEXEC  yLOG_char   ("bullet"    , s_bulletproof);
   s_interactive  = a_interactive;
   DEBUG_YEXEC  yLOG_char   ("interact"  , s_interactive);
   s_children     = a_children;
   DEBUG_YEXEC  yLOG_char   ("children"  , s_children);
   s_signaler   = a_signaler;
   DEBUG_YEXEC  yLOG_point  ("s_signaler", s_signaler);
   /*---(start with ignores)-------------*/
   sa.sa_handler   = SIG_IGN;
   sa.sa_sigaction = NULL;
   sa.sa_flags     = SA_RESTART;
   sigfillset(&sa.sa_mask);
   sa.sa_restorer  = NULL;
   if (s_interactive != 'y') {
      DEBUG_YEXEC  yLOG_info  ("keyboard"  , "ignore INT, QUIT, CONT, and STOP");
      rc  = 0;
      rc += sigaction(SIGINT ,  &sa      , NULL);       /* keyboard interrupt          */
      rc += sigaction(SIGQUIT,  &sa      , NULL);       /* keyboard quit               */
      rc += sigaction(SIGCONT,  &sa      , NULL);       /* keyboard continue           */
      rc += sigaction(SIGTSTP,  &sa      , NULL);       /* keyboard stop               */
      if (rc != 0) {
         DEBUG_YEXEC  yLOG_info  ("warning"   , "at least one sigaction returned error");
         DEBUG_YEXEC  yLOG_value ("returned"  , rc);
      }
   }
   if (s_interactive != 'y') {
      DEBUG_YEXEC  yLOG_info  ("terminal"  , "ignore TIN and TOUT");
      rc  = 0;
      rc += sigaction(SIGTTIN,  &sa      , NULL);       /* tty input for background    */
      rc += sigaction(SIGTTOU,  &sa      , NULL);       /* tty output for background   */
      if (rc != 0) {
         DEBUG_YEXEC  yLOG_info  ("warning"   , "at least one sigaction returned error");
         DEBUG_YEXEC  yLOG_value ("returned"  , rc);
      }
   }
   if (s_children != 'y' && s_children != 'a') {
      DEBUG_YEXEC  yLOG_info  ("children"  , "ignore children, CHLD");
      rc  = 0;
      rc += sigaction(SIGCHLD,  &sa      , NULL);    /* ignore children             */
      if (rc != 0) {
         DEBUG_YEXEC  yLOG_info  ("warning"   , "at least one sigaction returned error");
         DEBUG_YEXEC  yLOG_value ("returned"  , rc);
      }
   }
   /*---(then to defaults)---------------*/
   sa.sa_handler   = SIG_DFL;
   sa.sa_sigaction = NULL;
   sa.sa_flags     = SA_RESTART;
   if (s_interactive == 'y') {
      DEBUG_YEXEC  yLOG_info  ("keyboard"  , "default INT, QUIT, CONT, and STOP");
      rc  = 0;
      rc += sigaction(SIGINT ,  &sa      , NULL);       /* keyboard interrupt          */
      rc += sigaction(SIGQUIT,  &sa      , NULL);       /* keyboard quit               */
      rc += sigaction(SIGCONT,  &sa      , NULL);       /* keyboard continue           */
      rc += sigaction(SIGTSTP,  &sa      , NULL);       /* keyboard stop               */
      if (rc != 0) {
         DEBUG_YEXEC  yLOG_info  ("warning"   , "at least one sigaction returned error");
         DEBUG_YEXEC  yLOG_value ("returned"  , rc);
      }
   }
   if (s_interactive == 'y') {
      DEBUG_YEXEC  yLOG_info  ("terminal"  , "default TIN and TOUT");
      rc  = 0;
      rc += sigaction(SIGTTIN,  &sa      , NULL);       /* tty input for background    */
      rc += sigaction(SIGTTOU,  &sa      , NULL);       /* tty output for background   */
      if (rc != 0) {
         DEBUG_YEXEC  yLOG_info  ("warning"   , "at least one sigaction returned error");
         DEBUG_YEXEC  yLOG_value ("returned"  , rc);
      }
   }
   /*---(then to handles)----------------*/
   sa.sa_handler   = NULL;
   sa.sa_sigaction = yEXEC__comm;
   sa.sa_flags     = SA_RESTART | SA_SIGINFO;
   sigfillset(&sa.sa_mask);
   sa.sa_restorer  = NULL;
   if (s_children == 'y' || s_children == 'a') {
      DEBUG_YEXEC  yLOG_info  ("children", "pay attention to children, CHLD");
      rc += sigaction(SIGCHLD,  &sa      , NULL);       /* watch for children          */
   }
   DEBUG_YEXEC  yLOG_info  ("program",  "look for HUP, USR1, USR2, and ALRM");
   rc  = 0;
   rc += sigaction(SIGHUP ,  &sa      , NULL);       /* hangup means refresh        */
   rc += sigaction(SIGUSR1,  &sa      , NULL);       /* custom (minor)              */
   rc += sigaction(SIGUSR2,  &sa      , NULL);       /* custom (major)              */
   rc += sigaction(SIGALRM,  &sa      , NULL);       /* alarm                       */
   if (rc != 0) {
      DEBUG_YEXEC  yLOG_info  ("warning"   , "at least one sigaction returned error");
      DEBUG_YEXEC  yLOG_value ("returned"  , rc);
   }
   DEBUG_YEXEC  yLOG_info  ("baddies",  "handle SEGV, TERM, and ABRT");
   rc  = 0;
   rc += sigaction(SIGTERM,  &sa      , NULL);       /* catch a kill                */
   rc += sigaction(SIGSEGV,  &sa      , NULL);       /* catch a segfault            */
   rc += sigaction(SIGABRT,  &sa      , NULL);       /* catch an abort              */
   if (rc != 0) {
      DEBUG_YEXEC  yLOG_info  ("warning"   , "at least one sigaction returned error");
      DEBUG_YEXEC  yLOG_value ("returned"  , rc);
   }
   /*---(complete)-----------------------*/
   DEBUG_YEXEC  yLOG_exit  (__FUNCTION__);
   return 0;
}
