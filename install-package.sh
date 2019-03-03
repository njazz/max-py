#!/bin/bash

mkdir package
mkdir package/max-py

cp -r build/py.eval.mxo package/max-py/externals/
cp -r build/py.exec.mxo package/max-py/externals/
cp -r build/py.func.mxo package/max-py/externals/
cp -r build/py.callback.mxo package/max-py/externals/
cp -r build/py.module.mxo package/max-py/externals/

mkdir ~/Documents/Max\ 7/Packages/max-py
mkdir ~/Documents/Max\ 7/Packages/max-py/externals

cp -r package/max-py ~/Documents/Max\ 7/Packages/
