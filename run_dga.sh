#!/bin/bash

# how to use
usage() {
	echo "Usage: $0 <input .out file> <dr> [-h] [-c]"
	echo "   -h: Help; print usage"
	echo "   -c: Rebuild and clear output directory"
	exit 1
}

# TOFIX: parsing arguments
while getopts "hc:" flag; do
	case $flag in
		h)
			usage
			;;
		c)
			clean_project=true
			;;
		\?)
			echo "Invalid option: -$OPTARG"
			usage
			;;
	esac
done

# cleaning/rebuilding (if specified)
if [ "$clean_project" = true ]; then
	echo "Cleaning..."
	rm -r output
	make clean
	echo "Rebuilding..."
	mkdir output
	make
	echo ""
fi

# running
build/dwarf_galaxy_analyzer.exe $1 $2 $3 $4 $5 $6 $7