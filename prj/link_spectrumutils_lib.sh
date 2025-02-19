
libdir=${PWD}/`dirname ${BASH_SOURCE[0]}`/../lib
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
	fi
done

(cd ${libdir}; gcc -shared -o ${libdir}/libSpectrumUtils.so ${objlist} -Wl,-rpath='${ORIGIN}':${rootlibdir} -L"${libdir}" -L"${rootlibdir}" -lCommonUtils -lFuncUtils ${rootlibs})
