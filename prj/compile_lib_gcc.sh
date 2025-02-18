
projectName=${1}
# other configurations not implemented yet
configuration=Release

compileonly=0
linkonly=0

for arg in ${@:2}
do
	if [ "-shared" = ${arg} ]; then
		compileonly=0
		linkonly=1
		continue
	fi

	option=${arg:0:2}
	case ${option} in
	"-c")
		linkonly=0
		compileonly=1
	;;
	"-I")
		if [ "x${includedir}" = "x" ]; then
			includedir="${arg}"
		else
			includedir="${includedir} ${arg}"
		fi
	;;
	"-L")
		if [ "x${libdir}" = "x" ]; then
			libdir="${arg}"
		else
			libdir="${libdir} ${arg}"
		fi
	;;
	"-l")
		if [ "x${lib}" = "x" ]; then
			lib="${arg}"
		else
			lib="${lib} ${arg}"
		fi
	;;
	"--")
		case ${arg} in
		"--root")
			if [ "x${linkobj}" = "x" ]; then
				linkobj="Root"
			else
				linkobj="${linkobj} Root"
			fi
		esac
	;;
	*)
		if [ "x${ldopt}" = "x" ]; then
			ldopt="${arg}"
		else
			ldopt="${ldopt} ${arg}"
		fi
	;;
	esac
done

shDir=${PWD}/`dirname ${BASH_SOURCE[0]}`
worldDir=${shDir}/../..
outDir=${worldDir}/build/gcc/${configuration}/lib
intDir=${worldDir}/build/gcc/${projectName}.build/${configuration}

mkdir -pv ${outDir}
mkdir -pv ${intDir}

if [ 0 == ${linkonly} ]; then
	for entry in ${worldDir}/src/lib/${projectName}/*.cxx
	do
		[ -f "${entry}" ] || break
		objname="${entry##*/}"
		objname="${objname%.cxx}"
		compile="gcc -c -Wall -fpic ${entry} -o "${intDir}/${objname}.o" -I"${worldDir}/src/include" ${includedir}"
		echo ${compile} > ${intDir}/${objname}.log
		${compile} >> ${intDir}/${objname}.log
		[ 0 == ${compileonly} ] && echo ${compile} > ${outDir}/${projectName}.log
	done
fi

if [ 0 == ${compileonly} ]; then
	exclude=': Root.o : Hdf.o : Gsl.o'

	for i in ${linkobj}
	do
		if [[ ${exclude} == *${i}* ]]; then
			exclude=${exclude//: ${i}.o/}
		fi
	done
	exclude="$(echo -e "${exclude}" | sed -e 's/^[[:space:]]*//')"
	exclude=${exclude//: /-I *}
	
	linkobj=$(ls -t ${intDir} ${exclude} | grep '\.o')
	link="gcc -shared -o "${outDir}/lib${projectName}.so" ${linkobj} ${libdir} ${ldopt} ${lib}"
	if [ 1 == ${linkonly} ]; then
		echo ${link} > ${outDir}/${projectName}.log
	else
		echo ${link} >> ${outDir}/${projectName}.log
	fi
	(cd ${intDir}; ${link} >> ${outDir}/${projectName}.log)
fi
