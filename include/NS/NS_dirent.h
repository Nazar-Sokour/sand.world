
#ifndef _NS_DIRENT_H_
# define _NS_DIRENT_H_

# if (defined _WIN32 || defined _WIN64)
#  if (defined __CINT__ || defined __CLING__ || defined __ACLIC__)
/* __CINT__ in ROOT5, __CLING__ in ROOT6 */
#	include "root-win/dirent.h"
#  else
#    include <dirent.h>
#  endif
# else
#  include <dirent.h>
# endif

#endif /* !defined(_NS_DIRENT_H_) */
