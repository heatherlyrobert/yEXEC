/*============================----beg-of-source---============================*/
#include   "yEXEC.h"
#include   "yEXEC_priv.h"


static   char    s_fifo   [LEN_PATH] = "";
static   char    s_fd     = -1;


char         /*--> verify and create fifo ------------------------------------*/
yexec_fifo__verify      (char a_force, cchar a_name [LEN_PATH])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   tSTAT       s;
   char        x_exist     =    0;
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YEXEC   yLOG_value   ("s_fd"      , s_fd);
   --rce;  if (s_fd >= 0) {
      DEBUG_YEXEC  yLOG_note    ("already open");
      DEBUG_YEXEC  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   DEBUG_YEXEC   yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name == NULL) {
      DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC   yLOG_info    ("a_name"    , a_name);
   /*---(check existance)----------------*/
   rc = stat (a_name, &s);
   DEBUG_YEXEC  yLOG_value   ("stat"      , rc);
   if (rc >= 0) {
      DEBUG_YEXEC  yLOG_note    ("already exists");
      x_exist = 1;
   }
   /*---(test forcing)-------------------*/
   --rce;  if (rc < 0 && a_force != 'y') {
      DEBUG_YEXEC  yLOG_note    ("fifo does not exist");
      DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(need to create)-----------------*/
   --rce;  if (rc < 0) {
      /*---(create)----------------------*/
      rc = mkfifo (a_name, 0600);
      DEBUG_YEXEC  yLOG_value   ("mkfifo"    , rc);
      --rce;  if (rc < 0) {
         DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      /*---(re-check existance)----------*/
      rc = stat (a_name, &s);
      DEBUG_YEXEC  yLOG_value   ("re-stat"   , rc);
      --rce;  if (rc < 0) {
         DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(change ownership)---------------*/
   rc = chown (a_name, 0, 0);
   DEBUG_YEXEC  yLOG_value   ("chown"     , rc);
   --rce;  if (rc != 0) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check file type)----------------*/
   --rce;  if (S_ISDIR (s.st_mode))  {
      DEBUG_YEXEC  yLOG_note    ("can not use a directory");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_note    ("not a directory");
   --rce;  if (S_ISLNK (s.st_mode))  {
      DEBUG_YEXEC  yLOG_note    ("can not use a symlink");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_note    ("not a symlink");
   --rce;  if (S_ISCHR (s.st_mode))  {
      DEBUG_YEXEC  yLOG_note    ("can not use a character device");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_note    ("not a symlink");
   --rce;  if (!S_ISFIFO (s.st_mode))  {
      DEBUG_YEXEC  yLOG_note    ("can only use fifo/pipe files");
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_note    ("confirmed as a fifo/pipe file");
   /*---(complete)-----------------------*/
   DEBUG_YEXEC  yLOG_exit    (__FUNCTION__);
   return x_exist;
}

char yEXEC_fifo_create  (cchar a_name [LEN_PATH]) { return yexec_fifo__verify ('y', a_name); }

char
yexec_fifo__open        (cchar a_name [LEN_PATH])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YEXEC   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YEXEC   yLOG_sint    (s_fd);
   --rce;  if (s_fd >= 0) {
      DEBUG_YEXEC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC   yLOG_spoint  (a_name);
   --rce;  if (a_name == NULL) {
      DEBUG_YEXEC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC   yLOG_snote   (a_name);
   /*---(open)---------------------------*/
   s_fd = open (a_name, O_RDONLY | O_NONBLOCK | O_ASYNC);
   DEBUG_YEXEC   yLOG_sint    (s_fd);
   --rce;  if (s_fd < 0) {
      DEBUG_YEXEC  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(save)---------------------------*/
   ystrlcpy (s_fifo, a_name, LEN_PATH);
   /*---(complete)-----------------------*/
   DEBUG_YEXEC   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
yexec_fifo__close       (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YEXEC   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YEXEC   yLOG_sint    (s_fd);
   --rce;  if (s_fd < 0) {
      DEBUG_YEXEC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(close)--------------------------*/
   rc = close (s_fd);
   DEBUG_YEXEC   yLOG_sint    (rc);
   --rce;  if (rc < 0) {
      DEBUG_YEXEC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(ground)-------------------------*/
   s_fd = -1;
   ystrlcpy (s_fifo, "", LEN_PATH);
   /*---(complete)-----------------------*/
   DEBUG_YEXEC   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char yEXEC_fifo_nolisten   (void) { return yexec_fifo__close (); }

char
yexec_fifo__listener    (cchar a_recd [LEN_RECD])
{
   FILE       *f           = NULL;
   f = fopen ("/tmp/test.fifo.output", "at");
   if (f >= 0) {
      fprintf (f, "%s\n", a_recd);
      fclose (f);
   }
   return 0;
}

char
yEXEC_fifo_listen       (char a_persist, cchar a_name [LEN_PATH], void *f_callback)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   int         x_rc        =    0;
   char        x_recd      [LEN_RECD];
   int         l           =    0;
   int         c           =    0;
   char      (*x_callback)   (cchar a_recd [LEN_RECD]);
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YEXEC  yLOG_point   ("f_callback", f_callback);
   --rce;  if (f_callback == NULL) {
      DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_callback = f_callback;
   /*---(verify fifo each time)----------*/
   rc = yexec_fifo__verify ('y', a_name);
   DEBUG_YEXEC  yLOG_value   ("verify"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(open fifo)----------------------*/
   DEBUG_YEXEC  yLOG_value   ("s_fd"      , s_fd);
   --rce;  if (s_fd < 0) {
      rc = yexec_fifo__open (a_name);
      DEBUG_YEXEC  yLOG_value   ("open"      , rc);
      if (rc < 0) {
         DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(read all)-----------------------*/
   while (1) {
      /*---(read)---------------------------*/
      ystrlcpy (x_recd, "", LEN_RECD);
      l  = 0;
      x_rc = 1;
      while (x_rc == 1) {
         x_rc = read (s_fd, x_recd + l, 1);
         DEBUG_YEXEC  yLOG_complex ("read"      , "%4drc, %2dl, %3d, %c", x_rc, l, x_recd [l], chrvisible (x_recd [l]));
         if (x_recd [l] == '\n')  break;
         if (x_rc == 1)  ++l;
      }
      /*---(callback)-----------------------*/
      l = strlen (x_recd);
      DEBUG_YEXEC  yLOG_value   ("l"         , l);
      if (l > 0) {
         if (x_recd [l - 1] == '\n')  x_recd [--l] = '\0';
         rc = x_callback (x_recd);
         ++c;
         DEBUG_YEXEC   yLOG_complex ("callback"  , "%4drc, %2dc", rc, c);
      }
      /*---(check for end)------------------*/
      if (x_rc <  0) {
         DEBUG_YEXEC  yLOG_exit    (__FUNCTION__);
         return c;
      }
      /*---(done)---------------------------*/
   }
   /*---(close)--------------------------*/
   if (a_persist != 'y') {
      rc = yexec_fifo__close ();
      DEBUG_YEXEC  yLOG_value   ("close"     , rc);
      if (rc < 0) {
         DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_YEXEC  yLOG_exit    (__FUNCTION__);
   return c;
}

char
yEXEC_fifo_destroy      (cchar a_name [LEN_PATH])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_enter   (__FUNCTION__);
   /*---(close if necessary)-------------*/
   if (s_fd >= 0) {
      rc = yexec_fifo__close ();
      DEBUG_YEXEC  yLOG_value   ("close"     , rc);
      if (rc < 0) {
         DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(verify fifo)--------------------*/
   rc = yexec_fifo__verify ('-', a_name);
   DEBUG_YEXEC  yLOG_value   ("verify"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(create)----------------------*/
   rc = remove (a_name);
   DEBUG_YEXEC  yLOG_value   ("remove"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YEXEC  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yEXEC_fifo_speak        (cchar a_name [LEN_PATH], cchar a_recd [LEN_RECD])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   int         x_fd        =   -1;
   char        x_recd      [LEN_RECD];
   int         l           =    0;
   int         c           =    0;
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YEXEC   yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name == NULL) {
      DEBUG_YEXEC   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC   yLOG_info    ("a_name"    , a_name);
   /*---(verify fifo)--------------------*/
   rc = yexec_fifo__verify ('y', a_name);
   DEBUG_YEXEC  yLOG_value   ("verify"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(open fifo)----------------------*/
   x_fd = open (a_name, O_WRONLY | O_NONBLOCK);
   DEBUG_YEXEC  yLOG_value   ("open"      , x_fd);
   --rce;  if (x_fd < 0) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write)--------------------------*/
   ystrlcpy (x_recd, a_recd, LEN_RECD);
   ystrlcat (x_recd, "\n", LEN_RECD);
   l = strlen (x_recd);
   rc = write (x_fd, x_recd, l);
   DEBUG_YEXEC  yLOG_value   ("write"     , rc);
   if (rc <  0) {
      DEBUG_YEXEC  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   if (rc == 0) {
      DEBUG_YEXEC  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   ++c;
   /*---(open fifo)----------------------*/
   rc = close (x_fd);
   DEBUG_YEXEC  yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_YEXEC  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YEXEC  yLOG_exit    (__FUNCTION__);
   return c;
}

