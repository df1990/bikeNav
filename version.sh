#!/bin/bash

VERSION=$(git tag -l --points-at HEAD)
if [ -z "$VERSION" ]; then
VERSION=$(git log -1 --pretty=format:%h) 
fi

echo BUILD VERSION: $VERSION
cd common
echo "#ifndef _VERSION_H_" > version.h
echo "#define _VERSION_H_" >> version.h
echo " " >> version.h
echo "static const char fw_version[] = \"$VERSION\";" >> version.h
#echo "static const char fw_version[] __attribute__((section (\".firmware_version\"))) = \"$VERSION\";" >> version.h
echo " " >> version.h
echo "#endif" >> version.h
