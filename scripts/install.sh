#!/usr/bin/env bash

VCPKG_GIT_URL=https://github.com/microsoft/vcpkg.git
VCPKG_GIT_TAG=2020.01
VCPKG_DIR=.vcpkg

if ! [ -e ${VCPKG_DIR} ]
then
    git clone --branch ${VCPKG_GIT_TAG} ${VCPKG_GIT_URL} ${VCPKG_DIR} || exit 1
fi

cd ${VCPKG_DIR} || exit 1

if ! [ -e vcpkg ]
then
    ./bootstrap-vcpkg.sh || exit 1
fi

./vcpkg install sfml
