/*============================----beg-of-source---============================*/


/*===[[ VERSION ]]========================================*/
/* rapidly evolving version number to aid with visual change confirmation     */
#define   YEXEC_VER_NUM      "1.1b"
#define   YEXEC_VER_TXT      "moved to make_program makefile enhancement"


#define          SUCCESS          0
#define          NOT_READY       -1
#define          FAILED          -2
#define          SKIPPING        -3

#define          MAX_RECD      1000
#define          MAX_FIELD      100

typedef struct cLOCAL tLOCAL;
struct cLOCAL {
   /*---(overall)-----------*/
   char      status;
   char      full  [MAX_RECD];
   /*---(parsing)-----------*/
   char      recd  [MAX_RECD];
};
extern  tLOCAL its;


char         /*--> break string into argv structure ------[ leaf-- [ ------ ]-*/
yEXEC__parse       (const char *a_cstring);

void             /* [------] receive signals ---------------------------------*/
yEXEC__comm        (
      int         a_signal,       /* signal number                            */
      siginfo_t  *a_info,         /* signal detail structure                  */
      void       *a_nada);        /* ignored                                  */

/*============================----end-of-source---============================*/
