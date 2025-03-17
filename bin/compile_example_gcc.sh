
projectName=sand_ex1
configuration=Release

shDir=${PWD}/`dirname ${BASH_SOURCE[0]}`
worldDir=${shDir}/..
outDir=${worldDir}/build/gcc/${configuration}/bin
intDir=${worldDir}/build/gcc/${projectName}.build/${configuration}

mkdir -pv ${outDir}
mkdir -pv ${intDir}

gcc -Wall "${worldDir}/examples/${projectName}.cpp" -o "${outDir}/${projectName}" -I"${worldDir}/include" \
 -L"${worldDir}/lib" -Wl,-rpath='${ORIGIN}'/../../../../lib -lCommonUtils -lFuncUtils -lSpectrumUtils
