/*============================----beg-of-source---============================*/
#include   "yEXEC.h"
#include   "yEXEC_priv.h"



char
yexec_data__pubname     (short a_rpid, char a_unit, char *r_public)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   char        x_file      [LEN_RECD]  = "";
   FILE       *f;
   char        x_recd      [LEN_RECD]  = "";
   int         l           =    0;
   /*---(header)------------------------*/
   DEBUG_YEXEC   yLOG_enter   (__FUNCTION__);
   /*---(default)-----------------------*/
   if (r_public != NULL)  ystrlcpy (r_public, "", LEN_TITLE);
   /*---(decide file)--------------------*/
   if (a_unit != 'y')  sprintf (x_file, "/proc/%d/comm", a_rpid);
   else                sprintf (x_file, "/tmp/%d_comm" , a_rpid);
   DEBUG_YEXEC   yLOG_info    ("x_file"    , x_file);
   /*---(open proc)----------------------*/
   f = fopen (x_file, "rt");
   DEBUG_YEXEC   yLOG_point   ("f"         , f);
   --rce;  if (f == NULL) {
      DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(read line)---------------------*/
   fgets (x_recd, LEN_RECD, f);
   l = strlen (x_recd);
   if (x_recd [l - 1] == '\n')  x_recd [--l] = '\0';
   DEBUG_YEXEC   yLOG_info    ("x_recd"    , x_recd);
   /*---(close file)--------------------*/
   rc = fclose (f);
   DEBUG_YEXEC   yLOG_value   ("close"     , rc);
   --rce;  if (f <  0) {
      DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save-back)---------------------*/
   if (r_public != NULL)  ystrlcpy (r_public, x_recd, LEN_TITLE);
   /*---(complete)----------------------*/
   DEBUG_YEXEC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yexec_data__cmdline     (short a_rpid, char a_unit, char *r_cmdline)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   char        x_file      [LEN_RECD]  = "";
   FILE       *f;
   char        x_recd      [LEN_RECD]  = "";
   int         i           =    0;
   int         l           =    0;
   /*---(header)------------------------*/
   DEBUG_YEXEC   yLOG_enter   (__FUNCTION__);
   /*---(default)-----------------------*/
   if (r_cmdline != NULL)  ystrlcpy (r_cmdline, "", LEN_RECD);
   /*---(defense)-----------------------*/
   if (a_unit != 'y')  sprintf (x_file, "/proc/%d/cmdline", a_rpid);
   else                sprintf (x_file, "/tmp/%d_cmdline" , a_rpid);
   DEBUG_YEXEC   yLOG_info    ("x_file"    , x_file);
   /*---(open proc)----------------------*/
   f = fopen (x_file, "rt");
   DEBUG_YEXEC   yLOG_point   ("f"         , f);
   --rce;  if (f == NULL) {
      DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(read line)---------------------*/
   fgets (x_recd, LEN_RECD, f);
   while (1) {
      if (x_recd [i] == '\0' && x_recd [i + 1] == '\0')  break;
      if (x_recd [i] == '\0')  x_recd [i] = ' ';
      ++i;
   }
   l = strlen (x_recd);
   if (x_recd [l - 1] == '\n')  x_recd [--l] = '\0';
   DEBUG_YEXEC   yLOG_info    ("x_recd"    , x_recd);
   /*---(close file)--------------------*/
   rc = fclose (f);
   DEBUG_YEXEC   yLOG_value   ("close"     , rc);
   --rce;  if (f <  0) {
      DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save-back)---------------------*/
   if (r_cmdline != NULL)  ystrlcpy (r_cmdline, x_recd, LEN_RECD);
   /*---(complete)----------------------*/
   DEBUG_YEXEC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yexec_data__ppid        (short a_rpid, char a_unit, char *r_state, int *r_ppid)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   char        x_file      [LEN_RECD]  = "";
   FILE       *f;
   char        x_recd      [LEN_RECD]  = "";
   char       *p           = NULL;
   char       *r           = NULL;
   char        c           =    0;
   char        x_state     =    0;
   int         x_ppid      =    0;
   /*---(header)------------------------*/
   DEBUG_YEXEC   yLOG_enter   (__FUNCTION__);
   /*---(default)-----------------------*/
   if (r_state   != NULL)  *r_state = '-';
   if (r_ppid    != NULL)  *r_ppid  =  -1;
   /*---(defense)-----------------------*/
   if (a_unit != 'y')  sprintf (x_file, "/proc/%d/stat", a_rpid);
   else                sprintf (x_file, "/tmp/%d_stat" , a_rpid);
   DEBUG_YEXEC   yLOG_info    ("x_file"    , x_file);
   /*---(open proc)----------------------*/
   f = fopen (x_file, "rt");
   DEBUG_YEXEC   yLOG_point   ("f"         , f);
   --rce;  if (f == NULL) {
      DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(read line)---------------------*/
   fgets (x_recd, LEN_RECD, f);
   p = strtok_r (x_recd, " ", &r);
   --rce;  while (p != NULL && c < 4) {
      DEBUG_YEXEC   yLOG_info    ("p"         , p);
      switch (c) {
      case  2 : x_state = p [0];    break;
      case  3 : x_ppid  = atoi (p); break;
      }
      ++c;
      p = strtok_r (NULL  , " ", &r);
   }
   /*---(close file)--------------------*/
   rc = fclose (f);
   DEBUG_YEXEC   yLOG_value   ("close"     , rc);
   --rce;  if (f <  0) {
      DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(trouble)-----------------------*/
   DEBUG_YEXEC   yLOG_value   ("c"         , c);
   --rce;  if (c <  4) {
      rc = fclose (f);
      DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save-back)---------------------*/
   if (r_state   != NULL)  *r_state = x_state;
   if (r_ppid    != NULL)  *r_ppid  = x_ppid;
   /*---(complete)----------------------*/
   DEBUG_YEXEC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yEXEC_proc_data         (short a_rpid, char *r_public, char *r_cmdline, char *r_state, int *r_ppid)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   /*---(header)------------------------*/
   DEBUG_YEXEC   yLOG_enter   (__FUNCTION__);
   /*---(public name)-------------------*/
   rc = yexec_data__pubname     (a_rpid, '-', r_public);
   DEBUG_YEXEC   yLOG_value   ("pubname"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(command line)------------------*/
   rc = yexec_data__cmdline     (a_rpid, '-', r_cmdline);
   DEBUG_YEXEC   yLOG_value   ("cmdline"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(parent and state)--------------*/
   rc = yexec_data__ppid        (a_rpid, '-', r_state, r_ppid);
   DEBUG_YEXEC   yLOG_value   ("ppid"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)----------------------*/
   DEBUG_YEXEC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yexec_proc__eterm       (short a_rpid, char a_unit, int *r_uid, char *r_home, char *r_pwd, char *r_cabbr, char *r_cname, char *r_stype, char *r_shex, char *r_rows, char *r_cols)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   char        x_file      [LEN_RECD]  = "";
   FILE       *f;
   char        x_recd      [LEN_HUGE]  = "";
   int         i           =    0;
   int         l           =    0;
   char       *p           = NULL;
   char       *r           = NULL;
   char        c           =    0;
   char        x_state     =    0;
   int         x_ppid      =    0;
   char        x_type      =  '·';
   char        t           [LEN_TERSE] = "";
   /*---(header)------------------------*/
   DEBUG_YEXEC   yLOG_enter   (__FUNCTION__);
   /*---(default)-----------------------*/
   if (r_uid     != NULL)  *r_uid    = -1;
   if (r_home    != NULL)  ystrlcpy (r_home  , "", LEN_PATH);
   if (r_pwd     != NULL)  ystrlcpy (r_pwd   , "", LEN_PATH);
   if (r_cabbr   != NULL)  *r_cabbr  = '·';
   if (r_cname   != NULL)  ystrlcpy (r_cname , "", LEN_TERSE);
   if (r_stype   != NULL)  *r_stype  = '·';
   if (r_shex    != NULL)  ystrlcpy (r_shex  , "", LEN_TERSE);
   if (r_rows    != NULL)  *r_rows   = 0;
   if (r_cols    != NULL)  *r_cols   = 0;
   /*---(defense)-----------------------*/
   if (a_unit != 'y')  sprintf (x_file, "/proc/%d/environ", a_rpid);
   else                sprintf (x_file, "/tmp/%d_environ" , a_rpid);
   DEBUG_YEXEC   yLOG_info    ("x_file"    , x_file);
   /*---(open proc)----------------------*/
   f = fopen (x_file, "rt");
   DEBUG_YEXEC   yLOG_point   ("f"         , f);
   --rce;  if (f == NULL) {
      DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(read line)---------------------*/
   fgets (x_recd, LEN_HUGE, f);
   while (1) {
      if (x_recd [i] == '\0' && x_recd [i + 1] == '\0')  break;
      if (x_recd [i] == '\0')  x_recd [i] = '';
      ++i;
   }
   l = strlen (x_recd);
   if (x_recd [l - 1] == '\n')  x_recd [--l] = '\0';
   /*> printf ("%70.70s\n", x_recd);                                                  <*/
   /*---(parse up)----------------------*/
   p = strtok_r (x_recd, "", &r);
   --rce;  while (p != NULL && c < 4) {
      DEBUG_YEXEC   yLOG_info    ("p"         , p);
      /*> printf ("å%.6\n", p);                                                       <*/
      switch (p [0]) {
      case 'U'  :
         if (strncmp (p, "UID="       ,  4) == 0 && r_uid   != NULL)  *r_uid  = atoi (p + 4);
         break;
      case 'H'  :
         if (strncmp (p, "HOME="      ,  5) == 0 && r_home  != NULL)  ystrlcpy (r_home, p + 5, LEN_PATH);
         break;
      case 'P'  :
         if (strncmp (p, "PWD="       ,  4) == 0 && r_pwd   != NULL)  ystrlcpy (r_pwd , p + 4, LEN_PATH);
         break;
      case 'C'  :
         if (strncmp (p, "COLUMNS="   ,  8) == 0 && r_cols  != NULL)  *r_cols = atoi (p + 8);
         break;
      case 'R'  :
         if (strncmp (p, "LINES="     ,  6) == 0 && r_rows  != NULL)  *r_rows = atoi (p + 6);
         break;
      case 't'  :
         if (strncmp (p, "theia_text=", 11) == 0) {
            if (r_stype != NULL)   *r_stype = p [12];
            if (r_shex  != NULL)   sprintf (*r_shex , "%2.2s", p + 16);
         }
         if (strncmp (p, "theia_tint=", 11) == 0) {
            if (r_cabbr != NULL)   *r_cabbr = p [12];
            if (r_cname != NULL)   sprintf (*r_cname, "%5.5s", p + 16);
         }
      }
      /*---(next)-----------------------*/
      p = strtok_r (NULL  , "", &r);
   }
   /*---(close file)--------------------*/
   rc = fclose (f);
   DEBUG_YEXEC   yLOG_value   ("close"     , rc);
   --rce;  if (f <  0) {
      DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save-back)---------------------*/

   /*---(complete)----------------------*/
   DEBUG_YEXEC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yEXEC_proc_eterm        (short a_rpid, int *r_uid, char *r_home, char *r_pwd, char *r_cabbr, char *r_cname, char *r_stype, char *r_shex, char *r_rows, char *r_cols)
{
   return yexec_proc__eterm (a_rpid, '-', r_uid, r_home, r_pwd, r_cabbr, r_cname, r_stype, r_shex, r_rows, r_cols);
}

char
yEXEC_find_my_eterm     (short a_rpid, int *a_eterm)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   int         x_rpid      =    0;
   int         x_ppid      =    0;
   char        x_pubname   [LEN_LABEL] = "";
   /*---(header)------------------------*/
   DEBUG_YEXEC   yLOG_enter   (__FUNCTION__);
   /*---(prepare)-----------------------*/
   if (a_eterm != NULL)  *a_eterm = -1;
   x_rpid = a_rpid;
   /*---(climb tree)--------------------*/
   while (x_rpid > 1) {
      /*---(public name)-------------------*/
      rc = yexec_data__pubname     (x_rpid, '-', x_pubname);
      DEBUG_YEXEC   yLOG_value   ("pubname"   , rc);
      --rce;  if (rc < 0) {
         DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_YEXEC   yLOG_info    ("x_pubname" , x_pubname);
      /*---(check)-------------------------*/
      if (strcmp (x_pubname, "Eterm") == 0)  {
         DEBUG_YEXEC   yLOG_note    ("found it");
         if (a_eterm != NULL)  *a_eterm = x_rpid;
         DEBUG_YEXEC   yLOG_exit    (__FUNCTION__);
         return 0;
      }
      /*---(find the parent)---------------*/
      rc = yexec_data__ppid        (x_rpid, '-', NULL, &x_ppid);
      DEBUG_YEXEC   yLOG_value   ("ppid"      , rc);
      --rce;  if (rc < 0) {
         DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      /*---(next)--------------------------*/
      x_rpid = x_ppid;
      /*---(done)--------------------------*/
   }
   /*---(not found)---------------------*/
   DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, --rce);
   return rce;
}


static short    s_rpid      =  0;
static char     s_pubname   [LEN_LABEL] = "";
static char     s_cmdline   [LEN_RECD]  = "";
static int      s_count     =  0;

char
yexec_find_child        (short a_rpid, char *a_pubname, char *a_cmdline, char a_state, short a_ppid)
{
   /*---(header)------------------------*/
   DEBUG_YEXEC   yLOG_enter   (__FUNCTION__);
   s_rpid = a_rpid;
   ystrlcpy (s_pubname, a_pubname, LEN_LABEL);
   ystrlcpy (s_cmdline, a_cmdline, LEN_RECD);
   ++s_count;
   DEBUG_YEXEC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yEXEC_find_eterm_use    (short a_rpid, char *a_lvl, int *a_lowest, char a_pubname [LEN_LABEL], char a_cmdline [LEN_RECD])
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   int         x_rpid      =    0;
   int         x_ppid      =    0;
   char        x_pubname   [LEN_LABEL] = "";
   char        x_lvl       =   -1;
   /*---(header)------------------------*/
   DEBUG_YEXEC   yLOG_enter   (__FUNCTION__);
   /*---(prepare)-----------------------*/
   x_ppid  = a_rpid;
   s_rpid  = 0;
   ystrlcpy (s_pubname, "", LEN_LABEL);
   ystrlcpy (s_cmdline, "", LEN_RECD);
   s_count = 1;
   /*---(walk down)---------------------*/
   while (s_count != 0) {
      s_count = 0;
      rc = yEXEC_data_filter (0, NULL, x_ppid, yexec_find_child);
      DEBUG_FILE   yLOG_complex ("s_count"   , "%4d, %2d, %6d, %s, %s", rc, s_count, s_rpid, s_pubname, s_cmdline);
      x_ppid = s_rpid;
      ++x_lvl;
   }
   /*---(save back)---------------------*/
   if (a_lvl     != NULL)  *a_lvl    = x_lvl;
   if (a_lowest  != NULL)  *a_lowest = s_rpid;
   if (a_pubname != NULL)  ystrlcpy (a_pubname, s_pubname, LEN_LABEL);
   if (a_cmdline != NULL)  ystrlcpy (a_cmdline, s_cmdline, LEN_RECD);
   /*---(not found)---------------------*/
   DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, 0);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                    walk the proc directory                   ----===*/
/*====================------------------------------------====================*/
static void      o___WALKING_________________o (void) {;}

static DIR        *s_dir       = NULL;
static tDIRENT    *s_file      = NULL;

char
yexec_data__open        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YEXEC   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YEXEC   yLOG_spoint  (s_dir);
   --rce;  if (s_dir != NULL) {
      DEBUG_YEXEC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(open)---------------------------*/
   DEBUG_YEXEC   yLOG_snote   ("/proc");
   s_dir = opendir ("/proc");
   DEBUG_YEXEC   yLOG_spoint  (s_dir);
   --rce;  if (s_dir == NULL) {
      DEBUG_YEXEC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YEXEC   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
yexec_data__close       (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YEXEC   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YEXEC   yLOG_spoint  (s_dir);
   --rce;  if (s_dir == NULL) {
      DEBUG_YEXEC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(close)--------------------------*/
   rc = closedir (s_dir);
   DEBUG_YEXEC   yLOG_sint    (rc);
   --rce;  if (rc < 0) {
      DEBUG_YEXEC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(ground)-------------------------*/
   s_dir = NULL;
   DEBUG_YEXEC   yLOG_spoint  (s_dir);
   /*---(complete)-----------------------*/
   DEBUG_YEXEC   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
yEXEC_data_filter       (short a_rpid, char *a_pubname, short a_ppid, void *f_callback)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   tDIRENT    *x_file      = NULL;
   int         x_rpid      =    0;
   char        x_pubname   [LEN_LABEL] = "";
   char        x_cmdline   [LEN_RECD]  = "";
   char        x_state     =    0;
   int         x_ppid      =    0;
   char      (*x_callback)   (short a_rpid, char *a_pubname, char *a_cmdline, char a_state, short a_ppid);
   char        x_call      =  '-';
   int         c           =    0;
   /*---(header)-------------------------*/
   DEBUG_YEXEC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YEXEC  yLOG_point   ("f_callback", f_callback);
   --rce;  if (f_callback == NULL) {
      DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_callback = f_callback;
   /*---(open)---------------------------*/
   rc = yexec_data__open ();
   DEBUG_YEXEC   yLOG_value   ("open"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(walk files)---------------------*/
   DEBUG_YEXEC   yLOG_note    ("walk through processes");
   while ((x_file = readdir (s_dir)) != NULL) {
      /*---(simple filtering)------------*/
      if (strchr (YSTR_NUMBER, x_file->d_name [0]) == NULL) {
         DEBUG_YEXEC   yLOG_note    ("not leading number");
         continue;
      }
      x_rpid = atoi (x_file->d_name);
      DEBUG_YEXEC   yLOG_value   ("x_rpid"    , x_rpid);
      if (x_rpid <= 0) {
         DEBUG_YEXEC   yLOG_note    ("not a process entry");
         continue;
      }
      /*---(public name)-----------------*/
      rc = yexec_data__pubname     (x_rpid, '-', x_pubname);
      DEBUG_YEXEC   yLOG_value   ("pubname"   , rc);
      --rce;  if (rc < 0) {
         DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      /*---(command line)----------------*/
      rc = yexec_data__cmdline     (x_rpid, '-', x_cmdline);
      DEBUG_YEXEC   yLOG_value   ("cmdline"   , rc);
      --rce;  if (rc < 0) {
         DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      /*---(parent and state)------------*/
      rc = yexec_data__ppid        (x_rpid, '-', &x_state, &x_ppid);
      DEBUG_YEXEC   yLOG_value   ("ppid"      , rc);
      --rce;  if (rc < 0) {
         DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      /*---(figure out callback)---------*/
      x_call = '-';
      if (a_pubname != NULL && strcmp (a_pubname, x_pubname) == 0)  x_call = 'y';
      else if (a_rpid    != 0    && a_rpid == x_rpid)               x_call = 'y';
      else if (a_ppid    != 0    && a_ppid == x_ppid)               x_call = 'y';
      if (x_call == 'y') {
         rc = x_callback (x_rpid, x_pubname, x_cmdline, x_state, x_ppid);
         DEBUG_YEXEC   yLOG_value   ("callback"  , rc);
      }
      /*---(done)------------------------*/
   }
   /*---(close)--------------------------*/
   rc = yexec_data__close ();
   DEBUG_YEXEC   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YEXEC   yLOG_exit    (__FUNCTION__);
   return rc;
}


