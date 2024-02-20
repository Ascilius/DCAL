library(ggplot2)
library(ggpubr)
library(tidyverse)

# ---------------------------------------------------------------------------
# input parameters
FILE_START <- 9
FILE_END <- 7779
FILE_STEP <- 10
dr <- 0.01
max_xm <- 1.0 # max r for bm
max_xp <- 0.25 # max r for bp, dp, ap

# ---------------------------------------------------------------------------
# file name generator
get_filename <- function(i, ending) {
  i_str = as.character(i) # converting i to string
  dr_str = as.character(dr)
  f_str = paste(i_str, "_dr", dr_str, ending, sep = "")
  return (f_str)
}

# ---------------------------------------------------------------------------
# getting plot maximums

# baryonic
max_bm <- 0.0
max_bp <- 0.0

# dark matter
max_dm <- 0.0
max_dp <- 0.0

# all matter
max_am <- 0.0
max_ap <- 0.0

i <- FILE_START
while (i <= FILE_END) {
  filename <- get_filename(i, ".csv")
  df <- read.csv(filename)
  print(paste("Analyzing '", filename, "'...", sep = ""))
  
  # getting maximums
  
  # baryonic
  max <- max(df$m_bary..SMU.)
  if (max > max_bm) {
    max_bm <- max
    print(paste("New max_bm at '", filename, "': ", max, sep = ""))
  }    
  max <- max(df$p_bary..SMU.kpc.3.)
  if (max > max_bp) {
    max_bp <- max
    print(paste("New max_bp at '", filename, "': ", max, sep = ""))
  }
  
  # dark matter
  max <- max(df$m_dark..SMU.)
  if (max > max_dm) {
    max_dm <- max
    print(paste("New max_dm at '", filename, "': ", max, sep = ""))
  }
  max <- max(df$p_dark..SMU.kpc.3.)
  if (max > max_dp) {
    max_dp <- max
    print(paste("New max_dp at '", filename, "': ", max, ep = ""))
  }
  
  # all matter
  max <- max(df$m_total..SMU.)
  if (max > max_am) {
    max_am <- max
    print(paste("New max_am at '", filename, "': ", max, sep = ""))
  }
  max <- max(df$p_total..SMU.kpc.3.)
  if (max > max_ap) {
    max_ap <- max
    print(paste("New max_am at '", filename, "': ", max, sep = ""))
  }
  
  i <- i + FILE_STEP
}

# adding buffer
max_bm <- max_bm * 1.1
max_bp <- max_bp * 1.1
max_dm <- max_dm * 1.1
max_dp <- max_dp * 1.1
max_am <- max_am * 1.1
max_ap <- max_ap * 1.1

# ---------------------------------------------------------------------------
# TOFIX: plotting
i <- FILE_START
while (i <= FILE_END) {
  # getting csv file
  filename <- get_filename(i, ".csv")
  df <- read.csv(filename)
  paste("Plotting '", filename, "'...", sep = "")
  
  # baryonic
  bm <- ggplot(df) +
    geom_bar(aes(x = r..kpc., y = m_bary..SMU.), stat = "identity") + 
    geom_line(aes(x = r..kpc., y = m_plum..SMU.), stat = "identity", size = 0.5, color = "red") + 
    theme_classic() +
    scale_x_continuous(limits = c(0, max_xm)) + 
    scale_y_continuous(limits = c(0, max_bm)) + 
    labs(x = "r (kpc)", y = "baryonic mass (SMU)")
  bm
  bp <- ggplot(df) +
    geom_bar(aes(x = r..kpc., y = p_bary..SMU.kpc.3.), stat = "identity") + 
    geom_line(aes(x = r..kpc., y = p_plum..SMU.kpc.3.), stat = "identity", size = 0.5, color = "red") + 
    theme_classic() +
    scale_x_continuous(limits = c(0, max_xp)) + 
    scale_y_continuous(limits = c(0, max_bp)) + 
    labs(title = filename, x = "r (kpc)", y = "baryonic density (SMU/kpc^3)")
  bp
  
  # dark matter
  dm <- ggplot(df) +
    geom_bar(aes(x = r..kpc., y = m_dark..SMU.), stat = "identity") + 
    geom_line(aes(x = r..kpc., y = m_sidm..SMU.), stat = "identity", size = 0.5, color = "red") + 
    theme_classic() +
    scale_y_continuous(limits = c(0, max_dm)) + 
    labs(x = "r (kpc)", y = "dark mass (SMU)")
  dm
  dp <- ggplot(df) +
    geom_bar(aes(x = r..kpc., y = p_dark..SMU.kpc.3.), stat = "identity") + 
    geom_line(aes(x = r..kpc., y = p_sidm..SMU.kpc.3.), stat = "identity", size = 0.5, color = "red") + 
    theme_classic() +
    scale_x_continuous(limits = c(0, max_xp)) + 
    scale_y_continuous(limits = c(0, max_dp)) + 
    labs(x = "r (kpc)", y = "dark matter density (SMU/kpc^3)")
  dp
  
  # all matter
  am <- ggplot(df) +
    geom_bar(aes(x = r..kpc., y = m_total..SMU.), stat = "identity") + 
    geom_line(aes(x = r..kpc., y = m_total_model..SMU.), stat = "identity", size = 0.5, color = "red") + 
    theme_classic() +
    scale_y_continuous(limits = c(0, max_am)) + 
    labs(x = "r (kpc)", y = "baryonic + dark mass (SMU)")
  am
  ap <- ggplot(df) +
    geom_bar(aes(x = r..kpc., y = p_total..SMU.kpc.3.), stat = "identity") + 
    geom_line(aes(x = r..kpc., y = p_total_model..SMU.kpc.3.), stat = "identity", size = 0.5, color = "red") + 
    theme_classic() +
    scale_x_continuous(limits = c(0, max_xp)) + 
    scale_y_continuous(limits = c(0, max_ap)) + 
    labs(x = "r (kpc)", y = "baryonic + dark matter density (SMU/kpc^3)")
  ap
  
  # combining plots
  ggarrange(bm, dm, am, bp, dp, ap, 
            labels = c("Baryonic", "Dark Matter", "All Matter"), 
            ncol = 3, nrow = 2)
  
  output_filename <- get_filename(i, ".png")
  print(paste("Saving to '", output_filename, "'...", sep = ""))
  ggsave(output_filename, scale = 2.0, width = 1920, height = 1080, units = "px")
  
  i <- i + FILE_STEP
}
