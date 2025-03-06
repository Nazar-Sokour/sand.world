
#ifndef _NS_DIRENT_H_
# define _NS_DIRENT_H_

# if (defined _WIN32 || defined _WIN64)
#  ifdef __CEROOT__
//#  if (defined __CINT__ || defined __CLING__ || defined __ACLIC__)
#   if (defined Build_CommonUtils_Lib || defined Build_FuncUtils_Lib || defined Build_SpectrumUtils_Lib)
#    include <dirent.h>
#	else
#	 include "NS_dirent.h"
#   endif /* !defined(Build_*Utils_Lib) */
#  else
#   include <dirent.h>
#  endif /* !defined(__CEROOT__) */
# else
#  include <dirent.h>
# endif

#endif /* !defined(_NS_DIRENT_H_) */