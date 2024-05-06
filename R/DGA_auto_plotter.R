library(ggplot2)
library(ggpubr)
library(tidyverse)

# ---------------------------------------------------------------------------
# input parameters
FILE_START <- 
FILE_END <- 
FILE_STEP <- 
dr <- 
dt <- 

PROFILE_FILENAME <- ""

# baryonic
max_xm <- 3.0 # max r for bm
max_xp <- 1.0 # max r for bp

# plummer
# max_xdm <- 5 # max r for dm, am
# max_xdp <- 1.5 # max r for dp, ap

# nfw/sidm
max_xdm <- 20 # max r for dm, am
max_xdp <- 0.5 # max r for dp, ap

# ---------------------------------------------------------------------------
# file name generator
get_filename <- function(i, ending) {
  i_str = as.character(i) # converting i to string
  dr_str = as.character(dr)
  f_str = paste(i_str, "_dr", dr_str, ending, sep = "")
  return (f_str)
}

# ---------------------------------------------------------------------------
# getting profile data
profile <- read.csv(PROFILE_FILENAME)

# ---------------------------------------------------------------------------
# getting plot maximums

# baryonic
max_bm <- max(profile$m_bary..SMU.)
max_bp <- max(profile$p_bary..SMU.kpc.3.)

# dark matter
max_dm <- max(profile$m_dark..SMU.)
max_dp <- max(profile$p_dark..SMU.kpc.3.)

# all matter
max_am <- max(profile$m_total..SMU.)
max_ap <- max(profile$p_total..SMU.kpc.3.)

# adding buffer
max_bm <- max_bm * 1.1
max_bp <- max_bp * 1.1
max_dm <- max_dm * 1.1
max_dp <- max_dp * 1.1
max_am <- max_am * 1.1
max_ap <- max_ap * 1.1

# ---------------------------------------------------------------------------
# plotting

# first file
i <- 0

# getting csv file
filename <- get_filename(i, ".csv")
data <- read.csv(filename)
paste("Plotting '", filename, "'...", sep = "")

# getting trimmed profile data
n <- nrow(data)
trimmed_profile <- head(profile, n)

# baryonic
bm <- ggplot(data) +
  geom_bar(aes(x = r..kpc., y = m_bary..SMU.), stat = "identity") + 
  geom_line(aes(x = trimmed_profile$r..kpc., y = trimmed_profile$m_bary..SMU.), stat = "identity", linewidth = 0.5, color = "red") + 
  theme_classic() + 
  scale_x_continuous(limits = c(0, max_xm)) + 
  coord_cartesian(ylim = c(0, max_bm)) + 
  labs(title = filename, x = "r (kpc)", y = "baryonic mass (SMU)")
bm
bp <- ggplot(data) +
  geom_bar(aes(x = r..kpc., y = p_bary..SMU.kpc.3.), stat = "identity") + 
  geom_line(aes(x = trimmed_profile$r..kpc., y = trimmed_profile$p_bary..SMU.kpc.3.), stat = "identity", linewidth = 0.5, color = "red") + 
  theme_classic() + 
  scale_x_continuous(limits = c(0, max_xp)) + 
  coord_cartesian(ylim = c(0, max_bp)) + 
  labs(x = "r (kpc)", y = "baryonic density (SMU/kpc^3)")
bp

# dark matter
dm <- ggplot(data) +
  geom_bar(aes(x = r..kpc., y = m_dark..SMU.), stat = "identity") + 
  geom_line(aes(x = trimmed_profile$r..kpc., y = trimmed_profile$m_dark..SMU.), stat = "identity", linewidth = 0.5, color = "red") + 
  theme_classic() +
  scale_x_continuous(limits = c(0, max_xdm)) + 
  coord_cartesian(ylim = c(0, max_dm)) + 
  labs(subtitle = paste((i * dt), "GY"), x = "r (kpc)", y = "dark mass (SMU)")
dm
dp <- ggplot(data) +
  geom_bar(aes(x = r..kpc., y = p_dark..SMU.kpc.3.), stat = "identity") + 
  geom_line(aes(x = trimmed_profile$r..kpc., y = trimmed_profile$p_dark..SMU.kpc.3.), stat = "identity", linewidth = 0.5, color = "red") + 
  theme_classic() +
  scale_x_continuous(limits = c(0, max_xdp)) + 
  coord_cartesian(ylim = c(0, max_dp)) + 
  labs(x = "r (kpc)", y = "dark matter density (SMU/kpc^3)")
dp

# all matter
am <- ggplot(data) +
  geom_bar(aes(x = r..kpc., y = m_total..SMU.), stat = "identity") + 
  geom_line(aes(x = trimmed_profile$r..kpc., y = trimmed_profile$m_total..SMU.), stat = "identity", linewidth = 0.5, color = "red") + 
  theme_classic() +
  scale_x_continuous(limits = c(0, max_xdm)) + 
  coord_cartesian(ylim = c(0, max_am)) + 
  labs(x = "r (kpc)", y = "baryonic + dark mass (SMU)")
am
ap <- ggplot(data) +
  geom_bar(aes(x = r..kpc., y = p_total..SMU.kpc.3.), stat = "identity") + 
  geom_line(aes(x = trimmed_profile$r..kpc., y = trimmed_profile$p_total..SMU.kpc.3.), stat = "identity", linewidth = 0.5, color = "red") + 
  theme_classic() +
  scale_x_continuous(limits = c(0, max_xdp)) + 
  coord_cartesian(ylim = c(0, max_ap)) + 
  labs(x = "r (kpc)", y = "baryonic + dark matter density (SMU/kpc^3)")
ap

# combining plots
ggarrange(bm, dm, am, bp, dp, ap, ncol = 3, nrow = 2)

# outputting combined graphic
output_filename <- get_filename(i, ".png")
print(paste("Saving to '", output_filename, "'...", sep = ""))
ggsave(output_filename, scale = 2.0, width = 1920, height = 1080, units = "px")

# multiple files
i <- FILE_START
while (i <= FILE_END) {
  # getting csv file
  filename <- get_filename(i, ".csv")
  data <- read.csv(filename)
  paste("Plotting '", filename, "'...", sep = "")
  
  # getting trimmed profile data
  n <- nrow(data)
  trimmed_profile <- head(profile, n)
  
  # baryonic
  bm <- ggplot(data) +
    geom_bar(aes(x = r..kpc., y = m_bary..SMU.), stat = "identity") + 
    geom_line(aes(x = trimmed_profile$r..kpc., y = trimmed_profile$m_bary..SMU.), stat = "identity", linewidth = 0.5, color = "red") + 
    theme_classic() + 
    scale_x_continuous(limits = c(0, max_xm)) + 
    coord_cartesian(ylim = c(0, max_bm)) + 
    labs(title = filename, x = "r (kpc)", y = "baryonic mass (SMU)")
  bm
  bp <- ggplot(data) +
    geom_bar(aes(x = r..kpc., y = p_bary..SMU.kpc.3.), stat = "identity") + 
    geom_line(aes(x = trimmed_profile$r..kpc., y = trimmed_profile$p_bary..SMU.kpc.3.), stat = "identity", linewidth = 0.5, color = "red") + 
    theme_classic() + 
    scale_x_continuous(limits = c(0, max_xp)) + 
    coord_cartesian(ylim = c(0, max_bp)) + 
    labs(x = "r (kpc)", y = "baryonic density (SMU/kpc^3)")
  bp
  
  # dark matter
  dm <- ggplot(data) +
    geom_bar(aes(x = r..kpc., y = m_dark..SMU.), stat = "identity") + 
    geom_line(aes(x = trimmed_profile$r..kpc., y = trimmed_profile$m_dark..SMU.), stat = "identity", linewidth = 0.5, color = "red") + 
    theme_classic() +
    scale_x_continuous(limits = c(0, max_xdm)) + 
    coord_cartesian(ylim = c(0, max_dm)) + 
    labs(subtitle = paste((i * dt), "GY"), x = "r (kpc)", y = "dark mass (SMU)")
  dm
  dp <- ggplot(data) +
    geom_bar(aes(x = r..kpc., y = p_dark..SMU.kpc.3.), stat = "identity") + 
    geom_line(aes(x = trimmed_profile$r..kpc., y = trimmed_profile$p_dark..SMU.kpc.3.), stat = "identity", linewidth = 0.5, color = "red") + 
    theme_classic() +
    scale_x_continuous(limits = c(0, max_xdp)) + 
    coord_cartesian(ylim = c(0, max_dp)) + 
    labs(x = "r (kpc)", y = "dark matter density (SMU/kpc^3)")
  dp
  
  # all matter
  am <- ggplot(data) +
    geom_bar(aes(x = r..kpc., y = m_total..SMU.), stat = "identity") + 
    geom_line(aes(x = trimmed_profile$r..kpc., y = trimmed_profile$m_total..SMU.), stat = "identity", linewidth = 0.5, color = "red") + 
    theme_classic() +
    scale_x_continuous(limits = c(0, max_xdm)) + 
    coord_cartesian(ylim = c(0, max_am)) + 
    labs(x = "r (kpc)", y = "baryonic + dark mass (SMU)")
  am
  ap <- ggplot(data) +
    geom_bar(aes(x = r..kpc., y = p_total..SMU.kpc.3.), stat = "identity") + 
    geom_line(aes(x = trimmed_profile$r..kpc., y = trimmed_profile$p_total..SMU.kpc.3.), stat = "identity", linewidth = 0.5, color = "red") + 
    theme_classic() +
    scale_x_continuous(limits = c(0, max_xdp)) + 
    coord_cartesian(ylim = c(0, max_ap)) + 
    labs(x = "r (kpc)", y = "baryonic + dark matter density (SMU/kpc^3)")
  ap
  
  # combining plots
  ggarrange(bm, dm, am, bp, dp, ap, ncol = 3, nrow = 2)
  
  # outputting combined graphic
  output_filename <- get_filename(i, ".png")
  print(paste("Saving to '", output_filename, "'...", sep = ""))
  ggsave(output_filename, scale = 2.0, width = 1920, height = 1080, units = "px")
  
  i <- i + FILE_STEP
}

