
#ifndef __COMMON_FILE_UTILS_TPL_HXX__
# define __COMMON_FILE_UTILS_TPL_HXX__

# include <errno.h>
# include <stdio.h>
# include <string.h>
# include "NS_err.h"


template < typename _Ty >
int NS_FReadAbstract(FILE *aStm, NS_abstract_tab *aTab, long Options)
{
	int sts;
	unsigned nrecords;
	int ncols;
	char const *sfmt;

	ncols = NS_FRA_COLM & Options;
	switch (ncols) {
	case 1: sfmt = "%lf";         break;
	case 2: sfmt = "%lf %lf";     break;
	case 3: sfmt = "%lf %lf %lf"; break;
	default:
		sts = -EINVAL;
		goto L_ret_from_routine;
	}

	sts = 0;
	if (NS_FRA_OVERWRITE == NS_FRA_SMTD(Options))
		aTab->count = 0;
	for (nrecords = 0; ; nrecords++) {
		int cnt;
		double v[3];

		if (aTab->count < nrecords) {
			goto L_error_io;
			/* NOTREACHED */
		}
		if (aTab->capacity <= aTab->count) {
			sts = NS_AbstractTabGrow(aTab);
			if (sts < 0) {
				break;
				/* NOTREACHED */
			}
		}
		cnt = fscanf(aStm, sfmt, &v[0], &v[1], &v[2]);
		if (ncols != cnt) {
			if (EOF == cnt) {
			// if (0 == cnt || EOF == cnt) {
				goto L_test_eof;
				/* NOTREACHED */
			}
			goto L_error_io;
			/* NOTREACHED */
		}
		if (NS_FRA_UPDATE == NS_FRA_SMTD(Options) && 1 < ncols) {
			if (v[0] != NS_AbstractTabGetValX< _Ty >(aTab, nrecords)) {
				goto L_error_io;
				/* NOTREACHED */
			}
			*NS_AbstractTabPtrY< _Ty >(aTab, nrecords) += v[ncols - 1];
		}
		else
			NS_AbstractTabPushBack< _Ty >(aTab, 1 == ncols ? 0 : v[0], v[ncols - 1]);
	}
 L_ret_from_routine:
	return (sts);
	/* NOTREACHED */

 L_error_io:
	if (ferror(aStm))
		sts = -errno;
	else
		sts = -ENODATA;     // no data available
	goto L_ret_from_routine;
	/* NOTREACHED */

 L_test_eof:
	if (feof(aStm))
		goto L_ret_from_routine;
	goto L_error_io;
}

template < typename _Ty >
int NS_FWriteAbstract(FILE *aStm, NS_abstract_tab *aTab, short anOption)
{
	int sts;

	sts = 0;
	for (unsigned count = 0; count < NS_AbstractTabCount< _Ty >(aTab); count++) {
		switch (NS_FRA_COLM & anOption) {
		case 1:
			fprintf(aStm, "%16.4lf\n", NS_AbstractTabGetValY< _Ty >(aTab, count));
			break;
		case 2:
			fprintf(aStm, "%16.4lf%16.4lf\n", NS_AbstractTabGetValX< _Ty >(aTab, count),
			                                  NS_AbstractTabGetValY< _Ty >(aTab, count));
		default:
			break;
		}
	}
 	fprintf(aStm, "\n");
	return (sts);
}

template < typename _Ty >
int NS_ReadAbstractFile(char const *aPath, NS_abstract_tab *aTab, long Options)
{
	int sts;
	FILE *stm;

	stm = fopen(aPath, "r");
	if (NULL != stm) {
		sts = NS_FReadAbstract< _Ty >(stm, aTab, Options);
		if (sts < 0)
			warnx("Error in NS_FReadAbstract(): '%s': %s\n", aPath, strerror(-sts));
		fclose(stm);
	}
	else {
		char strb[128];
		sts = -errno;
		strcpy(strb, "Error in NS_ReadAbstractFile(): ");
		strcat(strb, aPath);
		perror(strb);
	}
	return (sts);
}

template < typename _Ty >
int NS_WriteAbstractFile(char const *aPath, NS_abstract_tab *aTab, short anOption)
{
	int sts;
	FILE *stm;

	stm = fopen(aPath, "w");
	if (NULL != stm) {
		sts = NS_FWriteAbstract< _Ty >(stm, aTab, anOption);
		fclose(stm);
	}
	else
		sts = -errno;
	return (sts);
}

#endif /* !defined(__COMMON_FILE_UTILS_TPL_HXX__) */
