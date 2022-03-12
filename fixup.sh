#!/bin/sh

#find . -type f -exec dos2unix {} \;
find . -type f | xargs dos2unix -q