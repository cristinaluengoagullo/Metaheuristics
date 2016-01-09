inline ProblemInstance::ProblemInstance(vector<int> _d, vector<int> _k, vector<int> _f, vector<int> _s, vector<int> _m, vector<vector<bool> > _u) {
  u = _u;
  d = _d;
  k = _k;
  f = _f;
  s = _s;
  m = _m;
  for(int i = 0; i < f.size(); i++) sortedCenters.push_back(make_pair(f[i],i));
  sort(sortedCenters.begin(),sortedCenters.end());
}

inline ProblemInstance::ProblemInstance(const ProblemInstance& p) {
  u = p.getAllowedConnections();
  d = p.getDemmands();
  k = p.getCapacities();
  f = p.getFixedCosts();
  s = getSegmentsCosts();
  m = getMinCostsSegments();
}

inline vector<vector<bool> > ProblemInstance::getAllowedConnections() const {
  return u;
}

inline vector<int> ProblemInstance::getDemmands() const {
  return d;
}

inline vector<int> ProblemInstance::getSortedCenters() const {
  vector<int> centers;
  for(auto p : sortedCenters) centers.push_back(p.second);
  return centers;
}

inline vector<int> ProblemInstance::getCapacities() const {
  return k;
}

inline vector<int> ProblemInstance::getFixedCosts() const {
  return f;
}

inline vector<int> ProblemInstance::getSegmentsCosts() const {
  return s;
}

inline vector<int> ProblemInstance::getMinCostsSegments() const {
  return m;
}

inline bool ProblemInstance::connectionAllowed(int c, int o) const {
  return u[c][o];
}

inline int ProblemInstance::getDemmand(int o) const {
  return d[o];
}

inline int ProblemInstance::getCapacity(int c) const {
  return k[c];
}

inline int ProblemInstance::getFixedCost(int c) const {
  return f[c];
}
  
inline int ProblemInstance::getSegmentCost(int pbs) const {
  int cost = s[0];
  for(int i = 0; i < s.size(); i++) {
    if(pbs >= m[i]) {
      cost = s[i];
      break;
    } 
  }
  return cost;
}

inline int ProblemInstance::getMinCostSegment(int s) const {
  return m[s];
}

inline int ProblemInstance::getNOffices() const {
  return d.size();
}

inline int ProblemInstance::getNCenters() const {
  return k.size();
}
