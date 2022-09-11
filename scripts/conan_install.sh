#!/bin/bash

# When running from build directory use ".." for the first parameter
# The second should be "Release" or "Debug"
conan install $1 -pr:b=default -s compiler.cppstd=17 -s build_type=$2 --build=missing
#conan install $1 -pr:b=default -s compiler.cppstd=17 -s build_type=$2 --build=bgfx --build=missing
