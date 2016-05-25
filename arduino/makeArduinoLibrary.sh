#!/bin/bash
echo "Building Arduino-Library..."

BASEDIR=$(dirname "$0")

cd $BASEDIR

mkdir kBuffer

cp templates/* kBuffer
cp ../src/kBuffer/* kBuffer
mkdir kBuffer/doc
mkdir kBuffer/doc/html
cp ../doc/doxygen/latex/refman.pdf kBuffer/doc/kBuffer_documentation.pdf
cp -r ../doc/doxygen/html/* kBuffer/doc/html

zip -r kBuffer_Arduino.zip kBuffer

rm -r kBuffer