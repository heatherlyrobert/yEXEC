/*============================[[    beg-code    ]]============================*/
#include   "yEXEC.h"
#include   "yEXEC_priv.h"



static char     s_bulletproof = 'n';
static char     s_interactive = 'n';
static char     s_children    = 'n';
static void   (*s_signaler)   (int a_signal, siginfo_t *a_info, char *a_name, char *a_desc);
static char     s_output      [LEN_RECD]  = "";
static FILE    *f             = NULL;



#define      MAX_SIGS         24
typedef struct cSIGS tSIGS;
static struct cSIGS {
   char        num;
   char        name        [LEN_TERSE];
   char        desc        [LEN_TITLE];
};
static const tSIGS s_sigs [MAX_SIGS] = {
   /*--, "123456789"  "123456789-123456789-123456789" */
   {  0, "------"   , ""                              },
   {  1, "SIGHUP"   , "reload files, refresh data"    },
   {  2, "SIGINT"   , "C-c to kill a process"         },
   {  3, "SIGQUIT"  , "C-µ to kill a process"         },
   {  4, "SIGILL"   , "corrupted executable"          },
   {  5, "SIGTRAP"  , "tracing/breakpoint"            },
   {  6, "SIGABRT"  , "program controlled abort ()"   },
   {  7, "SIGBUS"   , "unaddressable system bus"      },
   {  8, "SIGFPE"   , "fatal arithmetic error"        },
   {  9, "SIGKILL"  , "un-handlable termination"      },
   { 10, "SIGUSR1"  , "user controlled"               },
   { 11, "SIGSEGV"  , "segfault, bad memory access"   },
   { 12, "SIGUSR2"  , "user controlled (daemon ping)" },
   { 13, "SIGPIPE"  , "broken pipe for ipc"           },
   { 14, "SIGALRM"  , "program controlled alarm ()"   },
   { 15, "SIGTERM"  , "graceful termination"          },
   { 16, "SIGSTK>"  , "stack fault on coprocessor"    },
   { 17, "SIGCHLD"  , "child process died"            },
   { 18, "SIGCONT"  , "restart a paused process"      },
   { 19, "SIGSTOP"  , "un-handlable process pause"    },
   { 20, "SIGTSTP"  , "C-z to pause a process"        },
   { 21, "SIGTTIN"  , "terminal input required"       },
   { 22, "SIGTTOU"  , "terminal output required"      },
   { 23, "------"   , ""                              },
   /*--, "123456789"  "123456789-123456789-123456789" */
};



/*====================------------------------------------====================*/
/*===----                       signal catching                        ----===*/
/*====================------------------------------------====================*/
static void      o___CATCH___________________o (void) {;}


void
yexec__done        (int a_signal, siginfo_t *a_info, char *a_name, char *a_desc)
{
   DEBUG_YEXEC  yLOG_note  ("not set to bulletproof, so terminating");
   if (f != NULL)  fclose (f);
   f = NULL;
   if (s_signaler != NULL)  s_signaler (a_signal, a_info, a_name, a_desc);
   DEBUG_YEXEC  yLOGS_end   ();
   exit(-1);
   return 0;
}

void             /* [------] receive signals ---------------------------------*/
yEXEC__comm        (int a_signal, siginfo_t *a_info, void *a_nada)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   char        x_looking   =    1;
   int         x_pid       = 0;
   char        t           [LEN_DESC]  = "";
   char        x_recd      [LEN_HUND]  = "";
   int         status      = 0;
   char        rchar       = 0;
   /*---(prepare for logging)-------------------*/
   if      (strlen (s_output) <= 0)            f = NULL;
   else if (strcmp (s_output, "stdsig") == 0)  f = fopen ("/tmp/signal.log"      , "at");
   else if (strcmp (s_output, "unit"  ) == 0)  f = fopen ("/tmp/signal_unit.log" , "at");
   else if (strcmp (s_output, "local" ) == 0)  f = fopen ("/tmp/signal_local.log", "at");
   else                                        f = fopen (s_output               , "at");
   /*---(message)-------------------------------*/
   x_pid     = getpid ();
   if (a_signal > 0 && a_signal < MAX_SIGS) {
      if (a_signal != SIGCHLD)  DEBUG_YEXEC  yLOG_complex ("SIGNAL", "%-7.7s (%2d) %s", s_sigs [a_signal].name, a_signal, s_sigs [a_signal].desc);
      sprintf (t, "yexec  %-7.7s (%2d) %s", s_sigs [a_signal].name, a_signal, s_sigs [a_signal].desc);
   } else {
      sprintf (t, "yexec  ERROR   (%2d) signal number out-of-range", a_signal);
   }
   yEXEC_heartbeat (x_pid, 0, t, NULL, x_recd);
   if (f != NULL) {
      fprintf (f, "%s\n", x_recd);
      fclose (f);
      f = NULL;
   }
   if (a_signal < 0 || a_signal >= MAX_SIGS)   return;
   /*---(children)------------------------------*/
   if (x_looking)  switch (a_signal) {
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
      x_looking = 0;
      break;
   }
   /*---(user/config)---------------------------*/
   if (x_looking)  switch (a_signal) {
   case  SIGHUP:
      x_looking = 0;
      break;
   case  SIGUSR1:
      x_looking = 0;
      break;
   case  SIGUSR2:
      x_looking = 0;
      break;
   case  SIGALRM:
      x_looking = 0;
      break;
   }
   /*---(baddies)-------------------------------*/
   if (x_looking)  switch (a_signal) {
   case  SIGTERM:
      if (s_bulletproof != 'y')  yexec__done (a_signal, a_info, s_sigs [a_signal].name, s_sigs [a_signal].desc);
      x_looking = 0;
      break;
   case  SIGSEGV:
      if (s_bulletproof != 'y')  yexec__done (a_signal, a_info, s_sigs [a_signal].name, s_sigs [a_signal].desc);
      x_looking = 0;
      break;
   case  SIGABRT:
      if (s_bulletproof != 'y')  yexec__done (a_signal, a_info, s_sigs [a_signal].name, s_sigs [a_signal].desc);
      x_looking = 0;
      break;
   case  SIGINT :
      if (s_bulletproof != 'y')  yexec__done (a_signal, a_info, s_sigs [a_signal].name, s_sigs [a_signal].desc);
      x_looking = 0;
      break;
   case  SIGQUIT:
      if (s_bulletproof != 'y')  yexec__done (a_signal, a_info, s_sigs [a_signal].name, s_sigs [a_signal].desc);
      x_looking = 0;
      break;
   }
   /*---(job control)---------------------------*/
   if (x_looking)  switch (a_signal) {
   case  SIGTSTP:
      x_looking = 0;
      break;
   case  SIGCONT:
      x_looking = 0;
      break;
   }
   /*---(catch all)-----------------------------*/
   /*> if (x_looking) {                                                                                                              <* 
    *>    DEBUG_YEXEC  yLOG_info  ("SIGNAL", "UNKNOWN, signal received, but not handled");                                           <* 
    *>    if (f != NULL)  fprintf (f, "%-20.20s  %-10ld  %-6d  yexec  SIG???? (%2d) unknown signal\n", t, x_now, x_pid, a_signal);   <* 
    *>    x_looking = 0;                                                                                                             <* 
    *> }                                                                                                                             <*/
   /*---(call local support)--------------------*/
   if (s_signaler != NULL)  s_signaler (a_signal, a_info, s_sigs [a_signal].name, s_sigs [a_signal].desc);
   /*---(re-handle)-----------------------------*/
   switch (a_signal) {
   case  SIGTSTP:
      kill (x_pid, SIGSTOP);
      break;
   }
   /*---(complete)------------------------------*/
   return;
}



/*====================------------------------------------====================*/
/*===----                       signal setting                         ----===*/
/*====================------------------------------------====================*/
static void      o___SET_____________________o (void) {;}

char
yexec__signal      (char *a_terse, char *a_desc, int a_signal, char a_action)
{
   /*---(locals)-----------+-----+-----+-*/
   tSIGACT     sa;
   int         rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_senter (__FUNCTION__);
   DEBUG_YEXEC  yLOG_snote  (a_terse);
   DEBUG_YEXEC  yLOG_snote  (a_desc);
   DEBUG_YEXEC  yLOG_sint   (a_signal);
   DEBUG_YEXEC  yLOG_schar  (a_action);
   /*---(create request)-----------------*/
   switch (a_action) {
   case 'i' :
      DEBUG_YEXEC  yLOG_snote   ("ignore, no trapping");
      sa.sa_handler   = SIG_IGN;
      sa.sa_sigaction = NULL;
      sa.sa_flags     = SA_RESTART;
      break;
   case 'd' :
      DEBUG_YEXEC  yLOG_snote   ("default, no trapping");
      sa.sa_handler   = SIG_DFL;
      sa.sa_sigaction = NULL;
      sa.sa_flags     = SA_RESTART;
      break;
   case 'y' :
      DEBUG_YEXEC  yLOG_snote   ("custom, trapped");
      sa.sa_handler   = NULL;
      sa.sa_sigaction = yEXEC__comm;
      sa.sa_flags     = SA_RESTART | SA_SIGINFO;
      break;
   }
   /*---(block all other signals)--------*/
   sigfillset (&sa.sa_mask);
   sa.sa_restorer  = NULL;
   /*---(call setup)---------------------*/
   rc = sigaction (a_signal, &sa, NULL);
   DEBUG_YEXEC  yLOG_sint   (rc);
   if (rc < 0)   DEBUG_YEXEC  yLOG_snote  ("BOOM");
   /*---(complete)-----------------------*/
   DEBUG_YEXEC  yLOG_sexit  (__FUNCTION__);
   return rc;
}

char yexec__ignore      (char *a_terse, char *a_desc, int a_signal)  { return yexec__signal (a_terse, a_desc, a_signal, 'i'); }
char yexec__default     (char *a_terse, char *a_desc, int a_signal)  { return yexec__signal (a_terse, a_desc, a_signal, 'd'); }
char yexec__custom      (char *a_terse, char *a_desc, int a_signal)  { return yexec__signal (a_terse, a_desc, a_signal, 'y'); }

char             /* [------] setup signal handling ---------------------------*/
yEXEC_signal       (char a_bulletproof, char a_interactive, char a_children, void *a_signaler, char *a_output)
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
   /*---(output)-------------------------*/
   if (s_output == NULL)  strlcpy (s_output, ""      , LEN_RECD);
   else                   strlcpy (s_output, a_output, LEN_RECD);
   /*---(keyboard)-----------------------*/
   DEBUG_YEXEC  yLOG_note  ("keyboard  : INT, TSTP, QUIT, CONT, TTIN, TTOU");
   if (s_interactive != 'y') {
      yexec__ignore  ("SIGTSTP", "keyboard stop (C-z)"      , SIGTSTP);
      yexec__ignore  ("SIGINT" , "keyboard interrupt (C-c)" , SIGINT );
      yexec__ignore  ("SIGQUIT", "keyboard quit (C-µ)"      , SIGQUIT);
      yexec__ignore  ("SIGTTIN", "terminal input"           , SIGTTIN);
      yexec__ignore  ("SIGTTOU", "terminal output"          , SIGTTOU);
   } else {
      yexec__custom  ("SIGTSTP", "keyboard stop (C-z)"      , SIGTSTP);
      yexec__custom  ("SIGINT" , "keyboard interrupt (C-c)" , SIGINT );
      yexec__custom  ("SIGQUIT", "keyboard quit (C-µ)"      , SIGQUIT);
      yexec__default ("SIGTTIN", "terminal input"           , SIGTTIN);
      yexec__default ("SIGTTOU", "terminal output"          , SIGTTOU);
   }
   yexec__custom  ("SIGCONT", "keyboard continue"        , SIGCONT);
   /*---(children)-----------------------*/
   DEBUG_YEXEC  yLOG_note  ("children  : CHLD");
   if (s_children != 'y' && s_children != 'a') {
      yexec__default ("SIGCHLD", "child process ended"      , SIGCHLD);
   } else {
      yexec__custom  ("SIGCHLD", "child process ended"      , SIGCHLD);
   }
   /*---(use involved)-------------------*/
   DEBUG_YEXEC  yLOG_info  ("program",  "look for HUP, USR1, USR2, and ALRM");
   yexec__custom  ("SIGHUP" , "reload and refresh"       , SIGHUP );
   yexec__custom  ("SIGUSR1", "user controlled"          , SIGUSR1);
   yexec__custom  ("SIGUSR2", "user controlled (ping)"   , SIGUSR2);
   yexec__custom  ("SIGALRM", "timer using alarm ()"     , SIGALRM);
   /*---(monsters)-----------------------*/
   DEBUG_YEXEC  yLOG_info  ("baddies",  "handle SEGV, TERM, and ABRT");
   yexec__custom  ("SIGTERM", "graceful termination"     , SIGTERM);
   yexec__custom  ("SIGSEGV", "memory/segment fault"     , SIGSEGV);
   yexec__custom  ("SIGABRT", "termination from abort ()", SIGABRT);
   /*---(complete)-----------------------*/
   DEBUG_YEXEC  yLOG_exit  (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       signal checking                        ----===*/
/*====================------------------------------------====================*/
static void      o___CHECK___________________o (void) {;}

int
yEXEC_signal_log        (char *a_name, int n, char *a_recd)
{
   char        rce         =  -10;
   FILE       *f           = NULL;
   char        t           [LEN_RECD]  = "";
   int         c           =    0;
   int         x_len       =    0;
   /*---(defense)------------------------*/
   --rce;  if (a_name == NULL)         return rce;
   --rce;  if (strlen (a_name) <= 0)   return rce;
   if (a_recd != NULL)  strlcpy (a_recd, "", LEN_RECD);
   /*---(open file)----------------------*/
   if      (strcmp (a_name, "stdsig") == 0)  f = fopen ("/tmp/signal.log"      , "rt");
   else if (strcmp (a_name, "unit"  ) == 0)  f = fopen ("/tmp/signal_unit.log" , "rt");
   else if (strcmp (a_name, "local" ) == 0)  f = fopen ("/tmp/signal_local.log", "rt");
   else                                      f = fopen (a_name                 , "rt");
   --rce;  if (f == NULL)              return rce;
   /*---(read records)-------------------*/
   while (1) {
      fgets (t, LEN_RECD, f);
      if (c != 0 && feof (f))  break;
      /*> printf ("%2d[%s]\n", strlen (t), t);                                        <*/
      if (n <  0 && a_recd != NULL)  strlcpy (a_recd, t, LEN_RECD);
      if (c == n && a_recd != NULL)  strlcpy (a_recd, t, LEN_RECD);
      ++c;
      if (feof (f))  break;

   }
   /*---(clean record)-------------------*/
   if (a_recd != NULL) {
      x_len = strlen (a_recd);
      if (x_len > 0 && a_recd [x_len - 1] == '\n')  a_recd [--x_len] = '\0';
   }
   /*---(close file)---------------------*/
   fclose (f);
   /*---(complete)-----------------------*/
   return c;
}



/*====================------------------------------------====================*/
/*===----                      unit test accessor                      ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char*            /*--> unit test accessor ------------------------------*/
yexec_sign__unit        (char *a_question, int n)
{ 
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   char        t           [LEN_RECD]  = "";
   int         c           =    0;
   /*---(prepare)------------------------*/
   strlcpy  (unit_answer, "SIGN             : question not understood", LEN_RECD);
   /*---(crontab name)-------------------*/
   if      (strcmp (a_question, "signal"        )  == 0) {
      yEXEC_signal_log ("stdsig", n, t);
      snprintf (unit_answer, LEN_RECD, "SIGN signal (%2d) : %3d  %2d[%.70s]", n, c, strlen (t), t);
   }
   else if (strcmp (a_question, "unit"          )  == 0) {
      c = yEXEC_signal_log ("unit", n, t);
      snprintf (unit_answer, LEN_RECD, "SIGN unit   (%2d) : %3d  %2d[%.70s]", n, c, strlen (t), t);
   }
   else if (strcmp (a_question, "local"         )  == 0) {
      c = yEXEC_signal_log ("local", n, t);
      snprintf (unit_answer, LEN_RECD, "SIGN local  (%2d) : %3d  %2d[%.70s]", n, c, strlen (t), t);
   }
   else if (strcmp (a_question, "ticker"        )  == 0) {
      c = yEXEC_signal_log ("/tmp/unit_ticker.txt", 0, t);
      snprintf (unit_answer, LEN_RECD, "SIGN ticker      : %3d  %2d[%.70s]", c, strlen (t), t);
   }
   else if (strcmp (a_question, "settings"      )  == 0) {
      c = yEXEC_signal_log ("local", n, t);
      snprintf (unit_answer, LEN_RECD, "SIGN settings    : bullet %c, inter  %c, child  %c, handle %-10p, output %s", s_bulletproof, s_interactive, s_children, s_signaler, s_output);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}

