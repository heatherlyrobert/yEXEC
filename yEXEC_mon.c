/*============================[[    beg-code    ]]============================*/
#include   "yEXEC.h"
#include   "yEXEC_priv.h"



static      char        s_print     [LEN_RECD] = "";

typedef struct cEXEC tEXEC;
typedef struct cPROC tPROC;
typedef struct cLIB  tLIB;
typedef struct cTIE  tTIE;



static struct cEXEC {
   /*---(master)------------*/
   char        name        [LEN_TITLE];
   long        inode;
   /*---(memory)------------*/
   int         m_code;
   /*---(size)--------------*/
   int         s_total;
   int         s_text;
   int         s_data;
   int         s_bss;
   int         s_disk;
   /*---(master list)-------*/
   tEXEC      *m_prev;
   tEXEC      *m_next;
   /*---(processes)---------*/
   tPROC      *p_head;
   tPROC      *p_tail;
   int         p_count;
   /*---(done)--------------*/
};
tEXEC      *e_head      = NULL;
tEXEC      *e_tail      = NULL;
tEXEC      *e_curr      = NULL;
int         e_count     = 0;



static struct cPROC {
   /*---(master)------------*/
   int         rpid;
   int         ppid;
   char        land;
   /*---(cpu)---------------*/
   char        c_state;
   long        c_utime;
   long        c_stime;
   char        c_snice;
   char        c_flag;
   /*---(memory)------------*/
   long        m_max;
   long        m_base;
   long        m_min;
   long        m_text;
   long        m_data;
   long        m_heap;
   long        m_stack;
   long        m_kern;
   long        m_libs;
   long        m_other;
   char        m_flag;
   /*---(disk)--------------*/
   long        d_read;
   long        d_write;
   char        d_flag;
   /*---(net)---------------*/
   long        n_read;
   long        n_write;
   char        n_flag;
   /*---(executable)--------*/
   tEXEC      *e_link;
   tPROC      *p_prev;
   tPROC      *p_next;
   /*---(master list)-------*/
   tPROC      *m_prev;
   tPROC      *m_next;
   /*---(libs)--------------*/
   tTIE       *t_head;
   tTIE       *t_tail;
   int         t_count;
   /*---(done)--------------*/
};
tPROC      *p_head      = NULL;
tPROC      *p_tail      = NULL;
tPROC      *p_curr      = NULL;
int         p_count     = 0;

static struct cLIB {
   /*---(master)------------*/
   char        terse       [LEN_TITLE];
   char        name        [LEN_TITLE];
   long        inode;
   /*---(memory)------------*/
   long        m_full;
   long        m_text;
   long        m_data;
   long        m_proc;
   /*---(procs)-------------*/
   tTIE       *t_head;
   tTIE       *t_tail;
   int         t_count;
   /*---(libs)--------------*/
   tLIB       *m_prev;
   tLIB       *m_next;
   /*---(done)--------------*/
};
tLIB       *l_head      = NULL;
tLIB       *l_tail      = NULL;
tLIB       *l_curr      = NULL;
int         l_count     = 0;

static struct cTIE {
   /*---(memory)------------*/
   long        m_data;
   long        m_heap;
   /*---(ties)--------------*/
   tTIE       *m_prev;
   tTIE       *m_next;
   /*---(links)-------------*/
   tPROC      *t_proc;
   tLIB       *t_lib;
   /*---(done)--------------*/
};
tTIE       *t_head      = NULL;
tTIE       *t_tail      = NULL;
tTIE       *t_curr      = NULL;
int         t_count     = 0;



/*====================------------------------------------====================*/
/*===----                        small support                         ----===*/
/*====================------------------------------------====================*/
static void  o___SUPPORT_________o () { return; }

char*
yexec_mon__ememory      (tEXEC *a_cur)
{
   int         n           =    0;
   if (a_cur == NULL) {
      strlcpy (s_print, "n/a", LEN_RECD);
      return s_print;
   }
   strlcpy (s_print, "å__.______.__.___æ", LEN_RECD);
   ++n;  if (a_cur->name [0]    != '\0')        s_print [n] = 'X';
   ++n;  if (a_cur->inode       >  0)           s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->m_code      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->s_total     >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->s_text      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->s_data      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->s_bss       >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->s_disk      >  0)           s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->m_prev      != NULL)        s_print [n] = 'X';
   ++n;  if (a_cur->m_next      != NULL)        s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->p_head      != NULL)        s_print [n] = 'X';
   ++n;  if (a_cur->p_tail      != NULL)        s_print [n] = 'X';
   ++n;  if (a_cur->p_count     >  0)           s_print [n] = 'X';
   return s_print;
}

char
yexec_mon__ewipe        (tEXEC *a_new, char a_type)
{
   /*---(design)-------------------------*/
   /*
    *  a_type == '*' means everything, '-' means just statistics
    *
    */
   /*---(memory)-------------------------*/
   a_new->m_code   = 0;
   /*---(master)-------------------------*/
   if (a_type == '-')  return 0;
   a_new->name [0] = '\0';
   a_new->inode    = 0;
   /*---(size)---------------------------*/
   a_new->s_total  = 0;
   a_new->s_text   = 0;
   a_new->s_data   = 0;
   a_new->s_bss    = 0;
   a_new->s_disk   = 0;
   /*---(executables)--------------------*/
   a_new->m_prev   = NULL;
   a_new->m_next   = NULL;
   /*---(processes)----------------------*/
   a_new->p_head   = NULL;
   a_new->p_tail   = NULL;
   a_new->p_count  = 0;
   /*---(done)---------------------------*/
   return 0;
}

char
yexec_mon__pwipe        (tPROC *a_new, char a_type)
{
   /*---(design)-------------------------*/
   /*
    *  a_type == '*' means everything, '-' means just statistics
    *
    */
   /*---(master)-------------------------*/
   if (a_type == '*') {
      a_new->rpid     = 0;
      a_new->ppid     = 0;
      a_new->land     = '-';
   }
   /*---(cpu)----------------------------*/
   a_new->c_state  = '-';
   a_new->c_utime  = 0;
   a_new->c_stime  = 0;
   a_new->c_snice  = 0;
   a_new->c_flag   = '-';
   /*---(memory--------------------------*/
   a_new->m_max    = 0;
   a_new->m_base   = 0;
   a_new->m_min    = 0;
   a_new->m_text   = 0;
   a_new->m_data   = 0;
   a_new->m_heap   = 0;
   a_new->m_stack  = 0;
   a_new->m_kern   = 0;
   a_new->m_libs   = 0;
   a_new->m_other  = 0;
   a_new->m_flag   = '-';
   /*---(disk)---------------------------*/
   a_new->d_read   = 0;
   a_new->d_write  = 0;
   a_new->d_flag   = '-';
   /*---(network)------------------------*/
   a_new->n_read   = 0;
   a_new->n_write  = 0;
   a_new->n_flag   = '-';
   /*---(procs)--------------------------*/
   if (a_type == '*') {
      a_new->e_link   = NULL;
      a_new->m_prev   = NULL;
      a_new->m_next   = NULL;
      a_new->t_head   = NULL;
      a_new->t_tail   = NULL;
      a_new->t_count  = 0;
   }
   /*---(done)---------------------------*/
   return 0;
}

char*
yexec_mon__pmemory      (tPROC *a_cur)
{
   int         n           =    0;
   if (a_cur == NULL) {
      strlcpy (s_print, "n/a", LEN_RECD);
      return s_print;
   }
   strlcpy (s_print, "å__.______.___________.___.___._.__.___æ", LEN_RECD);
   ++n;  if (a_cur->rpid        >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->ppid        >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->land        != '-')         s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->c_state     != '-')         s_print [n] = 'X';
   ++n;  if (a_cur->c_utime     >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->c_stime     >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->c_snice     >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->c_flag      != '-')         s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->m_max       >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->m_base      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->m_min       >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->m_text      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->m_data      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->m_heap      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->m_stack     >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->m_kern      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->m_libs      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->m_other     >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->m_flag      != '-')         s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->d_read      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->d_write     >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->d_flag      != '-')         s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->n_read      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->n_write     >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->n_flag      != '-')         s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->e_link      != NULL)        s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->m_prev      != NULL)        s_print [n] = 'X';
   ++n;  if (a_cur->m_next      != NULL)        s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->t_head      != NULL)        s_print [n] = 'X';
   ++n;  if (a_cur->t_tail      != NULL)        s_print [n] = 'X';
   ++n;  if (a_cur->t_count     >  0)           s_print [n] = 'X';
   return s_print;
}

char
yexec_mon__lwipe        (tLIB *a_new, char a_type)
{
   /*---(design)-------------------------*/
   /*
    *  a_type == '*' means everything, '-' means just statistics
    *
    */
   /*---(master)-------------------------*/
   if (a_type == '*') {
      a_new->terse [0] = '\0';
      a_new->name [0]  = '\0';
      a_new->inode     = 0;
   }
   /*---(memory--------------------------*/
   a_new->m_full   = 0;
   a_new->m_text   = 0;
   a_new->m_data   = 0;
   a_new->m_proc   = 0;
   /*---(links)--------------------------*/
   if (a_type == '*') {
      a_new->m_prev   = NULL;
      a_new->m_next   = NULL;
      a_new->t_head   = NULL;
      a_new->t_tail   = NULL;
      a_new->t_count  = 0;
   }
   /*---(done)---------------------------*/
   return 0;
}

char*
yexec_mon__lmemory      (tLIB *a_cur)
{
   int         n           =    0;
   if (a_cur == NULL) {
      strlcpy (s_print, "n/a", LEN_RECD);
      return s_print;
   }
   strlcpy (s_print, "å___.____.__.___æ", LEN_RECD);
   ++n;  if (a_cur->terse   [0] != '\0')        s_print [n] = 'X';
   ++n;  if (a_cur->name    [0] != '\0')        s_print [n] = 'X';
   ++n;  if (a_cur->inode       >  0)           s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->m_full      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->m_text      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->m_data      >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->m_proc      >  0)           s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->m_prev      != NULL)        s_print [n] = 'X';
   ++n;  if (a_cur->m_next      != NULL)        s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->t_head      != NULL)        s_print [n] = 'X';
   ++n;  if (a_cur->t_tail      != NULL)        s_print [n] = 'X';
   ++n;  if (a_cur->t_count     >  0)           s_print [n] = 'X';
   return s_print;
}



/*====================------------------------------------====================*/
/*===----                       allocation/memory                      ----===*/
/*====================------------------------------------====================*/
static void  o___MEMORY__________o () { return; }

char
yexec_mon__shared_new   (char a_type, void **a_new, char a_force)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   void       *x_new       = NULL;
   int         x_tries     =    0;
   int         x_size      =    0;
   tEXEC      *x_exec      = NULL;
   tPROC      *x_proc      = NULL;
   tTIE       *x_tie       = NULL;
   tLIB       *x_lib       = NULL;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_senter  (__FUNCTION__);
   DEBUG_DATA   yLOG_schar   (a_type);
   /*---(check return)-------------------*/
   DEBUG_DATA   yLOG_spoint  (a_new);
   --rce;  if (a_new == NULL) {
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DATA   yLOG_spoint  (*a_new);
   --rce;  if (a_force != 'y' && *a_new != NULL) {
      DEBUG_DATA   yLOG_snote   ("already set");
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(default)------------------------*/
   *a_new = NULL;
   /*---(check size)---------------------*/
   --rce;  switch (a_type) {
   case 'E' :
      x_size = sizeof (tEXEC);
      break;
   case 'P' :
      x_size = sizeof (tPROC);
      break;
   case 'T' :
      x_size = sizeof (tTIE );
      break;
   case 'L' :
      x_size = sizeof (tLIB );
      break;
   default  :
      DEBUG_DATA   yLOG_snote   ("unknown type");
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DATA   yLOG_sint    (x_size);
   /*---(allocate)-----------------------*/
   while (x_new == NULL) {
      ++x_tries;
      x_new = malloc (x_size);
      if (x_tries > 3)   break;
   }
   DEBUG_DATA   yLOG_sint    (x_tries);
   DEBUG_DATA   yLOG_spoint  (x_new);
   --rce;  if (x_new == NULL) {
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(wipe, attach, and increment)----*/
   --rce;  switch (a_type) {
   case 'E' :
      x_exec = (tEXEC *) x_new;
      yexec_mon__ewipe (x_exec, '*');
      if    (e_tail == NULL) { e_head = x_exec; }
      else { x_exec->m_prev  = e_tail; e_tail->m_next = x_exec; }
      e_tail = x_exec;
      ++e_count;
      break;
   case 'P' :
      x_proc = (tPROC *) x_new;
      yexec_mon__pwipe (x_proc, '*');
      if    (p_tail == NULL) { p_head = x_proc; }
      else { x_proc->m_prev  = p_tail; p_tail->m_next = x_proc; }
      p_tail = x_proc;
      ++p_count;
      break;
   case 'T' :
      /*> x_tie  = (tTIE *) x_new;                                                    <* 
       *> yexec_mon__twipe (x_tie, '*');                                              <* 
       *> if    (t_tail == NULL) { t_head = x_tie; }                                  <* 
       *> else { x_tie->m_prev  = t_tail; t_tail->m_next = x_tie; }                   <* 
       *> t_tail = x_tie;                                                             <* 
       *> ++t_count;                                                                  <*/
      break;
   case 'L' :
      x_lib  = (tLIB *) x_new;
      yexec_mon__lwipe (x_lib, '*');
      if    (l_tail == NULL) { l_head = x_lib; }
      else { x_lib->m_prev  = l_tail; l_tail->m_next = x_lib; }
      l_tail = x_lib;
      ++l_count;
      break;
   }
   /*---(save return)--------------------*/
   *a_new = x_new;
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
   return rc;
}

char
yexec_mon__shared_free  (char a_type, void **a_old)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   tEXEC      *x_exec      = NULL;
   tPROC      *x_proc      = NULL;
   tTIE       *x_tie       = NULL;
   tLIB       *x_lib       = NULL;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_senter  (__FUNCTION__);
   DEBUG_DATA   yLOG_schar   (a_type);
   /*---(check return)-------------------*/
   DEBUG_DATA   yLOG_spoint  (a_old);
   --rce;  if (a_old == NULL) {
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DATA   yLOG_spoint  (*a_old);
   --rce;  if (*a_old == NULL) {
      DEBUG_DATA   yLOG_snote   ("never set");
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(detach and decrement)-----------*/
   --rce;  switch (a_type) {
   case 'E' :
      x_exec = (tEXEC *) *a_old;
      if (x_exec->m_next != NULL) x_exec->m_next->m_prev   = x_exec->m_prev;
      else                        e_tail                   = x_exec->m_prev;
      if (x_exec->m_prev != NULL) x_exec->m_prev->m_next   = x_exec->m_next;
      else                        e_head                   = x_exec->m_next;
      --e_count;
      break;
   case 'P' :
      x_proc = (tPROC *) *a_old;
      if (x_proc->m_next != NULL) x_proc->m_next->m_prev   = x_proc->m_prev;
      else                        p_tail                   = x_proc->m_prev;
      if (x_proc->m_prev != NULL) x_proc->m_prev->m_next   = x_proc->m_next;
      else                        p_head                   = x_proc->m_next;
      --p_count;
      break;
   case 'T' :
      x_tie  = (tTIE *) *a_old;
      if (x_tie->m_next != NULL)  x_tie->m_next->m_prev    = x_tie->m_prev;
      else                        t_tail                   = x_tie->m_prev;
      if (x_tie->m_prev != NULL)  x_tie->m_prev->m_next    = x_tie->m_next;
      else                        t_head                   = x_tie->m_next;
      --t_count;
      break;
   case 'L' :
      x_lib  = (tLIB *) *a_old;
      if (x_lib->m_next != NULL)  x_lib->m_next->m_prev    = x_lib->m_prev;
      else                        l_tail                   = x_lib->m_prev;
      if (x_lib->m_prev != NULL)  x_lib->m_prev->m_next    = x_lib->m_next;
      else                        l_head                   = x_lib->m_next;
      l_tail = x_lib;
      --l_count;
      break;
   }
   /*---(clear and return)---------------*/
   free (*a_old);
   *a_old = NULL;
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
yexec_mon__shared_purge (char a_type)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   void       *x_curr      = NULL;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_senter  (__FUNCTION__);
   DEBUG_DATA   yLOG_schar   (a_type);
   /*---(prepare)------------------------*/
   --rce;  switch (a_type) {
   case 'E' :
      x_curr = e_head;
      break;
   case 'P' :
      x_curr = p_head;
      break;
   case 'T' :
      x_curr = t_head;
      break;
   case 'L' :
      x_curr = l_head;
      break;
      DEBUG_DATA   yLOG_snote   ("unknown type");
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   while (x_curr != NULL) {
      switch (a_type) {
      case 'E' :
         yexec_mon__efree (&x_curr);
         x_curr = e_head;
         break;
      case 'P' :
         yexec_mon__unhook_proc (&x_curr);
         x_curr = p_head;
         break;
      case 'T' :
         yexec_mon__tfree (&x_curr);
         x_curr = t_head;
         break;
      case 'L' :
         yexec_mon__lfree (&x_curr);
         x_curr = l_head;
         break;
      }
   }
   /*---(ground everything)--------------*/
   switch (a_type) {
   case 'E' :
      e_head   = e_tail   = e_curr   = NULL;
      e_count  = 0;
      break;
   case 'P' :
      p_head   = p_tail   = p_curr   = NULL;
      p_count  = 0;
      break;
   case 'T' :
      t_head   = t_tail   = t_curr   = NULL;
      t_count  = 0;
      break;
   case 'L' :
      l_head   = l_tail   = l_curr   = NULL;
      l_count  = 0;
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        simpifiers                            ----===*/
/*====================------------------------------------====================*/
static void  o___SIMPLIFIERS_____o () { return; }

char yexec_mon__enew   (void **a_new) { return yexec_mon__shared_new   ('E', a_new, '-'); }
char yexec_mon__eforce (void **a_new) { return yexec_mon__shared_new   ('E', a_new, 'y'); }
char yexec_mon__pnew   (void **a_new) { return yexec_mon__shared_new   ('P', a_new, '-'); }
char yexec_mon__pforce (void **a_new) { return yexec_mon__shared_new   ('P', a_new, 'y'); }
char yexec_mon__tnew   (void **a_new) { return yexec_mon__shared_new   ('T', a_new, '-'); }
char yexec_mon__tforce (void **a_new) { return yexec_mon__shared_new   ('T', a_new, 'y'); }
char yexec_mon__lnew   (void **a_new) { return yexec_mon__shared_new   ('L', a_new, '-'); }
char yexec_mon__lforce (void **a_new) { return yexec_mon__shared_new   ('L', a_new, 'y'); }

char yexec_mon__efree  (void **a_old) { return yexec_mon__shared_free  ('E', a_old); }
char yexec_mon__pfree  (void **a_old) { return yexec_mon__shared_free  ('P', a_old); }
char yexec_mon__tfree  (void **a_old) { return yexec_mon__shared_free  ('T', a_old); }
char yexec_mon__lfree  (void **a_old) { return yexec_mon__shared_free  ('L', a_old); }

char yexec_mon__epurge (void)         { return yexec_mon__shared_purge ('E'); }
char yexec_mon__ppurge (void)         { return yexec_mon__shared_purge ('P'); }
char yexec_mon__tpurge (void)         { return yexec_mon__shared_purge ('T'); }
char yexec_mon__lpurge (void)         { return yexec_mon__shared_purge ('L'); }



/*====================------------------------------------====================*/
/*===----                       mass data changes                      ----===*/
/*====================------------------------------------====================*/
static void  o___FIND____________o () { return; }

#define  IF_EXEC   if      (a_type == 'E')
#define  EL_PROC   else if (a_type == 'P')
#define  EL_TIE    else if (a_type == 'T')
#define  EL_LIB    else                   

char
yexec_mon__by_cursor    (char a_type, char a_move, void **a_curr)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tEXEC      *e_temp      = NULL;
   tPROC      *p_temp      = NULL;
   tTIE       *t_temp      = NULL;
   tLIB       *l_temp      = NULL;
   void       *x_curr      = NULL;
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_senter  (__FUNCTION__);
   /*---(defaults)-----------------------*/
   if (a_curr != NULL)  *a_curr = NULL;
   IF_EXEC  x_curr = e_temp = e_curr;
   EL_PROC  x_curr = p_temp = p_curr;
   EL_TIE   x_curr = t_temp = t_curr;
   EL_LIB   x_curr = l_temp = l_curr;
   /*---(defense)------------------------*/
   DEBUG_YEXEC  yLOG_spoint  (x_curr);
   --rce;  if (x_curr == NULL) {
      /*---(non-bounce)------------------*/
      if (strchr (YDLST_DREL, a_move) != NULL) {
         IF_EXEC  e_curr = e_temp;
         EL_PROC  p_curr = p_temp;
         EL_TIE   t_curr = t_temp;
         EL_LIB   l_curr = l_temp;
         DEBUG_YEXEC   yLOG_sexitr  (__FUNCTION__, rce);
         return rce;
      }
      /*---(bounce types)----------------*/
      IF_EXEC  x_curr = e_temp = e_head;
      EL_PROC  x_curr = p_temp = p_head;
      EL_TIE   x_curr = t_temp = t_head;
      EL_LIB   x_curr = l_temp = l_head;
      DEBUG_YEXEC   yLOG_spoint  (x_curr);
      if (x_curr == NULL) {
         DEBUG_YEXEC   yLOG_sexitr  (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(switch)-------------------------*/
   DEBUG_YEXEC  yLOG_schar   (a_move);
   --rce;  switch (a_move) {
   case YDLST_HEAD : case YDLST_DHEAD :
      IF_EXEC  x_curr = e_temp = e_head;
      EL_PROC  x_curr = p_temp = p_head;
      EL_TIE   x_curr = t_temp = t_head;
      EL_LIB   x_curr = l_temp = l_head;
      break;
   case YDLST_PREV : case YDLST_DPREV :
      IF_EXEC  x_curr = e_temp = e_temp->m_prev;
      EL_PROC  x_curr = p_temp = p_temp->m_prev;
      EL_TIE   x_curr = t_temp = t_temp->m_prev;
      EL_LIB   x_curr = l_temp = l_temp->m_prev;
      break;
   case YDLST_CURR : case YDLST_DCURR :
      break;
   case YDLST_NEXT : case YDLST_DNEXT :
      IF_EXEC  x_curr = e_temp = e_temp->m_next;
      EL_PROC  x_curr = p_temp = p_temp->m_next;
      EL_TIE   x_curr = t_temp = t_temp->m_next;
      EL_LIB   x_curr = l_temp = l_temp->m_next;
      break;
   case YDLST_TAIL : case YDLST_DTAIL :
      IF_EXEC  x_curr = e_temp = e_tail;
      EL_PROC  x_curr = p_temp = p_tail;
      EL_TIE   x_curr = t_temp = t_tail;
      EL_LIB   x_curr = l_temp = l_tail;
      break;
   default         :
      DEBUG_YEXEC  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_spoint  (x_curr);
   /*---(check end)----------------------*/
   --rce;  if (x_curr == NULL) {
      /*---(bounce off ends)-------------*/
      if (a_move == YDLST_PREV) {
         IF_EXEC  x_curr = e_temp = e_head;
         EL_PROC  x_curr = p_temp = p_head;
         EL_TIE   x_curr = t_temp = t_head;
         EL_LIB   x_curr = l_temp = l_head;
      }
      if (a_move == YDLST_NEXT) {
         IF_EXEC  x_curr = e_temp = e_tail;
         EL_PROC  x_curr = p_temp = p_tail;
         EL_TIE   x_curr = t_temp = t_tail;
         EL_LIB   x_curr = l_temp = l_tail;
      }
      /*---(no bounce)-------------------*/
      if (x_curr == NULL) {
         IF_EXEC  e_curr = e_temp;
         EL_PROC  p_curr = p_temp;
         EL_TIE   t_curr = t_temp;
         EL_LIB   l_curr = l_temp;
         DEBUG_YEXEC   yLOG_sexitr  (__FUNCTION__, rce);
         return rce;
      }
      /*---(mark trouble)----------------*/
      DEBUG_YEXEC   yLOG_snote   ("BOUNCE");
      rc = rce;
      /*---(done)------------------------*/
   }
   /*---(normal result)------------------*/
   IF_EXEC  x_curr = e_curr = e_temp;
   EL_PROC  x_curr = p_curr = p_temp;
   EL_TIE   x_curr = t_curr = t_temp;
   EL_LIB   x_curr = l_curr = l_temp;
   /*---(save back)----------------------*/
   if (a_curr != NULL)  *a_curr = x_curr;
   /*---(complete)-----------------------*/
   DEBUG_YDLST  yLOG_sexit   (__FUNCTION__);
   return rc;
}

char yexec_mon__by_ecursor   (char a_move, tEXEC **a_curr) { return yexec_mon__by_cursor ('E', a_move, a_curr); }
char yexec_mon__by_pcursor   (char a_move, tEXEC **a_curr) { return yexec_mon__by_cursor ('P', a_move, a_curr); }
char yexec_mon__by_tcursor   (char a_move, tEXEC **a_curr) { return yexec_mon__by_cursor ('T', a_move, a_curr); }
char yexec_mon__by_lcursor   (char a_move, tEXEC **a_curr) { return yexec_mon__by_cursor ('L', a_move, a_curr); }

char
yexec_mon__hook_proc    (tPROC **a_proc, int a_rpid)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tPROC      *p_temp      = NULL;
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YEXEC  yLOG_spoint  (a_proc);
   --rce;  if (a_proc == NULL) {
      DEBUG_YEXEC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(default)------------------------*/
   *a_proc = NULL;
   /*---(walk)---------------------------*/
   DEBUG_YEXEC  yLOG_spoint  (p_head);
   p_temp = p_head;
   while (p_temp != NULL) {
      if (p_temp->rpid == a_rpid) {
         DEBUG_YEXEC  yLOG_snote   ("existing");
         p_curr = p_temp;
         DEBUG_YEXEC  yLOG_spoint  (p_curr);
         *a_proc = p_curr;
         DEBUG_YDLST  yLOG_sexit   (__FUNCTION__);
         return 0;
      }
      p_temp = p_temp->m_next;
   }
   /*---(add if necessary)---------------*/
   DEBUG_YEXEC  yLOG_snote   ("must add");
   rc = yexec_mon__pnew (&p_temp);
   DEBUG_YEXEC  yLOG_sint    (rc);
   if (rc < 0) {
      DEBUG_YEXEC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(adding)-------------------------*/
   p_curr = p_temp;
   DEBUG_YEXEC  yLOG_spoint  (p_curr);
   p_temp->rpid = a_rpid;
   *a_proc = p_curr;
   /*---(complete)-----------------------*/
   DEBUG_YDLST  yLOG_sexit   (__FUNCTION__);
   return 1;
}

char
yexec_mon__hook_exec    (tPROC *a_proc, char *a_name)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tEXEC      *e_temp      = NULL;
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YEXEC  yLOG_spoint  (a_proc);
   --rce;  if (a_proc == NULL) {
      DEBUG_YEXEC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_spoint  (a_name);
   --rce;  if (a_name == NULL) {
      DEBUG_YEXEC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_snote   (a_name);
   /*---(walk)---------------------------*/
   e_temp = e_head;
   while (e_temp != NULL) {
      if (strcmp (e_temp->name, a_name) == 0) {
         e_curr = e_temp;
         break;
      }
      e_temp = e_temp->m_next;
   }
   /*---(add if necessary)---------------*/
   --rce;  if (e_temp == NULL) {
      DEBUG_YEXEC  yLOG_snote   ("must add");
      rc = yexec_mon__enew (&e_temp);
      DEBUG_YEXEC  yLOG_sint    (rc);
      if (rc < 0) {
         DEBUG_YEXEC   yLOG_sexitr  (__FUNCTION__, rce);
         return rce;
      }
      e_curr = e_temp;
      strlcpy (e_temp->name, a_name, LEN_TITLE);
      rc = 1;
   } else {
      DEBUG_YEXEC  yLOG_snote   ("existing");
      rc = 0;
   }
   DEBUG_YEXEC  yLOG_spoint  (e_curr);
   /*---(initialize)---------------------*/
   a_proc->p_prev  = a_proc->p_next = NULL;
   /*---(link exec to proc)--------------*/
   if (e_curr->p_head == NULL) {
      DEBUG_DATA   yLOG_snote  ("first");
      e_curr->p_head  = e_curr->p_tail = a_proc;
   } else {
      DEBUG_DATA   yLOG_snote   ("append");
      a_proc->p_prev          = e_curr->p_tail;
      e_curr->p_tail->p_next  = a_proc;
      e_curr->p_tail          = a_proc;
   }
   /*---(tie proc back to exec)----------*/
   a_proc->e_link = e_curr;
   /*---(update count)-------------------*/
   ++e_curr->p_count;
   /*---(complete)-----------------------*/
   DEBUG_YDLST  yLOG_sexit   (__FUNCTION__);
   return rc;
}

char
yexec_mon__unhook_exec  (tPROC *a_proc)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tEXEC      *x_exec      = NULL;
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YEXEC  yLOG_spoint  (a_proc);
   --rce;  if (a_proc == NULL) {
      DEBUG_YEXEC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   x_exec = a_proc->e_link;
   DEBUG_YEXEC  yLOG_spoint  (x_exec);
   if (x_exec == NULL) {
      DEBUG_YEXEC   yLOG_sexit   (__FUNCTION__);
      return 0;
   }
   /*---(unlink exec from proc)----------*/
   if (a_proc->p_next != NULL)  a_proc->p_next->p_prev = a_proc->p_prev;
   else                         x_exec->p_tail         = a_proc->p_prev;
   if (a_proc->p_prev != NULL)  a_proc->p_prev->p_next = a_proc->p_next;
   else                         x_exec->p_head         = a_proc->p_next;
   /*---(update count)-------------------*/
   --(x_exec->p_count);
   /*---(ground)-------------------------*/
   a_proc->e_link = NULL;
   /*---(remove if necesary)-------------*/
   if (x_exec->p_count <= 0)  yexec_mon__efree (&x_exec);
   /*---(complete)-----------------------*/
   DEBUG_YDLST  yLOG_sexit   (__FUNCTION__);
   return rc;
}

char
yexec_mon__unhook_proc  (tPROC **a_proc)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tEXEC      *x_exec      = NULL;
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YEXEC  yLOG_spoint  (a_proc);
   --rce;  if (a_proc == NULL) {
      DEBUG_YEXEC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YEXEC  yLOG_spoint  (*a_proc);
   --rce;  if (*a_proc == NULL) {
      DEBUG_YEXEC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(unlink exec)--------------------*/
   rc = yexec_mon__unhook_exec (*a_proc);
   /*---(remove base)--------------------*/
   rc = yexec_mon__pfree (a_proc);
   /*---(complete)-----------------------*/
   DEBUG_YDLST  yLOG_sexit   (__FUNCTION__);
   return rc;
}

char
yexec_mon__by_rpid      (int a_rpid, tPROC **a_curr)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tPROC      *p_temp      = NULL;
   /*---(header)-------------------------*/
   DEBUG_YEXEC  yLOG_senter  (__FUNCTION__);
   /*---(defaults)-----------------------*/
   if (a_curr != NULL)  *a_curr = NULL;
   /*---(defense)------------------------*/
   DEBUG_YEXEC  yLOG_spoint  (p_head);
   --rce;  if (p_head == NULL) {
      DEBUG_YEXEC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(walk)---------------------------*/
   p_temp = p_head;
   while (p_temp != NULL) {
      if (p_temp->rpid == a_rpid) {
         p_curr = p_temp;
         break;
      }
      p_temp = p_temp->m_next;
   }
   /*---(defense)------------------------*/
   DEBUG_YEXEC  yLOG_spoint  (p_temp);
   --rce;  if (p_temp == NULL) {
      DEBUG_YEXEC   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(normal result)------------------*/
   if (a_curr != NULL)  *a_curr = p_curr;
   /*---(complete)-----------------------*/
   DEBUG_YDLST  yLOG_sexit   (__FUNCTION__);
   return rc;
}

/*> char                                                                              <* 
 *> yexec_mon__by_inode     (int a_inode, void **a_curr)                              <* 
 *> {                                                                                 <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                       <* 
 *>    char        rce         =  -10;                                                <* 
 *>    char        rc          =    0;                                                <* 
 *>    tLIB       *l_temp      = NULL;                                                <* 
 *>    tPROC      *p_temp      = NULL;                                                <* 
 *>    /+---(header)-------------------------+/                                       <* 
 *>    DEBUG_YEXEC  yLOG_senter  (__FUNCTION__);                                      <* 
 *>    /+---(defaults)-----------------------+/                                       <* 
 *>    if (a_curr != NULL)  *a_curr = NULL;                                           <* 
 *>    /+---(defense)------------------------+/                                       <* 
 *>    DEBUG_YEXEC  yLOG_spoint  (l_head);                                            <* 
 *>    --rce;  if (l_head == NULL) {                                                  <* 
 *>       DEBUG_YEXEC   yLOG_sexitr  (__FUNCTION__, rce);                             <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    /+---(check process)------------------+/                                       <* 
 *>    p_temp = p_head;                                                               <* 
 *>    while (p_temp != NULL) {                                                       <* 
 *>       if (p_temp->inode == a_inode) {                                             <* 
 *>          DEBUG_YDLST  yLOG_snote   ("inode was process");                         <* 
 *>          if (a_curr != NULL)  *a_curr = p_curr;                                   <* 
 *>          DEBUG_YDLST  yLOG_sexit   (__FUNCTION__);                                <* 
 *>          return 1;                                                                <* 
 *>       }                                                                           <* 
 *>       p_temp = p_temp->m_next;                                                    <* 
 *>    }                                                                              <* 
 *>    /+---(check library)------------------+/                                       <* 
 *>    l_temp = l_head;                                                               <* 
 *>    while (l_temp != NULL) {                                                       <* 
 *>       if (l_temp->inode == a_inode) {                                             <* 
 *>          l_curr = l_temp;                                                         <* 
 *>          if (a_curr != NULL)  *a_curr = p_curr;                                   <* 
 *>          DEBUG_YDLST  yLOG_sexit   (__FUNCTION__);                                <* 
 *>          return 1;                                                                <* 
 *>          DEBUG_YDLST  yLOG_snote   ("inode was library");                         <* 
 *>          break;                                                                   <* 
 *>       }                                                                           <* 
 *>       l_temp = l_temp->m_next;                                                    <* 
 *>    }                                                                              <* 
 *>    /+---(defense)------------------------+/                                       <* 
 *>    DEBUG_YEXEC  yLOG_spoint  (l_temp);                                            <* 
 *>    --rce;  if (l_temp == NULL) {                                                  <* 
 *>       DEBUG_YEXEC   yLOG_sexitr  (__FUNCTION__, rce);                             <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    /+---(normal result)------------------+/                                       <* 
 *>    if (a_curr != NULL)  *a_curr = l_curr;                                         <* 
 *>    /+---(complete)-----------------------+/                                       <* 
 *>    DEBUG_YDLST  yLOG_sexit   (__FUNCTION__);                                      <* 
 *>    return 0;                                                                      <* 
 *> }                                                                                 <*/



/*====================------------------------------------====================*/
/*===----                     gathering statistics                     ----===*/
/*====================------------------------------------====================*/
static void  o___STATS___________o () { return; }

char
yexec_mon__cpu_detail   (int a_rpid, char *a_name, int *a_ppid, char *a_land, char *a_state, long *a_utime, long *a_stime, char *a_snice)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   FILE       *f;
   char        x_name      [LEN_HUND]  = "";
   char        x_recd      [LEN_RECD]  = "";
   char       *p           = NULL;
   char       *r           = NULL;
   char        c           =    0;
   /*---(header)------------------------*/
   DEBUG_YEXEC  yLOG_senter  (__FUNCTION__);
   /*---(defaults)----------------------*/
   if (a_name  != NULL)  strlcpy (a_name, "", LEN_TITLE);
   if (a_ppid  != NULL)  *a_ppid  = 0;
   if (a_land  != NULL)  *a_land  = '-';
   if (a_state != NULL)  *a_state = '-';
   if (a_utime != NULL)  *a_utime = 0;
   if (a_stime != NULL)  *a_stime = 0;
   if (a_snice != NULL)  *a_snice = 20;
   /*---(open proc)----------------------*/
   sprintf (x_name, "/proc/%d/stat", a_rpid);
   f = fopen (x_name, "rt");
   DEBUG_YEXEC  yLOG_spoint  (f);
   --rce;  if (f == NULL) {
      DEBUG_YEXEC  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(read line)---------------------*/
   fgets (x_recd, LEN_RECD, f);
   p = strtok_r (x_recd, " ", &r);
   DEBUG_YEXEC  yLOG_spoint  (p);
   --rce;  if (p == NULL) {
      DEBUG_YEXEC  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   while (p != NULL && c < 20) {
      /*> printf ("%2d  %-10p  å%.40sæ\n", c, p, p);                                  <*/
      switch (c) {
      case  1 :
         if (a_name  != NULL)  strlcpy (a_name, p + 1, strlen (p) - 1);
         break;
      case  2 :
         if (a_state != NULL)  *a_state = p [0];
         break;
      case  3 :
         if (a_ppid  != NULL)  *a_ppid  = atoi (p);
         if (a_land  != NULL) {
            switch (*a_ppid) {
            case   0 : *a_land = 'k';    break;
            case   2 : *a_land = 'k';    break;
            default  : *a_land = '-';    break;
            }
            if (a_rpid == 1)  *a_land = '-';
         }
         break;
      case 13 :
         if (a_utime != NULL)  *a_utime = atol (p);
         break;
      case 14 :
         if (a_stime != NULL)  *a_stime = atol (p);
         break;
      case 17 :
         if (a_snice != NULL) {
            *a_snice = atoi (p);
            if (*a_snice < -20)  *a_snice = -1;
         }
         break;
      }
      ++c;
      p = strtok_r (NULL  , " ", &r);
   }
   DEBUG_YEXEC  yLOG_sint    (c);
   --rce;  if (c <  20) {
      DEBUG_YEXEC  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(close file)--------------------*/
   rc = fclose (f);
   DEBUG_YEXEC  yLOG_sint    (rc);
   --rce;  if (f <  0) {
      DEBUG_YEXEC  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)----------------------*/
   DEBUG_YEXEC  yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
yexec_mon__cpu          (tPROC *a_proc)
{
   char        rce         =  -10;
   char        rc          =    0;
   char        x_name      [LEN_TITLE] = "";
   --rce;  if (a_proc == NULL)  return rce;
   rc = yexec_mon__cpu_detail (a_proc->rpid, x_name, &(a_proc->ppid), &(a_proc->land), &(a_proc->c_state), &(a_proc->c_utime), &(a_proc->c_stime), &(a_proc->c_snice));
   if (a_proc->e_link == NULL)  yexec_mon__hook_exec (a_proc, x_name);
   return rc;
}

char
yexec_mon__mem_detail   (int a_rpid, long *a_max, long *a_base, long *a_min, long *a_text, long *a_data, long *a_heap, long *a_stack, long *a_kern, long *a_libs, long *a_empty)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   FILE       *f;
   char        x_name      [LEN_HUND]  = "";
   char        x_recd      [LEN_RECD]  = "";
   int         c           =    0;
   int         i           =    0;
   int         l           =    0;
   char       *p           = NULL;
   char       *r           = NULL;
   long        x_full      =    0;
   long        x_rss       =    0;
   long        x_pvt       =    0;
   long        x_empty     =    0;
   char        x_perm      [LEN_TERSE] = "";
   char        x_part      =  '-';
   int         n           =    1;
   int         x_line      =    0;
   /*---(header)------------------------*/
   DEBUG_YEXEC  yLOG_senter  (__FUNCTION__);
   /*---(defaults)----------------------*/
   if (a_max   != NULL)  *a_max   = 0;
   if (a_base  != NULL)  *a_base  = 0;
   if (a_min   != NULL)  *a_min   = 0;
   if (a_text  != NULL)  *a_text  = 0;
   if (a_data  != NULL)  *a_data  = 0;
   if (a_heap  != NULL)  *a_heap  = 0;
   if (a_stack != NULL)  *a_stack = 0;
   if (a_kern  != NULL)  *a_kern  = 0;
   if (a_libs  != NULL)  *a_libs  = 0;
   if (a_empty != NULL)  *a_empty = 0;
   /*---(open proc)----------------------*/
   sprintf (x_name, "/proc/%d/smaps", a_rpid);
   f = fopen (x_name, "rt");
   DEBUG_YEXEC  yLOG_spoint  (f);
   --rce;  if (f == NULL) {
      DEBUG_YEXEC  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(read stanza)-----------------*/
   while (1) {
      /*> if (c % 5 == 0)  printf ("\nline    --full-  --rss--    p  --max--  --base-  --min--  --text-  --data-  -stack-  --heap-  --kern-  --libs-    -empty-\n");   <*/
      x_line = n;
      for (i = 0; i < 16; ++i) {
         fgets (x_recd, LEN_RECD, f);
         if (feof (f))  break;
         l = strlen (x_recd);
         if (l > 0 && x_recd [l - 1] == '\n')  x_recd [--l] = '\0';
         ++n;
         /*> printf ("%2d, %s\n", i, x_recd);                                         <*/
         p = strtok_r (x_recd, " \t", &r);
         if (p == NULL) break;
         p = strtok_r (NULL  , " \t", &r);
         if (p == NULL) break;
         switch (i) {
         case  0 : /* permissions */
            /*---(save perms)------------*/
            strlcpy (x_perm, p, LEN_TERSE);
            /*---(get prog/lib)----------*/
            p = strtok_r (NULL  , " \t", &r);
            p = strtok_r (NULL  , " \t", &r);
            p = strtok_r (NULL  , " \t", &r);
            p = strtok_r (NULL  , " \t", &r);
            /*---(assign prog/lib)-------*/
            /*> if (p != NULL)  strltrim (p, ySTR_BOTH, LEN_RECD);                    <* 
             *> printf ("%2då%sæ\n", strlen (p), p);                                  <*/
            x_part = 'd';
            /*---(executables)-----------*/
            if (strncmp (x_perm, "r-x"   , 3) == 0) {
               if (c < 3)                                             x_part = 't';
               else if (p != NULL && strcmp  (p, "[vsyscall]") == 0)  x_part = 'k';
               else if (p != NULL && strcmp  (p, "[vdso]"    ) == 0)  x_part = 'k';
               else                                                   x_part = '-';
            }
            /*---(data segments)---------*/
            else if (c < 3 || p == NULL || strcmp  (p, "") == 0) {
               if      (strncmp (x_perm, "r--", 3)  == 0)  x_part = 'd';
               else if (strncmp (x_perm, "rw-", 3)  == 0)  x_part = 'd';
               else                                        x_part = 'o';
            }
            /*---(kernel stuff)----------*/
            else if (strcmp  (p, "[vvar]"    ) == 0)       x_part = 'k';
            else if (strcmp  (p, "[heap]"    ) == 0)       x_part = 'h';
            else if (strcmp  (p, "[stack]"   ) == 0)       x_part = 's';
            /*---(library data)----------*/
            else {
               if      (strncmp (x_perm, "r--", 3)  == 0)  x_part = 'l';
               else if (strncmp (x_perm, "rw-", 3)  == 0)  x_part = 'l';
               else                                        x_part = 'o';
            }
            /*---(assign segment)--------*/
            break;
         case  1 : /* rss           */
            x_full  = atol (p);
            break;
         case  2 : /* rss           */
            x_rss   = atol (p);
            x_empty = x_full - x_rss;
            if (x_empty < 0)  x_empty = 0;
            break;
         case  6 : /* private clean */
            x_pvt   = atol (p);
            break; 
         case  7 : /* private dirty */
            x_pvt  += atol (p);
            break; 
         case 15 :
            switch (x_part) {
            case 't' : 
               if (a_max   != NULL)  *a_max   += x_full;
               if (a_base  != NULL)  *a_base  += x_full;
               if (a_text  != NULL)  *a_text  += x_full;
               break;
            case 'd' : 
               if (a_max   != NULL)  *a_max   += x_pvt;
               if (a_base  != NULL)  *a_base  += x_pvt;
               if (a_data  != NULL)  *a_data  += x_pvt;
               if (a_empty != NULL)  *a_empty += x_full - x_pvt;
               break;
            case 'h' : 
               if (a_max   != NULL)  *a_max   += x_rss;
               if (a_base  != NULL)  *a_base  += x_rss;
               if (a_heap  != NULL)  *a_heap  += x_rss;
               if (a_empty != NULL)  *a_empty += x_empty;
               break;
            case 's' : 
               if (a_max   != NULL)  *a_max   += x_rss;
               if (a_base  != NULL)  *a_base  += x_rss;
               if (a_stack != NULL)  *a_stack += x_rss;
               if (a_empty != NULL)  *a_empty += x_empty;
               break;
            case 'k' : /* mapped kernel space */
               if (a_max   != NULL)  *a_max   += x_full;
               if (a_base  != NULL)  *a_base  += x_full;
               if (a_kern  != NULL)  *a_kern  += x_full;
               break;
            case 'o' : 
               if (a_max   != NULL)  *a_max   += x_full;
               if (a_empty != NULL)  *a_empty += x_full;
               x_empty = 0;
               break;
            case 'l' : 
               if (a_max   != NULL)  *a_max   += x_pvt;
               if (a_base  != NULL)  *a_base  += x_pvt;
               if (a_libs  != NULL)  *a_libs  += x_pvt;
               if (a_empty != NULL)  *a_empty += x_full - x_pvt;
               break;
            }
            /*> printf ("%4d    %7ld  %7ld    %c  %7ld  %7ld  %7ld  %7ld  %7ld  %7ld  %7ld  %7ld  %7ld    %7ld\n",   <* 
             *>       x_line, x_full, x_rss, x_part,                                                                 <* 
             *>       *a_max, *a_base, *a_min, *a_text, *a_data, *a_stack,                                           <* 
             *>       *a_heap, *a_kern, *a_libs, *a_empty);                                                          <*/
            break;
         }
      }
      if (feof (f))  break;
      ++c;
   }
   /*---(close file)--------------------*/
   rc = fclose (f);
   DEBUG_YEXEC  yLOG_sint    (rc);
   --rce;  if (f <  0) {
      DEBUG_YEXEC  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)----------------------*/
   DEBUG_YEXEC  yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
yexec_mon__mem          (tPROC *a_proc)
{
   char        rce         =  -10;
   char        rc          =    0;
   --rce;  if (a_proc == NULL)  return rce;
   rc = yexec_mon__mem_detail (a_proc->rpid, &(a_proc->m_max), &(a_proc->m_base), &(a_proc->m_min), &(a_proc->m_text), &(a_proc->m_data), &(a_proc->m_heap), &(a_proc->m_stack), &(a_proc->m_kern), &(a_proc->m_libs), &(a_proc->m_other));
   return rc;
}

char         /*--> review and act on global crontabs -------------------------*/
yexec_mon__review       (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   DIR        *x_dir       = NULL;
   tDIRENT    *x_file      = NULL;
   int         x_rpid      =    0;
   tPROC      *x_new       = NULL;
   int         x_total     =    0;
   int         x_count     =    0;
   int         x_pass      =    0;
   char        x_prefix    [LEN_USER]  = "";
   char        x_central   [LEN_PATH]  = "";
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   x_dir = opendir ("/proc");
   DEBUG_INPT   yLOG_point   ("x_dir"     , x_dir);
   --rce;  if (x_dir == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(walk files)---------------------*/
   DEBUG_INPT   yLOG_note    ("walk through processes");
   while ((x_file = readdir (x_dir)) != NULL) {
      /*---(simple filtering)-------------------*/
      if (rc != 0)  ++x_total;
      if (strchr (LTRS_NUMBER, x_file->d_name [0]) == NULL) {
         DEBUG_INPT   yLOG_note    ("not leading number");
         continue;
      }
      x_rpid = atoi (x_file->d_name);
      DEBUG_INPT   yLOG_value   ("x_rpid"    , x_rpid);
      if (x_rpid <= 0) {
         DEBUG_INPT   yLOG_note    ("not a process entry");
         continue;
      }
      /*---(create)----------------------*/
      x_new = NULL;
      rc = yexec_mon__hook_proc (&x_new, x_rpid);
      if (rc < 0) {
         DEBUG_INPT   yLOG_note    ("can not add process");
         continue;
      }
      /*---(gather)----------------------*/
      rc = yexec_mon__cpu (x_new);
      rc = yexec_mon__mem (x_new);
      /*---(summary)---------------------*/
      if (x_count % 5 == 0)  printf ("\n#rpid  ---name--------- -ppid l   s -utime- -stime- sn   --text- --data-   --heap- -stack-\n");
      /*> printf ("%5d  %-16.16s %5d %c   %c %7ld %7ld %2d   %7ld %7ld   %7ld %7ld\n",   <* 
       *>       x_new->rpid, x_new->name, x_new->ppid, x_new->land,                      <* 
       *>       x_new->c_state, x_new->c_utime, x_new->c_stime, x_new->c_snice,          <* 
       *>       x_new->m_text , x_new->m_data , x_new->m_heap , x_new->m_stack);         <*/
      /*> printf ("%s\n",                                                             <* 
       *>       x_file->d_name);                                                      <*/
      /*---(total)-----------------------*/
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
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void  o___UNIT_TEST_______o () { return; }

char*        /*-> unit testing accessor --------------[ ------ [gs.HA0.1B3.K5]*/ /*-[02.0000.00#.#]-*/ /*-[--.---.---.--]-*/
yexec_mon__unit         (char *a_question, int n)
{
   /*---(locals)-----------+-----+-----+-*/
   int         x_fore      = 0;
   int         x_back      = 0;
   tEXEC      *x_exec      = NULL;
   tPROC      *x_proc      = NULL;
   tLIB       *x_lib       = NULL;
   char        rc          =    0;
   int         c           =    0;
   char        t           [LEN_RECD]  = "[]";
   char        s           [LEN_RECD]  = "";
   /*---(preprare)-----------------------*/
   strcpy (unit_answer, "MON              : question not understood");
   /*---(dependency list)----------------*/
   if      (strcmp (a_question, "e_count"  )      == 0) {
      x_exec = e_head; while (x_exec != NULL) { ++x_fore; x_exec = x_exec->m_next; }
      x_exec = e_tail; while (x_exec != NULL) { ++x_back; x_exec = x_exec->m_prev; }
      snprintf (unit_answer, LEN_RECD, "MON e_count      : num=%4d, fore=%4d, back=%4d", e_count, x_fore, x_back);
   }
   else if (strcmp (a_question, "e_list"      )   == 0) {
      snprintf (unit_answer, LEN_RECD, "MON e_list       : num=%4d, head=%-10p, tail=%p", e_count, e_head, e_tail);
   }
   else if (strcmp (a_question, "p_count"  )      == 0) {
      x_proc = p_head; while (x_proc != NULL) { ++x_fore; x_proc = x_proc->m_next; }
      x_proc = p_tail; while (x_proc != NULL) { ++x_back; x_proc = x_proc->m_prev; }
      snprintf (unit_answer, LEN_RECD, "MON p_count      : num=%4d, fore=%4d, back=%4d", p_count, x_fore, x_back);
   }
   else if (strcmp (a_question, "p_list"      )   == 0) {
      snprintf (unit_answer, LEN_RECD, "MON p_list       : num=%4d, head=%-10p, tail=%p", p_count, p_head, p_tail);
   }
   else if (strcmp (a_question, "l_count"  )      == 0) {
      x_lib = l_head; while (x_lib != NULL) { ++x_fore; x_lib = x_lib->m_next; }
      x_lib = l_tail; while (x_lib != NULL) { ++x_back; x_lib = x_lib->m_prev; }
      snprintf (unit_answer, LEN_RECD, "MON l_count      : num=%4d, fore=%4d, back=%4d", l_count, x_fore, x_back);
   }
   else if (strcmp (a_question, "l_list"      )   == 0) {
      snprintf (unit_answer, LEN_RECD, "MON l_list       : num=%4d, head=%-10p, tail=%p", l_count, l_head, l_tail);
   }
   else if (strcmp (a_question, "p_entry"  )      == 0) {
      x_proc = p_head;
      while (x_proc != NULL) {
         if (c == n)  break;
         ++c;
         x_proc = x_proc->m_next;
      }
      if (x_proc != NULL) {
         strcpy (t, " -åæ");
         strcpy (s, "-");
         if (x_proc->e_link != NULL) {
            sprintf  (t, "%2då%.10sæ", strlen (x_proc->e_link->name), x_proc->e_link->name);
            sprintf  (s, "%d", x_proc->e_link->inode);
         }
         snprintf (unit_answer, LEN_RECD, "MON p_entry (%2d) : %-5d %-14.14s %-9.9s %-5d %c  %c %7d %7d %2d %c",
               n, x_proc->rpid, t, s, x_proc->ppid, x_proc->land,
               x_proc->c_state, x_proc->c_utime, x_proc->c_stime, x_proc->c_snice, x_proc->c_flag);
      } else {
         snprintf (unit_answer, LEN_RECD, "MON p_entry (%2d) : -      -åæ           -         -     -  -       -       -  - -", n);
      }
      return unit_answer;
   }
   else if (strcmp (a_question, "e_entry"  )      == 0) {
      x_exec = e_head;
      while (x_exec != NULL) {
         if (c == n)  break;
         ++c;
         x_exec = x_exec->m_next;
      }
      if (x_exec != NULL) {
         sprintf  (t, "%2då%.10sæ", strlen (x_exec->name), x_exec->name);
         snprintf (unit_answer, LEN_RECD, "MON e_entry (%2d) : %-14.14s %-9d   %2d %-10p %p",
               n, t, x_exec->inode, x_exec->p_count, x_exec->p_head, x_exec->p_tail);
      } else {
         snprintf (unit_answer, LEN_RECD, "MON e_entry (%2d) :  -åæ           -            - -          -", n);
      }
      return unit_answer;
   }
   /*> else if (strcmp (a_question, "p_curr"   )      == 0) {                                                            <* 
    *>    x_proc = p_curr;                                                                                               <* 
    *>    if (x_proc != NULL) {                                                                                          <* 
    *>       sprintf  (t, "%2då%.16sæ", strlen (x_proc->name), x_proc->name);                                            <* 
    *>       snprintf (unit_answer, LEN_RECD, "MON p_curr       : %-20.20s  %5d  %d", t, x_proc->rpid, x_proc->inode);   <* 
    *>    } else {                                                                                                       <* 
    *>       snprintf (unit_answer, LEN_RECD, "MON p_curr       :  0[]                      -  -");                      <* 
    *>    }                                                                                                              <* 
    *>    return unit_answer;                                                                                            <* 
    *> }                                                                                                                 <*/
   else if (strcmp (a_question, "l_curr"   )      == 0) {
      x_lib  = l_curr;
      if (x_lib != NULL) {
         sprintf  (t, "%2då%.16sæ", strlen (x_lib->terse), x_lib->terse);
         snprintf (unit_answer, LEN_RECD, "MON l_curr       : %-20.20s  %d", t, x_lib->inode);
      } else {
         snprintf (unit_answer, LEN_RECD, "MON l_curr       :  0[]                      -  -");
      }
      return unit_answer;
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}


