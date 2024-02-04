#include <cmath>
#include <fstream>
#include <iostream>
#include <iomanip> // for cout precision
#include <ostream>
#include <string>
#include <vector>

#define NUM_BODIES 40000 // TOFIX: make it easier for others to change

#define BODY_MASS 500 // mass of each body in solar masses
                      // distance/r is in kiloparsecs (kpc)

// -----------------------------------------------------------------------------------------------------------------------------
// for debugging
const bool debug = false;
const bool verbose = false;
std::ostream& operator<<(std::ostream& out, const std::vector<double>& vec) {
	out << "[ ";
	int size = vec.size();
	for (int i = 0; i < size; ++i) {
		if (i != 0)
			out << ", ";
		out << vec[i];
	}
	out << " ]";
	return out;
}

// -----------------------------------------------------------------------------------------------------------------------------
// processing data from input file

// converts string to double from input file
double get_coord(std::ifstream& input_file) {
	// getting string
	std::string token;
	input_file >> token;
	
	// converting to double
	// token = token.substr(0, token.length() - 1); // TOREMOVE: useless? (stod takes care of the comma anyways)
	if (debug and verbose)
		std::cout << "Debug: token: " << token << "\n";
	double num = std::stod(token); // actually converting
	if (debug and verbose)
		std::cout << "Debug: num: " << num << "\n";
	
	return num;
}

// adding body to hash array
void add_body(std::ifstream& input_file, const double& dr, std::vector<double>* hash_array, const double& cx, const double& cy, const double& cz) {
	// getting body coords
	double x = get_coord(input_file);
	double y = get_coord(input_file);
	double z = get_coord(input_file);
	if (debug and verbose)
		std::cout << "Debug: Body: ( " << x << ", " << y << ", " << z << " )\n";

	// calculating r
	double r = sqrt(pow(x-cx, 2) + pow(y-cy, 2) + pow(z-cz, 2));
	if (debug and verbose)
		std::cout << "Debug: r: " << r << "\n";

	// hashing
	int i = int(r / dr);
	if (debug and verbose)
		std::cout << "Debug: i: " << i << "\n";

	// adding to array
	while (i > int(hash_array->size()) - 1) { // adding new histogram section/bar/whatever its called
		hash_array->push_back(0.0);
		if (debug and verbose)
			std::cout << "Debug: New section added.\n";
	}
	(*hash_array)[i] += 1.0; // incrementing
}

// calculates the density for each shell and replaces the values in the hash array
void densify(const double& dr, std::vector<double>* hash_array) {
	int size = hash_array->size();
	for (int i = 0; i < size; ++i) {
		// calculating volume
		double r = dr * (i+1);
		double A = 4 * M_PI * pow(r, 2);
		double V = A * dr;

		// calculating mass and density (assuming all bodies are the same mass)
		double mass = (*hash_array)[i] * BODY_MASS;
		double rho = mass / V;
		(*hash_array)[i] = rho;
	}
}

// actually processing data; TOFIX: jank
void process_data(std::ifstream& input_file, const double& dr, std::vector<double>* hash_array) {
	// getting rid of unused lines
	std::string line;
	std::getline(input_file, line); // <bodies>
	std::getline(input_file, line); // cartesian    = 0
	std::getline(input_file, line); // lbr & xyz    = 1
	
	// confirming null potential
	std::string token;
	input_file >> token; // hasMilkyway
	input_file >> token; // =
	input_file >> token; // 0/1
	if (std::stoi(token) == 1)
		std::cout << "Warning: Null potential detected.\n";
	
	// getting center of mass
	double cx, cy, cz;
	std::cout << std::setprecision(16); // increasing precision of doubles in cout
	input_file >> token; // centerOfMass
	input_file >> token; // =
	cx = get_coord(input_file);
	cy = get_coord(input_file);
	cz = get_coord(input_file);
	if (debug)
		std::cout << "Debug: Center of Mass: ( " << cx << ", " << cy << ", " << cz << " )\n";

	// getting rid of more unused lines
	std::getline(input_file, line); // centerOfMomentum = ...
	std::getline(input_file, line); // # ignore   id   x   y   z   l   b   r   v_x   v_y   v_z   mass   v_los

	// filling hash array
	for (int i = 0; i < NUM_BODIES; ++i) {
		input_file >> token; // unused: # ignore
		input_file >> token; // unused: id

		add_body(input_file, dr, hash_array, cx, cy, cz);

		std::getline(input_file, line); // clearing rest of line
	}

	// done reading; closing input file
	input_file.close();

	// calculating densities
	densify(dr, hash_array);
}

// -----------------------------------------------------------------------------------------------------------------------------
// outputting

void output_hist(const double& dr, std::vector<double>* hash_array, std::string& output_filename) {
	if (debug)
		std::cout << "Debug: hash_array: " << *hash_array << "\n";

	// opening output file
	std::ofstream output_file(output_filename);
	if (!output_file.good()) {
		std::cout << "ERROR: Unable to write to \"" << output_filename << "\"!" << std::endl;
		exit(1);
	}

	// writing to output file
	std::cout << "Writing to \"" << output_filename << "\"...\n";
	// output_file << "dr, " << dr << "\n";
	output_file << "#, r, count\n";
	int size = hash_array->size();
	for (int i = 0; i < size; ++i)
		output_file << (i+1) << ", " << (i*dr) << ", " << (*hash_array)[i] << "\n";

	output_file.close(); // done writing; close
}

// -----------------------------------------------------------------------------------------------------------------------------
int main(int argc, char** argv) {
	// input checking
	if (argc != 3) {
		std::cout << "ERROR: Invalid inputs\n";
		std::cout << "Usage: ./density_calculator.exe <input file (.out file)> <dr>" << std::endl;
		return 1;
	}

	// getting input file name
	std::string input_filename = argv[1];
	if (debug and verbose)
		std::cout << "Debug: input_filename: " << input_filename << "\n";

	// opening input file
	std::ifstream input_file(input_filename);
	if (!input_file.good()) {
		std::cout << "ERROR: Unable to read \"" << input_filename << "\"!" << std::endl;
		return 1;
	}

	// parameters
	const double dr = std::stod(argv[2]);
	if (debug and verbose)
		std::cout << "Debug: dr: " << dr << "\n";

	// counting table
	std::vector<double>* hash_array = new std::vector<double>();

	// processing data
	std::cout << "Reading \"" << input_filename << "\"...\n";
	process_data(input_file, dr, hash_array);

	// writing out to file
	size_t i = input_filename.rfind('.');
	std::string input_filetoken = input_filename.substr(0, i); // getting file name without .out ending
	std::string output_filename = input_filetoken + "_dr" + std::to_string(dr) + ".csv"; // generating new file name
	
	if (debug and verbose)
		std::cout << "Debug: output_filename: " << output_filename << "\n";
	
	output_hist(dr, hash_array, output_filename);

	// successful process
	std::cout << "Done!" << std::endl;
	return 0;
}