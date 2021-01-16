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
