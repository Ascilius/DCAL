#include <iostream>

#include "frame.h"
#include "profiles.h"

const std::string version = "v5.1";

int main(int argc, char** argv) {
  // input checking
  if (argc < 7 or 12 < argc) {
    std::cerr << "ERROR: Invalid inputs\n\n   Usage:\n";
    std::cerr << "\n  Single: ./dwarf_galaxy_analyzer.exe <i> <nb> <nd> <pb> <pd> <dr> [debug [verbose]]\n";
    std::cerr << "       i: input file\n";
    std::cerr << "      nb: number of baryonic matter bodies\n";
    std::cerr << "      nd: number of dark matter bodies\n";
    std::cerr << "      pb: baryonic matter profile (plummer)\n";
    std::cerr << "      pd: dark matter profile (plummer | nfw | sidm)\n";
    std::cerr << "      dr: bin size in kpc\n";
    std::cerr << "\nMultiple: ./dwarf_galaxy_analyzer.exe <i> <n1> <n2> <n3> <nb> <nd> <pb> <pd> <dr> [debug [verbose]]\n";
    std::cerr << "       i: input directory\n";
    std::cerr << "      n1: first file number\n";
    std::cerr << "      n2: last file number\n";
    std::cerr << "      n3: file number step\n";
    std::cerr << "      nb: number of baryonic matter bodies\n";
    std::cerr << "      nd: number of dark matter bodies\n";
    std::cerr << "      pb: baryonic matter profile (plummer)\n";
    std::cerr << "      pd: dark matter profile (plummer | nfw | sidm)\n";
    std::cerr << "      dr: bin size in kpc\n";
    std::cerr << "\n   debug: on (1) / off (0), verbose: on (1) / off (0)" << std::endl;
    return 1;
  }

  std::cout << "Dwarf Galaxy Analyzer (DGA)\n" << version << "\n";

  // determining mode
  bool multiple = (argc > 9) ? true : false;
  bool debug, verbose; // debug settings

  // analyzing multiple steps
  if (multiple) {
    // getting debug settings
    debug = (argc >= 11) ? ((argv[10][0] == '1') ? true : false) : false;
    verbose = (argc >= 12) ? ((argv[11][0] == '1') ? true : false) : false;
    if (debug) {
      std::cout << "\nDebug: debug: " << debug << "\n";
      std::cout << "Debug: verbose: " << verbose << "\n";
    }

    // getting inputs
    const std::string dir = argv[1];
    
    const int FILE_FIRST = std::stoi(argv[2]);
    const int FILE_LAST = std::stoi(argv[3]);
    const int FILE_STEP = std::stoi(argv[4]);
    
    const int N_BARY = std::stoi(argv[5]);
    const int N_DARK = std::stoi(argv[6]);
    const std::string PROFILE1 = argv[7];
    const std::string PROFILE2 = argv[8];

    const double dr = std::stod(argv[9]);
    
    if (debug) {
      std::cout << "\nDebug: Analyzing multiple steps\n";
      std::cout << "  dir: " << dir << "\n";
      std::cout << "  FILE_FIRST: " << FILE_FIRST << "\n";
      std::cout << "  FILE_LAST: " << FILE_LAST << "\n";
      std::cout << "  FILE_STEP: " << FILE_STEP << "\n";
      std::cout << "  N_BARY: " << N_BARY << "\n";
      std::cout << "  N_DARK: " << N_DARK << "\n";
      std::cout << "  PROFILE1: " << PROFILE1 << "\n";
      std::cout << "  PROFILE2: " << PROFILE2 << "\n";
      std::cout << "  dr: " << dr << "\n";
    }

    // processing multiple files
    int max = 0;
    std::string input_filename = "";
    for (int file_i = FILE_FIRST; file_i <= FILE_LAST; file_i += FILE_STEP) {
      input_filename = dir + std::to_string(file_i);

      Frame frame(input_filename, N_BARY + N_DARK, dr);
      int bins = frame.analyze(debug, verbose);
      if (bins > max) {
        max = bins; // keeping track of max bin size
        if (debug)
          std::cout << "\nDebug: new max: " << max << "\n";
      }
      frame.output_csv(argv[9]);
    }
    if (debug)
      std::cout << "\nDebug: max: " << max << "\n";

    // generating profiles
    Profile profile(PROFILE1, PROFILE2, dr, max);
    profile.gen_profiles(debug, verbose);
    profile.output_profiles(argv[9]);
  }

  // analyzing single step
  else {
    // getting debug settings
    debug = (argc >= 8) ? ((argv[7][0] == '1') ? true : false) : false;
    verbose = (argc >= 9) ? ((argv[8][0] == '1') ? true : false) : false;
    if (debug) {
      std::cout << "\nDebug: debug: " << debug << "\n";
      std::cout << "Debug: verbose: " << verbose << "\n";
    }

    // getting inputs
    const std::string input_filename = argv[1];
    
    const int N_BARY = std::stoi(argv[2]);
    const int N_DARK = std::stoi(argv[3]);
    const std::string PROFILE1 = argv[4];
    const std::string PROFILE2 = argv[5];

    const double dr = std::stod(argv[6]);
    
    if (debug) {
      std::cout << "\nDebug: Analyzing single step\n";
      std::cout << "  input_filename: " << input_filename << "\n";
      std::cout << "  N_BARY: " << N_BARY << "\n";
      std::cout << "  N_DARK: " << N_DARK << "\n";
      std::cout << "  PROFILE1: " << PROFILE1 << "\n";
      std::cout << "  PROFILE2: " << PROFILE2 << "\n";
      std::cout << "  dr: " << dr << "\n";
    }

    // analyzing
    Frame frame(input_filename, N_BARY + N_DARK, dr);
    int max = frame.analyze(debug, verbose);
    frame.output_csv(argv[6]);
    if (debug)
      std::cout << "\nDebug: max: " << max << "\n";

    // generating profiles
    Profile profile(PROFILE1, PROFILE2, dr, max);
    profile.gen_profiles(debug, verbose);
    profile.output_profiles(argv[6]);
  }

  // successful process
  std::cout << "\nDone!" << std::endl;
  return 0;
}