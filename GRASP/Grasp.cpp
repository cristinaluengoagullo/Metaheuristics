#include "Grasp.hpp"

Grasp::Grasp(ProblemInstance& _p) : p(_p), d(_p.getDemmands()), used(p.getNCenters(),false), sortedCenters(p.getSortedCenters()), totalFixedCost(0) {
  nStore.resize(p.getNOffices());
}

matrix Grasp::grasp() {
  matrix solution(p.getNOffices(),vector<int>(p.getNCenters()));
  int optCost;
  for(int i = 0; i < MAX_ITER; i++) {
    solution = constructivePhase();
    if(not solution.size()) {
      cout << endl << "No solution found! " << endl << endl;
      return solution;
    }
    matrix solutionLocal = localSearchPhase(solution);
    int costConstructive = solutionCost(solution);
    int costLocal = solutionCost(solutionLocal);
    if(costConstructive > costLocal) {
      solution = solutionLocal;
      optCost = costLocal;
    }
    else optCost = costConstructive;
  }
  cout << endl << "* Optimal cost = " << optCost << " *" << endl << endl;
  //cout << endl << "* Optimal cost = " << solutionCost(solution) << " *" << endl << endl;
  return solution;
}

matrix Grasp::constructivePhase() {
  matrix solution(p.getNOffices(),vector<int>(p.getNCenters(),0));
  for(int o = 0; o < p.getNOffices(); o++) nStore[o] = vector<int>(p.getNCenters(),0);
  for(int i = 0; i < d.size(); i++) d[i] = p.getDemmand(i)*2;
  candidates = set<int>();
  candidatesInitialization();
  while(candidates.size() > 0) {
    int i = 0; 
    int max = 0, min = numeric_limits<int>::max();
    vector<int> costs(candidates.size());
    set<int>::iterator it;
    for(it = candidates.begin(); it != candidates.end(); it++) {
      d[*it] = p.getDemmand(*it)*2;
      nStore[*it] = vector<int>(p.getNCenters(),0);
      int gCost = greedyCost(*it,solution);
      if(gCost == -1) 
	return vector<vector<int> >();
      if(gCost > max) max = gCost;
      if(gCost < min) min = gCost;
      costs[i] = gCost;
      i++;
    }
    int cost = min + 0.5*(max-min); 
    i = 0;
    rcl = set<int>();
    for(it = candidates.begin(); it != candidates.end(); it++) {
      if(costs[i] <= cost) 
	rcl.insert(*it);
      i++;
    }
    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> rand(0,rcl.size()-1);
    int r = rand(mt);
    i = 0;
    for(it = rcl.begin(); it != rcl.end() and i < r; it++) i++;
    if(i > 0) it--; 
    solution[*it] = vector<int>(nStore[*it]);
    for(int c : solution[*it]) {
      if(solution[*it][c] > 0 and not used[c]) {
	used[c] = true;
	totalFixedCost += p.getFixedCost(c);
      }
    }
    candidates.erase(*it);
  }
  return solution;
}

void Grasp::candidatesInitialization() {
  for(int o = 0; o < p.getNOffices(); o++) 
    candidates.insert(o);
}

int Grasp::greedyCost(int o, const matrix& solution) {
  int cost = numeric_limits<int>::max();
  for(int i = 0; i < sortedCenters.size(); i++) {
    int c = sortedCenters[i];
    if(not d[o]) 
      break;
    if(p.connectionAllowed(c,o)) {
      int totalStored = 0;
      for(int o = 0; o < p.getNOffices(); o++) {
	totalStored += solution[o][c];
      }
      int demmand = p.getDemmand(o);
      if(d[o] < demmand) demmand = d[o]; 
      int data = totalStored + demmand;
      if(data > p.getCapacity(c)) {
	data = p.getCapacity(c);
	demmand = p.getCapacity(c) - totalStored;
      }
      d[o] -= demmand;
      int fixedCost = totalFixedCost;
      if(not used[c]) fixedCost += p.getFixedCost(c);
      if(p.getSegmentCost(data) < 0) return -1;
      cost = data*p.getSegmentCost(data) + fixedCost;
      nStore[o][c] += demmand;
    }
  }
  if(d[o]) 
    return -1;
  return cost;
}

matrix Grasp::localSearchPhase(const matrix& solution) {
  pair<int,matrix> aux = findBestNeighbor(solution);
  pair<int,matrix> neighbor;
  while(aux.first >= 0) {
    neighbor = findBestNeighbor(aux.second);
    aux = neighbor;
  }
  return neighbor.second;
}

pair<int,matrix> Grasp::findBestNeighbor(const matrix& solution) {
  matrix neighbor(solution);
  int origCost = solutionCost(solution);
  for(int o1 = 0; o1 < p.getNOffices(); o1++) {
    // Simulacro de q los centros en los q esta conectado desaparece su demanda
    // y ver si se puede cubrir la demanda de la oficina en cuestion con esos centros
    // (a los q se pueda conectar teniendo en cuenta q la otra oficina ya no esta)
    // Que tengan almenos un centro diferente.
    for(int o2 = 0; o2 < p.getNOffices(); o2++) {
      if(o1 != o2) {
	bool atLeastOneDiff = false;
	int free1 = 0, free2 = 0;
	for(int c = 0; c < p.getNCenters(); c++) {
	  if(not(solution[o1][c] > 0 and solution[o2][c] > 0)) 
	    atLeastOneDiff = true;
	  int totalStored = 0;
	  for(int o = 0; o < p.getNOffices(); o++) 
	    totalStored += solution[o][c];
	  if(p.connectionAllowed(c,o1)) {
	    free1 += p.getCapacity(c) - totalStored;
	    if(solution[o2][c] > 0) free1 += solution[o2][c];
	    if(solution[o1][c] > 0) free1 += solution[o1][c];
	  }
	  if(p.connectionAllowed(c,o2)) {
	    free2 += p.getCapacity(c) - totalStored;
	    if(solution[o1][c] > 0) free2 += solution[o1][c];
	    if(solution[o2][c] > 0) free2 += solution[o2][c];
	  }
	}
	if(free1 >= p.getDemmand(o1) and free2 >= p.getDemmand(o2) and atLeastOneDiff) {
	  d[o1] = p.getDemmand(o1)*2;
	  d[o2] = p.getDemmand(o2)*2;
	  nStore[o1] = vector<int>(p.getNCenters(),0);
	  nStore[o2] = vector<int>(p.getNCenters(),0);
	  matrix auxSol(solution);
	  for(int i = 0; i < sortedCenters.size(); i++) {
	    int c = sortedCenters[i];
	    if(not d[o1]) {
	      neighbor[o1] = nStore[o1];
	      auxSol[o1] = nStore[o1];
	      break;
	    }
	    if(p.connectionAllowed(c,o1)) {
	      nStore[o1][c] = 0;
	      int totalStored = 0;
	      for(int o = 0; o < p.getNOffices(); o++) {
		if(o != o1 and o != o2) totalStored += auxSol[o][c];
	      }
	      int demmand = p.getDemmand(o1);
	      if(d[o1] < demmand) demmand = d[o1]; 
	      int data = totalStored + demmand;
	      if(data > p.getCapacity(c)) {
		data = p.getCapacity(c);
		demmand = p.getCapacity(c) - totalStored;
	      }
	      d[o1] -= demmand;
	      nStore[o1][c] += demmand;
	    }
	  }
	  for(int i = 0; i < sortedCenters.size(); i++) {
	    int c = sortedCenters[i];
	    if(not d[o2]) {
	      neighbor[o2] = nStore[o2];
	      break;
	    }
	    if(p.connectionAllowed(c,o2)) {
	      nStore[o2][c] = 0;
	      int totalStored = 0;
	      for(int o = 0; o < p.getNOffices(); o++) {
		if(o != o2) totalStored += auxSol[o][c];
	      }
	      int demmand = p.getDemmand(o2);
	      if(d[o2] < demmand) demmand = d[o2]; 
	      int data = totalStored + demmand;
	      if(data > p.getCapacity(c)) {
		data = p.getCapacity(c);
		demmand = p.getCapacity(c) - totalStored;
	      }
	      d[o2] -= demmand;
	      nStore[o2][c] += demmand;
	    }
	  }
	}
	int neighborCost = solutionCost(neighbor); 
	if(neighborCost < origCost) {
	  return make_pair(neighborCost,neighbor);
	}
      }
    }
  }
  int neighborCost = solutionCost(neighbor); 
  if(neighborCost >= origCost) neighborCost = -1;
  return make_pair(neighborCost,neighbor);
}

int Grasp::solutionCost(const matrix& solution) const {
  int totalFixed = 0;
  int totalStored = 0;
  for(int c = 0; c < p.getNCenters(); c++) {
    int stored = 0;
    for(int o = 0 ; o < p.getNOffices(); o++) 
      stored += solution[o][c];
    totalStored += stored*p.getSegmentCost(stored);
    if(stored > 0) totalFixed += p.getFixedCost(c);
  }
  return totalStored + totalFixed;
}

void Grasp::printSolution(const matrix& sol) {
  for(int o = 0; o < sol.size(); o++) {
    cout << "---------- Office " << o << " (" << p.getDemmand(o) << " PBs) ----------" << endl;
    for(int c = 0; c < sol[o].size(); c++) {
      cout << "    Center " << c << " (" << p.getCapacity(c) << " PBs) : " << sol[o][c] << " PBs" << endl;
    }
    cout << "------------------------------" << endl;
  }
}
