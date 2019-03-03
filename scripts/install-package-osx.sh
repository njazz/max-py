#!/bin/bash
./make-package-osx.sh

cd ..

mkdir ~/Documents/Max\ 7/Packages/max-py
mkdir ~/Documents/Max\ 7/Packages/max-py/externals

cp -r package/max-py ~/Documents/Max\ 7/Packages/
