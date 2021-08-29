#!/bin/bash

apt update

# CMake
apt install cmake

# X11 tools
apt install xorg-dev

# Compiler
apt install gcc g++ cmake

# Vulkan install
wget -qO - https://packages.lunarg.com/lunarg-signing-kek-pub.asc | apt-key add -
wget -qO /etc/apt/sources.list.d/lunarg-vulkan-1.2.182-focal.list https://packages.lunarg.com/vulkan/1.2.182/lunarg-vulkan-1.2.182-focal.list
apt update
apt install vulkan-sdk

# OpenGL install
apt install mesa-utils
apt install libglfw3-dev

# Freetype install
apt install libfreetype-dev