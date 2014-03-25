#! /bin/bash

if [[ $# -lt 1 ]];
then 
	echo "Version needed."
	exit -1
fi

[[ -z $SOURCE_D ]] && SOURCE_D=~/devel/gcc-4.8.2
[[ -z $DIFF_D ]] && DIFF_D=~/devel/gcc-spim-diff


SPIM_PATH=gcc/config/spim
SPIM_LINKS=( spim{.c,.h,.md,-protos.h} )
N=${#SPIM_LINKS[@]}

VER=$1
if [[ -n $2 ]]
then
	OLD_VER=$2
	SPIM_FILES=( spim$OLD_VER{.c,.h,.md,-protos.h} )
	for ((i=0; i<$N;++i))
	do
		cp $SOURCE_D/$SPIM_PATH/${SPIM_LINKS[$i]} $DIFF_D/mod/${SPIM_FILES[$i]}
	done
fi

SPIM_FILES=( spim$VER{.c,.h,.md,-protos.h} )

for (( i=0; i<$N; ++i ))
do
	rm -f $SOURCE_D/$SPIM_PATH/${SPIM_LINKS[$i]} $DIFF_D/$SPIM_PATH/${SPIM_LINKS[$i]}
	cp $DIFF_D/orig/${SPIM_FILES[$i]} $SOURCE_D/$SPIM_PATH/${SPIM_LINKS[$i]}
	cp $DIFF_D/orig/${SPIM_FILES[$i]} $DIFF_D/$SPIM_PATH/${SPIM_LINKS[$i]}
	echo ${SPIM_FILES[$i]}
done
