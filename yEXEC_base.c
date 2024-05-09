/*============================----beg-of-source---============================*/
#include   "yEXEC.h"
#include   "yEXEC_priv.h"





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
   yexec_heart_reset ();
   return 0;
}

char       /*----: set up program urgents/debugging --------------------------*/
yexec__unit_loud        (void)
{
   yLOGS_begin   ("yEXEC_unit", YLOG_SYS, YLOG_NOISE);
   yURG_by_name     ("yexec", YURG_ON);
   yURG_err_none ();
   yURG_msg_none ();
   DEBUG_YEXEC  yLOG_info     ("yEXEC"     , yEXEC_version   ());
   yexec_heart_reset ();
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
