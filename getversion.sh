#!/bin/bash

VERSION=$(git tag -l --points-at HEAD)
if [ -z "$VERSION" ]; then
VERSION=$(git log -1 --pretty=format:%h) 
fi

echo BUILD VERSION: $VERSION

echo "static const char sw_version[] = \"$VERSION\";" > version.h
