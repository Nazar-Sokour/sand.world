
sanddir=..
nsutilsdir=${sanddir}/../NSutils.world
declare -a src=("${nsutilsdir}/build/gcc/Release/bin/*"
				"${nsutilsdir}/build/gcc/Release/lib/lib*.so"
				"${nsutilsdir}/build/gcc/SpectrumUtils.build/Release/*.o"
                "${nsutilsdir}/src/bin/sand/sand.hpp"
				"${nsutilsdir}/src/include/NS/Common*"
				"${nsutilsdir}/src/include/NS/FuncUtils.hxx"
				"${nsutilsdir}/src/include/NS/Spectrum*.hxx"
				"${nsutilsdir}/src/include/NS/*.h"
                "${nsutilsdir}/src/examples/sand/*"
                "${nsutilsdir}/doc/sand/*"
				"${nsutilsdir}/prj/sh/compile_lib_gcc.sh")
declare -a dest=("${sanddir}/bin"
                 "${sanddir}/lib"
                 "${sanddir}/lib"
                 "${sanddir}/include"
				 "${sanddir}/include/NS"
				 "${sanddir}/include/NS"
				 "${sanddir}/include/NS"
				 "${sanddir}/include/NS"
				 "${sanddir}/examples"
                 "${sanddir}/doc"
				 "${sanddir}/prj")
arraylen=${#src[@]}

for (( i=0; i<${arraylen}; i++ )); do
# 	# ln ${file} ${dest:}
	for ifile in ${src[${i}]}; do
		# echo ${ifile##*/}
		# echo ${dest[${i}]}/${ifile##*/}
		mkdir -pv ${dest[${i}]}
		ln ${ifile} ${dest[${i}]}/${ifile##*/}
	done
# 	echo ${dest[$i]}
done
