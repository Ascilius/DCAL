# Dwarf Galaxy Analyzer (DGA)
Formerly the (Shell) Density Calculator (DCAL).
A set of tools for analyzing dwarf galaxies using .out files from [MilkyWay@home](https://github.com/Milkyway-at-home/milkywayathome_client).

### todo
 - fix run_dga.sh
    - flags won't work :(
 - automate R plotting immediate after DGA analysis
 - optimize: generate Plummer and SIDM curves once, output in separate file
 - figure out how to adjust graph headers

### v5
 - project renamed to Dwarf Galaxy Analyzer (DGA)
 - density_calculator.cpp and SIDM_profile.cpp removed
    - single program now handles both single and multiple step processing
 - density_auto_calculator.cpp renamed to main.cpp
    - profile functions moved to profile.cpp
       - added Plummer model function
    - frame analyzing functions moved to frame.cpp
       - each frame is read into Frame objects
       - now outputs much more information
          - 4 sections
         1. universal data: bin min/max, r, a, v
         2. baryonic matter: baryonic body count, baryonic mass, baryonic density, Plummer mass, Plummer density
         3. same as 2 but for dark matter (and Plummer model is replaced with SIDM model)
         4. same as 2 but the sum of 2 and 3 (Plummer and SIDM models are just added on top of each other)
 - updated R plotter to now plot 6 graphs (baryonic mass/density, dark matter mass/density, baryonic+dark mass/density) 

### v4
 - polished density_calculator.cpp and SIDM_profile.cpp
    - now uses Structural Mass Units (SMUs)
    - outputs more information (bin range, shell radius/area/volume, count, mass)
 - added Makefile
 - density calculation and SIDM profiling is now automatic
    - Usage: run_dcal.sh \<input .out file> \<dr> \[-h] \[-c]
 - polished density_auto_calculator.cpp
    - basically the same changes as density_calculator.cpp
    - SIDM curve calculation is within this file
 - added R plotting code
    - added SIDM function plotting

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