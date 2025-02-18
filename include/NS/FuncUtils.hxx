
#ifndef __FUNC_UTILS_HXX__
# define __FUNC_UTILS_HXX__

# include "NSUtilsCF.h"

# ifdef _WIN32
#  include "NS_Win.h"
# endif
# include <stdlib.h>


/* ============================================================================
 * Forward declarations
 */

# ifdef __cplusplus

class TF1;

# else /* !defined(__cplusplus) */

struct TF1;
typedef struct TF1                  TF1;

# endif /* defined(__cplusplus) */


/* ============================================================================
 * Constant/Type declarations
 */

# ifndef FNCULIB_API
#  if defined _MSC_VER && (defined _WIN32 || defined _WIN64)
#   ifdef Build_FuncUtils_Lib
#    define FNCULIB_API  __declspec(dllexport)
#   else
#    define FNCULIB_API  __declspec(dllimport)
#   endif
#  else
#   define FNCULIB_API extern
#  endif
# endif


/* ============================================================================
 * Function prototypes
 */

FNCULIB_API double        NS_PolF                     (double const [], unsigned, double);
FNCULIB_API double        NS_ExpDec                   (double const [], double);

/*
 * Pulse: A * (1 - exp(-(1/tau1)(x - t0)))^p1 * exp(-(1/tau2)(x - t0))
 *
 * aParm[0]: A
 * aParm[1]: t0
 * aParm[2]: tau1
 * aParm[3]: tau2
 * aParm[4]: p1
 */
FNCULIB_API double        NS_Pulse                    (double [], double);
FNCULIB_API double        NS_PulseNorm                (double [], double);
FNCULIB_API double        NS_PulseNorm_m              (double [], double);
FNCULIB_API double        NS_PulseNorm2_m             (double [], double);
FNCULIB_API double        NS_PulseNorm3_m             (double [], double);
FNCULIB_API double        NS_PulseDoubleExpSum        (double [], double);
FNCULIB_API double        NS_PulseDoubleExpSum_m      (double [], double);
FNCULIB_API double        NS_PulseDoubleExpSum2_m     (double [], double);
FNCULIB_API double        NS_PulseDoubleExpSum3_m     (double [], double);

// @todo: fixme - remove or not remove
// # if 1 || defined(__CINT__) || defined(__CLING__) || defined(__ACLIC__)
# ifdef __CEROOT__

/* ============================================================================
 * Constant/Type declarations
 */

struct NS_fitfunc_tab {
	unsigned                        nfuncs;
	TF1                           **funcv;

#  ifdef __cplusplus
	NS_fitfunc_tab() : nfuncs(0), funcv(NULL) {}
	~NS_fitfunc_tab();
#  endif
};


/* ============================================================================
 * Function prototypes
 */

FNCULIB_API double        NS_Ln                       (double);
FNCULIB_API double        NS_TF_ExpDec                (double const *, double const *);
FNCULIB_API double        NS_TF_ExpDecGauss           (double const *, double const *);
FNCULIB_API double        NS_TF_Gauss                 (double const *, double const *);

FNCULIB_API void          NS_FitFuncTabFini           (struct NS_fitfunc_tab *);

FNCULIB_API void          NS_FitFuncTabNewExpDec      (struct NS_fitfunc_tab *, int, float *, float *);
FNCULIB_API void          NS_FitFuncTabNewExpDecGauss (struct NS_fitfunc_tab *, int, float *, float *);
FNCULIB_API void          NS_FitFuncTabNewGauss       (struct NS_fitfunc_tab *, int, float *, float *);

// # endif /* defined(__CINT__) || defined(__CLING__) || defined(__ACLIC__) */
# endif /* defined(__CEROOT__) */

#endif /* !defined(__FUNC_UTILS_HXX__) */
