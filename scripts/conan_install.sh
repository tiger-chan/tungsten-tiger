#!/bin/bash

conan install $1 -pr:b=default -s compiler.cppstd=17 -s build_type=$2 --build=bgfx
