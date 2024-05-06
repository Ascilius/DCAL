#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "bin.h"
#include "frame.h"
#include "io_helper.h"

// -----------------------------------------------------------------------------------------------------------------------------
// reading .out file and hashing data into histogram
int Frame::analyze(const bool& debug, const bool& verbose) {
  // opening file
  std::ifstream input_file(input_filename);
  if (!input_file.good()) {
    std::cerr << "\nERROR: Unable to read \"" << input_filename << "\"!" << std::endl;
    exit(1);
  }
  std::cout << "\nReading \"" << input_filename << "\"...\n";

  // getting rid of unused lines
  std::string line;
  std::getline(input_file, line);   // <bodies> (or "cartesian    = 0" depending on the input file)
  if (line[0] == '<')
    std::getline(input_file, line); // cartesian    = 0
  clear_line(input_file);           // lbr & xyz    = 1

  // confirming null potential
  std::getline(input_file, line);   // hasMilkyway = 0/1
  bool mw_pot = line[line.size()-1] == '1'; // presence of Milky Way potential
  if (mw_pot == 1)
    std::cerr << "Warning: Milky Way potential detected!" << std::endl;

  // getting center of mass
  std::string tok;
  input_file >> tok;                // centerOfMass
  input_file >> tok;                // =
  double cx = read_double(input_file);
  double cy = read_double(input_file);
  double cz = read_double(input_file);
  if (debug)
    std::cout << "\nDebug: Center of Mass: ( " << cx << ", " << cy << ", " << cz << " )\n";

	// getting rid of more unused lines
  clear_line(input_file);           // centerOfMomentum = ...
  clear_line(input_file);           // # ignore   id   x   y   z   l   b   r   v_x   v_y   v_z   mass   v_los

  // ---------------------------------------------------------------------------
  // filling histograms
  for (int i = 0; i < n_bodies; ++i) {
    // matter type
    bool dark = read_int(input_file);   // # ignore (0 (baryonic) / 1 (dark matter))
    
    read_int(input_file);               // id (unused)
    
    // location
    double x = read_double(input_file); // x
    double y = read_double(input_file); // y
    double z = read_double(input_file); // z
    
    // more unused numbers
    read_double(input_file);            // l (unused)
    read_double(input_file);            // b (unused)
    read_double(input_file);            // r (unused)
    read_double(input_file);            // v_x (unused)
    read_double(input_file);            // v_y (unused)
    read_double(input_file);            // v_z (unused)
    
    // mass
    double m = read_double(input_file); // mass

    read_double(input_file);            // v_los (unused)

    clear_line(input_file); 		        // clear the rest of the line (TOFIX: out files from older versions of mwah need this but the current version does not)

    if (debug and (i < 5 or verbose))
    	std::cout << "Debug: Body " << (i+1) << ": " << x << " " << y << " " << z << " " << m << "\n";

    // calculating bin index
    double d = sqrt(pow(x - cx, 2) + pow(y - cy, 2) + pow(z - cz, 2));
    int histo_i = d / dr;

    // adding to histogram
    while (histogram.size() <= histo_i) // adding more bins (if needed)
    	histogram.push_back(Bin());
    Bin* bin = &(histogram[histo_i]); // getting bin
    if (dark) {
    	bin->n_dark++;
	    bin->m_dark += m;	
    } else {
	    bin->n_bary++;
	    bin->m_bary += m;	
    }
  }
  if (debug and !verbose)
  	std::cout << "...\n";

  int n_bins = histogram.size();
  std::cout << "\n" << n_bodies << " bodies read into " << n_bins << " bins!\n";
  input_file.close(); // done reading

  // ---------------------------------------------------------------------------
  // verifying (debug only)
  if (debug) {
  	int n_bary_total = 0;
  	int n_dark_total = 0;
  	for (int i = 0; i < n_bins; ++i) {
  		n_bary_total += histogram[i].n_bary;
  		n_dark_total += histogram[i].n_dark;
  	}
	  std::cout << "Debug: " << n_bary_total << " baryonic bodies, " << n_dark_total << " dark matter bodies\n";
  }

  // ---------------------------------------------------------------------------
  // calculating other bin data
  std::cout << "\nCalculating...\nCrunching the numbers...\nDotting our i's and crossing our t's...\n";
  for (int i = 0; i < n_bins; ++i) {
  	Bin* bin = &(histogram[i]);
  	
  	// bin bounds
  	double r_min = i * dr;
  	double r_max = (i+1) * dr;
  	
  	// calculating volume
  	double r = (r_min + r_max) / 2;
	  double a = 4 * M_PI * pow(r, 2);
	  double v = a * dr;

	  // baryonic
	  double p_bary = bin->m_bary / v;
	  
		// dark matter
	  double p_dark = bin->m_dark / v;

	  // saving
	  bin->r_min = r_min;
	  bin->r_max = r_max;
	  bin->r = r;
	  bin->a = a;
	  bin->v = v;
	  bin->p_bary = p_bary;
	  bin->p_dark = p_dark;
	  bin->n_total = bin->n_bary + bin->n_dark;
	  bin->m_total = bin->m_bary + bin->m_dark;
	  bin->p_total = p_bary + p_dark;
	}

  if (debug and verbose)
  	std::cout << "Debug: histogram:\n" << *(this) << "\n";

  return n_bins;
}

// -----------------------------------------------------------------------------------------------------------------------------
// outputting to csv file
void Frame::output_csv(const std::string dr_str) {
	// generating output file name
	size_t i = input_filename.rfind('/');
	std::string output_filename = "output/" + input_filename.substr(i+1) + "_dr" + dr_str + ".csv";
	
	// opening file
	std::ofstream output_file(output_filename);
	if (!output_file.good()) {
    std::cerr << "\nERROR: Unable to open \"" << output_filename << "\"!" << std::endl;
    exit(1);
  }
  std::cout << "\nWriting to \"" << output_filename << "\"...\n";

  // writing
  output_file << *(this) << "\n";

  // done writing
  output_file.close();
}

// -----------------------------------------------------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& out, const Frame& frame) {
	// column headers
	out << "r_min (kpc), r_max (kpc), r (kpc), a (kpc^2), v (kpc^3), "; // universal
	out << "n_bary, m_bary (SMU), p_bary (SMU/kpc^3), "; // baryonic
	out << "n_dark, m_dark (SMU), p_dark (SMU/kpc^3), "; // dark matter
	out << "n_total, m_total (SMU), p_total (SMU/kpc^3)"; // both/all matter

	// actual data
	const std::vector<Bin> histogram = frame.get_histo();
	int n_bins = histogram.size();
	for (int i = 0; i < n_bins; ++i) {
		const Bin* bin = &(histogram[i]);

		// universal
	  out << "\n" << bin->r_min << ", "; // minimum radial distance from galactic center of mass (kpc)
	  out << bin->r_max << ", ";         // maximum radial distance (kpc)
	  out << bin->r << ", ";             // distance used for this bin's area/volume/density calculations (kpc)
	  out << bin->a << ", ";             // shell area (kpc^2)
	  out << bin->v << ", ";             // shell volume (kpc^3)
	  
	  // baryonic
	  out << bin->n_bary << ", ";        // baryonic body count
	  out << bin->m_bary << ", ";        // total baryonic mass in this bin (SMU)
	  out << bin->p_bary << ", ";        // baryonic density (SMU/kpc^3)
	  
	  // dark matter
	  out << bin->n_dark << ", ";        // dark matter body count
	  out << bin->m_dark << ", ";        // total dark mass in this bin (SMU)
	  out << bin->p_dark << ", ";        // dark density (SMU/kpc^3)
	  
	  // all matter
	  out << bin->n_total << ", ";       // total body count
	  out << bin->m_total << ", ";       // total mass (SMU)
	  out << bin->p_total;               // total density (SMU/kpc^3)
	}

	return out;
}