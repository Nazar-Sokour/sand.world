
#ifndef __SPECTRUM_UTILS_HXX__
# define __SPECTRUM_UTILS_HXX__

# include "NSUtilsCF.h"

# ifdef _WIN32
#  include "NS_Win.h"
# endif

# include "CommonUtils.hxx"


/* ============================================================================
 * Constant/Type declarations
 */

# ifndef SPEULIB_API
#  if defined _MSC_VER && (defined _WIN32 || defined _WIN64)
#   ifdef Build_SpectrumUtils_Lib
#    define SPEULIB_API  __declspec(dllexport)
#   else
#    define SPEULIB_API  __declspec(dllimport)
#   endif
#  else
#   define SPEULIB_API extern
#  endif
# endif


// # define NS_SPECTRUM_TAB_INITSIZE   64
# define NS_SPECTRUM_BGRD_PARM_MAX  8

struct NS_spectrum {
	NS_abstract_tab data;
	double time; // in seconds
	NS_abs_buf calibcoef; // calibration coefficients
};
typedef struct NS_spectrum          NS_spectrum;


/* ============================================================================
 * Function prototypes
 */

SPEULIB_API int           NS_SpectrumInit             (NS_spectrum *);
SPEULIB_API int           NS_SpectrumFini             (NS_spectrum *);
SPEULIB_API int           NS_ReadCalibrationCoeffiecients
                                                      (char const *, NS_abs_buf *, NS_abs_buf *);


template < typename _Ty >
void                      NS_SpectrumNormalization    (NS_spectrum *, double, double);  // no norma, norma
template < typename _Ty >
void                      NS_SpectrumTimeNormalization(NS_spectrum *, double);
template < typename _Ty >
int                       NS_SpectrumSAND             (NS_spectrum *, NS_spectrum *);

# include "SpectrumUtils_tpl.hxx" /* template function definitions */

# ifdef __CEROOT__

/* ============================================================================
 * Forward declarations
 */

#  ifdef __cplusplus

class TH1;

#  else /* !defined(__cplusplus) */

struct TH1;
typedef struct TH1                  TH1;

#  endif /* defined(__cplusplus) */


/* ============================================================================
 * Constant/Type declarations
 */

struct NS_spectrum_root {
	TH1 *data;
	NS_abs_buf calibcoef;
	char type;
};
typedef struct NS_spectrum_root     NS_spectrum_root;

struct NS_analyse_calib_source_parm {
	struct NS_fitfunc_tab               fitfuncTab;
	float                              *peaksCalib;
	unsigned                            npeaks;
	double                              spectreRangeMax;
	double                              calibcurveRangeMax;
};


/* ============================================================================
 * Function prototypes
 */

SPEULIB_API int           NS_SpectrumRootInit         (NS_spectrum_root *);
SPEULIB_API int           NS_SpectrumRootFini         (NS_spectrum_root *);
SPEULIB_API int           NS_SpectrumSANDRoot         (NS_spectrum_root const *, NS_spectrum_root *);
// int                         NS_FitUserFunc              (TH1F *, struct NS_abstract_tab *);
#  if 0
int                         NS_AnalyseCalibSource       (char const *, char const *, struct NS_analyse_calib_source_parm, 
                                                         float *, char const *, long NS_defval(0)); // (last - 1) arg - unit ("MeV", "keV" etc)
#  endif
# endif /* defined(__CEROOT__) */

#endif /* !defined(__SPECTRUM_UTILS_HXX__) */
