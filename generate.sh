#!/bin/bash

# set initial values
SUBDIR="end"
NO_INIT=0
CLEAN=0
FLAGS=""

# read options
TEMP=`getopt -o s:ncl: --long subdir:,flags:,no-init,clean,lab: -n $0 -- "$@"`
eval set -- "$TEMP"

# extract options and their arguments
while true ; do
    case "$1" in
        -s|--subdir)
            case "$2" in
                "") shift 2 ;;
                *) SUBDIR=$2 ; shift 2 ;;
            esac ;;
	-f|--flags)
            case "$2" in
                "") shift 2 ;;
                *) FLAGS=$2 ; shift 2 ;;
            esac ;;
	-n|--no-init) NO_INIT=1 ; shift ;;
        -c|--clean) CLEAN=1 ; shift ;;
        -l|--lab)
            case "$2" in
                "") shift 2 ;;
                *) LAB=$2 ; shift 2;;
            esac ;;

        --) shift ; break ;;
        *) echo "Internal error!" ; exit 1 ;;
    esac
done

if [ "$CLEAN" -eq 1 ]; then
  rm -rf xv6-public-*
  exit 0
fi

if [ -z "$LAB" ]; then
  echo "usage: $0 --lab lab [--subdir subdir] [--no-init] [--clean]"
  exit 1
fi

DST=xv6-public-$(basename "$LAB")
mkdir -p "$DST"

if [ $NO_INIT -eq 0 ]; then
  rm -rf "$DST"
  cp -r xv6-public "$DST"
fi

cp -r "$LAB"/"$SUBDIR"/* "$DST"
cd "$DST"
make SCHEDPOLICY=$FLAGS
make qemu-nox 

exit 0
