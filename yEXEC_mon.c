/*============================[[    beg-code    ]]============================*/
#include   "yEXEC.h"
#include   "yEXEC_priv.h"



static      char        s_print     [LEN_RECD] = "";



typedef struct cPROC tPROC;
typedef struct cLIB  tLIB;

static struct cPROC {
   /*---(master)------------*/
   int         rpid;
   char        name        [LEN_TITLE];
   long        inode;
   /*---(cpu)---------------*/
   char        c_state;
   long        c_utime;
   long        c_stime;
   char        c_snice;
   char        c_flag;
   /*---(memory)------------*/
   long        m_max;
   long        m_base;
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
   /*---(procs)-------------*/
   tPROC      *p_prev;
   tPROC      *p_next;
   int         p_count;
   /*---(libs)--------------*/
   tLIB       *l_head;
   tLIB       *l_tail;
   int         l_count;
   /*---(done)--------------*/
};
tPROC      *p_head      = NULL;
tPROC      *p_tail      = NULL;
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
   tPROC      *p_head;
   tPROC      *p_tail;
   int         p_count;
   /*---(libs)--------------*/
   tLIB       *l_prev;
   tLIB       *l_next;
   int         l_count;
   /*---(done)--------------*/
};
tLIB       *l_head      = NULL;
tLIB       *l_tail      = NULL;
int         l_count     = 0;



/*====================------------------------------------====================*/
/*===----                        small support                         ----===*/
/*====================------------------------------------====================*/
static void  o___SUPPORT_________o () { return; }

char
yexec_mon__pwipe        (tPROC *a_new)
{
   /*---(master)-------------------------*/
   a_new->rpid     = 0;
   a_new->name [0] = '\0';
   a_new->inode    = 0;
   /*---(cpu)----------------------------*/
   a_new->c_state  = '-';
   a_new->c_utime  = 0;
   a_new->c_stime  = 0;
   a_new->c_snice  = 0;
   a_new->c_flag   = '-';
   /*---(memory--------------------------*/
   a_new->m_max    = 0;
   a_new->m_base   = 0;
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
   a_new->p_prev   = NULL;
   a_new->p_next   = NULL;
   a_new->p_count  = 0;
   a_new->l_head   = NULL;
   a_new->l_tail   = NULL;
   a_new->l_count  = 0;
   /*---(done)---------------------------*/
   return 0;
}

char*
yexec_mon__pmemory      (tPROC *a_cur)
{
   int         n           =    0;
   strlcpy (s_print, "å___._____.__________.___.___.______æ", LEN_RECD);
   ++n;  if (a_cur->rpid        >= 0)           s_print [n] = 'X';
   ++n;  if (a_cur->name    [0] != '\0')        s_print [n] = 'X';
   ++n;  if (a_cur->inode       >= 0)           s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->c_state     != '-')         s_print [n] = 'X';
   ++n;  if (a_cur->c_utime     >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->c_stime     >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->c_snice     >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->c_flag      != '-')         s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->m_max       >  0)           s_print [n] = 'X';
   ++n;  if (a_cur->m_base      >  0)           s_print [n] = 'X';
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
   ++n;  if (a_cur->p_prev      != NULL)        s_print [n] = 'X';
   ++n;  if (a_cur->p_next      != NULL)        s_print [n] = 'X';
   ++n;  if (a_cur->p_count     >  0)           s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->l_head      != NULL)        s_print [n] = 'X';
   ++n;  if (a_cur->l_tail      != NULL)        s_print [n] = 'X';
   ++n;  if (a_cur->l_count     >  0)           s_print [n] = 'X';
   return s_print;
}



/*====================------------------------------------====================*/
/*===----                       allocation/memory                      ----===*/
/*====================------------------------------------====================*/
static void  o___MALLOC__________o () { return; }

char
yexec_mon__pnew         (tPROC **a_new, int a_rpid, char *a_name)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   tPROC      *x_new       = NULL;
   int         x_tries     =    0;
   /*---(header)-------------------------*/
   DEBUG_DEPS   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_DEPS   yLOG_spoint  ("a_new");
   --rce;  if (a_new == NULL) {
      DEBUG_DEPS   yLOG_snote  ("no return pointer");
      DEBUG_DEPS   yLOG_sexitr (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DEPS   yLOG_spoint  (*a_new);
   --rce;  if (*a_new != NULL) {
      DEBUG_DEPS   yLOG_snote  ("return pointer already assigned");
      DEBUG_DEPS   yLOG_sexitr (__FUNCTION__, rce);
      return rce;
   }
   /*---(create)-------------------------*/
   DEBUG_DEPS   yLOG_snote   ("malloc");
   while (x_new == NULL && x_tries < 10) {
      x_new = (tPROC *) malloc (sizeof (tPROC));
      DEBUG_DEPS   yLOG_sint    (sizeof (tPROC));
      DEBUG_DEPS   yLOG_spoint  (x_new);
      ++x_tries;
   }
   DEBUG_DEPS   yLOG_sint    (x_tries);
   --rce;  if (x_new == NULL) {
      DEBUG_DEPS   yLOG_snote  ("FAILED");
      DEBUG_DEPS   yLOG_sexitr (__FUNCTION__, rce);
      return rce;
   }
   /*---(wipe)---------------------------*/
   yexec_mon__pwipe (x_new);
   /*---(proc list)----------------------*/
   DEBUG_DEPS   yLOG_snote   ("list");
   if (p_tail == NULL) {
      p_head         = x_new;
   } else {
      x_new->p_prev  = p_tail;
      p_tail->p_next = x_new;
   }
   p_tail         = x_new;
   ++p_count;
   DEBUG_DEPS   yLOG_sint    (p_count);
   /*---(populate)-----------------------*/
   x_new->rpid    = a_rpid;
   strlcpy (x_new->name, a_name, LEN_TITLE);
   /*---(save back)----------------------*/
   DEBUG_DEPS   yLOG_snote   ("save");
   *a_new = x_new;
   DEBUG_DEPS   yLOG_spoint  (*a_new);
   /*---(complete)-----------------------*/
   DEBUG_DEPS   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char         /*-> destroy a single dependency --------[ leaf   [fp.B42.102.E0]*/ /*-[11.0000.034.5]-*/ /*-[--.---.---.--]-*/
yexec_mon__pfree        (tPROC **a_old)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   tPROC      *x_old       = NULL;
   /*---(header)-------------------------*/
   DEBUG_DEPS   yLOG_senter  (__FUNCTION__);
   /*---(defense: null cell)-------------*/
   DEBUG_DEPS   yLOG_spoint  (a_old);
   --rce;  if (a_old       == NULL) {
      DEBUG_DEPS   yLOG_snote  ("no proc pointer");
      DEBUG_DEPS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DEPS   yLOG_spoint  (*a_old);
   --rce;  if (*a_old      == NULL) {
      DEBUG_DEPS   yLOG_snote  ("proc pointer already empty");
      DEBUG_DEPS   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   x_old = *a_old;
   /*---(remove from dependency list)----*/
   DEBUG_DEPS   yLOG_snote   ("main list");
   if (x_old->p_next != NULL) x_old->p_next->p_prev   = x_old->p_prev;
   else                       p_tail                  = x_old->p_prev;
   if (x_old->p_prev != NULL) x_old->p_prev->p_next   = x_old->p_next;
   else                       p_head                  = x_old->p_next;
   --p_count;
   /*---(free)---------------------------*/
   DEBUG_DEPS   yLOG_spoint  (*a_old);
   free (*a_old);
   *a_old = NULL;
   /*---(complete)-----------------------*/
   DEBUG_DEPS   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
yexec_mon__ppurge       (void)
{
   tPROC      *x_curr      = NULL;
   x_curr = p_head;
   while (x_curr != NULL) {
      yexec_mon__pfree (&x_curr);
      x_curr = p_head;
   }
   return 0;
}

char
yEXEC_cpu_proc          (int a_rpid, char a_cpu, char *a_state, long *a_utime, long *a_stime, char *a_snice)
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
      printf ("%2d  %-10p  %-10.10s\n", c, p, p);
      switch (c) {
      case  2 :
         if (a_state != NULL)  *a_state = p [0];
         break;
      case 13 :
         if (a_utime != NULL)  *a_utime = atol (p);
         break;
      case 14 :
         if (a_stime != NULL)  *a_stime = atol (p);
         break;
      case 17 :
         if (a_snice != NULL)  *a_snice = atoi (p);
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
yEXEC_mem_proc          (int a_rpid, char a_mem, long *a_total, long *a_true, long *a_text, long *a_data, long *a_heap, long *a_stack, long *a_kern, long *a_libs, long *a_empty, char *a_flag)
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
   if (a_total != NULL)  *a_total = 0;
   if (a_true  != NULL)  *a_true  = 0;
   if (a_text  != NULL)  *a_text  = 0;
   if (a_data  != NULL)  *a_data  = 0;
   if (a_heap  != NULL)  *a_heap  = 0;
   if (a_stack != NULL)  *a_stack = 0;
   if (a_kern  != NULL)  *a_kern  = 0;
   if (a_libs  != NULL)  *a_libs  = 0;
   if (a_empty != NULL)  *a_empty = 0;
   if (a_flag  != NULL)  *a_flag  = '-';
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
      if (c % 5 == 0)  printf ("\nline    --full-  --rss--    p  -total-  --true-  --text-  --data-  -stack-  --heap-  --kern-  --libs-    -empty-\n");
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
               if (a_total != NULL)  *a_total += x_full;
               if (a_true  != NULL)  *a_true  += x_full;
               if (a_text  != NULL)  *a_text  += x_full;
               break;
            case 'd' : 
               if (a_total != NULL)  *a_total += x_pvt;
               if (a_true  != NULL)  *a_true  += x_pvt;
               if (a_data  != NULL)  *a_data  += x_pvt;
               if (a_empty != NULL)  *a_empty += x_full - x_pvt;
               break;
            case 'h' : 
               if (a_total != NULL)  *a_total += x_rss;
               if (a_true  != NULL)  *a_true  += x_rss;
               if (a_heap  != NULL)  *a_heap  += x_rss;
               if (a_empty != NULL)  *a_empty += x_empty;
               break;
            case 's' : 
               if (a_total != NULL)  *a_total += x_rss;
               if (a_true  != NULL)  *a_true  += x_rss;
               if (a_stack != NULL)  *a_stack += x_rss;
               if (a_empty != NULL)  *a_empty += x_empty;
               break;
            case 'k' : /* mapped kernel space */
               if (a_total != NULL)  *a_total += x_full;
               if (a_true  != NULL)  *a_true  += x_full;
               if (a_kern  != NULL)  *a_kern  += x_full;
               break;
            case 'o' : 
               if (a_total != NULL)  *a_total += x_full;
               if (a_empty != NULL)  *a_empty += x_full;
               x_empty = 0;
               break;
            case 'l' : 
               if (a_total != NULL)  *a_total += x_pvt;
               if (a_true  != NULL)  *a_true  += x_pvt;
               if (a_libs  != NULL)  *a_libs  += x_pvt;
               if (a_empty != NULL)  *a_empty += x_full - x_pvt;
               break;
            }
            printf ("%4d    %7ld  %7ld    %c  %7ld  %7ld  %7ld  %7ld  %7ld  %7ld  %7ld  %7ld    %7ld\n",
                  x_line, x_full, x_rss, x_part,
                  *a_total, *a_true, *a_text, *a_data, *a_stack,
                  *a_heap, *a_kern, *a_libs, *a_empty);
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
   tPROC      *x_proc      = NULL;
   tLIB       *x_lib       = NULL;
   char        rc          =    0;
   int         c           =    0;
   /*---(preprare)-----------------------*/
   strcpy (unit_answer, "MON              : question not understood");
   /*---(dependency list)----------------*/
   if      (strcmp (a_question, "p_count"  )      == 0) {
      x_proc = p_head; while (x_proc != NULL) { ++x_fore; x_proc = x_proc->p_next; }
      x_proc = p_tail; while (x_proc != NULL) { ++x_back; x_proc = x_proc->p_prev; }
      snprintf (unit_answer, LEN_RECD, "MON p_count      : num=%4d, fore=%4d, back=%4d", p_count, x_fore, x_back);
   }
   else if (strcmp (a_question, "p_list"      )   == 0) {
      snprintf (unit_answer, LEN_RECD, "MON p_list       : num=%4d, head=%-10p, tail=%p", p_count, p_head, p_tail);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}


