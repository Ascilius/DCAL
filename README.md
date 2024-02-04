# (Shell) Density Calculator (DCAL)
A tool I wrote for calculating shell densities from .out files from [MilkyWay@home](https://github.com/Milkyway-at-home/milkywayathome_client)

### todo
 - add R plotter
    - plots the densities in a column graph
    - i already have it written i just need to add it to this repo
 - automate calculating and plotting
    - bash script?

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