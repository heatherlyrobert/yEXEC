/*============================----beg-of-source---============================*/
#include   "yEXEC.h"
#include   "yEXEC_priv.h"



#define MAX_FLAGS   100
typedef struct cFLAGS tFLAGS;
static struct cFLAGS {
   char        c;   /* category */
   char        v;   /* value    */
   char        d           [LEN_TERSE];
   char        desc        [LEN_DESC];
};
static const tFLAGS s_flags [MAX_FLAGS] = {
   /*---(importance)--------*/
   {  0, 'a', "absolute"   , ""                                         },
   {  0, 'n', "need"       , ""                                         },
   {  0, 'v', "value"      , ""                                         },
   {  0, 'c', "crave"      , ""                                         },
   {  0, 'g', "good"       , ""                                         },
   {  0, 'l', "like"       , ""                                         },
   {  0, 'i', "idea"       , ""                                         },
   {  0, '-', "unset"      , ""                                         },
   {  0, '·', "unset"      , ""                                         },
   /*---(tracking/cum)------*/
   {  1, 'p', "profile"    , ""                                         },
   {  1, 'u', "usage"      , ""                                         },
   {  1, 'c', "console"    , ""                                         },
   {  1, 'y', "beg/end"    , ""                                         },
   {  1, '-', "unset"      , ""                                         },
   {  1, '·', "unset"      , ""                                         },
   /*---(rolling)-----------*/
   {  2, '[', "1day"       , ""                                         },
   {  2, '3', "3days"      , ""                                         },
   {  2, '6', "6days"      , ""                                         },
   {  2, '9', "9days"      , ""                                         },
   {  2, '<', "15days"     , ""                                         },
   {  2, '|', "30days"     , ""                                         },
   {  2, '>', "45days"     , ""                                         },
   {  2, ']', "60days"     , ""                                         },
   {  2, '´', "60days"     , ""                                         },
   {  2, '-', "unset"      , ""                                         },
   {  2, '·', "unset"      , ""                                         },
   /*---(strictness)--------*/
   {  3, 'g', "graceful"   , "TERM if overruns maximum estimate"        },
   {  3, 'k', "violent"    , "KILL of overruns maximum estimate"        },
   {  3, ']', "till end"   , "allow to run till end of group"           },
   {  3, 'r', "rules"      , "notify khaos/sysadmin, and reports"       },
   {  3, 'w', "warning"    , "note on reports only"                     },
   {  3, 'a', "advisory"   , "no action, just for planning"             },
   {  3, '-', "unset"      , "no action"                                },
   {  3, '·', "unset"      , "no action"                                },
   /*---(minimum)-----------*/
   {  4, '=', "1.00"       , ""                                         },
   {  4, '9', "0.90"       , ""                                         },
   {  4, '7', "0.75"       , ""                                         },
   {  4, 'h', "0.50"       , ""                                         },
   {  4, 'q', "0.25"       , ""                                         },
   {  4, 't', "0.10"       , ""                                         },
   {  4, '-', "zero"       , ""                                         },
   {  4, '·', "zero"       , ""                                         },
   /*---(maximum)-----------*/
   {  5, '=', "1.00"       , ""                                         },
   {  5, '1', "1.10"       , ""                                         },
   {  5, '2', "1.25"       , ""                                         },
   {  5, 'H', "1.50"       , ""                                         },
   {  5, 'D', "2.00"       , ""                                         },
   {  5, 'T', "3.00"       , ""                                         },
   {  5, '-', "infinite"   , ""                                         },
   {  5, '·', "infinite"   , ""                                         },
   /*---(grouping)----------*/
   {  6, '·', "unset"      , ""                                         },
   {  6, '-', "unset"      , ""                                         },
   /*---(flexibility)-------*/
   {  7, '=', "none"       , "scheduled time is unmovable"              },
   {  7, '/', "minutes"    , "schedule can be shifted +/- 30min"        },
   {  7, '+', "hours"      , "schedule can be shifted +/- 2hrs"         },
   {  7, '~', "shift"      , "schedule can be shifted +/- 8hrs"         },
   {  7, '*', "day"        , "schedule can be shifted +/- 24hrs"        },
   {  7, '-', "unset"      , ""                                         },
   {  7, '·', "unset"      , ""                                         },
   /*---(throttle)----------*/
   {  8, 'g', "graceful"   , "TERM process if it exceeds limits"        },
   {  8, 'k', "violent"    , "KILL process if it exceeds limits"        },
   {  8, 't', "throttle"   , "throttle process to keep in limits"       },
   {  8, 'r', "rules"      , "notify khaos/sysadmin, and reports"       },
   {  8, 'w', "warning"    , "note on reports only"                     },
   {  8, 'a', "advisory"   , "no action, just for planning"             },
   {  8, '·', "unset"      , "no action"                                },
   {  8, '-', "unset"      , "no action"                                },
   /*---(cpu use)-----------*/
   {  9, '5', "1.00"       , ""                                         },
   {  9, '4', "0.75"       , ""                                         },
   {  9, '3', "0.50"       , ""                                         },
   {  9, '2', "0.25"       , ""                                         },
   {  9, '1', "0.05"       , ""                                         },
   {  9, '0', "0.00"       , ""                                         },
   {  9, '-', "unset"      , ""                                         },
   {  9, '·', "unset"      , ""                                         },
   /*---(disk use)----------*/
   { 10, '5', "1.00"       , ""                                         },
   { 10, '4', "0.75"       , ""                                         },
   { 10, '3', "0.50"       , ""                                         },
   { 10, '2', "0.25"       , ""                                         },
   { 10, '1', "0.05"       , ""                                         },
   { 10, '0', "0.00"       , ""                                         },
   { 10, '-', "unset"      , ""                                         },
   { 10, '·', "unset"      , ""                                         },
   /*---(network use)-------*/
   { 11, '5', "1.00"       , ""                                         },
   { 11, '4', "0.75"       , ""                                         },
   { 11, '3', "0.50"       , ""                                         },
   { 11, '2', "0.25"       , ""                                         },
   { 11, '1', "0.05"       , ""                                         },
   { 11, '0', "0.00"       , ""                                         },
   { 11, '-', "unset"      , ""                                         },
   { 11, '·', "unset"      , ""                                         },
   /*---(done)--------------*/
   { -1, -1 , "end-list"   , ""                                         },
};

   char        x_flags     [LEN_LABEL] = "ithsnxgftcdn";
char
yEXEC_controls          (void)
{
   printf ("yEXEC                                                                                                    \n");
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
   printf ("IMPORTANCE |                                       | GROUPING     \n");
   printf ("a absolute |                                       | è to ÿ       \n");
   printf ("n need     |              JOB CONTROL              | · unset      \n");
   printf ("v value    |               (7 chars)               |              \n");
   printf ("c crave    ³-------´                       ´-------³              \n");
   printf ("w want     |       |                       |       |              \n");
   printf ("l like             ´------´ ay/s92· ´------´                      \n");
   printf ("m might   ´------------------´   ´------------------´             \n");
   printf ("- tbd     |       ´-----------´´´-----------´       |             \n");
   printf ("          |       |            |            |       |             \n");
   printf ("----------³  -----³-----  -----³-----  -----³-----  ³----------   \n");
   printf ("TRACKING     ROLLING      STRICTNESS   MINIMUM       MAXIMUM      \n");
   printf ("p profile    · full60     g graceful   = 1.00        = 1.00       \n");
   printf ("u usage      [ single     k violent    9 0.90        1 1.10       \n");
   printf ("c console    < last15     ] till end   7 0.75        2 1.25       \n");
   printf ("y beg/end    | last30     r rules      h 0.50        H 1.50       \n");
   printf ("- none       > last45     w warning    q 0.25        D 2.00       \n");
   printf ("             ] full6a advisory   t 0.10        T 3.00       \n");
   printf ("             ´ full60     - passive    - 0.00        - infinite   \n");
   printf ("                                                                  \n");
   printf ("                                                                  \n");
   printf ("                                                                  \n");
   printf ("                     RESOURCE EXTENSION                           \n");
   printf ("          ´----------------´ =s321 ´----------------´             \n");
   printf ("          |       ´-----------´´´-----------´       |             \n");
   printf ("          |       |            |            |       |             \n");
   printf ("----------³  -----³-----  -----³-----  -----³-----  ³----------   \n");
   printf ("FLEX         THROTTLE     CPU/USE      DISK/USE     NET/USE       \n");
   printf ("= none       g graceful   5 full       5 full       5 full        \n");
   printf ("/ minutes    k violent    4 high       4 high       4 high        \n");
   printf ("+ hours      t throttle   3 half       3 half       3 half        \n");
   printf ("* day        r rules      2 low        2 low        2 low         \n");
   printf ("· unset      w warning    1 trivial    1 trivial    1 trivial     \n");
   printf ("             a advisory   - unset      - unset      - unset       \n");
   printf ("             - passive                                            \n");
   printf ("                                                                  \n");
   printf ("                                                                  \n");
   printf ("                                                                  \n");
   printf ("                         ithsnxgftcdn                             \n");
   printf ("                                                                  \n");
   printf ("                                                                  \n");
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
   printf ("m months     w want       h 30m        x cancelled                \n");
   printf ("y years      l like       q 15m                                   \n");
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
   int         i           =    0;
   float       x_value     =  0.0;
   /*---(default)------------------------*/
   if (a_dur != NULL)  *a_dur = 0;
   /*---(defense)------------------------*/
   --rce;  if (a_text == NULL)  return rce;
   --rce;  if (a_dur  == NULL)  return rce;
   /*---(prepare)------------------------*/
   ystrlcpy (t, a_text, LEN_TERSE);
   x_len = strlen (t);
   /*---(metis abbreviations)------------*/
   --rce;  if (x_len == 1)  switch (t [0])  {
   case '-' : *a_dur = 0;             return 0;  break;
   case 'L' : *a_dur = 60 * 60;       return 0;  break;
   case 'M' : *a_dur = 60 * 30;       return 0;  break;
   case 'S' : *a_dur = 60 * 15;       return 0;  break;
   case '!' : *a_dur = 60 * 5;        return 0;  break;
   case '.' : return rce;  break;
   }
   /*---(find unit)----------------------*/
   if (x_len > 1 && strchr (YSTR_NUMBER, t [x_len - 1]) == NULL) {
      x_unit = t [--x_len];
      t [x_len] = '\0';
   }
   /*---(check characters)---------------*/
   --rce;  for (i = 0; i < x_len; ++i) {
      if (strchr (YSTR_FLOAT, t [i]) == NULL)  return rce;
   }
   /*---(calculate value)----------------*/
   x_value = atof (t);
   --rce;  if (x_value < 0) {
      *a_dur = 0;
      return rce;
   }
   /*---(scale by unit)------------------*/
   --rce;  switch (x_unit) {
   case 's' :  x_value *= 1;                  break;
   case 'm' :  x_value *= 60;                 break;
   case 'h' :  x_value *= 60 * 60;            break;
   case 'd' :  x_value *= 60 * 60 * 24;       break;
   case 'o' :  x_value *= 60 * 60 * 24 * 30;  break;
   case 'y' :  x_value *= 60 * 60 * 24 * 365; break;
   case '-' :  x_value *= 1;                  break;
   default  :  *a_dur = 0; return rce;        break;
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
   case '-' : *a_maxdur =   900000;           break;   /* 15min */
   default  : *a_maxdur =   900000; rc = rce; break;   /* 15min */
   }
   /*---(correct)------------------------*/
   if (*a_maxdur > 86400001)    *a_maxdur = 86400001;  /* 24hrs */
   if (*a_maxdur < a_mindur)    *a_maxdur = a_mindur;
   /*---(complete)-----------------------*/
   return rc;
}

char   s_last      = 'g';
char   s_terse     [LEN_HUND]  = "";
char   s_fancy     [LEN_RECD]  = "";

char
yexec__findflag         (char a_cat, char a_val, char *a_real)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   char        t           [LEN_LABEL] = "";
   if (a_cat < 0 || a_cat > 11)         return -2;
   if (a_val == '·')  a_val = '-';
   for (i = 0; i < MAX_FLAGS; ++i) {
      /*---(filter)----------------------*/
      if (s_flags [i].c <  0)          break;      /* end-of-list         */
      if (s_flags [i].c > a_cat)       break;      /* too late            */
      if (s_flags [i].c != a_cat)      continue;
      if (s_flags [i].v != a_val)      continue;
      /*---(save-back)-------------------*/
      if (a_real != NULL)  *a_real = s_flags [i].v;
      if (s_last != 'g') {
         if (a_cat > 0)  ystrlcat (s_terse, BOLD_OFF, LEN_RECD);
         ystrlcat (s_terse, BOLD_GRN, LEN_HUND);
      }
      sprintf (t, "%c", a_val);
      if ((a_cat == 6 || a_cat == 8) && a_val == '-')  strcpy (t, "·");
      ystrlcat (s_terse, t, LEN_HUND);
      if (s_last != 'g') {
         if (a_cat > 0)  ystrlcat (s_fancy, BOLD_OFF, LEN_RECD);
         ystrlcat (s_fancy, BOLD_GRN, LEN_RECD);
      }
      sprintf (t, "%c=%s", a_val, s_flags [i].d);
      if (a_cat > 0)  ystrlcat (s_fancy, ", ", LEN_RECD);
      ystrlcat (s_fancy, t, LEN_RECD);
      s_last = 'g';
      return 0;
   }
   if (a_real != NULL)  *a_real = '-';
   if (s_last != 'e')  ystrlcat (s_terse, BOLD_ERR, LEN_HUND);
   sprintf (t, "%c", a_val);
   ystrlcat (s_terse, t, LEN_HUND);
   if (s_last != 'e')  ystrlcat (s_fancy, BOLD_ERR, LEN_RECD);
   sprintf (t, "%c=ILLEGAL", a_val);
   if (a_cat > 0)  ystrlcat (s_fancy, ", ", LEN_RECD);
   ystrlcat (s_fancy, t, LEN_RECD);
   s_last = 'e';
   return -1;
}

char
yEXEC_flags_more        (int a_dur, int a_floor, char *a_flags, char *a_value, char *a_track, char *a_rolling, char *a_strict, char *a_min, int *a_mindur, char *a_max, int *a_maxdur, char *a_remedy, char *a_flex, char *a_throttle, char *a_cpu, char *a_disk, char *a_net)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_final     =    0;
   int         x_len       =    0;
   int         x_min       =    0;
   int         x_max       =    0;
   char        i           =    0;
   char        j           =    0;
   char        c           =  '-';
   char        x_real      =  '-';
   char        x_flags     [LEN_LABEL] = "--´---·-·---";
   char        t           [LEN_LABEL] = "";
   char       *p           = NULL;
   /*---(defaults)-----------------------*/
   if (a_value    != NULL)  *a_value    = '·';
   if (a_track    != NULL)  *a_track    = '·';
   if (a_rolling  != NULL)  *a_rolling  = '´';
   if (a_strict   != NULL)  *a_strict   = '·';
   if (a_min      != NULL)  *a_min      = '·';
   if (a_mindur   != NULL)  *a_mindur   =   0;
   if (a_max      != NULL)  *a_max      = '·';
   if (a_maxdur   != NULL)  *a_maxdur   = 9999999;
   if (a_remedy   != NULL)  *a_remedy   = '·';
   if (a_flex     != NULL)  *a_flex     = '·';
   if (a_throttle != NULL)  *a_throttle = '·';
   if (a_cpu      != NULL)  *a_cpu      = '·';
   if (a_disk     != NULL)  *a_disk     = '·';
   if (a_net      != NULL)  *a_net      = '·';
   /*---(prepare)------------------------*/
   s_last = '-';
   ystrlcpy (s_terse, "", LEN_HUND);
   ystrlcpy (s_fancy, "", LEN_RECD);
   /*---(defense)------------------------*/
   --rce;  if (a_flags != NULL) {
      x_len = strlen (a_flags);
      if (x_len >= 12)  x_len = 12;
      for (j = 0; j < x_len; ++j)   x_flags [j] = a_flags [j];
   }
   /*---(importance)---------------------*/
   c = x_flags [i++];
   switch (c) {
   case 'H' :  c = 'v';  break;
   case 'M' :  c = 'c';  break;
   case 'L' :  c = 'g';  break;
   }
   rc = yexec__findflag (0, c, &x_real);
   --rce;  if (rc < 0)  x_final = rce;
   if (a_value   != NULL)  *a_value = x_real;
   /*---(tracking)-----------------------*/
   c = x_flags [i++];
   rc = yexec__findflag (1, c, &x_real);
   --rce;  if (rc < 0)  x_final = rce;
   if (a_track   != NULL)  *a_track   = x_real;
   /*---(rolling)------------------------*/
   c = x_flags [i++];
   if (strchr ("-·", c) != NULL)  c = '´';
   rc = yexec__findflag (2, c, &x_real);
   --rce;  if (rc < 0)  x_final = rce;
   if (a_rolling != NULL)  *a_rolling = x_real;
   /*---(strictness)---------------------*/
   c = x_flags [i++];
   rc = yexec__findflag (3, c, &x_real);
   --rce;  if (rc < 0)  x_final = rce;
   if (a_strict  != NULL)  *a_strict  = x_real;
   /*---(minimum estimate)---------------*/
   c = x_flags [i++];
   rc = yexec__findflag (4, c, &x_real);
   --rce;  if (rc < 0)  x_final = rce;
   if (a_min     != NULL)  *a_min     = x_real;
   rc = yexec_min_in_msec (a_dur, x_real, a_floor, &x_min);
   --rce;  if (rc < 0)  x_final = rce;
   else if (a_mindur  != NULL)  *a_mindur = x_min;
   /*---(maximum estimate)---------------*/
   c = x_flags [i++];
   rc = yexec__findflag (5, c, &x_real);
   --rce;  if (rc < 0)  x_final = rce;
   if (a_max     != NULL)  *a_max     = x_real;
   rc = yexec_max_in_msec (a_dur, x_real, x_min  , &x_max);
   --rce;  if (rc < 0)  x_final = rce;
   else if (a_maxdur  != NULL)  *a_maxdur = x_max;
   /*---(recovery)-----------------------*/
   c = x_flags [i++];
   rc = yexec__findflag (6, c, &x_real);
   --rce;  if (rc < 0)  x_final = rce;
   if (a_remedy  != NULL)  *a_remedy  = x_real;
   /*---(flexibility)--------------------*/
   c = x_flags [i++];
   rc = yexec__findflag (7, c, &x_real);
   --rce;  if (rc < 0)  x_final = rce;
   if (a_flex    != NULL)  *a_flex  = x_real;
   /*---(throttling)---------------------*/
   c = x_flags [i++];
   rc = yexec__findflag (8, c, &x_real);
   --rce;  if (rc < 0)  x_final = rce;
   if (a_throttle != NULL)  *a_throttle = x_real;
   /*---(cpu use)------------------------*/
   c = x_flags [i++];
   rc = yexec__findflag (9, c, &x_real);
   --rce;  if (rc < 0)  x_final = rce;
   if (a_cpu      != NULL)  *a_cpu      = x_real;
   /*---(disk use)-----------------------*/
   c = x_flags [i++];
   rc = yexec__findflag (10, c, &x_real);
   --rce;  if (rc < 0)  x_final = rce;
   if (a_disk     != NULL)  *a_disk     = x_real;
   /*---(net use)------------------------*/
   c = x_flags [i++];
   rc = yexec__findflag (11, c, &x_real);
   --rce;  if (rc < 0)  x_final = rce;
   if (a_net      != NULL)  *a_net      = x_real;
   /*---(wrap)---------------------------*/
   ystrlcat (s_terse, BOLD_OFF, LEN_HUND);
   ystrlcat (s_fancy, BOLD_OFF, LEN_RECD);
   s_last = '-';
   /*---(complete)-----------------------*/
   return x_final;
}

char
yEXEC_flags_feedback    (char *a_terse, char *a_fancy)
{
   if (a_terse != NULL)  ystrlcpy (a_terse, s_terse, LEN_HUND);
   if (a_fancy != NULL)  ystrlcpy (a_fancy, s_fancy, LEN_RECD);
   return 0;
}

char
yEXEC_flags             (int a_dur, int a_floor, char *a_flags, char *a_value, char *a_track, char *a_rolling, char *a_strict, char *a_min, int *a_mindur, char *a_max, int *a_maxdur, char *a_remedy)
{
   return yEXEC_flags_more (a_dur, a_floor, a_flags, a_value, a_track, a_rolling, a_strict, a_min, a_mindur, a_max, a_maxdur, a_remedy, NULL, NULL, NULL, NULL, NULL);
}


/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;};

char          unit_answer [LEN_RECD];

char*            /*--> unit test accessor ------------------------------*/
yexec_base__unit        (char *a_question)
{
   /*---(locals)-----------+-----+-----+-*/
   char        s           [LEN_RECD]  = "";
   char        t           [LEN_RECD]  = "";
   char        u           [LEN_RECD]  = "";
   int         c           =    0;
   /*---(prepare)------------------------*/
   strncpy  (unit_answer, "BASE             : question not understood", LEN_RECD);
   /*---(crontab name)-------------------*/
   if      (strcmp (a_question, "terse"   )        == 0) {
      snprintf (unit_answer, LEN_RECD, "BASE terse       : [%-60s]", s_terse);
   }
   else if (strcmp (a_question, "fancy"   )        == 0) {
      snprintf (unit_answer, LEN_RECD, "BASE fancy       : [%-60s]", s_fancy);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}

char       /*----: set up program urgents/debugging --------------------------*/
yexec__unit_quiet       (void)
{
   yURG_err_none ();
   yURG_msg_none ();
   return 0;
}

char       /*----: set up program urgents/debugging --------------------------*/
yexec__unit_loud        (void)
{
   yLOGS_begin   ("yEXEC", YLOG_SYS, YLOG_NOISE);
   yURG_by_name     ("yexec", YURG_ON);
   yURG_err_none ();
   yURG_msg_none ();
   DEBUG_YEXEC  yLOG_info     ("yEXEC"     , yEXEC_version   ());
   return 0;
}

char       /*----: stop logging ----------------------------------------------*/
yexec__unit_end         (void)
{
   yexec_mon__ppurge ();
   yLOGS_end     ();
   return 0;
}




/*============================----end-of-source---============================*/
