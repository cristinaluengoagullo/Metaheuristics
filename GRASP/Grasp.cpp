#include "Grasp.hpp"

Grasp::Grasp(ProblemInstance& _p) : p(_p), d(_p.getDemmands()), used(p.getNCenters(),false), sortedCenters(p.getSortedCenters()), totalFixedCost(0), nStore(p.getNOffices()), orders(p.getNOffices(),0) {}

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
    if(solutionLocal.size()) {
      int costConstructive = solutionCost(solution);
      int costLocal = solutionCost(solutionLocal);
      if(costConstructive > costLocal) {
	solution = solutionLocal;
	optCost = costLocal;
      }
      else optCost = costConstructive;
    }
    else optCost = solutionCost(solution);
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
  int n = 0; 
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
    orders[*it] = n;
    n++;
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
    if(not d[o]) {
      break;
    }
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
  pair<int,matrix> neighbor = findBestNeighbor(solution);
  while(neighbor.first < 0) {
    neighbor = findBestNeighbor(neighbor.second);
  }
  /*if(not neighbor->first) return neighbor->second;
  while(aux->first >= 0) {
     pair<int,matrix>* aux = findBestNeighbor(neighbor.second);
     aux = neighbor;
  }
  return bestSol.second;*/
  return neighbor.second;
}

pair<int,matrix> Grasp::findBestNeighbor(const matrix& solution) {
  vector<vector<bool> > processed(p.getNOffices(),vector<bool>(p.getNOffices(),false));
  matrix neighbor(solution);
  int origCost = solutionCost(solution);
  for(int i = 0; i < p.getNOffices()-1; i++) {
    for(int j = i+1; j < p.getNOffices(); j++) {
      int o1 = i, o2 = j;
      if(orders[i] < orders[j]) {
	o1 = j;
	o2 = i;
      }
      if(not processed[o1][o2]) {
	processed[o1][o2] = true;
	d[o1] = 2*p.getDemmand(o1);
	d[o2] = 2*p.getDemmand(o2);
	nStore = matrix(solution);
	nStore[o1] = vector<int>(p.getNCenters(),0);
	nStore[o2] = vector<int>(p.getNCenters(),0);
	for(int i = 0; i < sortedCenters.size(); i++) {
	  int c = sortedCenters[i];	  int totalStored = 0;
	  int demmand = p.getDemmand(o1);
	  if(not d[o1] and not d[o2]) {
	    break;
	  }
	  for(int o = 0; o < p.getNOffices(); o++) {
	    if(o != o1 and o != o2) totalStored += nStore[o][c];
	  }
	  int storedFromO1 = 0;
	  if(p.connectionAllowed(c,o1)) {
	    demmand = p.getDemmand(o1);
	    if(d[o1] < demmand) demmand = d[o1]; 
	    int data = totalStored + demmand;
	    if(data > p.getCapacity(c)) {
	      data = p.getCapacity(c);
	      demmand = p.getCapacity(c) - totalStored;
	    }
	    d[o1] -= demmand;
	    nStore[o1][c] += demmand;
	    storedFromO1 = demmand;
	  }
	  if(p.connectionAllowed(c,o2)) {
	    totalStored += storedFromO1;
	    demmand = p.getDemmand(o2);
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
	int cost = solutionCost(nStore);
	if(cost < origCost) 
	  return make_pair(cost,nStore);
      }
    }
  }
  return make_pair(-1,matrix());
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
