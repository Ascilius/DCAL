#ifndef BIN_H
#define BIN_H

// histogram bin
struct Bin {
  // universal
  double r_min;         // minimum radial distance from galactic center of mass (kpc)
  double r_max;         // maximum radial distance (kpc)
  double r;             // distance used for this bin's area/volume/density calculations (kpc)
  double a;             // shell area (kpc^2)
  double v;             // shell volume (kpc^3)
  
  // baryonic
  int n_bary;           // baryonic body count
  double m_bary;        // total baryonic mass in this bin (SMU)
  double p_bary;        // baryonic density (SMU/kpc^3)
  
  // dark matter
  int n_dark;           // dark matter body count
  double m_dark;        // total dark mass in this bin (SMU)
  double p_dark;        // dark density (SMU/kpc^3)
  
  // all matter
  int n_total;          // total body count
  double m_total;       // total mass (SMU)
  double p_total;       // total density (SMU/kpc^3)
};

#endif