#!/bin/bash
./build.sh

cd ..

mkdir package
mkdir package/max-py

cp -r build/py.eval.mxo package/max-py/externals/
cp -r build/py.exec.mxo package/max-py/externals/
cp -r build/py.func.mxo package/max-py/externals/
cp -r build/py.callback.mxo package/max-py/externals/
cp -r build/py.module.mxo package/max-py/externals/
