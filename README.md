# (Shell) Density Calculator (DCAL)
A tool I wrote for calculating shell densities from .out files from [MilkyWay@home](https://github.com/Milkyway-at-home/milkywayathome_client)

### todo
 - add R plotter
    - plots the densities in a column graph
    - i already have it written i just need to add it to this repo
 - automate calculating and plotting
    - bash script?

### v3
 - added SIDM profile generator (SIDM_profile)
    - just generates a bunch of points based on the piecewise SIDM profle function into a csv file
 - added option for dark matter in density_calculator.cpp
    - 1 is to calculate shell density for dark matter only
    - 0 is the former normal calculation (all matter)
 - density_calculator no longer outputs in the directory the input file is in, now outputs in the current working directory

### v2
 - added units
    - each body is 500 solar masses and count is now multiplied by that 
    - distances from the .out file are in kiloparsecs (kpc)
       - so the density should be in solar masses per kiloparsec cubed (M_sol/kpc^3)
 - fixed usage specification
 - removed dr from output file
    - for easier reading into R
    - dr is now in the output file name
 - output file name is not an option anymore
    - generated automatically from input file name

### v1
 - wrote this tool
 - currently a count density because i dont know the units right now