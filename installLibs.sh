#!/bin/sh
if [ "$(uname)" == "Darwin" ]; then
    if which brew | grep -q 'brew' ; then
        echo "Already has home brew, good for you!"
    else
        ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
    fi
    brew install gtk+3
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
    sudo apt-get install gtk+3
fi
