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
   snprintf (yEXEC_ver, 100, "%s   %s : %s", t, YEXEC_VER_NUM, YEXEC_VER_TXT);
   return yEXEC_ver;
}



/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;};

char          unit_answer [LEN_RECD];

char       /*----: set up program urgents/debugging --------------------------*/
yexec__unit_quiet       (void)
{
   yLOG_begin ("yEXEC", yLOG_SYSTEM, yLOG_QUIET);
   return 0;
}

char       /*----: set up program urgents/debugging --------------------------*/
yexec__unit_loud        (void)
{
   yLOG_begin   ("yEXEC", yLOG_SYSTEM, yLOG_NOISE);
   yURG_name    ("yexec", YURG_ON);
   DEBUG_YEXEC  yLOG_info     ("yEXEC"     , yEXEC_version   ());
   return 0;
}

char       /*----: stop logging ----------------------------------------------*/
yexec__unit_end         (void)
{
   yLOG_end     ();
   return 0;
}




/*============================----end-of-source---============================*/
