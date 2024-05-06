#ifndef PROFILES_H
#define PROFILES_H

#include <ostream>
#include <string>
#include <vector>

#include "profile_bin.h"

class Profile {
public:
  Profile(const std::string p1, const std::string p2, const double d, const int b);

  // specific parameter loading
  void load_plum1_params();
  void load_plum2_params();
  void load_nfw_params();
  void load_sidm_params();

  // loading parameters
  void load_params();

  // profile functions
  double plum1_func(const double& r);
  double plum2_func(const double& r);
  double nfw_func(const double& r);
  double sidm_func(const double& r);

  // generation
  void gen_profiles(const bool& debug, const bool& verbose);

  // output
  std::vector<Profile_Bin> get_histo() const {return histogram;}
  void output_profiles(const std::string dr_str);

private:
  // profiles
  std::string PROFILE1, PROFILE2;
  double dr;
  int n_bins;
  double prof1(const double& r); // redirects to correct profile function
  double prof2(const double& r);

  // parameters
  double M1, a1; // Plummer (https://milkyway.cs.rpi.edu/milkyway/publications/Sid_thesis.pdf)
  double M2, a2; // secondary Plummer profile
  double nfw_rs, nfw_p0; // NFW
  double rc, rs, r1, p0, ps; // SIDM

  // histogram
  std::vector<Profile_Bin> histogram;
};

std::ostream& operator<<(std::ostream& out, const Profile& profile);

#endif