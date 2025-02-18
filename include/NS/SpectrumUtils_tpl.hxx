
#ifndef __SPECTRUM_UTILS_TPL_HXX__
# define __SPECTRUM_UTILS_TPL_HXX__

# include "FuncUtils.hxx"

# include <stdio.h>
# include <stdlib.h>


template < typename _Ty >
int NS_SpectrumSAND(NS_spectrum *aSpectrumOld, NS_spectrum *aSpectrumNew)
{
	int sts;
	_Ty *xbaseOld, *ybaseOld, *xbaseNew, *ybaseNew; 
	double *ccOld, *ccNew;
	unsigned chnOld, chnNew;

	ccOld = (double *)(aSpectrumOld->calibcoef.base);
	ccNew = (double *)(aSpectrumNew->calibcoef.base);

	/* push upper value of last spectrum channel needed for recalibration */
	NS_AbstractTabPushBack< _Ty >(&aSpectrumOld->data,
	                              NS_PolF(ccOld, NS_AbsBufCount< double >(&aSpectrumOld->calibcoef) - 1, NS_AbstractTabCount< _Ty >(&aSpectrumOld->data)),
	                              0);
	aSpectrumOld->data.count -= sizeof(_Ty);

	xbaseOld = (_Ty *)aSpectrumOld->data.xv;
	ybaseOld = (_Ty *)aSpectrumOld->data.yv;
	sts = NS_AbstractTabExtend2(&aSpectrumNew->data, (NS_AbstractTabCount< _Ty >(&aSpectrumOld->data) + 1), sizeof(_Ty));
	if (sts < 0) {
		goto L_ret_from_routine;
		/* NOTREACHED */
	}
	xbaseNew = (_Ty *)aSpectrumNew->data.xv;
	ybaseNew = (_Ty *)aSpectrumNew->data.yv;

	for (chnNew = 0; chnNew < NS_AbstractTabCount< _Ty >(&aSpectrumOld->data) + 1; chnNew++) {
		xbaseNew[chnNew] = NS_PolF(ccNew, NS_AbsBufCount< double >(&aSpectrumNew->calibcoef) - 1, chnNew);   // new calibration
		ybaseNew[chnNew] = 0;
	}
	aSpectrumNew->data.count = aSpectrumOld->data.count;

	sts = 0;
	if ( xbaseOld[NS_AbstractTabCount< _Ty >(&aSpectrumOld->data)] <= xbaseNew[0] ||
	     xbaseNew[NS_AbstractTabCount< _Ty >(&aSpectrumOld->data)] <= xbaseOld[0] ) {
		printf("%s\n", "Error in NS_SpectrumSAND: old calibration and new calibration don't intersect.");
		printf("\txbaseOld first = %lf, xbaseOld last = %lf, xbaseNew first = %lf, xbaseNew last = %lf\n",
		        xbaseOld[0], xbaseOld[NS_AbstractTabCount< _Ty >(&aSpectrumOld->data)], xbaseNew[0], xbaseNew[NS_AbstractTabCount< _Ty >(&aSpectrumOld->data)]);
		sts = -ERANGE;
		goto L_ret_from_routine;
		/* NOTREACHED */
	}

	chnOld = 0, chnNew = 0;
	/* skip first old/new channels that don't intersect with new/old channels */
	while (chnOld < NS_AbstractTabCount< _Ty >(&aSpectrumOld->data) && xbaseOld[chnOld + 1] <= xbaseNew[chnNew]) { chnOld++; }
	while (chnNew < NS_AbstractTabCount< _Ty >(&aSpectrumOld->data) && xbaseNew[chnNew + 1] <= xbaseOld[chnOld]) { chnNew++; }
	for (; chnNew < NS_AbstractTabCount< _Ty >(&aSpectrumOld->data) && chnOld < NS_AbstractTabCount< _Ty >(&aSpectrumOld->data);) {
		if (xbaseNew[chnNew] < xbaseOld[chnOld]) {
			while (xbaseOld[chnOld + 1] <= xbaseNew[chnNew + 1]) {
				ybaseNew[chnNew] += ybaseOld[chnOld];
				if (NS_AbstractTabCount< _Ty >(&aSpectrumOld->data) <= ++chnOld)
					goto L_ret_from_routine;
					/* NOTREACHED */
			}
			ybaseNew[chnNew] += ybaseOld[chnOld] * (xbaseNew[chnNew + 1] - xbaseOld[chnOld]) / (xbaseOld[chnOld + 1] - xbaseOld[chnOld]);
			chnNew++;
		}
		else {
			while (xbaseNew[chnNew + 1] < xbaseOld[chnOld + 1]) {
				ybaseNew[chnNew] += ybaseOld[chnOld] * (xbaseNew[chnNew + 1] - xbaseNew[chnNew]) / (xbaseOld[chnOld + 1] - xbaseOld[chnOld]);
				if (NS_AbstractTabCount< _Ty >(&aSpectrumOld->data) <= ++chnNew)
					goto L_ret_from_routine;
					/* NOTREACHED */
			}
			ybaseNew[chnNew] += ybaseOld[chnOld] * (xbaseOld[chnOld + 1] - xbaseNew[chnNew]) / (xbaseOld[chnOld + 1] - xbaseOld[chnOld]);
			chnOld++;
			xbaseNew[chnNew + 1] <= xbaseOld[chnOld] ? chnNew++ : 0;
		}
	}
 L_ret_from_routine:
	return (sts);
}

template < typename _Ty >
void NS_SpectrumNormalization(NS_spectrum *aSpectre, double aNoNorma, double aNorma)
{	
	double normafactor = aNorma / aNoNorma;

	for (_Ty *itr = NS_AbstractTabPtrY< _Ty >(aSpectre->data, 0); NS_AbstractTabEndPtrY< _Ty >(aSpectre->data) != itr; itr++) {
		*itr = (_Ty)(*itr * normafactor);
	}
	return;
}

template < typename _Ty >
void NS_SpectrumTimeNormalization(NS_spectrum *aSpectre, double aNewTime)
{
	double timefactor = aNewTime / aSpectre->time;

	for (_Ty *itr = NS_AbstractTabPtrY< _Ty >(aSpectre->data, 0); NS_AbstractTabEndPtrY< _Ty >(aSpectre->data) != itr; itr++) {
		*itr = (_Ty)(*itr * timefactor);
	}
	aSpectre->time = aNewTime;
	return;
}

#endif /* !defined(__SPECTRUM_UTILS_TPL_HXX__) */
