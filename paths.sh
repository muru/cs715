#! /bin/bash
export LIBRARY_PATH=/usr/lib/$(gcc -print-multiarch)
export C_INCLUDE_PATH=/usr/include/$(gcc -print-multiarch)
export CPLUS_INCLUDE_PATH=/usr/include/$(gcc -print-multiarch)
export PATH=/opt/cs715/bin:$PATH
export LD_LIBRARY_PATH=/opt/cs715/lib

