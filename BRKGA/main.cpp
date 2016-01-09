#include <iostream>
#include <fstream>
#include <string.h>
#include "Decoder.hpp"
#include "MTRand.h"
#include "BRKGA.h"

using namespace std;

void parseInput(ifstream& file, int& nOffices, int& nCenters, vector<int>& d, vector<int>& k, vector<int>& f, vector<int>& s, vector<int>& m, vector<vector<bool> >& u) {
  string sAct, sAnt; 
  u.push_back(vector<bool>());
  int i = 0;
  while (getline(file, sAct, ' ')) {
    if(sAnt.find("o") < sAnt.size()) nOffices = stoi(sAct);
    else if(sAnt.find("c") < sAnt.size()) nCenters = stoi(sAct);
    else if(sAnt.find("d") < sAnt.size() and sAct.find(".") >= sAct.size()) {
      d.push_back(stoi(sAct));
      sAct = sAnt;
    }
    else if(sAnt.find("k") < sAnt.size() and sAct.find(".") >= sAct.size()) {
      k.push_back(stoi(sAct));
      sAct = sAnt;
    }
    else if(sAnt.find("f") < sAnt.size() and sAct.find(".") >= sAct.size()) {
      f.push_back(stoi(sAct));
      sAct = sAnt;
    }
    else if(sAnt.find("s") < sAnt.size() and sAct.find(".") >= sAct.size()) {
      s.push_back(stoi(sAct));
      sAct = sAnt;
    }
    else if(sAnt.find("m") < sAnt.size() and sAct.find(".") >= sAct.size()) {
      m.push_back(stoi(sAct));
      sAct = sAnt;
    }
    else if(sAnt.find("u") < sAnt.size() and sAct.find(".") >= sAct.size()) {
      if(sAct == ",") {
	u.push_back(vector<bool>());
	i++;
      }
      else u[i].push_back(stoi(sAct));
      sAct = sAnt;
    }
    sAnt = sAct;
  }
  file.close();
}

void printParameters(int nOffices, int nCenters, const vector<int>& d, const vector<int>& k, const vector<int>& f, const vector<int>& s, const vector<int>& m, const vector<vector<bool> >& u) {
  cout << "nOffices = " << nOffices << endl;
  cout << "nCenters = " << nCenters << endl;
  cout << "d = [ ";
  for(int i = 0; i < d.size(); i++) cout << d[i] << " ";
  cout << "]" << endl;
  cout << "k = [ ";
  for(int i = 0; i < k.size(); i++) cout << k[i] << " ";
  cout << "]" << endl;
  cout << "f = [ ";
  for(int i = 0; i < f.size(); i++) cout << f[i] << " ";
  cout << "]" << endl;
  cout << "s = [ ";
  for(int i = 0; i < s.size(); i++) cout << s[i] << " ";
  cout << "]" << endl;
  cout << "m = [ ";
  for(int i = 0; i < m.size(); i++) cout << m[i] << " ";
  cout << "]" << endl;
  cout << "u = [";
  for(int i = 0; i < u.size(); i++) {
    cout << "[ ";
    for(int j = 0; j < u[i].size(); j++) {
      cout << u[i][j] << " ";
    }
    cout << "]";
  }
  cout << "]" << endl;
}

int showUsageMessage(const char* arg0) {
  cout << endl << "Usage: " << arg0 << "(-a <# tests> | -s <filename>)" << endl << endl;
  cout << "     -a: Run all tests" << endl;
  cout << "     -s: Run a single test" << endl << endl;
  return 3;
}

void processTestFile(string filename) {
  cout << endl << "**************** " << filename << " ****************" << endl << endl;
  ifstream file(filename);
  if(file.is_open()) {
    int nOffices, nCenters;
    vector<int> d;
    vector<int> k;
    vector<int> f;
    vector<int> s;
    vector<int> m;
    vector<vector<bool> > u;
    parseInput(file,nOffices,nCenters,d,k,f,s,m,u);
    //printParameters(nOffices,nCenters,d,k,f,s,m,u);
    ProblemInstance prob (d,k,f,s,m,u);
    const unsigned n = nCenters; // size of chromosomes
    const unsigned p = 100;	// size of population
    const double pe = 0.20;		// fraction of population to be the elite-set
    const double pm = 0.10;		// fraction of population to be replaced by mutants
    const double rhoe = 0.70;	// probability that offspring inherit an allele from elite parent
    const unsigned K = 1;		// number of independent populations
    const unsigned MAXT = 2;	// number of threads for parallel decoding
	
    Decoder decoder(prob);			// initialize the decoder
	
    const long unsigned rngSeed = 0;	// seed to the random number generator
    MTRand rng(rngSeed);				// initialize the random number generator
	
    // initialize the BRKGA-based heuristic
    BRKGA< Decoder, MTRand > algorithm(n, p, pe, pm, rhoe, decoder, rng, K, MAXT);
	
    unsigned generation = 0;		// current generation
    const unsigned X_INTVL = 100;	// exchange best individuals at every 100 generations
    const unsigned X_NUMBER = 2;	// exchange top 2 best
    const unsigned MAX_GENS = 1000;	// run for 1000 gens
    do {
      algorithm.evolve();	// evolve the population for one generation
		
      if((++generation) % X_INTVL == 0) {
	algorithm.exchangeElite(X_NUMBER);	// exchange top individuals
      }
    } while (generation < MAX_GENS);
	
    if(algorithm.getBestFitness() == numeric_limits<int>::max()) cout << endl << "No solution found! " << endl << endl;
    else {
      matrix sol = decoder.getBestFit();
      cout << endl << "* Optimal cost = " << algorithm.getBestFitness() << " *" << endl << endl;
      for(int o = 0; o < sol.size(); o++) {
	/*cout << "---------- Office " << o << " (" << d[o] << " PBs) ----------" << endl;
	for(int c = 0; c < sol[o].size(); c++) {
	  cout << "    Center " << c << " (" << k[c] << " PBs) : " << sol[o][c] << " PBs" << endl;
	}
	cout << "------------------------------" << endl;*/
      }
    }	
    cout << "*******************************************" << endl;
  }
  else cout << "Unable to open file"; 
}

int main(int argc, char* argv[]) {
  if(argc > 3 or argc < 3) 
    return showUsageMessage(argv[0]); 
  if(not strcmp(argv[1],"-a")) {
    // Run all tests
    int n = stoi(argv[2]);
    for(int i = 1; i <= n; i ++) {
      processTestFile("Tests/test" + to_string(i) + ".dat");
    }
  }
  else if(not strcmp(argv[1],"-s")) {
    string filename = argv[2];
    processTestFile(filename);
  }
}
