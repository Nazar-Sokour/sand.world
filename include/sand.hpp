
#ifndef __NS_SAND_HXX__
# define __NS_SAND_HXX__

# include "NS/CommonUtils.hxx"
# include "NS/SpectrumUtils.hxx"


# define PATH_MIN     64
# if (defined _WIN32 || defined _WIN64)
#  define ENV_HOME "USERPROFILE"
# else
#  define ENV_HOME "HOME"
# endif

# if (defined _WIN32 || defined _WIN64)
#  include <stdarg.h>
#  include <io.h>

#  define access(...) _access(__VA_ARGS__)
#  ifndef R_OK
#   define R_OK       4
#  endif
# endif


/*
 * bit order (from LSb to MSb)
 *  1. interactive mode
 *  2. soft mode (not implemented yet)
 *  3. verbose mode (under developing)
 *  4. reserve
 *  5. input file path
 *  6. input file name
 *  7. output file path
 *  8. output file name
 *  9. calibration coefficients file path
 * 10. calibration coefficients file name
 * 11. directory of input and calibration coefficients files
 * 12. number of columns in input file
 * 13. number of columns in output file
 * 14. initial calibration coefficient from command line
 * 15. final calibration coefficient from command line
 */
# define OPT_INTERACTVE_MODE              0
# define OPT_SOFT_MODE                    1
# define OPT_VERBOSE_MODE                 2
# define OPT_MODIFIER_BASE                5
# define OPT_INPUT_PATH                  (OPT_MODIFIER_BASE - 1)
# define OPT_INPUT_NAME                  (OPT_MODIFIER_BASE)
# define OPT_OUTPUT_PATH                 (OPT_MODIFIER_BASE + 1)
# define OPT_OUTPUT_NAME                 (OPT_MODIFIER_BASE + 2)
# define OPT_CC_PATH                     (OPT_MODIFIER_BASE + 3)
# define OPT_CC_NAME                     (OPT_MODIFIER_BASE + 4)
# define OPT_DIRECTORY                   (OPT_MODIFIER_BASE + 5)
# define OPT_INPUT_NCOLS                 (OPT_MODIFIER_BASE + 6)
# define OPT_OUTPUT_NCOLS                (OPT_MODIFIER_BASE + 7)
# define OPT_CC_INITIAL                  (OPT_MODIFIER_BASE + 8)
# define OPT_CC_FINALE                   (OPT_MODIFIER_BASE + 9)

# define OPT_MASK1_BITS                  ((ushort)1)
# define OPT_MASK2_BITS                  ((ushort)3)
# define OPT_FLAG1(v, shift)             ((ushort)(((v) & OPT_MASK1_BITS) << (shift)))
# define OPT_FLAG2(v, shift)             ((ushort)(((v) & OPT_MASK2_BITS) << (shift)))
# define OPT_DECODE_FLAG(mask, shift)    ((ushort)((programBitMap >> (shift)) & mask))
# define OPT_DECODE_FLAG1(shift)         ((ushort)((programBitMap >> (shift)) & OPT_MASK1_BITS))
# define OPT_DECODE_FLAG2(shift)         ((ushort)((programBitMap >> (shift)) & OPT_MASK2_BITS))

# define OPT_MASK_ALLNAME                (OPT_FLAG1(1, OPT_INPUT_NAME) | OPT_FLAG1(1, OPT_OUTPUT_NAME) | OPT_FLAG1(1, OPT_CC_NAME))


# define ER_ERRNO                         -1
# define ER_INVALID_ARGUMENT              -2
# define ER_MODIFIER_REUSE                -3
# define ER_NO_ARGUMENT                   -4
# define ER_INVALID_INPUT                 -5
# define ER_IO                            -6


/* ------------------------------------------------------------------------- */

struct sand_working {
	struct pathio io;
	NS_abs_buf cc;
	NS_abs_buf dir;
	NS_spectrum ispectrum, ospectrum;
	int iNcols, oNcols;
	char istack[PATH_MIN], ostack[PATH_MIN], ccstack[PATH_MIN], dirstack[PATH_MIN];
};

struct sand_err {
	int errNo;
	char buf[PATH_MIN];
};


int             ParseCommandLine              (int, char **);
int             ParseProgramBitMap            ();
int             PrintErrorAndExit             ();
int             ReadFunction                  ();
int             ReadCalibrationCoefficients   (char const *);
int             WriteFunction                 ();

#endif /* !defined(__NS_SAND_HXX__) */
