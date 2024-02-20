#include <iostream>

#include "frame.h"
#include "profiles.h"

#define N_BODIES 40000

const std::string version = "v5";

int main(int argc, char** argv) {
  // input checking
  if (argc < 3 or 8 < argc) {
    std::cerr << "ERROR: Invalid inputs\nUsage:\n";
    std::cerr << "  Single: ./dwarf_galaxy_analyzer.exe <input file> <dr> [1 (debug) [1 (verbose)]] " << std::endl;
    std::cerr << "  Multiple: ./dwarf_galaxy_analyzer.exe <input directory> <first file number> <last file number> <file number step> <dr> [1 (debug) [1 (verbose)]]" << std::endl;
    return 1;
  }

  // getting debug settings
  bool debug, verbose;
  debug = (argc == 4 or argc == 5) ? std::stoi(argv[3]) : ((argc == 7 or argc == 8) ? std::stoi(argv[6]) : false);
  verbose = (argc == 5) ? std::stoi(argv[4]) : ((argc == 8) ? std::stoi(argv[7]) : false);
  if (debug) {
    std::cout << "Debug: debug: " << debug << "\n";
    std::cout << "Debug: verbose: " << verbose << "\n\n";
  }

  std::cout << "Dwarf Galaxy Analyzer (DGA)\n" << version << "\n";
  load_params(); // loading profile parameters

  // determining mode
  bool multiple = (argc > 5) ? true : false;

  // analyzing multiple steps
  if (multiple) {
    // getting inputs
    const std::string dir = argv[1];
    const int FILE_FIRST = std::stoi(argv[2]);
    const int FILE_LAST = std::stoi(argv[3]);
    const int FILE_STEP = std::stoi(argv[4]);
    const double dr = std::stod(argv[5]);
    if (debug) {
      std::cout << "\nDebug: Analyzing multiple steps\n";
      std::cout << "Debug: dir: " << dir << "\n";
      std::cerr << "Debug: FILE_FIRST: " << FILE_FIRST << "\n";
      std::cerr << "Debug: FILE_LAST: " << FILE_LAST << "\n";
      std::cerr << "Debug: FILE_STEP: " << FILE_STEP << "\n";
      std::cerr << "Debug: dr: " << dr << "\n";
    }

    // processing multiple files
    std::string input_filename = "";
    for (int file_i = FILE_FIRST; file_i <= FILE_LAST; file_i += FILE_STEP) {
      input_filename = dir + std::to_string(file_i);

      Frame frame(input_filename, N_BODIES, dr);
      frame.analyze(debug, verbose);
      frame.output_csv(argv[5]);
    }
  }

  // analyzing single step
  else {
    // getting inputs
    std::string input_filename = argv[1];
    double dr = std::stod(argv[2]);
    if (debug) {
      std::cout << "\nDebug: Analyzing single step\n";
      std::cout << "Debug: input_filename: " << input_filename << "\n";
      std::cout << "Debug: dr: " << dr << "\n";
    }

    // analyzing
    Frame frame(input_filename, N_BODIES, dr);
    frame.analyze(debug, verbose);
    frame.output_csv(argv[2]);
  }

  // successful process
  std::cout << "\nDone!" << std::endl;
  return 0;
}