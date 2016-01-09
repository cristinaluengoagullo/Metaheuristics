#include <fstream>
#include <iostream>
#include <string.h> 
#include <vector>
#include "Grasp.hpp"

using namespace std;

void parseInput(ifstream& file, int& nOffices, int& nCenters, vector<int>& d, vector<int>& k, vector<int>& f, vector<int>& s, vector<int>& m, vector<vector<bool> >& u) {
  string sAct, sAnt; 
  u.push_back(vector<bool>());
  int i = 0;
  while (getline(file, sAct, ' ')) {    
    if(sAnt.find("no") < sAnt.size()) nOffices = stoi(sAct);
    else if(sAnt.find("nc") < sAnt.size()) nCenters = stoi(sAct);
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
  cout << endl << "*********** " << filename << " ***********" << endl << endl;
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
    ProblemInstance p (d,k,f,s,m,u);
    Grasp g(p);
    matrix sol = g.grasp();
    for(int o = 0; o < sol.size(); o++) {
      cout << "---------- Office " << o << " (" << d[o] << " PBs) ----------" << endl;
      for(int c = 0; c < sol[o].size(); c++) {
	cout << "    Center " << c << " (" << k[c] << " PBs) : " << sol[o][c] << " PBs" << endl;
      }
      cout << "--------------------------------------" << endl << endl;
    }
    cout << endl << "******************************************" << endl << endl;
  }
  else cout << "Unable to open file"; 
}

int main(int argc, char **argv) {
  if(argc > 3 or argc < 3) 
    return showUsageMessage(argv[0]); 
  if(not strcmp(argv[1],"-a")) {
    // Run all tests
    int n = stoi(argv[2]);
    for(int i = 1; i <= n; i ++) {
      processTestFile("../Tests/test" + to_string(i) + ".dat");
    }
  }
  else if(not strcmp(argv[1],"-s")) {
    string filename = argv[2];
    processTestFile(filename);
  }
}
