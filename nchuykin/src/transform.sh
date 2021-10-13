#!/bin/bash

rm -r -f result
mkdir result
mkdir result/c2eo
cp -r eo/. result/c2eo
#mkdir result/c2eo/eo
cp cmake-build-debug/*.eo result/c2eo/eo/c2eo
cd result/c2eo
mvn clean compile