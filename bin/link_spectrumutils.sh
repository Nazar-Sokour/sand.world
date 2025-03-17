
sanddir=${PWD}/`dirname ${BASH_SOURCE[0]}`/..
libdir=${sanddir}/lib
objlist="SpectrumUtils.o"

for i in ${@:1}
do
	opt=${i%%=*}
	val=${i#*=}
	echo "opt: ${opt}"
	echo "val: ${val}"
	if [ "-rootlibdir" = ${opt} ]; then
		rootlibs="-lCore -lHist"
		rootlibdir=${val}
		objlist="${objlist} SpectrumUtilsRoot.o"

		mv ${sanddir}/include/NS/NSUtilsCF.h ${sanddir}/include/NS/NSUtilsCF.h.old
		mv ${sanddir}/include/NS/NSUtilsCF.h.root ${sanddir}/include/NS/NSUtilsCF.h
	fi
done

(cd ${libdir}; gcc -shared -o ${libdir}/libSpectrumUtils.so ${objlist} -Wl,-rpath='${ORIGIN}':${rootlibdir} -L"${libdir}" -L"${rootlibdir}" -lCommonUtils -lFuncUtils -lm ${rootlibs})

rootlibdir=
rootlibs=
objlist=
