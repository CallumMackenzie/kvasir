#!/bin/sh

# Ensure brew is installed
which -s brew
if [[ $? != 0 ]] ; then
	/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
else
	brew update
fi

# Install dependencies
brew install cmake
brew install mesa
brew install vulkan-headers
brew install molten-vk
brew install freetype