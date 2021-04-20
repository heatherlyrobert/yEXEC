/*============================----beg-of-source---============================*/
#include   "yEXEC.h"
#include   "yEXEC_priv.h"



/*====================------------------------------------====================*/
/*===----                         file checking                        ----===*/
/*====================------------------------------------====================*/
static void      o___FILES___________________o (void) {;};

char
yexec__naming           (uchar *a_name)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         l           =    0;
   int         i           =    0;
   int         c           =    0;
   uchar      *p           = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name == NULL || strlen (a_name) <= 0) {
      yURG_err ('f', "file name can not be null or empty (blatant error)");
      DEBUG_INPT   yLOG_note    ("file name can not be null or empty");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_name"    , a_name);
   yURG_msg ('-', "name å%sæ is not empty/null", a_name);
   /*---(path)---------------------------*/
   p = strchr (a_name, '/');
   DEBUG_INPT   yLOG_point   ("/"         , p);
   --rce;  if (p != NULL) {
      yURG_err ('f', "file name can not include relative/absolute path (security risk)");
      DEBUG_INPT   yLOG_note    ("file name can not include relative/absolute path (security risk)");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_note    ("cronfile name is path local (pathless)");
   yURG_msg ('-', "file name is pathless/local");
   /*---(name quality)-------------------*/
   l = strlen (a_name);
   DEBUG_INPT   yLOG_value   ("l"         , l);
   --rce;  for (i = 0; i < l; ++i) {
      if (strchr (LTRS_FILES, a_name [i]) == NULL) {
         yURG_err ('f', "file name has an illegal character (%c) at position %d (security risk)", a_name [i], i);
         DEBUG_INPT   yLOG_complex ("bad char"  , "can not include %c at %d", a_name [i], i);
         DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   DEBUG_INPT   yLOG_note    ("all name characters are acceptable");
   yURG_msg ('-', "all the file name characters are legal [A-Za-z0-9_.]");
   /*---(hidden file)--------------------*/
   DEBUG_INPT   yLOG_char    ("first char", a_name [0]);
   --rce;  if (a_name [0] == '.') {
      yURG_err ('f', "file can not be a hidden file, lead period (security risk)");
      DEBUG_INPT   yLOG_note    ("file can not be a hidden file");
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "file is not a hidden file, no lead period");
   /*---(exactly one period)-------------*/
   c = strldcnt (a_name, '.', LEN_RECD);
   DEBUG_INPT   yLOG_value   ("periods"   , c);
   --rce;  if (c < 1) {
      yURG_err ('f', "file name does not have at least one separator/period (standard)");
      DEBUG_INPT   yLOG_note    ("name must have exactly one separator/period");
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "file has at least one separator/period");
   /*---(description length)-------------*/
   p = strchr (a_name, '.');
   c = p - a_name;
   --rce;  if (c < 3) {
      yURG_err ('f', "file prefix can not be shorter than 3 characters (illegal)");
      DEBUG_INPT   yLOG_note    ("prefix too short (< 3 chars)");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "name prefix is adequate length, >= 3 characters");
   c = l - c - 1;
   --rce;  if (c < 3) {
      yURG_err ('f', "file description can not be shorter than 3 characters (lazy)");
      DEBUG_INPT   yLOG_note    ("description too short (< 3 chars)");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "name description is adequate length, >= 3 characters");
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yexec__location         (cchar a_runas, cchar a_loc, cchar *a_home, cchar *a_root, uchar *a_name, cchar *a_muser, int a_muid, char *a_fuser, int *a_fuid, char *a_dir)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_cwd       [LEN_PATH]  = "";
   char       *p           = NULL;
   char        t           [LEN_HUND]  = "";
   char        x_user      [LEN_PATH]  = "";
   int         l           =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(defaults)-----------------------*/
   --rce;  if (a_home  == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_home"    , a_home);
   --rce;  if (a_root  == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_root"    , a_root);
   --rce;  if (a_name  == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_name"    , a_name);
   DEBUG_INPT  yLOG_point   ("a_muser"   , a_muser);
   --rce;  if (a_muser == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_muser"   , a_muser);
   DEBUG_INPT   yLOG_value   ("a_muid"    , a_muid);
   --rce;  if (a_fuser == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   strcpy (a_fuser, "");
   --rce;  if (a_fuid  == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   *a_fuid  = -1;
   --rce;  if (a_dir   == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   strcpy (a_dir, "");
   /*---(check directory)----------------*/
   p = getcwd (a_dir, LEN_PATH);
   DEBUG_INPT   yLOG_point   ("getcwd"    , p);
   --rce;  if (p == NULL) {
      yURG_err ('f', "can not obtain current working directory");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   if (a_loc == YEXEC_LOCAL)  strlcat (a_dir, "/", LEN_PATH);
   DEBUG_INPT   yLOG_info    ("a_dir"     , a_dir);
   yURG_msg ('-', "current working directory is å%sæ", a_dir);
   /*---(check normal install)-----------*/
   --rce;  if (a_loc == YEXEC_LOCAL && a_muid != 0) {
      sprintf (t, "%s%s", a_home, a_muser);
      DEBUG_INPT   yLOG_info    ("expect"    , t);
      /*> yURG_msg ('-', "compare to å%sæ", t);                                       <*/
      l = strlen (t);
      if (strncmp (a_dir, t, l) != 0) {
         yURG_err ('f', "user not in or below their own home directory (security risk)");
         DEBUG_INPT   yLOG_note    ("user not in or below their own home directory");
         DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      strlcpy (a_fuser, a_muser, LEN_USER);
      yURG_msg ('-', "normal user file in or below their own home directory");
   }
   /*---(check root install)-------------*/
   --rce;  if (a_loc == YEXEC_LOCAL && a_muid == 0) {
      DEBUG_INPT   yLOG_note    ("root user verifying");
      sprintf (t, "%s", a_root);
      l = strlen (t);
      DEBUG_INPT   yLOG_complex ("cdir"      , "%2d å%sæ vs å%sæ", l, t, a_dir);
      if (strncmp (a_dir, t, l) == 0) {
         DEBUG_INPT   yLOG_note    ("root in root user home directory tree");
         strlcpy (a_fuser, "root", LEN_USER);
         yURG_msg ('-', "root user file in or below root home directory");
      } else {
         sprintf (t, "%s", a_home);
         l = strlen (t);
         if (strncmp (a_dir, t, l) == 0) {
            DEBUG_INPT   yLOG_note    ("root in another user home directory tree");
            strlcpy  (x_user, a_dir + l, LEN_PATH);
            strldchg (x_user, '/', '\0', LEN_PATH);
            strlcpy (a_fuser, x_user, LEN_USER);
            yURG_msg ('-', "root user file in or below %s users home directory", x_user);
         } else {
            yURG_err ('f', "root, but not in or below any valid user home directory (security risk)");
            DEBUG_INPT   yLOG_note    ("root, but not in or below a valid user home directory");
            DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
            return rce;
         }
      }
   }
   /*---(check central audit)------------*/
   --rce;  if (a_loc == YEXEC_CENTRAL) {
      /*---(verify user)--------------------*/
      strlcpy  (x_user, a_name, LEN_USER);
      strldchg (x_user, '.', '\0', LEN_USER);
      strlcpy  (a_fuser, x_user, LEN_USER);
      yURG_msg ('-', "central file prefix is å%sæ", a_fuser);
   }
   DEBUG_INPT   yLOG_info    ("a_fuser"    , a_fuser);
   /*---(check for registered)-----------*/
   rc = yEXEC_userdata (a_fuser, a_fuid, NULL, NULL, NULL);
   DEBUG_INPT   yLOG_value   ("userdata"  , rc);
   if (rc < 0) {
      if (a_loc == YEXEC_LOCAL) {
         yURG_err ('f', "user directory not associated with a registered user (security risk)");
      } else {
         yURG_err ('f', "user name prefix å%sæ not registered on system (security risk)", a_fuser);
      }
      DEBUG_INPT   yLOG_note    ("user is not registered in the system");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "file user is registered with system, %s, uid %d", a_fuser, *a_fuid);
   /*---(name prefix)--------------------*/
   --rce;  if (a_loc == YEXEC_LOCAL) {
      if (a_runas == 'k')  strcpy (t, "khronos.");
      else                 strcpy (t, "job.");
      if (strncmp (a_name, t, strlen (t)) != 0) {
         yURG_err ('f', "local file name must be prefixed with å%sæ (standard)", t);
         DEBUG_INPT   yLOG_note    ("name does not begin with khronos/job");
         DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      yURG_msg ('-', "name is prefixed appropriately with å%sæ", t);
   }
   --rce;  if (a_loc == YEXEC_CENTRAL) {
      if (a_muid != 0) {
         if (strcmp (a_muser, a_fuser) != 0) {
            yURG_err ('f', "run-time user å%sæ can not review this file (security risk)", a_muser);
            DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
            return rce;
         }
         yURG_msg ('-', "run-time user has the appropriate access");
      } else {
         yURG_msg ('-', "run-time is root, so all access is allowed");
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yexec__stats            (cchar a_loc, cchar *a_dir, uchar *a_name, cchar *a_muser, int a_muid, uchar *a_user, int a_uid)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_full      [LEN_PATH]  = "";
   tSTAT       s;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(get stats)----------------------*/
   sprintf (x_full, "%s%s", a_dir, a_name);
   rc = lstat (x_full, &s);
   DEBUG_YEXEC  yLOG_value   ("stat"      , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "actual file could not be located in å%sæ", a_dir);
      DEBUG_INPT   yLOG_note    ("actual file could not be found");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "located the actual file in å%sæ", a_dir);
   /*---(check file type)----------------*/
   --rce;  if (S_ISDIR (s.st_mode))  {
      yURG_err ('f', "file actually refers to a directory (security risk)");
      DEBUG_YEXEC  yLOG_note    ("can not use a directory");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_note    ("not a directory");
   --rce;  if (S_ISLNK (s.st_mode))  {
      yURG_err ('f', "file actually refers to a symbolic link (security risk)");
      DEBUG_YEXEC  yLOG_note    ("can not use a symlink");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_note    ("not a symlink");
   --rce;  if (!S_ISREG (s.st_mode))  {
      yURG_err ('f', "file is not a regular file (security risk)");
      DEBUG_YEXEC  yLOG_note    ("can only use regular files");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_note    ("confirmed as regular file");
   yURG_msg ('-', "file is confirmed as a regular file, not a symlink/directory");
   /*---(ownership)----------------------*/
   DEBUG_YEXEC  yLOG_complex ("owner"     , "user %4d, run %4d, own %4d", a_uid, a_muid, s.st_uid);
   --rce;  if (a_loc == YEXEC_LOCAL) {
      if (s.st_uid != a_uid) {
         yURG_err ('f', "file owner does not match home directory owner (security risk)");
         DEBUG_YEXEC  yLOG_note    ("file owner does not match home directory owner (security risk)");
         DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      if (a_muid == 0) {
         yURG_msg ('-', "running as root, can verify/install any user file");
      } else if (a_muid == s.st_uid) {
         yURG_msg ('-', "run-time user matches file ownership");
      } else {
         yURG_err ('f', "run-time user does not match file owner (security risk)");
         DEBUG_YEXEC  yLOG_note    ("file owner does not match home directory owner (security risk)");
         DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   --rce;  if (a_loc == YEXEC_CENTRAL) {
      if (a_muid == 0) {
         yURG_msg ('-', "running as root, can audit any files");
      } else if (strcmp (a_muser, a_user) == 0) {
         yURG_msg ('-', "prefix matches user å%sæ, can audit this file", a_muser);
      } else {
         yURG_err ('f', "user prefix in file name does not match run-time user (security risk)");
         DEBUG_YEXEC  yLOG_note    ("file owner is not runtime user");
         DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      if (s.st_uid != 0 || s.st_gid != 0) {
         yURG_err ('f', "central file owner/group is not root (security risk)");
         DEBUG_YEXEC  yLOG_note    ("central contab file owner/group is not root (security risk)");
         DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      yURG_msg ('-', "central owner and group is root");
   }
   /*---(permissions)--------------------*/
   DEBUG_ENVI   yLOG_value   ("perms"     , s.st_mode & 00777);
   if  ((s.st_mode & 00777) != 00600)  {
      yURG_err ('f', "file permissions are not owner-only read/write 0600 (security risk)");
      DEBUG_ENVI   yLOG_note    ("permissions not 0600 (private to user)");
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_note    ("permissions are 0600 (private)");
   yURG_msg ('-', "file permissions confirmed as owner-only write/read (0600)");
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yEXEC_acceptable_full    (cchar a_runas, cchar *a_home, cchar *a_root, cchar *a_name, cchar *a_muser, int a_muid, /*->>-*/ char *a_fuser, int *a_fuid, char *a_fdesc, char *a_dir)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_cwd       [LEN_PATH]  = "";
   char        x_fuser     [LEN_LABEL] = "";
   int         x_fuid      =   -1;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   yURG_msg ('>', "local file verification...");
   /*---(defaults)-----------------------*/
   if (a_fuser != NULL)  strcpy (a_fuser, "");
   if (a_fuid  != NULL)  *a_fuid = -1;
   if (a_fdesc != NULL)  strcpy (a_fdesc, "");
   if (a_dir   != NULL)  strcpy (a_dir  , "");
   /*---(naming)-------------------------*/
   rc = yexec__naming (a_name);
   DEBUG_INPT  yLOG_value   ("naming"    , rc);
   --rce;  if (rc < 0) {
      yURG_msg (' ', "");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(verify user)--------------------*/
   rc = yexec__location (a_runas, YEXEC_LOCAL, a_home, a_root, a_name, a_muser, a_muid, x_fuser, &x_fuid, x_cwd);
   DEBUG_INPT  yLOG_value   ("location"  , rc);
   --rce;  if (rc < 0) {
      yURG_msg (' ', "");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(stats)--------------------------*/
   rc = yexec__stats (YEXEC_LOCAL, x_cwd, a_name, a_muser, a_muid, x_fuser, x_fuid);
   DEBUG_INPT  yLOG_value   ("stats"     , rc);
   --rce;  if (rc < 0) {
      yURG_msg (' ', "");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update globals)-----------------*/
   if (a_fuser != NULL)  strlcpy (a_fuser, x_fuser, LEN_USER);
   if (a_fuid  != NULL)  *a_fuid = x_fuid;
   if (a_fdesc != NULL)  strlcpy (a_fdesc, a_name + 8, LEN_DESC);
   if (a_dir   != NULL)  strlcpy (a_dir  , x_cwd, LEN_PATH);
   /*---(finish)-------------------------*/
   yURG_msg ('-', "SUCCESS, job/khronos file acceptable");
   yURG_msg (' ', "");
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yEXEC_acceptable         (cchar a_runas, cchar *a_name, /*->>-*/ char *a_fuser, int *a_fuid, char *a_fdesc, char *a_dir)
{
   char        rc          =    0;
   char        x_user      [LEN_LABEL] = "";
   int         x_uid       =    0;
   if (rc < 0) rc = yEXEC_whoami (NULL, NULL, &x_uid, NULL, &x_user, 'n');
   if (rc < 0) rc = yEXEC_acceptable_full (a_runas, YEXEC_HOME, YEXEC_ROOT, a_name, x_user, x_uid, a_fuser, a_fuid, a_fdesc, a_dir);
   return rc;
}

char
yEXEC_central_full       (cchar a_runas, cchar *a_central, cchar *a_name, cchar *a_muser, int a_muid, /*->>-*/ char *a_fuser, int *a_fuid, char *a_fdesc)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_cwd       [LEN_PATH]  = "";
   char        x_fuser     [LEN_LABEL] = "";
   int         x_fuid      =   -1;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   yURG_msg ('>', "central file check...");
   /*---(defaults)-----------------------*/
   if (a_fuser != NULL)  strcpy (a_fuser, "");
   if (a_fuid  != NULL)  *a_fuid = -1;
   if (a_fdesc != NULL)  strcpy (a_fdesc, "");
   /*---(naming)-------------------------*/
   rc = yexec__naming (a_name);
   DEBUG_INPT  yLOG_value   ("naming"    , rc);
   --rce;  if (rc < 0) {
      yURG_msg (' ', "");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(verify user)--------------------*/
   rc = yexec__location (a_runas, YEXEC_CENTRAL, YEXEC_HOME, YEXEC_ROOT, a_name, a_muser, a_muid, x_fuser, &x_fuid, x_cwd);
   DEBUG_INPT  yLOG_value   ("location"  , rc);
   --rce;  if (rc < 0) {
      yURG_msg (' ', "");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(stats)--------------------------*/
   rc = yexec__stats (YEXEC_CENTRAL, a_central, a_name, a_muser, a_muid, x_fuser, x_fuid);
   DEBUG_INPT  yLOG_value   ("stats"     , rc);
   --rce;  if (rc < 0) {
      yURG_msg (' ', "");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update globals)-----------------*/
   if (a_fuser != NULL)  strlcpy (a_fuser, x_fuser, LEN_USER);
   if (a_fuid  != NULL)  *a_fuid = x_fuid;
   if (a_fdesc != NULL)  strlcpy (a_fdesc, a_name + strlen (x_fuser) + 1 , LEN_DESC);
   /*---(finish)-------------------------*/
   yURG_msg ('-', "SUCCESS, job/khronos file acceptable");
   yURG_msg (' ', "");
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yEXEC_central_dir       (cchar a_runas, cchar *a_name, char *a_dir, char *a_file)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(directories)--------------------*/
   --rce;  switch (a_runas) {
   case IAM_KHRONOS  :
      strcpy (a_dir, "/var/spool/khronos/");
      break;
   case IAM_UKHRONOS :
      strcpy (a_dir, "/tmp/khronos_test/khronos/");
      break;
   case IAM_EOS  : case IAM_ASTRAIOS  : case IAM_HYPNOS  :
      strcpy (a_dir, "/etc/");
      break;
   case IAM_UEOS : case IAM_UASTRAIOS : case IAM_UHYPNOS :
      strcpy (a_dir, "/tmp/eos_test/etc/");
      break;
   case IAM_HERACLES  :
      strcpy (a_dir, "/var/spool/heracles/");
      break;
   case IAM_UHERACLES :
      strcpy (a_dir, "/tmp/eos_test/heracles/");
      break;
   default  :
      yURG_err ('f', "a_runas (%c) is not recognized", a_runas);
      return rce;
      break;
   }
   /*---(check if file too)--------------*/
   if (a_file == NULL)  return 0;
   /*---(files)--------------------------*/
   switch (a_runas) {
   case IAM_KHRONOS   : case IAM_UKHRONOS  :
      strlcpy (a_file, a_name, LEN_PATH);
      break;
   case IAM_EOS       : case IAM_UEOS      : 
      strlcpy (a_file, "eos.conf"     , LEN_PATH);
      break;
   case IAM_ASTRAIOS  : case IAM_UASTRAIOS : 
      strlcpy (a_file, "asterios.conf", LEN_PATH);
      break;
   case IAM_HYPNOS    : case IAM_UHYPNOS   : 
      strlcpy (a_file, "hypnos.conf"  , LEN_PATH);
      break;
   case IAM_HERACLES  : case IAM_UHERACLES : 
      strlcpy (a_file, a_name, LEN_PATH);
      break;
   default  :
      yURG_err ('f', "a_runas (%c) is not recognized", a_runas);
      return rce;
      break;
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
yEXEC_central            (cchar a_runas, cchar *a_name, /*->>-*/ char *a_fuser, int *a_fuid, char *a_fdesc, char *a_dir)
{
   char        rce         =  -10;
   char        rc          =    0;
   char        x_user      [LEN_LABEL] = "";
   int         x_uid       =    0;
   char        x_center    [LEN_PATH]  = "";
   char        x_file      [LEN_PATH]  = "";
   if (a_dir   != NULL)  strcpy (a_dir  , "");
   rc = yEXEC_whoami (NULL, NULL, &x_uid, NULL, &x_user, 'n');
   --rce;  if (rc < 0) {
      yURG_err ('f', "could not identify current user (yEXEC_whoami)");
      return rce;
   }
   rc = yEXEC_central_dir (a_runas, a_name, x_center, x_file);
   --rce;  if (rc < 0) {
      yURG_err ('f', "could not identify central directory");
      return rce;
   }
   if (a_dir   != NULL)  strlcpy (a_dir, x_center, LEN_PATH);
   rc = yEXEC_central_full (a_runas, x_center, x_file, x_user, x_uid, a_fuser, a_fuid, a_fdesc);
   return rc;
}


/*====================------------------------------------====================*/
/*===----                       khronos/eos actions                    ----===*/
/*====================------------------------------------====================*/
static void      o___ACTIONS_________________o (void) {;};

static char (*s_assimilate) (cchar a_loc, cchar *a_name);

char
yEXEC_act_verify        (cchar a_runas, cchar a_act, cchar *a_oneline, cchar *a_name, void *a_assimilate)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   DEBUG_INPT  yLOG_point   ("a_runas"   , a_runas);
   /*---(defense)------------------------*/
   DEBUG_INPT  yLOG_point   ("a_oneline" , a_oneline);
   --rce;  if (a_oneline    == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT  yLOG_point   ("a_assim"   , a_assimilate);
   --rce;  if (a_assimilate == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   s_assimilate = a_assimilate;
   /*---(verify header)------------------*/
   DEBUG_INPT  yLOG_point   ("a_act"     , a_act);
   --rce;  IF_VERIFY {
      yURG_msg ('>', "%s", a_oneline);
      yURG_msg ('>', "  option --vverify, check details of local job/khronos file, but do not install");
      yURG_msg (' ', "");
   } else {
      DEBUG_INPT   yLOG_note    ("action requested is not a verify type, trouble");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(verify contents)--------------------*/
   rc = s_assimilate (YEXEC_LOCAL, a_name);
   DEBUG_INPT   yLOG_value   ("assimilate", rc);
   --rce;  if (rc < 0) {
      if (a_act == ACT_CVERIFY )   yURG_msg_live ();
      if (a_act == ACT_CVERIFY )   yURG_msg ('>', "FAILED, local job/khronos file not installable, run --vverify to identify reasons");
      if (a_act == ACT_VVERIFY )   yURG_msg ('>', "FAILED, local job/khronos file not installable, the reasons are shown above");
      if (a_act == ACT_CVERIFY )   yURG_msg_mute ();
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   if (a_act == ACT_CVERIFY )   yURG_msg_live ();
   yURG_msg ('>', "SUCCESS, local job/khronos file installable, but installation not requested");
   if (a_act == ACT_CVERIFY )   yURG_msg_mute ();
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;

}

char       /* PURPOSE : install a local crontab file -------------------------*/
yexec_act__intake       (cchar *a_name, cchar *a_new, cchar *a_central)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   char        x_new       [LEN_RECD]  = "";
   char        x_cmd       [LEN_RECD]  = "";
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(header)-------------------------*/
   yURG_msg ('>', "install file in central directory...");
   /*---(defense)------------------------*/
   DEBUG_INPT  yLOG_point   ("a_new"     , a_new);
   --rce;  if (a_new        == NULL) {
      yURG_err ('f', "new job/khronos file name was null/empty");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT  yLOG_point   ("a_central" , a_central);
   --rce;  if (a_central    == NULL) {
      yURG_err ('f', "central directory name was null/empty");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(make full new file name)------------*/
   snprintf (x_new, LEN_RECD, "%s%s", a_central, a_new);
   yURG_msg ('-', "central file name å%sæ", x_new);
   /*---(copy file)--------------------------*/
   snprintf (x_cmd, LEN_RECD, "cp -f %s %s > /dev/null  2>&1", a_name, x_new);
   DEBUG_INPT   yLOG_info    ("x_cmd"     , x_cmd);
   rc = system   (x_cmd);
   DEBUG_INPT   yLOG_value   ("system"    , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "could not copy to å%sæ", a_central);
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_value   ("wifexited" , WIFEXITED(rc));
   if (WIFEXITED(rc) <  0) {
      yURG_err ('f', "could not copy to å%sæ", a_central);
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "copied file to å%sæ", a_central);
   /*---(change ownership)-------------------*/
   snprintf (x_cmd, LEN_RECD, "chown root:root %s > /dev/null  2>&1", x_new);
   DEBUG_INPT   yLOG_info    ("x_cmd"     , x_cmd);
   rc = system   (x_cmd);
   DEBUG_INPT   yLOG_value   ("system"    , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "could not change ownership to root");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_value   ("wifexited" , WIFEXITED(rc));
   if (WIFEXITED(rc) <  0) {
      yURG_err ('f', "could not change ownership to root");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "changed owner and group to root");
   /*---(change permissions)-----------------*/
   snprintf (x_cmd, LEN_RECD, "chmod 0600 %s > /dev/null  2>&1", x_new);
   DEBUG_INPT   yLOG_info    ("x_cmd"     , x_cmd);
   rc = system   (x_cmd);
   DEBUG_INPT   yLOG_value   ("system"    , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "could not change permissions to root-only write/read (0600)");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_value   ("wifexited" , WIFEXITED(rc));
   if (WIFEXITED(rc) <  0) {
      yURG_err ('f', "could not change permissions to root-only write/read (0600)");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "changed permissions to root-only write/read (0600)");
   yURG_msg ('-', "SUCCESS, job/khronos file moved to central");
   yURG_msg (' ', "");
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yEXEC_act_install       (cchar a_runas, cchar a_act, cchar *a_oneline, cchar *a_name, void *a_assimilate, cchar *a_new)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   char        x_dir       [LEN_PATH]  = "";
   char        x_file      [LEN_PATH]  = "";
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   DEBUG_INPT  yLOG_point   ("a_runas"   , a_runas);
   /*---(defense)------------------------*/
   DEBUG_INPT  yLOG_point   ("a_oneline" , a_oneline);
   --rce;  if (a_oneline    == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT  yLOG_point   ("a_assim"   , a_assimilate);
   --rce;  if (a_assimilate == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   s_assimilate = a_assimilate;
   /*---(install header)-----------------*/
   DEBUG_INPT  yLOG_point   ("a_act"     , a_act);
   --rce;  IF_INSTALL {
      yURG_msg ('>', "%s", a_oneline);
      yURG_msg ('>', "  option --vinstall, check details of local job/khronos file and install if clean");
      yURG_msg (' ', "");
   } else {
      DEBUG_INPT   yLOG_note    ("action requested is not an install type, trouble");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(verify contents)--------------------*/
   rc = s_assimilate (YEXEC_LOCAL, a_name);
   DEBUG_INPT   yLOG_value   ("assimilate", rc);
   --rce;  if (rc < 0) {
      if (a_act == ACT_CINSTALL)   yURG_msg_live ();
      if (a_act == ACT_CINSTALL)   yURG_msg ('>', "FAILED, local job/khronos file not installable, run --vinstall to identify reasons");
      if (a_act == ACT_VINSTALL)   yURG_msg ('>', "FAILED, local job/khronos file not installable, the reasons are shown above");
      if (a_act == ACT_CINSTALL)   yURG_msg_mute ();
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(install file)-----------------------*/
   yEXEC_central_dir (a_runas, a_new, x_dir, x_file);
   rc = yexec_act__intake (a_name, x_file, x_dir);
   DEBUG_INPT   yLOG_value   ("intake"    , rc);
   --rce;  if (rc < 0) {
      if (a_act == ACT_CINSTALL)   yURG_msg_live ();
      if (a_act == ACT_CINSTALL)   yURG_msg ('>', "FAILED, local job/khronos file installable, but could not be, run --vinstall to identify reasons");
      if (a_act == ACT_VINSTALL)   yURG_msg ('>', "FAILED, local job/khronos file installable, but could not be, the reasons are shown above");
      if (a_act == ACT_CINSTALL)   yURG_msg_mute ();
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   if (a_act == ACT_CINSTALL)   yURG_msg_live ();
   yURG_msg ('>', "SUCCESS, local job/khronos file installed.  ready to use, restart, or --reload");
   if (a_act == ACT_CINSTALL)   yURG_msg_mute ();
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yEXEC_act_check         (cchar a_runas, cchar a_act, cchar *a_oneline, cchar *a_name, void *a_assimilate)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   DEBUG_INPT  yLOG_point   ("a_runas"   , a_runas);
   /*---(defense)------------------------*/
   DEBUG_INPT  yLOG_point   ("a_oneline" , a_oneline);
   --rce;  if (a_oneline    == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT  yLOG_point   ("a_assim"   , a_assimilate);
   --rce;  if (a_assimilate == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   s_assimilate = a_assimilate;
   /*---(verify)-------------------------*/
   DEBUG_INPT  yLOG_point   ("a_act"     , a_act);
   --rce;  IF_CHECK {
      yURG_msg ('>', "%s", a_oneline);
      yURG_msg ('>', "  option --vcheck, check details of installed job/khronos file");
   } else IF_AUDIT   {
      yURG_msg ('>', "CHECKING %s ===================================", a_name);
   } else IF_DAEMON  {
      yURG_msg ('>', "CHECKING %s ===================================", a_name);
   } else IF_PRICKLY {
      yURG_msg ('>', "CHECKING %s ===================================", a_name);
   } else IF_NORMAL  {
      yURG_msg ('>', "CHECKING %s ===================================", a_name);
   } else {
      DEBUG_INPT   yLOG_note    ("action requested is not an check, audit, or daemon type, trouble");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg (' ', "");
   /*---(verify contents)--------------------*/
   rc = s_assimilate (YEXEC_CENTRAL, a_name);
   DEBUG_INPT   yLOG_value   ("assimilate", rc);
   --rce;  if (rc < 0) {
      IF_VCENTRAL  yURG_msg ('>', "FAILED, installed job/khronos file not runable, the reasons are shown above");
      IF_CCENTRAL  yURG_msg_live ();
      IF_CCHECK    yURG_msg ('>', "FAILED, installed job/khronos file not runable, run --vcheck to identify reasons");
      /*> IF_CREVIEW   yURG_msg ('>', "%-30.30s  FAILED, job/khronos file not runable, run --vcheck to identify reasons", a_name);   <*/
      IF_CCENTRAL  yURG_msg_mute ();
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   IF_VCENTRAL  yURG_msg ('>', "SUCCESS, installed job/khronos file runable, all lines checked");
   IF_CCENTRAL  yURG_msg_live ();
   IF_CCHECK    yURG_msg ('>', "SUCCESS, installed job/khronos file runable, all lines checked");
   /*> IF_CREVIEW   yURG_msg ('>', "%-30.30s  SUCCESS, installed job/khronos file runable, all lines checked", a_name);   <*/
   IF_CCENTRAL  yURG_msg_mute ();
   /*---(footer)-------------------------*/
   if (a_act == ACT_VAUDIT || a_act == ACT_VDAEMON) {
      yURG_msg (' ', "");
      yURG_msg ('>', "COMPLETE %s ===================================", a_name);
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
yEXEC_act_remove        (cchar a_runas, cchar a_act, cchar *a_oneline, cchar *a_name)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   char        x_dir       [LEN_PATH]  = "";
   char        x_file      [LEN_PATH]  = "";
   char        x_old       [LEN_RECD]  = "";
   char        x_cmd       [LEN_RECD]  = "";
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   DEBUG_INPT  yLOG_point   ("a_runas"   , a_runas);
   /*---(defense)------------------------*/
   DEBUG_INPT  yLOG_point   ("a_oneline" , a_oneline);
   --rce;  if (a_oneline    == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(remove header)------------------*/
   DEBUG_INPT  yLOG_point   ("a_act"     , a_act);
   --rce;  IF_REMOVE {
      yURG_msg ('>', "%s", a_oneline);
      yURG_msg ('>', "  option --vremove, remove installed job/khronos file if found");
      yURG_msg (' ', "");
   } else {
      DEBUG_INPT   yLOG_note    ("action requested is not a remove type, trouble");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(verify contents)--------------------*/
   rc = yEXEC_central (a_runas, a_name, NULL, NULL, NULL, NULL);
   DEBUG_INPT   yLOG_value   ("central"   , rc);
   --rce;  if (rc < 0) {
      if (a_act == ACT_CREMOVE )   yURG_msg_live ();
      if (a_act == ACT_CREMOVE )   yURG_msg ('>', "FAILED, installed job/khronos file not found/proper, run --vremove to identify reasons");
      if (a_act == ACT_VREMOVE )   yURG_msg ('>', "FAILED, installed job/khronos file not found/proper, the reasons are shown above");
      if (a_act == ACT_CREMOVE )   yURG_msg_mute ();
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(remove physical)--------------------*/
   yEXEC_central_dir (a_runas, a_name, x_dir, x_file);
   snprintf (x_old, LEN_RECD, "%s%s", x_dir, x_file);
   snprintf (x_cmd, LEN_RECD, "rm -f %s 2> /dev/null", x_old);
   DEBUG_INPT   yLOG_info    ("x_cmd"     , x_cmd);
   rc = system   (x_cmd);
   DEBUG_INPT   yLOG_value   ("system"    , rc);
   DEBUG_INPT   yLOG_value   ("wifexited" , WIFEXITED(rc));
   --rce;  if (rc < 0 || WIFEXITED (rc) < 0) {
      if (a_act == ACT_CREMOVE )   yURG_msg_live ();
      if (a_act == ACT_CREMOVE )   yURG_msg ('>', "FAILED, installed job/khronos file not deleted, run --vremove for reasons");
      if (a_act == ACT_VREMOVE )   yURG_msg ('>', "FAILED, installed job/khronos file not deleted, the reasons are shown above");
      if (a_act == ACT_CREMOVE )   yURG_msg_mute ();
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   if (a_act == ACT_CREMOVE )   yURG_msg_live ();
   yURG_msg ('>', "SUCCESS, installed job/khronos file uninstalled.  restart or --reload to retire");
   if (a_act == ACT_CREMOVE )   yURG_msg_mute ();
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       security audit                         ----===*/
/*====================------------------------------------====================*/
static void      o___SECURITY________________o (void) {;}

char
yexec_act__checkdir     (cchar *a_dir, int a_perms)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tSTAT       s;
   int         x_perms     =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   DEBUG_INPT   yLOG_info    ("a_dir"     , a_dir);
   /*---(check existance)----------------*/
   rc = lstat (a_dir, &s);
   DEBUG_INPT   yLOG_value   ("stat"      , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "å%sæ directory does not exist (bad configuration)", a_dir);
      DEBUG_INPT   yLOG_note    ("can not open /var");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (S_ISLNK (s.st_mode))  {
      yURG_err ('f', "å%sæ actually refers to a symbolic link (security risk)", a_dir);
      DEBUG_YEXEC  yLOG_note    ("can not use a symlink");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (!S_ISDIR (s.st_mode))  {
      yURG_err ('f', "å%sæ is not a directory/symlink (bad configuration)", a_dir);
      DEBUG_INPT   yLOG_note    ("can not use a directory");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "å%sæ directory exists and is not a symlink", a_dir);
   /*---(ownership)----------------------*/
   --rce;  if (s.st_uid != 0) {
      yURG_err ('f', "å%sæ is not owned by root (security risk)", a_dir);
      DEBUG_YEXEC  yLOG_note    ("/var/spool/khronos not owned by root (security risk)");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_note    ("ownership is root (private)");
   yURG_msg ('-', "å%sæ directory ownership is root", a_dir);
   --rce;  if (s.st_gid != 0) {
      yURG_err ('f', "å%sæ is not root group (security risk)", a_dir);
      DEBUG_YEXEC  yLOG_note    ("/var/spool/khronos not group of root (security risk)");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "å%sæ directory group ownership is root", a_dir);
   DEBUG_YEXEC  yLOG_note    ("group ownership is root (private)");
   /*---(permissions)--------------------*/
   x_perms = s.st_mode & 0777;
   DEBUG_ENVI   yLOG_complex ("x_perms"   , "%04o", x_perms);
   DEBUG_ENVI   yLOG_complex ("a_perms"   , "%04o", a_perms);
   if  (x_perms != a_perms)  {
      yURG_err ('f', "å%sæ perms are %04o, not the requested %04o (security risk)", a_dir, s.st_mode & 0777, a_perms);
      DEBUG_ENVI   yLOG_note    ("permissions not set correctly (private to user)");
      DEBUG_ENVI   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "å%sæ directory permissions confirmed as %04o", a_dir, a_perms);
   DEBUG_YEXEC  yLOG_note    ("permissions are propiate (private)");
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yEXEC_act_security      (cchar a_runas, cchar a_act, cchar *a_oneline)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_orig      [LEN_PATH]  = "";
   char        x_path      [LEN_PATH]  = "";
   char       *p           = NULL;
   char       *r           = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(defense)-------------------------------*/
   DEBUG_INPT  yLOG_point   ("a_oneline" , a_oneline);
   --rce;  if (a_oneline    == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(verify)-------------------------*/
   yURG_msg ('>', "%s", a_oneline);
   if (a_act == ACT_VAUDIT )  yURG_msg ('>', "  option --vaudit, check current central setup and security");
   if (a_act == ACT_VDAEMON)  yURG_msg ('>', "  option --vdaemon, verbosely launch in daemon mode");
   if (a_act == ACT_VPRICKLY) yURG_msg ('>', "  option --vprickly, verbosely launch in prickly daemon mode");
   if (a_act == ACT_VNORMAL)  yURG_msg ('>', "  option --vnormal, verbosely launch in normal mode");
   yURG_msg (' ', "");
   yURG_msg ('>', "central directory setup/security...");
   /*---(defense)-------------------------------*/
   rc = yEXEC_central_dir  (a_runas, NULL, x_orig, NULL);
   --rce;  if (rc <  0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("x_orig"    , x_orig);
   yURG_msg ('-', "for (%c), directory å%sæ selected", a_runas, x_orig);
   /*---(get first level)-----------------------*/
   p = strtok_r (x_orig, "/", &r);
   DEBUG_INPT   yLOG_point   ("p"         , p);
   --rce;  if (p == NULL) {
      yURG_err ('f', "can not parse directory name");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check each level)----------------------*/
   --rce;  while (p != NULL) {
      sprintf (x_path, "%s/%s", x_path, p);
      if      (strcmp (p, "khronos")  == 0)   rc = yexec_act__checkdir (x_path, 0700);
      else if (strcmp (p, "heracles") == 0)   rc = yexec_act__checkdir (x_path, 0700);
      else if (strcmp (p, "tmp"     ) == 0)   rc = yexec_act__checkdir (x_path, 0777);
      else                                    rc = yexec_act__checkdir (x_path, 0755);
      if (rc < 0) {
         DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      p = strtok_r (NULL, "/", &r);
   }
   /*---(complete)------------------------------*/
   yURG_msg ('-', "SUCCESS, central directory basic security measures confirmed");
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     review a directory                       ----===*/
/*====================------------------------------------====================*/
static void      o___REVIEW__________________o (void) {;}

char
yexec_act__filter       (cchar *a_name, cchar *a_prefix, int a_muid)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         l           =    0;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name == NULL || strlen (a_name) <= 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_name"    , a_name);
   /*---(just up/cur dirs)---------------*/
   if      (strcmp (a_name, "." ) == 0 || strcmp (a_name, "..") == 0) {
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(name quality)-------------------*/
   l = strlen (a_name);
   DEBUG_INPT   yLOG_value   ("l"         , l);
   --rce;  for (i = 0; i < l; ++i) {
      if (strchr (LTRS_FILES, a_name [i]) == NULL) {
         DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(hidden file)--------------------*/
   DEBUG_INPT   yLOG_char    ("first char", a_name [0]);
   --rce;  if (a_name [0] == '.') {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(user prefix)--------------------*/
   if (a_muid != 0) {
      if (strncmp (a_name, a_prefix, strlen (a_prefix)) != 0) {
         DEBUG_INPT  yLOG_exit    (__FUNCTION__);
         return 0;
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 1;
}

char
yexec_act__prepare      (cchar a_runas, cchar a_act, cchar *a_oneline, cchar *a_muser, cchar *a_regex, char *a_prefix, char *a_dir, void *a_assimilate)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   char        x_dir       [LEN_PATH]  = "";
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   DEBUG_INPT   yLOG_char    ("a_act"     , a_act);
   /*---(defaults)-----------------------*/
   if (a_prefix != NULL)  strcpy (a_prefix, "");
   if (a_dir    != NULL)  strcpy (a_dir   , "");
   /*---(defense)------------------------*/
   DEBUG_INPT  yLOG_point   ("a_oneline" , a_oneline);
   --rce;  if (a_oneline    == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_point   ("a_regex"   , a_regex);
   --rce;  if (a_regex == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_regex"   , a_regex);
   DEBUG_INPT   yLOG_point   ("a_muser"   , a_muser);
   --rce;  if (a_muser == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_point   ("a_dir"     , a_dir);
   --rce;  if (a_dir == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_point   ("a_prefix"  , a_prefix);
   --rce;  if (a_prefix == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT  yLOG_point   ("a_assim"   , a_assimilate);
   --rce;  if (a_assimilate == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   s_assimilate = a_assimilate;
   /*---(regex)--------------------------*/
   rc = yREGEX_comp (a_regex);
   DEBUG_INPT   yLOG_value   ("comp"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_note    ("could not compile search");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get directory)------------------*/
   rc = yEXEC_central_dir  (a_runas, NULL, a_dir, NULL);
   DEBUG_INPT   yLOG_info    ("a_dir"     , a_dir);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(user prefix)--------------------*/
   sprintf (a_prefix, "%s.", a_muser);
   /*---(check security)-----------------*/
   --rce;  switch (a_act) {
   case ACT_AUDIT      : case ACT_CAUDIT     : case ACT_VAUDIT     :
   case ACT_DAEMON     : case ACT_CDAEMON    : case ACT_VDAEMON    :
   case ACT_PRICKLY    : case ACT_CPRICKLY   : case ACT_VPRICKLY   :
   case ACT_NORMAL     : case ACT_CNORMAL    : case ACT_VNORMAL    :
      rc = yEXEC_act_security (a_runas, a_act, a_oneline);
      if (rc < 0) {
         IF_CREVIEW   yURG_msg_live ();
         IF_CREVIEW   yURG_msg ('>', "FAILED, central directory insecure, run --vaudit to identify reasons");
         IF_VREVIEW   yURG_msg ('>', "FAILED, central directory insecure, the reasons are shown above");
         IF_CREVIEW   yURG_msg_mute ();
         DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      } else {
         yURG_msg ('>', "SUCCESS, central directory basic security measures confirmed");
      }
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> review and act on global crontabs -------------------------*/
yEXEC_act_review        (cchar a_runas, cchar a_act, cchar *a_oneline, cchar *a_muser, int a_muid, cchar *a_regex, void *a_assimilate)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   DIR        *x_dir       = NULL;
   tDIRENT    *x_file      = NULL;
   int         x_total     =    0;
   int         x_count     =    0;
   int         x_pass      =    0;
   char        x_prefix    [LEN_USER]  = "";
   char        x_central   [LEN_PATH]  = "";
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   rc = yexec_act__prepare (a_runas, a_act, a_oneline, a_muser, a_regex, x_prefix, x_central, a_assimilate);
   DEBUG_INPT   yLOG_value   ("prepare"   , rc);
   --rce;  if (rc < 0)  {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("x_central" , x_central);
   x_dir = opendir (x_central);
   DEBUG_INPT   yLOG_point   ("x_dir"     , x_dir);
   --rce;  if (x_dir == NULL) {
      /*> RUN_USER     printf ("fatal, can not open khronos central directory\n");    <*/
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(walk files)---------------------*/
   DEBUG_INPT   yLOG_note    ("walk through directory files");
   while ((x_file = readdir (x_dir)) != NULL) {
      /*---(simple filtering)-------------------*/
      rc = yexec_act__filter (x_file->d_name, x_prefix, a_muid);
      if (rc != 0)  ++x_total;
      if (rc <= 0) {
         DEBUG_INPT   yLOG_note    ("not a valid job/khronos file");
         continue;
      }
      /*---(filter using regex)-----------------*/
      rc = yREGEX_exec (x_file->d_name);
      DEBUG_INPT   yLOG_value   ("exec"      , rc);
      if (rc <= 0) {
         DEBUG_INPT   yLOG_note    ("does not match regex, skipping");
         continue;
      }
      /*---(actions)----------------------------*/
      switch (a_act) {
      case ACT_LIST      :
         yURG_msg_live ();
         yURG_msg ('>', "%s", x_file->d_name);
         yURG_msg_mute ();
         rc = 0;
         break;
      case ACT_AUDIT     : case ACT_CAUDIT    : case ACT_VAUDIT    :
      case ACT_NORMAL    : case ACT_CNORMAL   : case ACT_VNORMAL   :
      case ACT_DAEMON    : case ACT_CDAEMON   : case ACT_VDAEMON   :
      case ACT_PRICKLY   : case ACT_CPRICKLY  : case ACT_VPRICKLY  :
         yURG_msg (' ', "");
         rc = yEXEC_act_check   (a_runas, a_act, a_oneline, x_file->d_name, s_assimilate);
         break;
      }
      ++x_count;
      if (rc >= 0)  ++x_pass;
      DEBUG_INPT   yLOG_complex ("counts"    , "%d total, %d count, %d pass", x_total, x_count, x_pass);
      /*---(done)------------------------*/
   }
   /*---(summary)------------------------*/
   DEBUG_INPT   yLOG_value   ("found"     , x_total);
   DEBUG_INPT   yLOG_value   ("processed" , x_count);
   DEBUG_INPT   yLOG_value   ("passed"    , x_pass);
   /*---(close)--------------------------*/
   rc = closedir (x_dir);
   DEBUG_INPT   yLOG_point   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(summary)------------------------*/
   rc = 1;
   --rce;  if (x_count <= 0) {
      DEBUG_INPT   yLOG_note    ("job/khronos count is zero");
      yURG_msg (' ', "");
      IF_VREVIEW  yURG_msg ('>', "WARNING, secure, but no job/khronos files found installed in central directory");
      IF_CREVIEW  yURG_msg_live ();
      IF_CREVIEW  yURG_msg ('>', "WARNING, secure, but no job/khronos files found installed in central directory");
      IF_CREVIEW  yURG_msg_mute ();
      rc = 0;
      IF_AUDIT    rc = rce;
      IF_PRICKLY  rc = rce;
   }
   --rce;  if (x_count != x_pass) {
      DEBUG_INPT   yLOG_note    ("job/khronos count not equal passed");
      yURG_msg (' ', "");
      IF_VREVIEW  yURG_msg ('>', "WARNING, secure, but not all job/khronos files passed, only %d of %d passed", x_pass, x_count);
      if (rc == 1) {
         IF_CREVIEW  yURG_msg_live ();
         IF_CREVIEW  yURG_msg ('>', "WARNING, secure, but not all job/khronos files passed, only %d of %d passed", x_pass, x_count);
         IF_CREVIEW  yURG_msg_mute ();
      }
      rc = 0;
      IF_AUDIT    rc = rce;
      IF_PRICKLY  rc = rce;
   }
   --rce;  if (x_total != x_count) {
      DEBUG_INPT   yLOG_note    ("job/khronos count not equal to total files");
      yURG_msg (' ', "");
      IF_VREVIEW  yURG_msg ('>', "WARNING, secure, but garbage non-job/khronos file(s) found, %d unknown of %d", x_total - x_count, x_total);
      if (rc == 1) {
         IF_CREVIEW  yURG_msg_live ();
         IF_CREVIEW  yURG_msg ('>', "WARNING, secure, but garbage non-job/khronos file(s) found, %d unknown of %d", x_total - x_count, x_total);
         IF_CREVIEW  yURG_msg_mute ();
      }
      rc = 0;
      IF_AUDIT    rc = rce;
      IF_PRICKLY  rc = rce;
   }
   IF_LIST   rc = 1;
   if (rc == 1) {
      DEBUG_INPT   yLOG_note    ("all results golden");
      yURG_msg (' ', "");
      IF_VREVIEW  yURG_msg ('>', "SUCCESS, environment and %d job/khronos file(s) passed all checks", x_pass);
      IF_CREVIEW  yURG_msg_live ();
      IF_CREVIEW  yURG_msg ('>', "SUCCESS, environment and %d job/khronos file(s) passed all checks", x_pass);
      IF_CREVIEW  yURG_msg_mute ();
   }
   if (rc >= 0) {
      if (x_count > 100) x_count = 100;
      rc = x_count;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
yexec_act__assim        (cchar a_loc, cchar *a_name)
{
   if (strstr (a_name, "bad") != NULL)  return -10;
   return 0;
}



