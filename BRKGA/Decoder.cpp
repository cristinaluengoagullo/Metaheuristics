#include "Decoder.hpp"

Decoder::Decoder(ProblemInstance& _p) : p(_p), minCost(numeric_limits<int>::max()) { }

Decoder::~Decoder() { }

double Decoder::decode(const std::vector< double >& chromosome)  {
  matrix nStore(p.getNOffices(),vector<int>(p.getNCenters(),0));
  vector<int> d(p.getDemmands());
  for(int i = 0; i < d.size(); i++) d[i] = d[i]*2;
  vector<pair<int,int> > ranking(chromosome.size());
  matrix solution(p.getNOffices(),vector<int>(p.getNCenters(),0));
  for(int o = 0; o < p.getNOffices(); o++) {
    for(int c = 0; c < p.getNCenters(); c++) 
      ranking[c] = make_pair(greedyCost(o,c)*chromosome[c],c);
    sort(ranking.begin(),ranking.end());
    for(int i = 0; i < p.getNCenters(); i++) {
      int c = ranking[i].second;
      if(not d[o]) {
	solution[o] = vector<int>(nStore[o]);
	break;
      }
      if(p.connectionAllowed(c,o)) {
	int totalStored = 0;
	for(int o = 0; o < p.getNOffices(); o++) 
	  totalStored += solution[o][c];
	if(p.getCapacity(c) - totalStored > 0) {
	  int demmand = p.getDemmand(o);
	  if(d[o] < demmand) demmand = d[o]; 
	  int data = totalStored + demmand;
	  if(data > p.getCapacity(c)) {
	    data = p.getCapacity(c);
	    demmand = p.getCapacity(c) - totalStored;
	  }
	  d[o] -= demmand;
	  nStore[o][c] += demmand;
	}
      }
    }
    if(d[o]) return numeric_limits<int>::max();
  }
  int cost = solutionCost(solution);
  if(cost < minCost) {
    minCost = cost;
    bestFit = solution;
  }
  return cost;
}

matrix Decoder::getBestFit() const {
  return bestFit;
}

int Decoder::greedyCost(int o, int c) const {
  if(not p.connectionAllowed(c,o)) return numeric_limits<int>::max();
  return p.getFixedCost(c);
}

int Decoder::solutionCost(const matrix& solution) const {
  int totalFixed = 0;
  int totalStored = 0;
  for(int c = 0; c < p.getNCenters(); c++) {
    int stored = 0;
    for(int o = 0 ; o < p.getNOffices(); o++) 
      stored += solution[o][c];
    if(p.getSegmentCost(stored) < 0) return numeric_limits<int>::max();
    totalStored += stored*p.getSegmentCost(stored);
    if(stored > 0) totalFixed += p.getFixedCost(c);
  }
  return totalStored + totalFixed;
}

