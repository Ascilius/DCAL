#include <cmath>
#include <fstream>
#include <iostream>
#include <ostream>

const bool debug = false;
const bool verbose = true;

// -----------------------------------------------------------------------------------------------------------------------------
// struct for storing data

struct Datum { double r, a, v, p, m; };

std::ostream& operator<<(std::ostream& out, const Datum& dtm) {
	out << "r = " << dtm.r  << ", a = " << dtm.a  << ", v = " << dtm.v << ", p = " << dtm.p << ", m = " << dtm.m;
	return out;
}

// -----------------------------------------------------------------------------------------------------------------------------
// SIDM profile function
double rc, rs, r1, p0, ps;

double SIDM_func(const double& r) {
	double p = -1.0; // if this function returns -1, then something has gone wrong

	// piecewise function; NOTE: it is VERY important that this is correct
	if (r < r1)
		p = p0 / (1 + pow(r / rc, 2));
	else if (r > r1)
		p = ps * rs / (r * pow(1 + (r / rs), 2));

	if (debug)
		std::cout << "Debug: p(" << r << ") = " << p << "\n";
	return p;
}

// calculating mass from density (and r)
void massify(const double& dr, const int& n, Datum* data) {
	for (int i = 0; i < n; ++i) {
		Datum* datum = &(data[i]);
		double r = datum->r;
		double a = 4 * M_PI * pow(r, 2);
		double v = a * dr;
		double m = datum->p * v;

		datum->a = a;
		datum->v = v;
		datum->m = m;

		if (debug and verbose)
			std::cout << "Debug: " << *datum << "\n";
	}
}

// -----------------------------------------------------------------------------------------------------------------------------
// file I/O

// getting parameters from txt file
void get_param() {
	// opening paramters file
	std::ifstream param_file("src/SIDM_parameters.txt");
	if (!param_file.good()) {
		std::cerr << "ERROR: Unable to read \"SIDM_parameters.txt\"!" << std::endl;
		exit(1);
	}

	// reading parameters
	std::cout << "Reading \"SIDM_parameters.txt\"...\n";
	std::cout << "Loading parameters...\n";
	std::string tok;
	
	param_file >> tok;
	rc = std::stod(tok);
	param_file >> tok;
	rs = std::stod(tok);
	param_file >> tok;
	r1 = std::stod(tok);
	param_file >> tok;
	p0 = std::stod(tok);
	param_file >> tok;
	ps = std::stod(tok);
	
	param_file.close();

	// confirming parameters
	std::cout << "SIDM Parameters:\n";
	std::cout << "rc = " << rc << "\n";
	std::cout << "rs = " << rs << "\n";
	std::cout << "r1 = " << r1 << "\n";
	std::cout << "p0 = " << p0 << "\n";
	std::cout << "ps = " << ps	 << "\n";
}

// outputting to csv file
void output_to_file(const std::string& output_filename, const double& n, const Datum* data) {
	std::ofstream output_file(output_filename);
	if (!output_file.good()) {
		std::cerr << "ERROR: Unable to write to \"" << output_filename << "\"!" << std::endl;
		exit(1);
	}

	// writing to file
	std::cout << "Writing to \"" << output_filename << "\"...\n";
	output_file << "#, r (kpc), a (kpc^2), v (kpc^3), density (SMU/kpc^3), mass (SMU)\n";
	for (int i = 0; i < n; ++i) {
		Datum datum = data[i];
		output_file << (i + 1) << ", ";
		output_file << datum.r << ", ";
		output_file << datum.a << ", ";
		output_file << datum.v << ", ";
		output_file << datum.p << ", ";
		output_file << datum.m << "\n";
	}

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
	Datum* data = new Datum[n];
	for (int i = 0; i < n; ++i) {
		Datum* datum = &(data[i]);
		double r = dr * i + dr / 2;
		datum->r = r;
		datum->p = SIDM_func(r);
	}

	// massifying
	massify(dr, n, data);

	// outputting
	std::string output_filename = "output/SIDM_dr" + std::string(argv[1]) + "_n" + std::string(argv[2]) + ".csv";
	output_to_file(output_filename, n, data);
	std::cout << "Done!" << std::endl;

	// memory management
	delete data;

	return 0; // successful process
}