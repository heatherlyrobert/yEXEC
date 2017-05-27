/*============================----beg-of-source---============================*/

#include   "yEXEC.h"
#include   "yEXEC_priv.h"



/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;};

#define       LEN_TEXT  2000
char          unit_answer [ LEN_TEXT ];

char*      /*----: provide grey-box insight to the unit test -----------------*/
yEXEC_greybox      (char *a_question, int a_num)
{
   /*---(locals)-------------------------*/
   int       x_fore = 0, x_back = 0;   /* counts to verify doubly-linked list */
   /*> tEXEC    *x_exec    = NULL;                                                    <*/
   /*---(exec dll verification)----------*/
   /*> if        (strncmp(a_question, "exec_ends"    , 20)   == 0) {                                          <* 
    *>    snprintf(unit_answer, LEN_TEXT, "Exec Endpoints   : %3d %10p %10p", nexec, ehead, etail);           <* 
    *> } else if (strncmp(a_question, "exec_count"   , 20)   == 0) {                                          <* 
    *>    x_exec = ehead; while (x_exec != NULL) { ++x_fore; x_exec = x_exec->enext; }                        <* 
    *>    x_exec = etail; while (x_exec != NULL) { ++x_back; x_exec = x_exec->eprev; }                        <* 
    *>    snprintf(unit_answer, LEN_TEXT, "Exec Link Counts : n=%4d, f=%4d, b=%4d", nexec, x_fore, x_back);   <* 
    *> }                                                                                                      <*/
   /*---(locals)-------------------------*/
   /*> else {                                                                                         <* 
    *>    snprintf(unit_answer, LEN_TEXT, "unknown          : unit_accessor question not defined");   <* 
    *> }                                                                                              <*/
   return unit_answer;
}


/*============================----end-of-source---============================*/
