/*============================[[    beg-code    ]]============================*/
#include   "yEXEC.h"
#include   "yEXEC_priv.h"

/*
 *  heartbeats help identify frozen and zombie daemons
 *
 *
 */

static llong  s_start   = 0;
static char   s_elapsed [LEN_LABEL] = "";


/*====================------------------------------------====================*/
/*===----                       informational                          ----===*/
/*====================------------------------------------====================*/
static void      o___HEARTBEAT_______________o (void) {;}

char        s_heartbeat      [LEN_HUND];

char yexec_heart_reset   (void)  { return s_start = 0; }

char         /*--> write the heartbeat file ----------------------------------*/
yexec__heartbeat        (char a_type, int a_rpid, long a_now, short a_update, char *a_suffix, char *a_file, char *a_heartbeat)
{  /*---(design)-------------------------*/
   /*
    * strftime    %u   dow   1 (mo) - 7 (su)
    *                           ISO starts on sun and at 0
    *
    *             %W   woy   full weeks are 1-52, partial 0 and 53 (YES!!!)
    *                           ISO makes first partial part of last year ;(
    *
    *             both these match yLOG and ySCHED, and everywhere
    */
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   long        x_now       =    0;
   tTIME      *x_broke     = NULL;
   char        t           [LEN_TITLE] = "";
   FILE       *f           = NULL;
   char        x_update    [LEN_SHORT] = "";
   char        x_duration  [LEN_SHORT] = "";
   /*---(header)-------------------------*/
   if (a_type != 'q') { DEBUG_YEXEC  yLOG_enter   (__FUNCTION__); }
   /*---(set time)-----------------------*/
   if (a_now > 0)  x_now  = a_now;
   else            x_now  = time (NULL);
   if (a_type != 'q') { DEBUG_YEXEC  yLOG_value   ("x_now"     , x_now); }
   if (s_start == 0)    s_start = x_now;
   /*---(break it down)------------------*/
   x_broke   = localtime (&x_now);
   /*---(heartbeat)----------------------*/
   strftime (t, LEN_TITLE, "%y.%m.%d.%H.%M.%S.%u.%W.%j", x_broke);
   if (a_type != 'q') { DEBUG_YEXEC  yLOG_info    ("t"         , t); }
   sprintf  (s_heartbeat, "%-26.26s  %-10d  %6d", t, x_now, a_rpid);
   if (a_type == 'L') {
      /*---(update period)---------------*/
      ystrldur  (a_update, 'u', x_update);
      ystrlcat  (s_heartbeat, "   "     , LEN_HUND);
      ystrlcat  (s_heartbeat, x_update  , LEN_HUND);
      /*---(running duration)------------*/
      ystrlager (x_now, s_start , 'u', x_duration);
      ystrlcat  (s_heartbeat, "   "     , LEN_HUND);
      ystrlcat  (s_heartbeat, x_duration, LEN_HUND);
      /*---(done)------------------------*/
   }
   if (a_suffix != NULL) {
      ystrlcat (s_heartbeat, "  "     , LEN_HUND);
      ystrlcat (s_heartbeat, a_suffix , LEN_HUND);
   }
   if (a_type != 'q') { DEBUG_YEXEC  yLOG_info    ("heartbeat" , s_heartbeat); }
   if (a_heartbeat != NULL)  ystrlcpy (a_heartbeat, s_heartbeat, LEN_HUND);
   if (a_type != 'q') { DEBUG_YEXEC  yLOG_point   ("a_file"    , a_file); }
   /*---(write file)---------------------*/
   if (a_file != NULL) {
      /*---(open)------------------------*/
      if (a_type != 'q') { DEBUG_YEXEC  yLOG_info    ("a_file"    , a_file); }
      f = fopen (a_file, "wt");
      if (a_type != 'q') { DEBUG_YEXEC  yLOG_point   ("f"         , f); }
      --rce;  if (rc < 0) {
         if (a_type != 'q') { DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce); }
         return rce;
      }
      /*---(write)-----------------------*/
      fprintf (f, "%s\n", s_heartbeat);
      /*---(close)-----------------------*/
      rc = fclose (f);
      if (a_type != 'q') { DEBUG_YEXEC  yLOG_value   ("close"     , rc); }
      --rce;  if (rc < 0) {
         if (a_type != 'q') { DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce); }
         return rce;
      }
   }
   /*---(complete------------------------*/
   if (a_type != 'q') { DEBUG_YEXEC  yLOG_exit    (__FUNCTION__); }
   return 0;
}

char         /*--> write the heartbeat file ----------------------------------*/
yEXEC_heartbeat         (int a_rpid, long a_now, char *a_suffix, char *a_file, char *a_heartbeat)
{
   return yexec__heartbeat ('-', a_rpid, a_now, 0, a_suffix, a_file, a_heartbeat);
}

char         /*--> write the heartbeat file ----------------------------------*/
yEXEC_heartquiet        (int a_rpid, long a_now, char *a_suffix, char *a_file, char *a_heartbeat)
{
   return yexec__heartbeat ('q', a_rpid, a_now, 0, a_suffix, a_file, a_heartbeat);
}

char         /*--> write the heartbeat file ----------------------------------*/
yEXEC_heartlong         (int a_rpid, long a_now, short a_update, char *a_suffix, char *a_file, char *a_heartbeat)
{
   return yexec__heartbeat ('L', a_rpid, a_now, a_update, a_suffix, a_file, a_heartbeat);
}

char         /*--> read the last heartbeat -----------------------------------*/
yEXEC_heartbeat_check   (char *a_file, char *a_heartbeat)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   FILE       *f           = NULL;
   int         l           =    0;
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_enter   (__FUNCTION__);
   /*---(open)---------------------------*/
   DEBUG_YEXEC  yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_info    ("a_file"    , a_file);
   f = fopen (a_file, "rt");
   DEBUG_YEXEC  yLOG_point   ("f"         , f);
   --rce;  if (rc < 0) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write)--------------------------*/
   fgets (a_heartbeat, LEN_HUND, f);
   DEBUG_YEXEC  yLOG_info    ("heartbeat" , a_heartbeat);
   l = strlen (a_heartbeat) - 1;
   if (a_heartbeat [l] == '\n')  a_heartbeat [l] = '\0';
   /*---(close)--------------------------*/
   rc = fclose (f);
   DEBUG_YEXEC  yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YEXEC  yLOG_exit    (__FUNCTION__);
   return 0;
}




/*====================------------------------------------====================*/
/*===----                       informational                          ----===*/
/*====================------------------------------------====================*/
static void      o___INFO____________________o (void) {;}

char         /*--> verify a user name ----------------------------------------*/
yEXEC_userdata          (char *a_user, int *a_uid, int *a_gid, char *a_dir, char *a_shell)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         x_len       =    0;
   int         i           =    0;
   char       *x_valid     = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
   tPASSWD    *x_pass      = NULL;
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_enter   (__FUNCTION__);
   /*---(default save backs)-------------*/
   if (a_uid   != NULL)  *a_uid = -1;
   if (a_gid   != NULL)  *a_gid = -1;
   if (a_dir   != NULL)  strcpy (a_dir, "");
   if (a_shell != NULL)  strcpy (a_shell, "");
   /*---(defense)------------------------*/
   DEBUG_YEXEC  yLOG_point   ("a_user"    , a_user);
   --rce;  if (a_user == NULL) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_info    ("a_user"    , a_user);
   /*---(check length)-------------------*/
   x_len = strlen (a_user);
   DEBUG_YEXEC  yLOG_value   ("x_len"     , x_len);
   DEBUG_YEXEC  yLOG_value   ("min"       , 3);
   --rce;  if (x_len <=  3) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_value   ("max"       , LEN_USER);
   --rce;  if (x_len >= LEN_USER) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check letters)------------------*/
   --rce;  for (i = 0; i < x_len; ++i) {
      if (strchr (x_valid, a_user [i]) != NULL)  continue;
      DEBUG_YEXEC  yLOG_value   ("position"  , i);
      DEBUG_YEXEC  yLOG_char    ("illegal"   , a_user [i]);
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check validity)-----------------*/
   x_pass    = getpwnam (a_user);
   DEBUG_YEXEC  yLOG_point   ("x_pass"    , x_pass);
   --rce;  if (x_pass == NULL) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save)---------------------------*/
   DEBUG_YEXEC  yLOG_value   ("uid"       , x_pass->pw_uid);
   if (a_uid   != NULL)  *a_uid = x_pass->pw_uid;
   DEBUG_YEXEC  yLOG_value   ("gid"       , x_pass->pw_gid);
   if (a_gid   != NULL)  *a_gid = x_pass->pw_gid;
   DEBUG_YEXEC  yLOG_info    ("dir"       , x_pass->pw_dir);
   if (a_dir   != NULL)  strcpy (a_dir, x_pass->pw_dir);
   DEBUG_YEXEC  yLOG_info    ("shell"     , x_pass->pw_shell);
   if (a_shell != NULL)  strcpy (a_shell, x_pass->pw_shell);
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      unit test accessor                      ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char*            /*--> unit test accessor ------------------------------*/
yexec_spec__unit        (char *a_question, char *a_text)
{ 
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   char        x_heartbeat [LEN_HUND];
   /*---(prepare)------------------------*/
   ystrlcpy  (unit_answer, "SPEC             : question not understood", LEN_RECD);
   /*---(crontab name)-------------------*/
   if      (strcmp (a_question, "heartbeat"     )  == 0) {
      snprintf (unit_answer, LEN_RECD, "SELF heartbeat   : %2d[%s]", strlen (s_heartbeat), s_heartbeat);
   }
   else if (strcmp (a_question, "lastbeat"      )  == 0) {
      yEXEC_heartbeat_check (a_text, x_heartbeat);
      snprintf (unit_answer, LEN_RECD, "SELF lastbeat    : %2d[%s]", strlen (x_heartbeat), x_heartbeat);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}



