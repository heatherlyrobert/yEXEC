/*============================[[    beg-code    ]]============================*/
#include   "yEXEC.h"
#include   "yEXEC_priv.h"



/*====================------------------------------------====================*/
/*===----                      consoles and ttys                       ----===*/
/*====================------------------------------------====================*/
static void      o___TTYS____________________o (void) {;}

char         /*--> validate a tty device file -------[ ------ [abc.de.fghijk]-*/
yEXEC_tty_valid         (char *a_dev)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_good      =  '-';
   tSTAT       s;
   /*---(header)-------------------------*/
   DEBUG_YEXEC   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YEXEC   yLOG_point   ("a_dev"     , a_dev);
   --rce;  if (a_dev == NULL) {
      DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC   yLOG_info    ("a_dev"     , a_dev);
   /*---(check path)---------------------*/
   if (strncmp (a_dev, "/dev/tty", 8) == 0)   x_good = 'y';
   if (strncmp (a_dev, "/dev/pts", 8) == 0)   x_good = 'y';
   DEBUG_YEXEC   yLOG_char    ("x_good"    , x_good);
   --rce;  if (x_good != 'y') {
      DEBUG_YEXEC   yLOG_note    ("path must be either /dev/tty## or /dev/pts/##");
      DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check device file)--------------*/
   rc = lstat (a_dev, &s);
   DEBUG_YEXEC   yLOG_value   ("stat"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_YEXEC   yLOG_note    ("can not find device file");
      DEBUG_YEXEC   yLOG_exitr  (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (S_ISDIR (s.st_mode))  {
      DEBUG_YEXEC   yLOG_note    ("can not use a directory");
      DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (S_ISLNK (s.st_mode))  {
      DEBUG_YEXEC   yLOG_note    ("can not use a symlink");
      DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (S_ISREG (s.st_mode))  {
      DEBUG_YEXEC   yLOG_note    ("can not use regular files");
      DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (S_ISBLK (s.st_mode))  {
      DEBUG_YEXEC   yLOG_note    ("can not use block files");
      DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (!S_ISCHR (s.st_mode))  {
      DEBUG_YEXEC   yLOG_note    ("can only use character devices");
      DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(confirmation)-------------------*/
   DEBUG_YEXEC   yLOG_note   ("successfully found character device file");
   /*---(complete)-----------------------*/
   DEBUG_YEXEC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yexec_tty__standard     (int a_fd)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tTERMIOS    x_tty;
   /*---(get attributes)-----------------*/
   rc = tcgetattr (a_fd, &x_tty);
   DEBUG_YEXEC  yLOG_value   ("getattr"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(change attributes)--------------*/
   x_tty.c_cflag        &= CBAUD|CBAUDEX|CSIZE|CSTOPB|PARENB|PARODD;
   x_tty.c_cflag        |= HUPCL|CLOCAL|CREAD;
   x_tty.c_cc[VINTR]	    = CINTR;
   x_tty.c_cc[VQUIT]	    = CQUIT;
   x_tty.c_cc[VERASE]    = CERASE; /* ASCII DEL (0177) */
   x_tty.c_cc[VKILL]	    = CKILL;
   x_tty.c_cc[VEOF]	    = CEOF;
   x_tty.c_cc[VTIME]	    = 0;
   x_tty.c_cc[VMIN]	    = 1;
   x_tty.c_cc[VSWTC]	    = _POSIX_VDISABLE;
   x_tty.c_cc[VSTART]    = CSTART;
   x_tty.c_cc[VSTOP]	    = CSTOP;
   x_tty.c_cc[VSUSP]	    = CSUSP;
   x_tty.c_cc[VEOL]	    = _POSIX_VDISABLE;
   x_tty.c_cc[VREPRINT]  = CREPRINT;
   x_tty.c_cc[VDISCARD]  = CDISCARD;
   x_tty.c_cc[VWERASE]   = CWERASE;
   x_tty.c_cc[VLNEXT]    = CLNEXT;
   x_tty.c_cc[VEOL2]	    = _POSIX_VDISABLE;
   x_tty.c_iflag         = IGNPAR|ICRNL|IXON|IXANY;
   x_tty.c_oflag         = OPOST|ONLCR;
   x_tty.c_lflag         = ISIG|ICANON|ECHO|ECHOCTL|ECHOPRT|ECHOKE;
   x_tty.c_iflag        |=  IGNBRK;
   x_tty.c_iflag        &= ~(BRKINT|INLCR|IGNCR|IXON);
   x_tty.c_oflag        &= ~(OCRNL|ONLRET);
   /*---(set attributes)-----------------*/
   rc = tcsetattr (a_fd, TCSANOW, &x_tty);
   DEBUG_YEXEC  yLOG_value   ("setattr"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(flush data)---------------------*/
   rc = tcflush (a_fd, TCIOFLUSH);
   DEBUG_YEXEC  yLOG_value   ("flush"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YEXEC   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yEXEC_tty_noecho        (int a_fd)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         c           =    0;
   tTERMIOS    x_termios;
   /*---(header)-------------------------*/
   DEBUG_LOOP   yLOG_enter   (__FUNCTION__);
   /*---(get current term settings)------*/
   fcntl (a_fd, F_SETFL, 0);
   /*---(get current term settings)------*/
   tcgetattr(a_fd, &x_termios);
   /*---(prepare new one)----------------*/
   x_termios.c_lflag  &= ~ECHO;
   tcsetattr (a_fd, TCSANOW, &x_termios);
   /*---(clear existing in/out)----------*/
   rc = tcflush (a_fd, TCIOFLUSH);  /* flush both input/output */
   DEBUG_LOOP   yLOG_value   ("flush"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_LOOP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_LOOP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yEXEC_tty_open          (char *a_dev, int *a_fd, char a_std, char a_keep)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tTERMIOS    x_tty;
   int         x_fd        =   -1;
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YEXEC  yLOG_point   ("a_dev"     , a_dev);
   --rce;  if (a_dev == NULL) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_info    ("a_dev"     , a_dev);
   /*---(change ownership)---------------*/
   rc = chown (a_dev, 0, 0);
   DEBUG_YEXEC   yLOG_value   ("chown"     , rc);
   --rce;  if (rc != 0) {
      DEBUG_YEXEC   yLOG_note    ("can not change ownership to root only");
      DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(change permissions)-------------*/
   rc = chmod (a_dev, 0600);
   DEBUG_YEXEC   yLOG_value   ("chmod"     , rc);
   --rce;  if (rc != 0) {
      DEBUG_YEXEC   yLOG_note    ("can not change permissions to 0600");
      DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(environment)--------------------*/
   rc = putenv ("TERM=Eterm");
   DEBUG_YEXEC  yLOG_value   ("putenv"    , rc);
   --rce;  if (rc != 0) {
      DEBUG_YEXEC  yLOG_note    ("can not set TERM environment variable");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(open term)----------------------*/
   x_fd = open (a_dev, O_RDWR | O_NOCTTY | O_NONBLOCK);
   DEBUG_YEXEC  yLOG_value   ("open"      , x_fd);
   --rce;  if (x_fd < 0) {
      DEBUG_YEXEC  yLOG_note    ("can not get control/open device");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(test terminal)------------------*/
   DEBUG_YEXEC   yLOG_value   ("isatty"    , isatty (x_fd));
   --rce;  if (!isatty (x_fd)) {
      DEBUG_YEXEC   yLOG_note    ("not a terminal device");
      close (x_fd);
      DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(set standard attributes)--------*/
   rc = yexec_tty__standard (x_fd);
   DEBUG_YEXEC  yLOG_value   ("standard"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(duplicate)----------------------*/
   DEBUG_YEXEC  yLOG_char    ("a_std"     , a_std);
   switch (a_std) {
   case YEXEC_STDALL :
      DEBUG_YEXEC  yLOG_note    ("connecting stdin, stdout, and stderr");
      dup2 (x_fd, 0);
      dup2 (x_fd, 1);
      dup2 (x_fd, 2);
      break;
   case YEXEC_STDOUT :
      DEBUG_YEXEC  yLOG_note    ("connecting stdout only");
      dup2 (x_fd, 1);
      break;
   }
   /*---(close term)---------------------*/
   if (a_keep != YEXEC_YES) {
      rc = yEXEC_tty_close (&x_fd);
      DEBUG_YEXEC  yLOG_value   ("close"     , rc);
      --rce;  if (rc < 0) {
         DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(save)---------------------------*/
   if (a_fd != NULL)  *a_fd = x_fd;
   /*---(complete)-----------------------*/
   DEBUG_YEXEC  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yEXEC_tty_close         (int *a_fd)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YEXEC  yLOG_point   ("a_fd"      , a_fd);
   --rce;  if (a_fd == NULL) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_value   ("*a_fd"     , *a_fd);
   --rce;  if (*a_fd < 0) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(set standard attributes)--------*/
   rc = yexec_tty__standard (*a_fd);
   DEBUG_YEXEC  yLOG_value   ("standard"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(close term)---------------------*/
   rc = close (*a_fd);
   DEBUG_YEXEC  yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(ground pointer)-----------------*/
   *a_fd = -1;
   /*---(complete)-----------------------*/
   DEBUG_YEXEC  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yEXEC_tty_clear          (int a_fd)
{
   tcflush (a_fd, TCIFLUSH);
   printf  ("\033c");
   return 0;
}

char
yEXEC_tty_user          (char *a_user, char a_rotate, char *a_true)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   int         j           =    0;
   int         x_len       =    0;
   char        x_user      [LEN_LABEL] = "";
   tPASSWD    *x_pass;
   tSHADOW    *x_shad;
   /*---(header)-------------------------*/
   DEBUG_USER   yLOG_enter   (__FUNCTION__);
   /*---(default)------------------------*/
   if (a_true != NULL)  strlcpy (a_true, "", LEN_LABEL);
   /*---(defense)------------------------*/
   DEBUG_YEXEC  yLOG_point   ("a_user"    , a_user);
   --rce;  if (a_user == NULL) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_info    ("a_user"    , a_user);
   DEBUG_YEXEC  yLOG_value   ("a_rotate"  , a_rotate);
   --rce;  if (a_rotate < 0 || a_rotate > 4) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check size)---------------------*/
   x_len = strlen (a_user);
   DEBUG_USER   yLOG_value   ("x_len"     , x_len);
   --rce;  if (x_len < 4) {
      DEBUG_USER   yLOG_note    ("length too short (<4)");
      DEBUG_USER   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (x_len > 15) {
      DEBUG_USER   yLOG_note    ("length too long (>15)");
      DEBUG_USER   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(rotate)-------------------------*/
   for (i = 0; i < x_len; ++i) {
      j = i + a_rotate;
      if (j <  x_len) {
         x_user [j] = a_user [i];
      } else {
         x_user [j - x_len] = a_user [i];
      }
   }
   x_user [x_len] = '\0';
   DEBUG_USER   yLOG_info    ("x_user"    , x_user);
   /*---(check for root)-----------------*/
   --rce;  if (strcmp (x_user, "root") == 0) {
      DEBUG_USER   yLOG_note    ("can not log directly into root");
      DEBUG_USER   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   if (strcmp (x_user, "kurios") == 0) {
      DEBUG_USER   yLOG_note    ("kurios is the alias of root");
      strcpy (x_user, "root");
      DEBUG_USER   yLOG_info    ("x_user"    , x_user);
   }
   /*---(check)--------------------------*/
   x_pass  = getpwnam (x_user);
   DEBUG_USER   yLOG_point   ("x_pass"   , x_pass);
   if (x_pass == NULL) {
      DEBUG_USER   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_shad = getspnam (x_user);
   DEBUG_USER   yLOG_point   ("x_shad"   , x_shad);
   if (x_shad == NULL) {
      DEBUG_USER   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save back)----------------------*/
   if (a_true != NULL)  strlcpy (a_true, x_user, LEN_LABEL);
   /*---(complete)-----------------------*/
   DEBUG_USER   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yEXEC_tty_password      (char *a_user, char *a_pass)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         x_len       =    0;
   tSHADOW    *x_shad;
   char       *x_pass      = NULL;
   char       *x_salt      = NULL;
   char       *x_encrypt   = NULL;
   /*---(header)-------------------------*/
   DEBUG_USER   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YEXEC  yLOG_point   ("a_user"    , a_user);
   --rce;  if (a_user == NULL) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_info    ("a_user"    , a_user);
   DEBUG_YEXEC  yLOG_point   ("a_pass"    , a_pass);
   --rce;  if (a_pass == NULL) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_info    ("a_pass"    , a_pass);
   /*---(get user name)------------------*/
   x_len = strlen (a_pass);
   DEBUG_USER   yLOG_value   ("x_len"     , x_len);
   --rce;  if (x_len < 4) {
      DEBUG_USER   yLOG_note    ("length too short (<4)");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (x_len > 15) {
      DEBUG_USER   yLOG_note    ("length too long (>15)");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check)--------------------------*/
   x_shad = getspnam (a_user);
   DEBUG_USER   yLOG_point   ("x_shad"   , x_shad);
   if (x_shad == NULL) {
      DEBUG_USER   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(password)-----------------------*/
   x_pass    = strdup (x_shad->sp_pwdp);
   DEBUG_USER   yLOG_info    ("x_pass"    , x_pass);
   x_salt    = strdup (x_shad->sp_pwdp);
   DEBUG_USER   yLOG_info    ("x_salt"    , x_salt);
   x_encrypt = crypt (a_pass, x_salt);
   DEBUG_USER   yLOG_point   ("x_encrypt" , x_encrypt);
   --rce;  if (strcmp (x_encrypt, x_pass) != 0) {
      free (x_pass);
      free (x_salt);
      DEBUG_USER   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(free)---------------------------*/
   free (x_pass);
   free (x_salt);
   /*---(complete)-----------------------*/
   DEBUG_USER   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yexec_tty__userdel     (cchar *a_name)
{
   char        rce          =  -10;
   char        rc           =    0;
   char        t            [LEN_RECD ] = "";
   --rce;  if (a_name == NULL) return rce;
   snprintf (t, LEN_RECD, "userdel --remove %s >> /dev/null 2>&1", a_name);
   rc = system (t);
   --rce;  if (rc < 0)   return rce;
   return 0; 
}

char
yexec_tty__useradd      (cchar *a_name, cchar *a_pass)
{
   char        rce          =  -10;
   char        rc           =    0;
   char        t            [LEN_RECD  ] = "";
   --rce;  if (a_name == NULL) return rce;
   --rce;  if (a_pass == NULL) return rce;
   yexec_tty__userdel (a_name);
   snprintf (t, LEN_RECD, "useradd --gid nobody --create-home --no-user-group %s >> /dev/null 2>&1", a_name);
   rc = system (t);
   --rce;  if (rc < 0)   return rce;
   snprintf (t, LEN_RECD, "printf \"%s\n%s\n\" | passwd %s >> /dev/null 2>&1", a_pass, a_pass, a_name);
   rc = system (t);
   --rce;  if (rc < 0)   return rce;
   return 0; 
}


static char s [LEN_DESC] = "";

char*
yEXEC_challenge         (int a_seed, char *a_challenge)
{
   int         i           =    0;
   char        a, b, c, d;
   char       *t           = "'+";
   if (a_seed < 0) {
      strlcpy (s, a_challenge, LEN_DESC);
      return s;
   }
   srand (a_seed);
   for (i =  0; i <=  5; ++i)      s [i] = t [rand () %  2];
   for (i =  6; i <= 19; ++i)      s [i] = (rand () % 10) + '0';
   for (i =  8; i <= 19; i += 3)   s [i] = '\'';
   s [20] = '\'';
   a = (rand () %  4) + ((rand () % 2) ? 'a' : 'A');
   b = (rand () % 19) + ((rand () % 2) ? 'e' : 'E');
   c = (rand () %  3) + ((rand () % 2) ? 'x' : 'X');
   d = rand () % 3;
   switch (d) {
   case 0 : s [21] = a; a = c;  break;
   case 1 : s [21] = b; b = c;  break;
   case 2 : s [21] = c;         break;
   }
   d = rand () % 2;
   switch (d) {
   case 0 : s [22] = a; s [23] = b;  break;
   case 1 : s [22] = b; s [23] = a;  break;
   }
   s [24] = '\'';
   for (i = 25; i <= 35; ++i)      s [i] = (rand () % 10) + '0';
   for (i = 28; i <= 35; i += 4)   s [i] = '\'';
   /*> for (i = 36; i <= 41; ++i)      s [i] = (rand () %  2 == 0) ? '\'' : '+';      <*/
   for (i = 36; i <= 41; ++i)      s [i] = t [rand () %  2];
   if (a_challenge != NULL)  strlcpy (a_challenge, s, LEN_DESC);
   return s;
}

/*> 123456789-123456789-123456789-123456789-12                                        <*/
/*> '++'+'12'63'78'44'23'AJC'123'454'454'++'+'                                        <*/
/*> '++'23bermem/8testing/454¥                                    <*/

static char s_phase      = '-';
static char s_judgement  = '-';
static char s_rot        =   0;
static char s_off        =   0;
static char s_pos        =   0;

#define  FAILURE    { s_judgement = 'F';  if (a_phase != NULL)  *a_phase = s_phase;  if (a_judgement != NULL)  *a_judgement = s_judgement;  if (a_position != NULL)  *a_position = s_pos - 1;  return rce; }
#define  PARTIAL    {                     if (a_phase != NULL)  *a_phase = s_phase;  if (a_judgement != NULL)  *a_judgement = s_judgement;  if (a_position != NULL)  *a_position = s_pos - 1;  return rce; }
#define  UPDATE     if (a_phase != NULL)  *a_phase = s_phase;  if (a_judgement != NULL)  *a_judgement = s_judgement;  if (a_position != NULL)  *a_position = s_pos - 1;

char
yEXEC_response          (char *a_response, char *a_phase, char *a_judgement, char *a_position, char *a_user)
{
   char        rce         =  -10;
   char        rc          =    0;
   int         x_len       =    0;
   char        n           =    0;
   char        c           =    0;
   char       *p           = NULL;
   char       *q           = NULL;
   char       *r           = NULL;
   char        x_pref      [LEN_LABEL] = "";
   char        x_user      [LEN_LABEL] = "";
   char        t           [LEN_DESC]  = "";
   char        x_off       =    0;
   /*---(default save backs)-------------*/
   if (a_phase     != NULL)  *a_phase     = '-';
   if (a_judgement != NULL)  *a_judgement = '-';
   if (a_position  != NULL)  *a_position  = 0;
   if (a_user      != NULL)  strlcpy (a_user, "", LEN_LABEL);
   /*---(initialize globals)-------------*/
   s_phase     = '-';
   s_judgement = 'i';
   s_rot       =   0;
   s_off       =   0;
   s_pos       =   0;
   /*---(defense)------------------------*/
   --rce;  if (a_response  == NULL)  return rce;
   /*---(preparation)--------------------*/
   strlcpy (x_pref, a_response, 7);
   x_len = strlen (x_pref);
   /*---(make sure exact can not match)--*/
   strldchg (x_pref, '+' , '-' , LEN_LABEL);
   strldchg (x_pref, '\'', '-' , LEN_LABEL);
   /*---(transform)----------------------*/
   strldchg (x_pref, '.' , '+' , LEN_LABEL);
   strldchg (x_pref, ' ' , '\'', LEN_LABEL);
   /*---(unit testing)-------------------*/
   strldchg (x_pref, '¬' , '\'', LEN_LABEL);
   strldchg (x_pref, '·' , '\'', LEN_LABEL);
   /*---(knock)--------------------------*/
   s_phase = 'k';
   UPDATE;
   --rce;  ++s_pos;  if (x_len >= s_pos && x_pref [s_pos - 1] != s [ 1])  FAILURE;
   --rce;  if (x_len <= s_pos)   PARTIAL;
   --rce;  ++s_pos;  if (x_len >= s_pos && x_pref [s_pos - 1] != s [ 4])  FAILURE;
   --rce;  if (x_len <= s_pos)   PARTIAL;
   --rce;  ++s_pos;  if (x_len >= s_pos && x_pref [s_pos - 1] != s [37])  FAILURE;
   --rce;  if (x_len <= s_pos)   PARTIAL;
   --rce;  ++s_pos;  if (x_len >= s_pos && x_pref [s_pos - 1] != s [40])  FAILURE;
   --rce;  if (x_len <= s_pos)   PARTIAL;
   /*---(prefix)-------------------------*/
   s_phase = 'p';
   UPDATE;
   --rce;  ++s_pos;  if (x_len >= s_pos && x_pref [s_pos - 1] != s [ 6])  FAILURE;
   --rce;  if (x_len <= s_pos)   PARTIAL;
   --rce;  ++s_pos;  if (x_len >= s_pos && x_pref [s_pos - 1] != s [18])  FAILURE;
   /*---(user)---------------------------*/
   x_len = strlen (a_response);
   --rce;  if (x_len <= s_pos)   PARTIAL;
   s_phase = 'u';
   UPDATE;
   if      (strchr ("ABCD", s [21]) != NULL)  s_rot = s [21] - 'A' + 1;
   else if (strchr ("abcd", s [21]) != NULL)  s_rot = s [21] - 'a' + 1;
   else if (strchr ("ABCD", s [22]) != NULL)  s_rot = s [22] - 'A' + 1;
   else if (strchr ("abcd", s [22]) != NULL)  s_rot = s [22] - 'a' + 1;
   else if (strchr ("ABCD", s [23]) != NULL)  s_rot = s [23] - 'A' + 1;
   else if (strchr ("abcd", s [23]) != NULL)  s_rot = s [23] - 'a' + 1;
   c = strldcnt (a_response, '/', LEN_DESC);
   --rce;  if (c < 1)        PARTIAL;
   strlcpy (t, a_response + 6, LEN_DESC);
   p = strtok_r (t, "/", &r);
   UPDATE;
   --rce;  if (p == NULL)    PARTIAL;
   x_len = strlen (p);
   s_pos += x_len;
   UPDATE;
   rc = yEXEC_tty_user (p, s_rot, x_user);
   --rce;  ++s_pos;  if (rc < 0)  FAILURE;
   /*---(infix)--------------------------*/
   p = strtok_r (NULL, "/", &r);
   --rce;  if (p == NULL)    PARTIAL;
   s_phase = 'i';
   UPDATE;
   x_len = strlen (p);
   --rce;  ++s_pos;  if (x_len >= 1 && p [ 0] != s [13])  FAILURE;
   --rce;  if (x_len <= 1)   PARTIAL;
   /*---(password)-----------------------*/
   s_phase = 'c';
   UPDATE;
   c = strldcnt (a_response, '/', LEN_DESC);
   --rce;  if (c < 2)        PARTIAL;
   ++p;
   x_len = strlen (p);
   s_pos += x_len;
   UPDATE;
   rc = yEXEC_tty_password (x_user, p);
   --rce;  ++s_pos;  if (rc < 0)  FAILURE;
   x_len = strlen (a_response);
   --rce;  if (a_response [x_len - 1] == '/')  PARTIAL;
   /*---(suffix)-------------------------*/
   s_phase = 's';
   UPDATE;
   if      (strchr ("XYZ" , s [21]) != NULL)  s_off = s [21] - 'X';
   else if (strchr ("xyz" , s [21]) != NULL)  s_off = s [21] - 'x';
   else if (strchr ("XYZ" , s [22]) != NULL)  s_off = s [22] - 'X';
   else if (strchr ("xyz" , s [22]) != NULL)  s_off = s [22] - 'x';
   else if (strchr ("XYZ" , s [23]) != NULL)  s_off = s [23] - 'X';
   else if (strchr ("xyz" , s [23]) != NULL)  s_off = s [23] - 'x';
   x_off = 25 + (s_off * 4);
   /*> printf ("suffix     [%-3.3s]\n", s + x_off);                                   <*/
   p = strtok_r (NULL, "/", &r);
   --rce;  if (p == NULL)    PARTIAL;
   /*> printf ("remain   %2d[%s]\n", x_len, p);                                       <*/
   --rce;  ++s_pos;  if (x_len >= s_pos  && p [ 0] != s [x_off + 0])  FAILURE;
   --rce;  if (x_len <= s_pos)   PARTIAL;
   --rce;  ++s_pos;  if (x_len >= s_pos && p [ 1] != s [x_off + 1])  FAILURE;
   --rce;  if (x_len <= s_pos)   PARTIAL;
   --rce;  ++s_pos;  if (x_len >= s_pos && p [ 2] != s [x_off + 2])  FAILURE;
   --rce;  if (x_len <= s_pos)   PARTIAL;
   /*---(final)--------------------------*/
   s_phase = 'f';
   UPDATE;
   --rce;  ++s_pos;  if (x_len >= s_pos && (p [ 3] != '¥' && p [ 3] != G_KEY_ESCAPE))   FAILURE;
   s_judgement = 'Y';
   UPDATE;
   /*---(save back)----------------------*/
   if (a_user != NULL)  strlcpy (a_user, x_user, LEN_LABEL);
   /*---(complete)-----------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      unit test accessor                      ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char*            /*--> unit test accessor ------------------------------*/
yexec_tty__unit         (char *a_question)
{ 
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   char        x_heartbeat [LEN_HUND];
   /*---(prepare)------------------------*/
   strlcpy  (unit_answer, "TTY              : question not understood", LEN_RECD);
   /*---(crontab name)-------------------*/
   if      (strcmp (a_question, "response"      )  == 0) {
      snprintf (unit_answer, LEN_RECD, "TTY response     : phase %c, judge %c, s_rot %d, s_off %d, s_pos %2d", s_phase, s_judgement, s_rot, s_off, s_pos);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}







