#include <cmath>
#include <fstream>
#include <iostream>

#include "io_helper.h"
#include "profiles.h"

// parameters
double M, a; // Plummer parameters (https://milkyway.cs.rpi.edu/milkyway/publications/Sid_thesis.pdf)
double rc, rs, r1, p0, ps; // SIDM parameters

// reading/loading parameters from file
void load_params() {
  // loading Plummer parameters
  std::ifstream param_file("param/Plummer_parameters.txt");
  if (!param_file.good()) {
    std::cerr << "\nERROR: Unable to read \"Plummer_parameters.txt\"!" << std::endl;
    exit(1);
  }

  // reading parameters
  std::cout << "\nReading \"Plummer_parameters.txt\"...\n";
  std::cout << "Loading parameters...\n";
  clear_line(param_file); // M a
  M = read_double(param_file);
  a = read_double(param_file);
  param_file.close();

  // confirming parameters
  std::cout << "Plummer Parameters:\n";
  std::cout << "M = " << M << "\n";
  std::cout << "a = " << a << "\n";

  // loading SIDM paramters
  param_file = std::ifstream("param/SIDM_parameters.txt");
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

// Plummer model profile
double plum_func(const double& r) {
  double p = ((3 * M) / (4 * M_PI * pow(a, 3))) * pow(1 + ((r*r)/(a*a)), -5/2.0);
  return p;
}

// SIDM profile function thing
double sidm_func(const double& r) {
  double p = -1.0; // if this function returns -1, then something has gone wrong

  // piecewise function; NOTE: it is VERY important that this is correct
  if (r < r1)
    p = p0 / (1 + pow(r / rc, 2));
  else if (r > r1)
    p = ps * rs / (r * pow(1 + (r / rs), 2));

  return p;
}