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
n=$(build/density_calculator.exe $1 $2 1 1)
echo ""
# echo $n
build/SIDM_profile.exe $2 $n

echo ""
echo "Done!"