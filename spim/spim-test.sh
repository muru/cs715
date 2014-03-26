#! /bin/bash
SOURCE_D=~/devel/gcc-4.8.2
BUILD_D=~/devel/spim_build
SPIM_D=~/devel/gcc-spim-diff/mod

for VER in 0.0 0.1 0.2 1 2 3 4
do
	MOD_FILES=( spim$VER{.c,.h,.md,-protos.h} )
	COM_FILES=( spim{.c,.h,.md,-protos.h} )
	N=${#MOD_FILES[@]}
	for (( i=0; i<$N;++i ))
	do
		cp $SPIM_D/${MOD_FILES[$i]} $SOURCE_D/gcc/config/spim/${COM_FILES[$i]}
	done

	cd $BUILD_D
	make clean
	make distclean
	$SOURCE_D/configure --enable-languages=c --target=spim
	echo $VER > spim.ver
	make cc1 >make.log 2> >(tee make.err 1>&2)
	echo -e '\a'
	if [[ -x gcc/cc1 ]] 
	then
		for file in ~/devel/cs715/spim/spim-test-cases/*
		do
			echo "########################### Spim v$VER ##########################" >> test.log
			gcc/cc1 $file >> test.log
		done
	else
		exit -1
	fi
done
