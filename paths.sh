#! /bin/bash
GCC_INSTALL_DIR=$( dirname $(dirname `find /opt -iname g++-4.7.2`) )
export LIBRARY_PATH=/usr/lib/$(gcc -print-multiarch)
export C_INCLUDE_PATH=/usr/include/$(gcc -print-multiarch)
export CPLUS_INCLUDE_PATH=/usr/include/$(gcc -print-multiarch)
export PATH=$GCC_INSTALL_DIR/bin:$PATH
export LD_LIBRARY_PATH=$GCC_INSTALL_DIR/lib

