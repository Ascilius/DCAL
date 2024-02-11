#include <cmath>
#include <fstream>
#include <iostream>
#include <iomanip> // for cout precision
#include <ostream>
#include <string>
#include <vector>

const int NUM_BODIES = 40000; // TOFIX: make it easier for others to change
const double BODY_MASS = 0.0006;	// mass of each body in solar masses
									// distance/r is in kiloparsecs (kpc)

// -----------------------------------------------------------------------------------------------------------------------------
// for debugging
const bool debug = true;
const bool verbose = false;
std::ostream& operator<<(std::ostream& out, double** data) {
	out << "r, v_los";
	for (int i = 0; i < NUM_BODIES; ++i)
		out << "\n" << data[0][i] << ", " << data[1][i];
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
void add_body(std::ifstream& input_file, double** data, const int i, const double& cx, const double& cy, const double& cz) {
	// getting body coords
	double x = get_coord(input_file);
	double y = get_coord(input_file);
	double z = get_coord(input_file);
	if (debug and verbose)
		std::cout << "Debug: Body: ( " << x << ", " << y << ", " << z << " )\n";

	// calculating r
	double r = sqrt(pow(x-cx, 2) + pow(y-cy, 2) + pow(z-cz, 2));
	if (debug and verbose)
		std::cout << "\tDebug: r: " << r << "\n";
	data[0][i] = r;
}

// actually processing data; TOFIX: jank
void process_data(std::ifstream& input_file, double** data) {
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
		std::cout << "Warning: Milky Way potential detected.\n";
	
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

	// filling data table
	for (int i = 0; i < NUM_BODIES; ++i) {
		if (debug and verbose)
			std::cout << "Debug: i: " << i << "\n";

		input_file >> token; // unused: # ignore
		input_file >> token; // unused: id

		add_body(input_file, data, i, cx, cy, cz);

		input_file >> token; // unused: l
		input_file >> token; // unused: b
		input_file >> token; // unused: r
		input_file >> token; // unused: v_x
		input_file >> token; // unused: v_y
		input_file >> token; // unused: v_z
		input_file >> token; // unused: mass
		
		// radial velocity (?)
		input_file >> token; // v_los
		double v_los = std::stod(token);
		data[1][i] = v_los;
		if (debug and verbose)
			std::cout << "\tDebug: v_los: " << v_los << "\n";
	}

	// done reading; closing input file
	input_file.close();
}

// -----------------------------------------------------------------------------------------------------------------------------
// outputting

void output_data(const std::string& output_filename, double** data) {
	if (debug and verbose)
		std::cout << "Debug: data:\n" << data << "\n";

	// opening output file
	std::ofstream output_file(output_filename);
	if (!output_file.good()) {
		std::cout << "ERROR: Unable to write to \"" << output_filename << "\"!" << std::endl;
		exit(1);
	}

	// writing to output file
	std::cout << "Writing to \"" << output_filename << "\"...\n";
	output_file << "r (kpc), v_los (kpc/s)\n";
	for (int i = 0; i < NUM_BODIES; ++i)
		output_file << data[0][i] << ", " << data[1][i] << "\n";

	output_file.close(); // done writing; close
}

// -----------------------------------------------------------------------------------------------------------------------------
int main(int argc, char** argv) {
	// input checking
	if (argc != 2) {
		std::cout << "ERROR: Invalid inputs\n";
		std::cout << "Usage: ./radvelocity_calculator.exe <input file (.out file)>" << std::endl;
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

	// data table
	double** data = new double*[2];
	data[0] = new double[NUM_BODIES];
	data[1] = new double[NUM_BODIES];

	// processing data
	std::cout << "Reading \"" << input_filename << "\"...\n";
	process_data(input_file, data);

	// writing out to file
	size_t i = input_filename.rfind('.');
	std::string input_filetoken = input_filename.substr(0, i); // getting file name without .out ending
	i = input_filetoken.rfind('/');
	if (i != std::string::npos)
		input_filetoken = input_filetoken.substr(i + 1, input_filetoken.size() - (i + 1));
	std::string output_filename = input_filetoken + ".csv"; // generating new file name
	
	if (debug and verbose)
		std::cout << "Debug: output_filename: " << output_filename << "\n";
	
	output_data(output_filename, data);

	// successful process
	std::cout << "Done!" << std::endl;
	return 0;
}