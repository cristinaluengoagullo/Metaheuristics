#ifndef GRASP_HPP
#define GRASP_HPP

#include<iostream>
#include<set>
#include <random>
#include "ProblemInstance.hpp" 
#include "ProblemInstance.inlines.hpp" 
using namespace std;

typedef vector<vector<int> > matrix;

class Grasp {

public:

  Grasp(ProblemInstance& p);

  matrix grasp();

  matrix constructivePhase();

  matrix localSearchPhase(const matrix& solution);

private:

  ProblemInstance& p;
  matrix nStore;
  vector<int> d;
  set<int> rcl;
  set<int> candidates;
  vector<bool> used;
  vector<int> sortedCenters;
  int totalFixedCost;
  vector<int> orders; 
  const int MAX_ITER = 1;

  void candidatesInitialization();

  int greedyCost(int o, const matrix& solutionConstructive);

  int getAmountFreeSpace(int c, int totalStored, int o1, int o2, const matrix& solution);

  pair<int,matrix> findBestNeighbor(const matrix& solution);

  int solutionCost(const matrix& solution) const;

  void printSolution(const matrix& sol);
};

#endif
