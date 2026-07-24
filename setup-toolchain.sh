#!/bin/bash
set -e # se comando fallisce, esce

# dove viene installato il cross compiler
PREFIX="$HOME/opt/cross"
# per quale piattaforma genero il codice
TARGET="i686-elf"
# aggiungo quella cartella al path
export PATH="$PREFIX/bin:$PATH"

# due variabili per le versioni
BINUTILS_VERSION="2.41"
GCC_VERSION="13.2.0"

# cartella dove scarico i sorgenti 
SRC_DIR="$HOME/src/i686-cross"

# la creo, con l'opzione che non da errore se esiste già
mkdir -p $SRC_DIR
cd $SRC_DIR

# scarico binutils
wget https://ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_VERSION.tar.gz

# scarico gcc
wget https://ftp.gnu.org/gnu/gcc/gcc-$GCC_VERSION/gcc-$GCC_VERSION.tar.gz

# estraggo con x, con z uso gzip e con f lo metto come file v è semplicemente verbose
tar xzfv binutils-$BINUTILS_VERSION.tar.gz
tar xzfv gcc-$GCC_VERSION.tar.gz

mkdir $SRC_DIR/build-binutils
mkdir $SRC_DIR/build-gcc

cd build-binutils
# genero il Makefile per binutils, con target/prefix impostati e opzioni per velocizzare/evitare falsi errori
../binutils-$BINUTILS_VERSION/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror

# compilo e installo in $PREFIX
make
make install

# mi sposto nella cartella 
cd ../build-gcc
# genero il Makefile
../gcc-$GCC_VERSION/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers

# compilo e installo
make all-gcc all-target-libgcc
make install-gcc install-target-libgcc