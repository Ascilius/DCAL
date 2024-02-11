library(ggplot2)
library(tidyverse)

# input parameters
FILE_START <- 9
FILE_END <- 1939
FILE_STEP <- 10
dr <- 0.01
x_limit <- 20
y_limit <- 0.30

i <- FILE_START
while(i <= FILE_END) {
  i_str = as.character(i) # converting i to string
  dr_str = as.character(dr)
  
  # getting csv file
  filename <- paste(i_str, "_dr", dr_str, ".csv", sep = "") # make sure the working directory is the output directory
  df <- read.csv(filename)
  
  # truncating data (outer, low-density areas are not needed)
  cut <- filter(df, r..kpc. < x_limit)
  
  paste("Plotting '", filename, "'...", sep = "")
  ggplot(cut) +
    geom_bar(aes(x = r..kpc., y = total.mass..SMU.), stat = "identity") + 
    geom_line(aes(x = r..kpc., y = m..SIDM.function.), stat = "identity", size = 0.5, color = "red") + 
    theme_classic() +
    scale_y_continuous(limits = c(0, y_limit)) + 
    labs(title = filename, x = "r (kpc)", y = "density (SMU/kpc^3)")
  
  output <- paste(i_str, "_dr", dr_str, "_mass.png", sep = "")
  print(paste("Saving to '", output, "'...", sep = ""))
  ggsave(output, scale = 2.0, width = 1920, height = 1080, units = "px")
  
  i <- i + FILE_STEP
}

