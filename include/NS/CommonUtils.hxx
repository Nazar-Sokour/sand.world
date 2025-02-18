
#ifndef __COMMON_UTILS_HXX__
# define __COMMON_UTILS_HXX__

# ifdef _WIN32
#  include "NS_Win.h"
# endif

# include <errno.h>
# include <dirent.h>
# include <stdio.h>
# include <string.h>
# include <stdint.h>
	/* intptr_t */
// # include <unistd.h>
# include "NS_err.h"


/* ============================================================================
 * Forward declarations
 */

# ifdef __cplusplus

class TObject;
class TCanvas;
class TF1;
class TFile;
class TGraph;
class TH1;

# else /* !defined(__cplusplus) */

struct TObject;
typedef struct TObject              TObject;
struct TCanvas;
typedef struct TCanvas              TCanvas;
struct TF1;
typedef struct TF1                  TF1;
struct TFile;
typedef struct TFile                TFile;
struct TGraph;
typedef struct TGraph               TGraph;
struct TH1;
typedef struct TH1                  TH1;

# endif /* defined(__cplusplus) */


/* ============================================================================
 * Constant/Type declarations
 */

# ifndef CMNULIB_API
#  if defined _MSC_VER && (defined _WIN32 || defined _WIN64)
#   ifdef Build_CommonUtils_Lib
#    define CMNULIB_API  __declspec(dllexport)
#   else
#    define CMNULIB_API  __declspec(dllimport)
#   endif
#  else
#   define CMNULIB_API extern
#  endif
# endif


# ifdef __cplusplus
#  define NS_defval(x)          = x
# else
#  define NS_defval(x)
# endif

# define NS_COUNTOF(a)         (sizeof(a) / sizeof((a)[0]))
# define NS_PRINTF(opt, ...)    if (opt) printf(__VA_ARGS__)

# define NS_ABSTRACT_TAB_INITSIZE   64
# define NS_FLINESIZE_MAX           256

# define NS_FRA_COLM               ((long)0xF) /* File column count Mask */

# define NS_FRA_SMTD_MASK          ((long)0xF) /* File Scan Method */
# define NS_FRA_SMTD_SHIFT          4
# define NS_FRA_SET_SMTD(x)        ((long)((x) << NS_FRA_SMTD_SHIFT))
# define NS_FRA_SMTD(x)            (((x) >> NS_FRA_SMTD_SHIFT) & NS_FRA_SMTD_MASK)
# define NS_FRA_OVERWRITE          ((long)0x1)
# define NS_FRA_UPDATE             ((long)0x2)

# define TD_UNKNOWN                ((intptr_t)1)
# define TD_VOID                   ((intptr_t)2)
# define TD_CHAR                   ((intptr_t)3)
# define TD_INT                    ((intptr_t)4)
# define TD_LONG                   ((intptr_t)5)
# define TD_FLOAT                  ((intptr_t)6)
# define TD_DOUBLE                 ((intptr_t)7)
# define TD_TOBJECT                ((intptr_t)8)
# define TD_TCANVAS                ((intptr_t)9)
# define TD_TF1                    ((intptr_t)10)
# define TD_TFILE                  ((intptr_t)11)
# define TD_TGRAPH                 ((intptr_t)12)
# define TD_TH1                    ((intptr_t)13)

/* ------------------------------------------------------------------------- */

struct NS_abs_buf {
	unsigned                dynamic: 1,
	                        capacity: 31,
	                        count;
	char                   *base;
};
typedef struct NS_abs_buf           NS_abs_buf;

struct NS_abstract_tab {
	unsigned                capacity, count;
	char                   *xv, *yv;
};
typedef struct NS_abstract_tab      NS_abstract_tab;

union NS_absptr { /* the most widespread types */
	intptr_t  ii;
	void     *vp;
	char     *sp;
	int      *ip;
	long     *lp;
	float    *fp;
	double   *dp;
	TObject  *objp;
	TCanvas  *tcp;
	TF1      *tf1p;
	TFile    *tfp;
	TH1      *th1p;
	TGraph   *tgrp;
};

struct NS_absvar {
	intptr_t type;
	union NS_absptr p;
};
typedef struct NS_absvar            NS_absvar;

struct pathio {
	NS_abs_buf in, out;
};

struct file_name {
	struct pathio path;
	char fname[64], fprefix[32], froot[32], fsuffix[32]; // prefix + root + suffix + 1 should be not longer then 64
	int namelen, prefixlen, rootlen, suffixlen; // rootlen = -1: means rootlen is variable and equal to number of digit
};


/* ============================================================================
 * Function prototypes
 */

 /* 
  * Common Utils
  */

CMNULIB_API bool          NS_IsBlank                  (int);
CMNULIB_API char         *NS_SkipBlanks               (char *);
CMNULIB_API int           NS_Readline                 (NS_abs_buf *, FILE *);
CMNULIB_API void          NS_StrTrim                  (NS_abs_buf *);

  /* Buf Utils */
CMNULIB_API int           NS_AbsBufInit               (NS_abs_buf *);
CMNULIB_API int           NS_AbsBufInitExtend         (NS_abs_buf *, unsigned, char *, unsigned, unsigned);
CMNULIB_API int           NS_AbsBufFini               (NS_abs_buf *);
CMNULIB_API int           NS_AbsBufExtend             (NS_abs_buf *, unsigned aNewCapacitySize);
CMNULIB_API int           NS_AbsBufExtend2            (NS_abs_buf *, unsigned aNewCapacityElem, unsigned anElemSize);
CMNULIB_API int           NS_AbsBufGrow               (NS_abs_buf *);
CMNULIB_API int           NS_AbsBufReserve            (NS_abs_buf *, unsigned anXtraSize);
CMNULIB_API int           NS_AbsBufReserve2           (NS_abs_buf *, unsigned anXtraSizeElem, unsigned anElemSize);

CMNULIB_API int           NS_AbsBufAppend             (NS_abs_buf *, void const *, unsigned aSize);
CMNULIB_API int           NS_AbsBufAppend2            (NS_abs_buf *, void const *, unsigned aNumElems, unsigned anElemSize);
CMNULIB_API char         *NS_AbsBufAsCStr             (NS_abs_buf *);
CMNULIB_API int           NS_AbsBufInsert             (NS_abs_buf *, unsigned aPos, void const *, unsigned aSize);
CMNULIB_API int           NS_AbsBufInsert2            (NS_abs_buf *, unsigned aPosElem, void const *, unsigned aNumElems, unsigned anElemSize);
CMNULIB_API void          NS_AbsBufRemove             (NS_abs_buf *, unsigned aPos, unsigned aSize);
CMNULIB_API void          NS_AbsBufRemove2            (NS_abs_buf *, unsigned aPosElem, unsigned anElemSize);


template < typename _Ty >
unsigned                  NS_AbsBufCount              (NS_abs_buf *);
template < typename _Ty >
unsigned                  NS_AbsBufCount              (NS_abs_buf const *);

template < typename _Ty >
_Ty                      *NS_AbsBufEndPtr             (NS_abs_buf *);
template < typename _Ty >
_Ty const                *NS_AbsBufEndPtr             (NS_abs_buf const *);

template < typename _Ty >
_Ty                      *NS_AbsBufPtr                (NS_abs_buf *, unsigned);
template < typename _Ty >
_Ty const                *NS_AbsBufPtr                (NS_abs_buf const *, unsigned);

template < typename _Ty >
_Ty                       NS_AbsBufGetVal             (NS_abs_buf const *, unsigned);
template < typename _Ty >
_Ty                       NS_AbsBufGetVal             (NS_abs_buf *, unsigned);

template < typename _Ty >
int                       NS_AbsBufPushBack           (NS_abs_buf *, _Ty const &);

template < typename _Ty >
int                       NS_AbsBufInsert             (NS_abs_buf *, _Ty const &, unsigned);

template < typename _Ty >
void                      NS_AbsBufPopBack            (NS_abs_buf *);

template < typename _Ty >
void                      NS_AbsBufRemove             (NS_abs_buf *, unsigned);

  /* Tab Utils */
CMNULIB_API int           NS_AbstractTabInit          (NS_abstract_tab *);
CMNULIB_API int           NS_AbstractTabFini          (NS_abstract_tab *);
CMNULIB_API NS_abstract_tab
                         *NS_AbstractTabCopy          (NS_abstract_tab const *);
CMNULIB_API int           NS_AbstractTabExtend        (NS_abstract_tab *, unsigned aNewCapacity);
CMNULIB_API int           NS_AbstractTabExtend2       (NS_abstract_tab *, unsigned aNewCapacityCount, unsigned anElementSize);
CMNULIB_API int           NS_AbstractTabGrow          (NS_abstract_tab *);
CMNULIB_API void          NS_AbstractTabRemove        (NS_abstract_tab *aTab, unsigned aPos, unsigned anElemSize);
CMNULIB_API int           NS_AbstractTabReserve       (NS_abstract_tab *, unsigned aXtraSize);
CMNULIB_API int           NS_AbstractTabReserve2      (NS_abstract_tab *, unsigned aXtraSize, unsigned anElementSize);
template < typename _Ty >
void                      NS_AbstractTabUniteElements (NS_abstract_tab *, unsigned);
template < typename _Ty >
int                       NS_AbstractTabUpdate        (NS_abstract_tab *, NS_abstract_tab const *);

CMNULIB_API double        NS_GetXforY                 (NS_abstract_tab const *, double, double NS_defval(0), double NS_defval(0));
CMNULIB_API int           NS_GetSeqNum                (NS_abstract_tab const *, double, unsigned NS_defval(0), unsigned NS_defval(0),
                                                       double NS_defval(0), int NS_defval(0));


template < typename _Ty >
unsigned                  NS_AbstractTabCount         (NS_abstract_tab *);
template < typename _Ty >
unsigned                  NS_AbstractTabCount         (NS_abstract_tab const *);

template < typename _Ty >
_Ty                      *NS_AbstractTabEndPtrX       (NS_abstract_tab *);
template < typename _Ty >
_Ty const                *NS_AbstractTabEndPtrX       (NS_abstract_tab const *);

template < typename _Ty >
_Ty                      *NS_AbstractTabEndPtrY       (NS_abstract_tab *);
template < typename _Ty >
_Ty const                *NS_AbstractTabEndPtrY       (NS_abstract_tab const *);

template < typename _Ty >
_Ty                      *NS_AbstractTabPtrX          (NS_abstract_tab *, unsigned);
template < typename _Ty >
_Ty const                *NS_AbstractTabPtrX          (NS_abstract_tab const *, unsigned);

template < typename _Ty >
_Ty                      *NS_AbstractTabPtrY          (NS_abstract_tab *, unsigned);
template < typename _Ty >
_Ty const                *NS_AbstractTabPtrY          (NS_abstract_tab const *, unsigned);

template < typename _Ty >
_Ty                       NS_AbstractTabGetValX       (NS_abstract_tab const *, unsigned);
template < typename _Ty >
_Ty                       NS_AbstractTabGetValX       (NS_abstract_tab *, unsigned);

template < typename _Ty >
_Ty                       NS_AbstractTabGetValY       (NS_abstract_tab const *, unsigned);
template < typename _Ty >
_Ty                       NS_AbstractTabGetValY       (NS_abstract_tab *, unsigned);

template < typename _Ty >
int                       NS_AbstractTabPushBack      (NS_abstract_tab *, _Ty const &, _Ty const &);

template < typename _Ty >
int                       NS_AbstractTabInsert        (NS_abstract_tab *, _Ty const &, _Ty const &, unsigned);

template < typename _Ty >
void                      NS_AbstractTabPopBack       (NS_abstract_tab *);

template < typename _Ty >
void                      NS_AbstractTabRemove        (NS_abstract_tab *, unsigned);

template < typename _Ty >
void                      NS_AbstractTabSetValYforPos (NS_abstract_tab *, _Ty &, unsigned);

# include "CommonUtils_tpl.hxx" /* template function definitions */

 /*
  * Common File Utils
  */

CMNULIB_API int           NS_WriteAbsBuf              (char const *, NS_abs_buf *, int);
CMNULIB_API int           NS_FWriteAbs                (FILE *, NS_abs_buf *, int);

template < typename _Ty >
int                       NS_ReadAbstractFile         (char const *, NS_abstract_tab *, long NS_defval(0));
template < typename _Ty >
int                       NS_FReadAbstract            (FILE *, NS_abstract_tab *, long NS_defval(0));
//
// Options: 0xZY
// Y - column's count in file
// Z - scan method (update or overwrite, default - overwrite)
//
template < typename _Ty >
int                       NS_WriteAbstractFile        (char const *, NS_abstract_tab *, short NS_defval(0));
template < typename _Ty >
int                       NS_FWriteAbstract           (FILE *, NS_abstract_tab *, short NS_defval(0));

# include "CommonFileUtils_tpl.hxx" /* template function definitions */

/*
 * 'i' : int
 * 'l' : long
 * 'f' : float
 * 'd' : double
 */
CMNULIB_API int           NS_FNumByKey                (FILE *, char const *, unsigned, unsigned, void *, int);
# define NS_FIntByKey(stm, key, n, idx, rp)       NS_FNumByKey(stm, key, n, idx, rp, 'i')
# define NS_FLongByKey(stm, key, n, idx, rp)      NS_FNumByKey(stm, key, n, idx, rp, 'l')
# define NS_FFloatByKey(stm, key, n, idx, rp)     NS_FNumByKey(stm, key, n, idx, rp, 'f')
# define NS_FDoubleByKey(stm, key, n, idx, rp)    NS_FNumByKey(stm, key, n, idx, rp, 'd')

CMNULIB_API void          NS_FileNameInit             (struct file_name *);
CMNULIB_API void          NS_FileNameSetPrefix        (struct file_name *, char const *);
CMNULIB_API void          NS_FileNameSetRootWithNumber(struct file_name *, int); // number and rootlen should be setted
CMNULIB_API void          NS_FileNameSetSuffix        (struct file_name *, char const *);
CMNULIB_API int           NS_FindFromDir              (char const *, DIR *);
CMNULIB_API int           NS_FormFileNameWithNumber   (struct file_name *, int); // fprefix and prefixlen should be setted before
CMNULIB_API int           NS_FormFileNameFromDir      (struct file_name *, DIR *); // fprefix and prefixlen should be setted before
CMNULIB_API void          NS_PathioInit               (struct pathio *);
CMNULIB_API void          NS_PathioFini               (struct pathio *);
CMNULIB_API int           NS_ReadNextFromDir          (char *, DIR *);

#endif /* !defined(__COMMON_UTILS_HXX__) */
