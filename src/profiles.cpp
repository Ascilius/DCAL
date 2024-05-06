#include <cmath>
#include <fstream>
#include <iostream>

#include "io_helper.h"
#include "profiles.h"

Profile::Profile(const std::string p1, const std::string p2, const double d, const int b) {
  // saving arguments
  PROFILE1 = p1;
  PROFILE2 = p2;
  dr = d;
  n_bins = b;

  // loading parameters
  load_params();
}

// -----------------------------------------------------------------------------------------------------------------------------

// loading Plummer profile parameters
void Profile::load_plum1_params() {
  std::string param_filename = "Plummer_parameters_1.txt";
  std::ifstream param_file("param/" + param_filename);
  if (!param_file.good()) {
    std::cerr << "\nERROR: Unable to read \"" << param_filename << "\"!" << std::endl;
    exit(1);
  }

  // reading parameters
  std::cout << "\nReading \"" << param_filename << "\"...\n";
  std::cout << "Loading parameters...\n";
  clear_line(param_file); // M a
  M1 = read_double(param_file);
  a1 = read_double(param_file);
  param_file.close();

  // confirming parameters
  std::cout << "Plummer Parameters:\n";
  std::cout << "M1 = " << M1 << "\n";
  std::cout << "a1 = " << a1 << "\n";
}

// loading secondary Plummer profile parameters
void Profile::load_plum2_params() {
  std::string param_filename = "Plummer_parameters_2.txt";
  std::ifstream param_file("param/" + param_filename);
  if (!param_file.good()) {
    std::cerr << "\nERROR: Unable to read \"" << param_filename << "\"!" << std::endl;
    exit(1);
  }

  // reading parameters
  std::cout << "\nReading \"" << param_filename << "\"...\n";
  std::cout << "Loading parameters...\n";
  clear_line(param_file); // M a
  M2 = read_double(param_file);
  a2 = read_double(param_file);
  param_file.close();

  // confirming parameters
  std::cout << "Plummer Parameters:\n";
  std::cout << "M2 = " << M2 << "\n";
  std::cout << "a2 = " << a2 << "\n";
}

// loading NFW parameters
void Profile::load_nfw_params() {
  std::ifstream param_file = std::ifstream("param/NFW_parameters.txt");
  if (!param_file.good()) {
    std::cerr << "\nERROR: Unable to read \"NFW_parameters.txt\"!" << std::endl;
    exit(1);
  }

  // reading parameters
  std::cout << "\nReading \"NFW_parameters.txt\"...\n";
  std::cout << "Loading parameters...\n";
  clear_line(param_file); // rc rs r1 p0 ps
  nfw_rs = read_double(param_file);
  nfw_p0 = read_double(param_file);
  param_file.close();

  // confirming parameters
  std::cout << "NFW Parameters:\n";
  std::cout << "nfw_rs = " << nfw_rs << "\n";
  std::cout << "nfw_p0 = " << nfw_p0 << "\n";
}

// loading SIDM paramters
void Profile::load_sidm_params() {
  std::ifstream param_file = std::ifstream("param/SIDM_parameters.txt");
  if (!param_file.good()) {
    std::cerr << "\nERROR: Unable to read \"SIDM_parameters.txt\"!" << std::endl;
    exit(1);
  }

  // reading parameters
  std::cout << "\nReading \"SIDM_parameters.txt\"...\n";
  std::cout << "Loading parameters...\n";
  clear_line(param_file); // rc rs r1 p0 ps
  rc = read_double(param_file);
  rs = read_double(param_file);
  r1 = read_double(param_file);
  p0 = read_double(param_file);
  ps = read_double(param_file);
  param_file.close();

  // confirming parameters
  std::cout << "SIDM Parameters:\n";
  std::cout << "rc = " << rc << "\n";
  std::cout << "rs = " << rs << "\n";
  std::cout << "r1 = " << r1 << "\n";
  std::cout << "p0 = " << p0 << "\n";
  std::cout << "ps = " << ps   << "\n";
}

// reading/loading parameters from file
void Profile::load_params() {
  // baryonic matter profile
  if (PROFILE1.compare("plummer") == 0)
    load_plum1_params();
  else {
    std::cerr << "\nERROR: Invalid profile option: \"" << PROFILE1 << "\"" << std::endl;
    exit(1);
  }

  // dark matter profile
  if (PROFILE2.compare("plummer") == 0)
    load_plum2_params();
  else if (PROFILE2.compare("nfw") == 0)
    load_nfw_params();
  else if (PROFILE2.compare("sidm") == 0)
    load_sidm_params();
  else {
    std::cerr << "\nERROR: Invalid profile option: \"" << PROFILE2 << "\"" << std::endl;
    exit(1);
  }
}

// -----------------------------------------------------------------------------------------------------------------------------

// Plummer profile function
double Profile::plum1_func(const double& r) {
  double p = ((3 * M1) / (4 * M_PI * pow(a1, 3))) * pow(1 + ((r*r)/(a1*a1)), -5/2.0);
  return p;
}

// secondary Plummer profile
double Profile::plum2_func(const double& r) {
  double p = ((3 * M2) / (4 * M_PI * pow(a2, 3))) * pow(1 + ((r*r)/(a2*a2)), -5/2.0);
  return p;
}

// NFW profile function
double Profile::nfw_func(const double& r) {
  double p = nfw_p0 / ((r / nfw_rs) * pow(1 + (r / nfw_rs), 2));
  return p;
}

// SIDM profile function
double Profile::sidm_func(const double& r) {
  double p = -1.0; // if this function returns -1, then something has gone wrong

  // piecewise function; NOTE: it is VERY important that this is correct
  if (r < r1)
    p = p0 / (1 + pow(r / rc, 2));
  else if (r > r1)
    p = ps * rs / (r * pow(1 + (r / rs), 2));

  return p;
}

double Profile::prof1(const double& r) {
  // baryonic matter profile
  if (PROFILE1.compare("plummer") == 0)
    return plum1_func(r);
  
  return -1;
}

double Profile::prof2(const double& r) {
  // dark matter profile
  if (PROFILE2.compare("plummer") == 0)
    return plum2_func(r);
  else if (PROFILE2.compare("nfw") == 0)
    return nfw_func(r);
  else if (PROFILE2.compare("sidm") == 0)
    return sidm_func(r);
  
  return -1;
}

// generate profiles into histogram
void Profile::gen_profiles(const bool& debug, const bool& verbose) {
  histogram.clear(); // clearing histo

  for (int i = 0; i < n_bins; ++i) {
    Profile_Bin new_bin;

    // bin bounds
    double r_min = i * dr;
    double r_max = (i+1) * dr;
    
    // calculating volume
    double r = (r_min + r_max) / 2;
    double a = 4 * M_PI * pow(r, 2);
    double v = a * dr;

    // baryonic
    double p_bary = prof1(r);
    double m_bary = p_bary * v;
    
    // dark matter
    double p_dark = prof2(r);
    double m_dark = p_dark * v;

    // totals
    double m_total = m_bary + m_dark;
    double p_total = p_bary + p_dark;

    // saving
    new_bin.r_min = r_min;
    new_bin.r_max = r_max;
    new_bin.r = r;
    new_bin.a = a;
    new_bin.v = v;
    new_bin.m_bary = m_bary;
    new_bin.p_bary = p_bary;
    new_bin.m_dark = m_dark;
    new_bin.p_dark = p_dark;
    new_bin.m_total = m_total;
    new_bin.p_total = p_total;

    // adding to histo
    histogram.push_back(new_bin);
  }

  if (debug) {
    std::cout << "\nDebug: gen_profiles: " << n_bins << " bins generated.\n";
    if (verbose)
      std::cout << "Debug: histogram:\n" << *(this) << "\n";
  }
}

// -----------------------------------------------------------------------------------------------------------------------------
// outputting profile data to csv file
void Profile::output_profiles(const std::string dr_str) {
  // generating filename
  std::string profiles_filename = "output/" + PROFILE1 + "_" + PROFILE2 + "_" + dr_str + ".csv";

  // opening file
  std::ofstream profiles_file(profiles_filename);
  if (!profiles_file.good()) {
    std::cerr << "\nERROR: Unable to open \"" << profiles_filename << "\"!" << std::endl;
    exit(1);
  }
  std::cout << "\nWriting to \"" << profiles_filename << "\"...\n";

  // writing
  profiles_file << *(this) << "\n";

  // done writing
  profiles_file.close();
}

// -----------------------------------------------------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& out, const Profile& profile) {
  // column headers
  out << "r_min (kpc), r_max (kpc), r (kpc), a (kpc^2), v (kpc^3), "; // universal
  out << "m_bary (SMU), p_bary (SMU/kpc^3), "; // baryonic
  out << "m_dark (SMU), p_dark (SMU/kpc^3), "; // dark matter
  out << "m_total (SMU), p_total (SMU/kpc^3)"; // both/all matter

  // actual data
  const std::vector<Profile_Bin> histogram = profile.get_histo();
  int n_bins = histogram.size();
  for (int i = 0; i < n_bins; ++i) {
    const Profile_Bin* bin = &(histogram[i]);

    // universal
    out << "\n" << bin->r_min << ", "; // minimum radial distance from galactic center of mass (kpc)
    out << bin->r_max << ", ";         // maximum radial distance (kpc)
    out << bin->r << ", ";             // distance used for this bin's area/volume/density calculations (kpc)
    out << bin->a << ", ";             // shell area (kpc^2)
    out << bin->v << ", ";             // shell volume (kpc^3)
    
    // baryonic
    out << bin->m_bary << ", ";        // total baryonic mass in this bin (SMU)
    out << bin->p_bary << ", ";        // baryonic density (SMU/kpc^3)
    
    // dark matter
    out << bin->m_dark << ", ";        // total dark mass in this bin (SMU)
    out << bin->p_dark << ", ";        // dark density (SMU/kpc^3)
    
    // all matter
    out << bin->m_total << ", ";       // total mass (SMU)
    out << bin->p_total;               // total density (SMU/kpc^3)
  }

  return out;
}