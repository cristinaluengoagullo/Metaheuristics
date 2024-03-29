#ifndef PROBELMINSTANCE_HPP
#define PROBELMINSTANCE_HPP

#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

class ProblemInstance{

public:

  ProblemInstance(const vector<int> d, const vector<int> k, const vector<int> f, const vector<int> s, const vector<int> m, const vector<vector<bool> > u);

  ProblemInstance(const ProblemInstance& p);
  
  vector<vector<bool> > getAllowedConnections() const;
		  
  vector<int> getDemmands() const;

  vector<int> getSortedCenters() const;

  vector<int> getCapacities() const;

  vector<int> getFixedCosts() const;

  vector<int> getSegmentsCosts() const;

  vector<int> getMinCostsSegments() const;

  bool connectionAllowed(int c, int o) const;

  int getCapacity(int c) const;

  int getDemmand(int o) const;

  int getFixedCost(int c) const;
  
  int getSegmentCost(int pbs) const;

  int getMinCostSegment(int s) const;

  int getNOffices() const;

  int getNCenters() const;

private:

  vector<vector<bool> > u;
  vector<int> d;
  vector<int> k;
  vector<int> f;
  vector<int> s;
  vector<int> m;
  vector<pair<int,int> > sortedCenters;

};

#endif
