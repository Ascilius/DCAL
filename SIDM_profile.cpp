#include <cmath>
#include <fstream>
#include <iostream>

const bool debug = true;

// -----------------------------------------------------------------------------------------------------------------------------
// SIDM profile function
double param[5]; // rc, rs, r1, p0, ps

double SIDM_func(const double& r) {
	double p = -1.0; // if this function returns -1, then something has gone wrong

	// piecewise function
	if (r < param[2])
		p = param[3] / (1 + pow(r / param[0], 2));
	else if (r > param[2])
		p = param[4] * param[1] / (r * pow(1 + (r / param[1]), 2));

	if (debug)
		std::cout << "Debug: p(" << r << ") = " << p << "\n";
	return p;
}

// -----------------------------------------------------------------------------------------------------------------------------
// file I/O

// getting parameters from txt file
void get_param() {
	// opening paramters file
	std::ifstream param_file("SIDM_parameters.txt");
	if (!param_file.good()) {
		std::cerr << "ERROR: Unable to read \"SIDM_parameters.txt\"!" << std::endl;
		exit(1);
	}

	// reading parameters
	std::cout << "Reading \"SIDM_parameters.txt\"...\n";
	std::cout << "Loading parameters...\n";
	int i = 0;
	std::string tok;
	while (param_file >> tok) { // if there's not the exact number of parameters the program will stop (crash)
		param[i] = std::stod(tok);
		i++;
	}
	param_file.close();

	// confirming parameters
	std::cout << "SIDM Parameters:\n";
	std::cout << "rc = " << param[0] << "\n";
	std::cout << "rs = " << param[1] << "\n";
	std::cout << "r1 = " << param[2] << "\n";
	std::cout << "p0 = " << param[3] << "\n";
	std::cout << "ps = " << param[4] << "\n";
}

// outputting to csv file
void output_to_file(const std::string& output_filename, const double& dr, const double& n, const double* curve) {
	std::ofstream output_file(output_filename);
	if (!output_file.good()) {
		std::cerr << "ERROR: Unable to write to \"" << output_filename << "\"!" << std::endl;
		exit(1);
	}

	// writing to file
	std::cout << "Writing to \"" << output_filename << "\"...\n";
	output_file << "#, r (kpc), density (M_sol/kpc^3)\n";
	for (int i = 0; i < n; ++i)
		output_file << (i + 1) << ", " << (dr * i) << ", " << curve[i] << "\n";

	output_file.close();
}

// -----------------------------------------------------------------------------------------------------------------------------
int main(int argc, char** argv) {
	// input checking
	if (argc != 3) {
		std::cout << "ERROR: Invalid inputs\n";
		std::cout << "Usage: ./SIDM_profile.exe <dr> <# of points>" << std::endl;
		return 1;
	}

	// getting inputs
	double dr = std::stod(argv[1]);
	int n = std::stoi(argv[2]);

	// getting parameters
	get_param();

	// generating curve points
	std::cout << "Calculating...\n";
	double curve[n];
	for (int i = 0; i < n; ++i)
		curve[i] = SIDM_func(dr * i);

	// outputting
	std::string output_filename = "SIDM_dr" + std::string(argv[1]) + "_n" + std::string(argv[2]) + ".csv";
	output_to_file(output_filename, dr, n, curve);
	std::cout << "Done!" << std::endl;

	return 0; // successful process
}