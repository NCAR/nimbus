/********************************************************************/
/*                                                                  */
/*                         gegg.h                                   */
/*      Revision  1.0                                    5/3/93     */
/*                                                                  */
/*         Header file containing type definitions  and             */
/*         constant definitions for the gegg.c routine.             */
/*                                                                  */
/********************************************************************/

#define TRUE            1
#define FALSE           0
#define ON              1
#define OFF             0
#define NULLP           (char *) 0
#define USI             unsigned short int
#define ESC             27 
#define RBYCP           0.2859

#define	NULLP		(char *) 0

#define INTA00          0x20            /* 8259 int ctrlr adr */
#define INTA01          0x21
#define INTB00          0xa0
#define INTB01          0xa1
#define RTC_PORT        0x70
#define RA              0x0a
#define RB              0x0b
#define RC              0x0c
#define INTMSK_DOSCLK   0x01            /* HW IRQ int 0 mask */
#define INTMSK_CLK      0x01            /* HW IRQ int 8 mask */
#define INTNUM_CLK      0x70            /* For IRQ 8 */
#define EOI             0x20            /* End of intrpt byte */

#define ANLG_RESOL      12                      /* Sci Solns board */
#define NANALOG         6                      /* Num analog channels */
#define RAW_ANALOG      6
#define ENG_ANALOG      6
#define TTBUFSIZ        32


#define SVOLTWOD        1.0                     /* Probe constants */


#define LMBASE          0x710                   /* Labmaster ports */
#define PORTA           0x71c
#define CTRLP           0x71f

#define F1      59                      /* kbd extended codes */
#define F2      60
#define F3      61
#define F4      62
#define F5      63
#define F6      64
#define F7      65
#define F8      66
#define F9      67
#define F10     68
#define PGUP    73
#define PGDN    81
#define INS     82

/* eggin structure - parameters passed to gegg.c routine */
typedef struct {
  double                vout6;     /* voltage from tc3 */
  double                vout7;     /* voltage from tc4 */
  double                vout8;     /* voltage from tc5 */
  double                vout9;     /* voltage from ad590 ref3 */
  double                vout10;    /* voltage from ad590 ref4 */
  double                vout11;    /* voltage from ad590 ref5 */
  double                vout12;    /* voltage from Egg angle of attack */
  double                vout13;    /* voltage from Egg ang of sideslip */
  double                vout14;    /* voltage from Egg total press */
  double                delp;      /* dynamic press in mb from SEA data sytem */
  double                pmb;       /* static press in mb from SEA data system */
  double                atb;       /* Rosemnt temp from Sea data system */
  double                trf;       /* RFT from SEA data system */
} eggin_;

/* eggout structure - parameters returned from gegg.c routine */
typedef struct {
  double                thmcp3;    /* tc3 temp in deg C */
  double                thmcp4;    /* tc4 temp in deg C */
  double                thmcp5;    /* tc5 temp in deg C */
  double                wg;        /* Vert gust vel in m/s */
  double                vg;        /* Lateral gust vel in m/s */
  double                ug;        /* Longit gust vel in m/s */
} eggout_;



/*----------------------------------------------------------------------*/
/*                      Hardware Type Definitions.                      */
/*----------------------------------------------------------------------*/

        /*  Scientific Solutions  HW Structure */
typedef struct {
  unsigned short        active[NANALOG];
  unsigned short        gain[NANALOG];
  float                 slope[NANALOG];
  float                 offset[NANALOG];
  char                  tag[NANALOG] [26];
  char                  units[NANALOG] [12];
  unsigned short        spare[NANALOG] [4];
} ssol_hw_;

/*----------------------------------------------------------------------*/
/*               Data Structure Typedefs                                */
/*----------------------------------------------------------------------*/


        /* Acquisition Buffer */
typedef struct {
  unsigned short                   tc3;              /* 1st 48 chnls analog */
  unsigned short                   tc4;
  unsigned short                   tc5;
  unsigned short                   ref3;
  unsigned short                   ref4;
  unsigned short                   ref5;
  unsigned short                   adif;
  unsigned short                   bdif;
  unsigned short                   qcp;
} aqnbuf_;

        /* Engineering units buffer */
typedef struct {
  float                 tc3;
  float                 tc4;
  float                 tc5;            /* Derived from rosemnt icing probe */
  float                 ref3;
  float                 ref4;
  float                 ref5;
  float                 adif;
  float                 bdif;
  float                 qcp;
} engbuf_;

        /* Run time structure */
typedef struct {
  unsigned short                   day;
  unsigned short                   mon;
  unsigned short                   year;
  unsigned short                   hour;
  unsigned short                   min;
  unsigned short                   sec;
  unsigned short                   msec;
  unsigned short                   tick;
  char 					              filnam[48];    /* Record file name */
  unsigned short                   aqnflg;
  unsigned short                   dskflg;
  unsigned short                   dmaflg;
  unsigned short                   recflg;
  unsigned short                   runflg;
  unsigned short                   deice;
  unsigned short                   exitflg;
  unsigned short                   probea_full_int;        /* For debug */
  unsigned short                   probea_ovld_end_int;
  unsigned short                   probea_ovld_int;
  unsigned short                   probea_stat;
} runtim_;

        /* Time Tag Buffer Structure */
typedef struct {
  unsigned short                   hour;
  unsigned short                   min;
  unsigned short                   sec;
  unsigned short                   msec;
} ttag_buf_;
