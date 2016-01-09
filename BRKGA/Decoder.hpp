
#ifndef DECODER_HPP
#define DECODER_HPP

#include <list>
#include <vector>
#include <algorithm>
#include "ProblemInstance.hpp"
#include "ProblemInstance.inlines.hpp"

using namespace std;

typedef vector<vector<int> > matrix;

class Decoder {

public:
  Decoder(ProblemInstance& p);

  ~Decoder();

  double decode(const std::vector< double >& chromosome);

  matrix getBestFit() const;

private:

  ProblemInstance& p;
  
  matrix bestFit;

  int minCost;

  int greedyCost(int o, int c) const;

  int solutionCost(const matrix& solution) const;
};

#endif
