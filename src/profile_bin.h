#ifndef PROFILE_BIN_H
#define PROFILE_BIN_H

// histogram bin
struct Profile_Bin {
  // universal
  double r_min;         // minimum radial distance from galactic center of mass (kpc)
  double r_max;         // maximum radial distance (kpc)
  double r;             // distance used for this bin's area/volume/density calculations (kpc)
  double a;             // shell area (kpc^2)
  double v;             // shell volume (kpc^3)
  
  // baryonic
  double m_bary;       // mass calculated from profile 1 function * v (SMU)
  double p_bary;       // density calculated from profile 1 function (SMU/kpc^3)
  
  // dark matter
  double m_dark;       // mass calculated from profile 2 function * v (SMU)
  double p_dark;       // density calculated from profile 2 function (SMU/kpc^3)
  
  // all matter
  double m_total; // total mass calculated from m_prof1 + m_prof2 (SMU)
  double p_total; // total density calculated from p_prof1 + p_prof2 (SMU/kpc^3)
};

#endif