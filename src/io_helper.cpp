#include <fstream>
#include <string>

#include "io_helper.h"

void clear_line(std::ifstream& param_file) {
  std::string line;
  std::getline(param_file, line);
}

int read_int(std::ifstream& param_file) {
  std::string tok;
  param_file >> tok;
  return std::stoi(tok);
}

double read_double(std::ifstream& param_file) {
  std::string tok;
  param_file >> tok;
  return std::stod(tok);
}