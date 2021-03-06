#!/bin/bash

apt update

# X11 tools
apt install xorg-dev gcc g++ cmake gdb mesa-utils libglfw3-dev

# Vulkan install
wget -qO - https://packages.lunarg.com/lunarg-signing-kek-pub.asc | apt-key add -
wget -qO /etc/apt/sources.list.d/lunarg-vulkan-1.2.182-focal.list https://packages.lunarg.com/vulkan/1.2.182/lunarg-vulkan-1.2.182-focal.list
apt update
apt install vulkan-sdk

# Freetype install
apt install libfreetype-dev
