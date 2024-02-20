#ifndef FRAME_H
#define FRAME_H

#include <ostream>
#include <string>
#include <vector>

#include "bin.h"

class Frame {
public:
  Frame(const std::string f, const int n, const double d): input_filename(f), n_bodies(n), dr(d) {};
  void analyze(const bool& debug, const bool& verbose);
  void output_csv(const std::string dr_str);

  std::vector<Bin> get_histo() const {return histogram;}
private:
  std::string input_filename;
  int n_bodies;
  double dr;

  // bins (histogram)
  std::vector<Bin> histogram;
};

std::ostream& operator<<(std::ostream& out, const Frame& frame);

#endif