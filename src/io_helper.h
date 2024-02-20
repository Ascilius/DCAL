#ifndef IO_HELPER_H
#define IO_HELPER_H

// helper functions for file input/output
void clear_line(std::ifstream& param_file);
int read_int(std::ifstream& param_file);
double read_double(std::ifstream& param_file);

#endif