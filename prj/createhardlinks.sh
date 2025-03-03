
sanddir=${PWD}/`dirname ${BASH_SOURCE[0]}`/..
nsutilsdir=${sanddir}/../NSutils.world
declare -a src=("${nsutilsdir}/build/gcc/Release/bin/sand"
				"${nsutilsdir}/src/bin/sand/sand_env.sh"
				"${nsutilsdir}/build/gcc/Release/lib/libCommonUtils.so"
				"${nsutilsdir}/build/gcc/Release/lib/libFuncUtils.so"
				"${nsutilsdir}/build/gcc/SpectrumUtils.build/Release/*.o"
				"${nsutilsdir}/src/bin/sand/sand.hpp"
				"${nsutilsdir}/src/include/NS/Common*"
				"${nsutilsdir}/src/include/NS/FuncUtils.hxx"
				"${nsutilsdir}/src/include/NS/Spectrum*.hxx"
				"${nsutilsdir}/src/include/NS/*.h"
				"${nsutilsdir}/src/examples/sand/*"
				"${nsutilsdir}/doc/sand/*")

declare -a dest=("${sanddir}/bin"
				 "${sanddir}/bin"
				 "${sanddir}/lib"
				 "${sanddir}/lib"
				 "${sanddir}/lib"
				 "${sanddir}/include"
				 "${sanddir}/include/NS"
				 "${sanddir}/include/NS"
				 "${sanddir}/include/NS"
				 "${sanddir}/include/NS"
				 "${sanddir}/examples"
				 "${sanddir}/doc")
arraylen=${#src[@]}

for (( i=0; i<${arraylen}; i++ )); do
	for ifile in ${src[${i}]}; do
		mkdir -pv ${dest[${i}]}
		ln ${ifile} ${dest[${i}]}/${ifile##*/}
	done
done
