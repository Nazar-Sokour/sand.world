
#ifndef __COMMON_UTILS_TPL_HXX__
# define __COMMON_UTILS_TPL_HXX__

# include <errno.h>
# include <string.h>
# include "NS_err.h"


template < typename _Ty >
unsigned NS_AbsBufCount(NS_abs_buf *aBuf) { return ( aBuf->count / sizeof(_Ty) ); }
template < typename _Ty >
unsigned NS_AbsBufCount(NS_abs_buf const *aBuf) { return ( aBuf->count / sizeof(_Ty) ); }

template < typename _Ty >
_Ty *NS_AbsBufEndPtr(NS_abs_buf *aBuf) { return (_Ty *)(aBuf->base + aBuf->count); }
template < typename _Ty >
_Ty const *NS_AbsBufEndPtr(NS_abs_buf const *aBuf) { return (_Ty const *)(aBuf->base + aBuf->count); }

template < typename _Ty >
_Ty *NS_AbsBufPtr(NS_abs_buf *aBuf, unsigned aPos) { return (_Ty *)(aBuf->base + aPos * sizeof(_Ty)); }
template < typename _Ty >
_Ty const *NS_AbsBufPtr(NS_abs_buf const *aBuf, unsigned aPos) { return (_Ty const *)(aBuf->base + aPos * sizeof(_Ty)); }

template < typename _Ty >
_Ty NS_AbsBufGetVal(NS_abs_buf const *aBuf, unsigned aPos)
{
	if (aBuf->count <= sizeof(_Ty) * aPos) {
		warnx("Warning in NS_AbsBufGetVal(): %lu: %s", sizeof(_Ty) * aPos, strerror(EINVAL));
		return _Ty();
	}
	return *NS_AbsBufPtr< _Ty >(aBuf, aPos);
}
template < typename _Ty >
_Ty NS_AbsBufGetVal(NS_abs_buf *aBuf, unsigned aPos)
{
	return NS_AbsBufGetVal< _Ty >((NS_abs_buf const *)aBuf, aPos);
}

template < typename _Ty >
int NS_AbsBufPushBack(NS_abs_buf *aBuf, _Ty const &aValue)
{
	int sts = NS_AbsBufReserve(aBuf, (sizeof aValue));
	if (0 == sts) {
		*(NS_AbsBufEndPtr< _Ty >(aBuf)) = aValue;
		aBuf->count += sizeof aValue;
	}
	return (sts);
}

template < typename _Ty >
int NS_AbsBufInsert(NS_abs_buf *aBuf, _Ty const &aValue, unsigned aPos)
{
	if (aBuf->count <= sizeof(_Ty) * aPos) {
		return NS_AbsBufPushBack< _Ty >(aBuf, aValue);
	}
	int sts = NS_AbsBufReserve(aBuf, (sizeof aValue));
	if (0 == sts) {
		memmove(NS_AbsBufPtr< _Ty >(aBuf, aPos + 1), NS_AbsBufPtr< _Ty >(aBuf, aPos), (NS_AbsBufEndPtr< _Ty >(aBuf) - NS_AbsBufPtr< _Ty >(aBuf, aPos)) * sizeof(_Ty));
		*(NS_AbsBufPtr< _Ty >(aBuf, aPos)) = aValue;
		aBuf->count += sizeof aValue;
	}
	return (sts);
}

template < typename _Ty >
void NS_AbsBufPopBack(NS_abs_buf *aBuf)
{
	if (sizeof(_Ty) < aBuf->count)
		aBuf->count -= sizeof(_Ty);
}

template < typename _Ty >
void NS_AbsBufRemove(NS_abs_buf *aBuf, unsigned aPos)
{
	if (aBuf->count <= sizeof(_Ty) * aPos)
		warnx("Warning in NS_AbsBufRemove(): %lu: %s", sizeof(_Ty) * aPos, strerror(EINVAL));
	else
		NS_AbsBufRemove(aBuf, aPos, sizeof(_Ty));
}

template < typename _Ty >
unsigned NS_AbstractTabCount(NS_abstract_tab *aTab) { return ( aTab->count / sizeof(_Ty) ); }
template < typename _Ty >
unsigned NS_AbstractTabCount(NS_abstract_tab const *aTab) { return ( aTab->count / sizeof(_Ty) ); }

template < typename _Ty >
_Ty *NS_AbstractTabEndPtrX(NS_abstract_tab *aTab) { return (_Ty *)(aTab->xv + aTab->count); }
template < typename _Ty >
_Ty const *NS_AbstractTabEndPtrX(NS_abstract_tab const *aTab) { return (_Ty const *)(aTab->xv + aTab->count); }

template < typename _Ty >
_Ty *NS_AbstractTabEndPtrY(NS_abstract_tab *aTab) { return (_Ty *)(aTab->yv + aTab->count); }
template < typename _Ty >
_Ty const *NS_AbstractTabEndPtrY(NS_abstract_tab const *aTab) { return (_Ty const *)(aTab->yv + aTab->count); }

template < typename _Ty >
_Ty *NS_AbstractTabPtrX(NS_abstract_tab *aTab, unsigned aPos) { return (_Ty *)(aTab->xv + aPos * sizeof(_Ty)); }
template < typename _Ty >
_Ty const *NS_AbstractTabPtrX(NS_abstract_tab const *aTab, unsigned aPos) { return (_Ty const *)(aTab->xv + aPos * sizeof(_Ty)); }

template < typename _Ty >
_Ty *NS_AbstractTabPtrY(NS_abstract_tab *aTab, unsigned aPos) { return (_Ty *)(aTab->yv + aPos * sizeof(_Ty)); }
template < typename _Ty >
_Ty const *NS_AbstractTabPtrY(NS_abstract_tab const *aTab, unsigned aPos) { return (_Ty const *)(aTab->yv + aPos * sizeof(_Ty)); }

template < typename _Ty >
_Ty NS_AbstractTabGetValX(NS_abstract_tab const *aTab, unsigned aPos)
{
	if (aTab->count <= sizeof(_Ty) * aPos) {
		warnx("Warning in NS_AbstractTabGetValX(): %lu: %s", sizeof(_Ty) * aPos, strerror(EINVAL));
		return _Ty();
	}
	return *NS_AbstractTabPtrX< _Ty >(aTab, aPos);
}
template < typename _Ty >
_Ty NS_AbstractTabGetValX(NS_abstract_tab *aTab, unsigned aPos)
{
	return NS_AbstractTabGetValX< _Ty >((NS_abstract_tab const *)aTab, aPos);
}

template < typename _Ty >
_Ty NS_AbstractTabGetValY(NS_abstract_tab const *aTab, unsigned aPos)
{
	if (aTab->count <= sizeof(_Ty) * aPos) {
		warnx("Warning in NS_AbstractTabGetValY(): %lu: %s", sizeof(_Ty) * aPos, strerror(EINVAL));
		return _Ty();
	}
	return *NS_AbstractTabPtrY< _Ty >(aTab, aPos);
}
template < typename _Ty >
_Ty NS_AbstractTabGetValY(NS_abstract_tab *aTab, unsigned aPos)
{
	return NS_AbstractTabGetValY< _Ty >((NS_abstract_tab const *)aTab, aPos);
}

template < typename _Ty >
int NS_AbstractTabPushBack(NS_abstract_tab *aTab, _Ty const &aValueX, _Ty const &aValueY)
{
	int sts = NS_AbstractTabReserve(aTab, (sizeof aValueX));
	if (0 == sts) {
		*(NS_AbstractTabEndPtrX< _Ty >(aTab)) = aValueX;
		*(NS_AbstractTabEndPtrY< _Ty >(aTab)) = aValueY;
		aTab->count += sizeof aValueX;
	}
	return (sts);
}

template < typename _Ty >
int NS_AbstractTabInsert(NS_abstract_tab *aTab, _Ty const &aValueX, _Ty const &aValueY, unsigned aPos)
{
	if (aTab->count <= sizeof(_Ty) * aPos) {
		return NS_AbstractTabPushBack< _Ty >(aTab, aValueX, aValueY);
		/* NOTREACHED */
	}
	int sts = NS_AbstractTabReserve(aTab, (sizeof aValueX));
	if (0 == sts) {
		memmove(NS_AbstractTabPtrX< _Ty >(aTab, aPos + 1), NS_AbstractTabPtrX< _Ty >(aTab, aPos), (NS_AbstractTabEndPtrX< _Ty >(aTab) - NS_AbstractTabPtrX< _Ty >(aTab, aPos)) * sizeof(_Ty));
		*(NS_AbstractTabPtrX< _Ty >(aTab, aPos)) = aValueX;
		memmove(NS_AbstractTabPtrY< _Ty >(aTab, aPos + 1), NS_AbstractTabPtrY< _Ty >(aTab, aPos), (NS_AbstractTabEndPtrY< _Ty >(aTab) - NS_AbstractTabPtrY< _Ty >(aTab, aPos)) * sizeof(_Ty));
		*(NS_AbstractTabPtrY< _Ty >(aTab, aPos)) = aValueY;
		aTab->count += sizeof aValueX;
	}
	return (sts);
}

template < typename _Ty >
void NS_AbstractTabPopBack(NS_abstract_tab *aTab)
{
	if (sizeof(_Ty) < aTab->count)
		aTab->count -= sizeof(_Ty);
}

template < typename _Ty >
void NS_AbstractTabRemove(NS_abstract_tab *aTab, unsigned aPos)
{
	if (aTab->count <= sizeof(_Ty) * aPos)
		warnx("Warning in NS_AbstractTabRemove(): %lu: %s", sizeof(_Ty) * aPos, strerror(EINVAL));
	else
		NS_AbstractTabRemove(aTab, aPos, sizeof(_Ty));
}

template <typename _Ty >
void NS_AbstractTabSetValYforPos(NS_abstract_tab *aTab, _Ty &aValue, unsigned aPos)
{
	*NS_AbstractTabPtrY< _Ty >(aTab, aPos) = aValue;
}


/* @todo: check what is this? */
template < typename _Ty >
void NS_AbstractTabUniteElements(NS_abstract_tab *aTab, unsigned aUnionSize)
{
	int sts, newcount;
	unsigned union1;

	newcount = 0;
	union1 = aUnionSize - 1;
	for (unsigned i = 0; i < NS_AbstractTabCount< _Ty >(aTab); i++) {
		unsigned r, q;
		r = i / aUnionSize;
		q = i % aUnionSize;
		if (0 == q) {
			NS_AbstractTabInsert< _Ty >(aTab, 
			                            NS_AbstractTabGetValX< _Ty >(aTab, i),
			                            NS_AbstractTabGetValY< _Ty >(aTab, i),
			                            r);
		}
		else {
			_Ty *yp;
			if (union1 == q) 
				newcount++;
			NS_AbstractTabSetValYforPos< _Ty >(aTab, NS_AbstractTabGetValY< _Ty >(aTab, i), r);
		}
	}
	aTab->count = newcount;
	return;
}

/* @todo: check what is this? */
template < typename _Ty >
int NS_AbstractTabUpdate(NS_abstract_tab *aTabRoot, NS_abstract_tab const *aTabCld)
{
	if (aTabRoot->count < aTabCld->count) {
		int sts;

		sts = NS_AbstractTabReserve(aTabRoot, aTabCld->count - aTabRoot->count);
		if (0 != sts) {
			return (sts);
			/* NOTREACHED */
		}
		aTabRoot->count = aTabCld->count;
	}
	memcpy(aTabRoot->xv, aTabCld->xv, aTabCld->count);
	for (unsigned count = 0; count < NS_AbstractTabCount< _Ty >(aTabCld); count++) {
		*NS_AbstractTabPtrY< _Ty >(aTabRoot, count) += NS_AbstractTabGetValY< _Ty >(aTabCld, count);
	}
	return (0);
}

#endif /* !defined(__COMMON_UTILS_TPL_HXX__) */
