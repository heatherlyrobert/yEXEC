/*============================----beg-of-source---============================*/
#include   "yEXEC.h"
#include   "yEXEC_priv.h"

/*
 *
 *   value (same as metis)
 *      A  absolute (life or death)
 *      N  need (clear to objective)
 *      V  valuable (tangible)
 *      c  crave (huge deal
 *      w  want
 *      l  like
 *      m  might
 *
 *   track
 *      C  messages to operator console
 *      Y  special tracking file
 *      y  notes in log
 *      ·  just record as usual
 *
 *   handoff (TBD)
 *      need to allocate codes, but this includes handoffs
 *      to haides (daemons), kharon, artemis, etc.
 *
 *   strict
 *      S  super strict, yell if too short, yell/kill if too long
 *      s  strict, warn of too short, yell/kill of too long
 *      a  advisory warnings only
 *      ·  just record as usual
 *
 *
 *
 *
 *
 *
 */

char
yEXEC_controls          (void)
{
   printf ("yEXEC                                                                                                    \n");
   printf ("                                                                                                         \n");
   printf ("                                          run-time controls                                              \n");
   printf ("                                                                                                         \n");
   printf ("     ´---------------------------------------´ ay/s92k ´---------------------------------------´         \n");
   printf ("     |              ´---------------------------´|||´---------------------------´              |         \n");
   printf ("     |              |              ´-------------´´´-------------´              |              |         \n");
   printf ("     |              |              |              |              |              |              |         \n");
   printf ("-----³-----    -----³-----    -----³-----    -----³-----    -----³-----    -----³-----    -----³-----    \n");
   printf ("importance     tracking       hand-off       strictness     minimum        maximum        remedy         \n");
   printf ("a absolute     C console      ^ tbd          S full         = 1.00         = 1.00         g graceful     \n");
   printf ("n need         Y formal       | tbd          s loose        9 0.90         1 1.10         k kill         \n");
   printf ("v value        y informal     / tbd          a adisory      8 0.80         2 1.20         r relaunch     \n");
   printf ("c crave        - passive      < tbd          - passive      7 0.70         3 1.30         ] let till end \n");
   printf ("w want                        > tbd                         6 0.60         4 1.40         · allow fail   \n");
   printf ("l like                        · none                        h 0.50         H 1.50                        \n");
   printf ("m might                                                     q 0.25         D 2.00                        \n");
   printf ("- tbd                                                       t 0.10         T 3.00                        \n");
   printf ("          H = value                                         z 0.00         X infinite                    \n");
   printf ("    ´---- M = want                                                                                       \n");
   printf ("   quick  L = like                                                                                       \n");
   printf ("                                                                                                         \n");
   printf ("                                                                                                         \n");
   printf ("                                            metis tagging                                                \n");
   printf ("                                                                                                         \n");
   printf ("          ´-----------------------------------´ wdto- ´-----------------------------------´              \n");
   printf ("          |                   ´------------------´|´------------------´                   |              \n");
   printf ("          |                   |                   |                   |                   |              \n");
   printf ("     -----³-----         -----³-----         -----³-----         -----³-----         -----³-----         \n");
   printf ("     urgency             importance           estimate            progress            sharing            \n");
   printf ("     ! now-now           ! now-now            * huge              - backlog           y public           \n");
   printf ("     t today             a absolute           8 eight hrs         < starting          w work-team        \n");
   printf ("     s soonest           n need               4 four hrs          o active            - private          \n");
   printf ("     d days              v value              2 two hrs           > checking                             \n");
   printf ("     w weeks             c crave              1 one hr            # complete                             \n");
   printf ("     m months            w want               m 30m               x cancelled                            \n");
   printf ("     y years             l like               s 15m                                                      \n");
   printf ("     - backlog           m might              ! 5m                                                       \n");
   printf ("                         - tbd                                                                           \n");
   printf ("                                                                                                         \n");
   printf ("                                                                                                         \n");
   printf ("                                                                                                         \n");
   printf ("               ´----------------------------´ process statuses ´-------------------------´               \n");
   printf ("               |                         ´-------´       ´-------´                       |               \n");
   printf ("               |                         |                       |                       |               \n");
   printf ("       -----launch------         -----active------       ----blow-out-----       -----normal------       \n");
   printf ("       A already running         r running               T term/kill             n normal  (=0)          \n");
   printf ("       ? no such command         # cant find/daemon      M seqf/memory           w warning (>0)          \n");
   printf ("       } could not exec                                  L limits                F failure (<0)          \n");
   printf ("                                                         H user halt                                     \n");
   printf ("                                                         d died/other                                    \n");
   printf ("                                                         ~ perms trouble                                 \n");
   printf ("                                                         { chmod on log                                  \n");
   printf ("                                                         | bad log file                                  \n");
   printf ("                                                                                                         \n");
   printf ("                                                                                                         \n");
   printf (" version ii                                                                                              \n");
   printf ("                                                                                                         \n");
   printf ("IMPORTANCE |                                       | REMEDY       \n");
   printf ("a absolute |                                       | g graceful   \n");
   printf ("n need     |              JOB CONTROL              | k kill       \n");
   printf ("v value    |               (7 chars)               | r relaunch   \n");
   printf ("c crave    ³-------´                       ´-------³ ] let to end \n");
   printf ("w want     |       |                       |       | · allow fail \n");
   printf ("l like             ´------´ ay/s92k ´------´                      \n");
   printf ("m might   ´------------------´   ´------------------´             \n");
   printf ("- tbd     |       ´-----------´´´-----------´       |             \n");
   printf ("          |       |            |            |       |             \n");
   printf ("----------³  -----³-----  -----³-----  -----³-----  ³----------   \n");
   printf ("TRACKING     HAND-OFF     STRICTNESS   MINIMUM       MAXIMUM      \n");
   printf ("C console    ^ tbd        S full       = 1.00        = 1.00       \n");
   printf ("Y formal     | tbd        s loose      9 0.90        1 1.10       \n");
   printf ("y informal   / tbd        a adisory    7 0.75        2 1.25       \n");
   printf ("- passive    < tbd        - passive    h 0.50        H 1.50       \n");
   printf ("             > tbd                     q 0.25        D 2.00       \n");
   printf ("             · none                    t 0.10        T 3.00       \n");
   printf ("                                       z 0.00        X infinite   \n");
   printf ("                                                                  \n");
   printf ("                                                                  \n");
   printf ("                                                                  \n");
   printf ("                                                                  \n");
   printf ("                         METIS TAGGING                            \n");
   printf ("                           (5 chars)                              \n");
   printf ("                                                                  \n");
   printf ("     ´---------------------´ wdto- ´---------------------´        \n");
   printf ("     |            ´-----------´|´-----------´            |        \n");
   printf ("     |            |            |            |            |        \n");
   printf ("-----³-----  -----³-----  -----³-----  -----³-----  -----³-----   \n");
   printf ("URGENCY      IMPORTANCE   ESTIMATE     PROGRESS     SHARING       \n");
   printf ("! now-now    ! now-now    * huge       - backlog    y public      \n");
   printf ("t today      a absolute   8 eight hrs  < starting   w work-team   \n");
   printf ("s soonest    n need       4 four hrs   o active     - private     \n");
   printf ("d days       v value      2 two hrs    > checking                 \n");
   printf ("w weeks      c crave      1 one hr     # complete                 \n");
   printf ("m months     w want       m 30m        x cancelled                \n");
   printf ("y years      l like       s 15m                                   \n");
   printf ("- tbd        m might      ! 5m                                    \n");
   printf ("             - tbd        - tbd                                   \n");
   printf ("                                                                  \n");
   printf ("                                                                  \n");
   return 0;
}



/*====================------------------------------------====================*/
/*===----                           utility                            ----===*/
/*====================------------------------------------====================*/
static void      o___UTILITY_________________o (void) {;}

char        yEXEC_ver [200] = "";

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
   snprintf (yEXEC_ver, 100, "%s   %s : %s", t, P_VERNUM, P_VERTXT);
   return yEXEC_ver;
}

char       /*lf--: PURPOSE : exit on termintation/signal ---------------------*/
yEXEC_term         (const char *a_func, const int a_exit)
{
   /*---(log)----------------------------*//*===fat=beg===*/
   if (strncmp(a_func, "", 1) != 0) DEBUG_YEXEC  yLOG_exit  (a_func);
   DEBUG_YEXEC  yLOGS_end   ();
   /*---(check for harsh exit)-----------*//*===fat=end===*/
   if (a_exit > 0) exit(a_exit);
   /*---(complete)-----------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       duration parsing                       ----===*/
/*====================------------------------------------====================*/
static void      o___DURATION________________o (void) {;};

char
yEXEC_dur_in_sec        (char *a_text, int *a_dur)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        t           [LEN_TERSE] = "";
   int         x_len       =    0;
   char        x_unit      =  '-';
   float       x_value     =  0.0;
   /*---(defense)------------------------*/
   --rce;  if (a_text == NULL)  return rce;
   --rce;  if (a_dur  == NULL)  return rce;
   /*---(prepare)------------------------*/
   strlcpy (t, a_text, LEN_TERSE);
   x_len = strlen (t);
   /*---(metis abbreviations)------------*/
   if (x_len == 1)  switch (t [0])  {
   case 'L' : *a_dur = 60 * 60;       return 0;  break;
   case 'M' : *a_dur = 60 * 30;       return 0;  break;
   case 'S' : *a_dur = 60 * 15;       return 0;  break;
   case '!' : *a_dur = 60 * 5;        return 0;  break;
   }
   /*---(find unit)----------------------*/
   if (x_len > 1 && strchr (LTRS_NUMBER, t [x_len - 1]) == NULL) {
      x_unit = t [--x_len];
      t [x_len] = '\0';
   }
   /*---(calculate value)----------------*/
   x_value = atof (t);
   --rce;  if (x_value < 0) {
      *a_dur = 0;
      return rce;
   }
   /*---(scale by unit)------------------*/
   --rce;  switch (x_unit) {
   case 's' :  x_value *= 1;             break;
   case 'm' :  x_value *= 60;            break;
   case 'h' :  x_value *= 60 * 60;       break;
   case 'd' :  x_value *= 60 * 60 * 24;  break;
   case '-' :  x_value *= 1;             break;
   default  :  *a_dur = 0; return rce;   break;
   }
   /*---(save back)----------------------*/
   *a_dur = round (x_value);
   /*---(complete)-----------------------*/
   return 0;
}

char
yexec_min_in_msec       (int a_dur, char a_min, int a_floor, int *a_mindur)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(defense)------------------------*/
   --rce;  if (a_mindur == NULL)  return rce;
   /*---(defaults)-----------------------*/
   --rce;  if (a_dur < 0) {
      a_dur = 0;
      rc    = rce;
   } else {
      a_dur *= 1000;
   }
   /*---(minimum estimate)---------------*/
   --rce;  switch (a_min) {
   case '=' : *a_mindur =  a_dur * 1.00;      break;
   case '9' : *a_mindur =  a_dur * 0.90;      break;
   case '8' : *a_mindur =  a_dur * 0.80;      break;  
   case '7' : *a_mindur =  a_dur * 0.70;      break;  
   case '6' : *a_mindur =  a_dur * 0.60;      break;  
   case 'h' : *a_mindur =  a_dur * 0.50;      break;  
   case 'q' : *a_mindur =  a_dur * 0.25;      break;  
   case 't' : *a_mindur =  a_dur * 0.10;      break;  
   case 'z' :
   case '·' :
   case '-' : *a_mindur =  0.00;              break;  
   default  : *a_mindur =  0.00; rc = rce;    break;
   }
   /*---(correct)------------------------*/
   if (*a_mindur < a_floor)  *a_mindur = a_floor;
   /*---(complete)-----------------------*/
   return rc;
}

char
yexec_max_in_msec       (int a_dur, char a_max, int a_mindur, int *a_maxdur)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(defense)------------------------*/
   --rce;  if (a_maxdur == NULL)  return rce;
   /*---(defaults)-----------------------*/
   --rce;  if (a_dur < 0) {
      a_max = '-';
      rc    = rce;
   } else {
      a_dur *= 1000;
   }
   /*---(minimum estimate)---------------*/
   --rce;  switch (a_max) {
   case '=' : *a_maxdur =  a_dur * 1.00;      break;
   case '1' : *a_maxdur =  a_dur * 1.10;      break;
   case '2' : *a_maxdur =  a_dur * 1.20;      break;  
   case '3' : *a_maxdur =  a_dur * 1.30;      break;  
   case '4' : *a_maxdur =  a_dur * 1.40;      break;  
   case 'H' : *a_maxdur =  a_dur * 1.50;      break;  
   case 'D' : *a_maxdur =  a_dur * 2.00;      break;  
   case 'T' : *a_maxdur =  a_dur * 3.00;      break;  
   case 'X' :
   case '·' :
   case '-' : *a_maxdur =  9999999;             break;  
   default  : *a_maxdur =  9999999; rc = rce;   break;
   }
   /*---(correct)------------------------*/
   if (*a_maxdur > 9999999)     *a_maxdur = 9999999;
   if (*a_maxdur < a_mindur)    *a_maxdur = a_mindur;
   /*---(complete)-----------------------*/
   return rc;
}

char
yEXEC_flags             (int a_dur, int a_floor, char *a_flags, char *a_value, char *a_track, char *a_handoff, char *a_strict, char *a_min, int *a_mindur, char *a_max, int *a_maxdur, char *a_remedy)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_len       =    0;
   int         x_min       =    0;
   int         x_max       =    0;
   char        i           =    0;
   char        j           =    0;
   char        c           =  '-';
   char        x_flags     [LEN_LABEL] = "--·---·";
   /*---(defaults)-----------------------*/
   if (a_value   != NULL)  *a_value    = '-';
   if (a_track   != NULL)  *a_track    = '-';
   if (a_strict  != NULL)  *a_strict   = '-';
   if (a_min     != NULL)  *a_min      = '-';
   if (a_mindur  != NULL)  *a_mindur   =   0;
   if (a_max     != NULL)  *a_max      = '-';
   if (a_maxdur  != NULL)  *a_maxdur   = 9999999;
   if (a_remedy  != NULL)  *a_remedy   = '-';
   if (a_handoff != NULL)  *a_handoff  = '-';
   /*---(defense)------------------------*/
   --rce;  if (a_flags != NULL) {
      x_len = strlen (a_flags);
      if (x_len >= 7)  x_len = 7;
      for (j = 0; j < x_len; ++j)   x_flags [j] = a_flags [j];
   }
   /*---(importance)---------------------*/
   c = x_flags [i++];
   --rce;  if (a_value   != NULL) {
      switch (c) {
      case 'H' :  c = 'v';  break;
      case 'M' :  c = 'w';  break;
      case 'L' :  c = 'l';  break;
      }
      if      (strchr ("-·"      , c) != NULL)   ;
      else if (strchr ("!anvcwlm", c) != NULL)   *a_value   = c;
      else return rce;
   }
   /*---(tracking)-----------------------*/
   /*> --rce;  c = x_flags [i++];  if (c != ' ' && c != '·')  return rce;             <*/
   c = x_flags [i++];
   --rce;  if (a_track   != NULL) {
      if      (strchr ("-·"      , c) != NULL)   ;
      else if (strchr ("CYy"     , c) != NULL)   *a_track   = c;
      else return rce;
   }
   /*---(handoff)------------------------*/
   /*> --rce;  c = x_flags [i++];  if (c != ' ' && c != '·')  return rce;             <*/
   c = x_flags [i++];
   --rce;  if (a_handoff != NULL) {
      if      (strchr ("-·"      , c) != NULL)   ;
      else if (strchr ("^|/<>"   , c) != NULL)   *a_handoff = c;
      else return rce;
   }
   /*---(strictness)---------------------*/
   /*> --rce;  c = x_flags [i++];  if (c != ' ' && c != '·')  return rce;             <*/
   c = x_flags [i++];
   --rce;  if (a_strict  != NULL) {
      if      (strchr ("-·"      , c) != NULL)   ;
      else if (strchr ("Ssa"     , c) != NULL)   *a_strict  = c;
      else return rce;
   }
   /*---(minimum estimate)---------------*/
   /*> --rce;  c = x_flags [i++];  if (c != ' ' && c != '·')  return rce;             <*/
   c = x_flags [i++];
   if (c == '·') c = '-';
   rc = yexec_min_in_msec (a_dur, c, a_floor, &x_min);
   if (rc < 0)  return rce;
   if (a_min     != NULL)  *a_min    = c;
   if (a_mindur  != NULL)  *a_mindur = x_min;
   /*---(maximum estimate)---------------*/
   /*> --rce;  c = x_flags [i++];  if (c != ' ' && c != '·')  return rce;             <*/
   c = x_flags [i++];
   if (c == '·') c = '-';
   rc = yexec_max_in_msec (a_dur, c, x_min  , &x_max);
   if (rc < 0)  return rce;
   if (a_max     != NULL)  *a_max    = c;
   if (a_maxdur  != NULL)  *a_maxdur = x_max;
   /*---(recovery)-----------------------*/
   /*> --rce;  c = x_flags [i++];  if (c != ' ' && c != '·')  return rce;             <*/
   c = x_flags [i++];
   --rce;  if (a_remedy  != NULL) {
      if      (strchr ("-·"      , c) != NULL)   ;
      else if (strchr ("fgkr]"   , c) != NULL)   *a_remedy  = c;
      else return rce;
   }
   /*---(complete)-----------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;};

char          unit_answer [LEN_RECD];

char       /*----: set up program urgents/debugging --------------------------*/
yexec__unit_quiet       (void)
{
   yLOGS_begin ("yEXEC", YLOG_SYS, YLOG_QUIET);
   return 0;
}

char       /*----: set up program urgents/debugging --------------------------*/
yexec__unit_loud        (void)
{
   yLOGS_begin   ("yEXEC", YLOG_SYS, YLOG_NOISE);
   yURG_name     ("yexec", YURG_ON);
   DEBUG_YEXEC  yLOG_info     ("yEXEC"     , yEXEC_version   ());
   return 0;
}

char       /*----: stop logging ----------------------------------------------*/
yexec__unit_end         (void)
{
   yLOGS_end     ();
   return 0;
}




/*============================----end-of-source---============================*/
