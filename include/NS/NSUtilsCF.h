
#ifndef __NS_UTILS_CF_H__
# define __NS_UTILS_CF_H__

/* comment the next define if you don't have or need the root data analysis framework */
# define __CEROOT__

# ifdef __CEROOT__
// # if defined(__CINT__) || defined(__CLING__) || defined(__ACLIC__)
// with "root" subdir (include/root lib/root etc)
#  define _ROOT_INCLUDE_PREFIX      1
#  define _ROOT_LIB_PREFIX          1
// # endif /* defined(__CINT__) || defined(__CLING__) || defined(__ACLIC__) */
# endif

// # define WITH_EXPLICIT_HDF5_DIR
// # define _HDF5_INCLUDE_PREFIX       1
// # define _HDF5_LIB_PREFIX           1

// # define WITH_EXPLICIT_GSL_DIR
// # define _GSL_INCLUDE_PREFIX        1
// # define _GSL_LIB_PREFIX            1

// # define WITH_EXPLICIT_FFTW3_DIR
// # define _FFTW3_INCLUDE_PREFIX      1
// # define _FFTW3_LIB_PREFIX          1

#endif /* !defined(__NS_UTILS_CF_H__) */
