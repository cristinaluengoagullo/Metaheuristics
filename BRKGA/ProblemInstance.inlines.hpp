inline ProblemInstance::ProblemInstance(const vector<int> _d, const vector<int> _k, const vector<int> _f, const vector<int> _s, const vector<int> _m, const vector<vector<bool> > _u) {
  u = _u;
  d = _d;
  k = _k;
  f = _f;
  s = _s;
  m = _m;
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
  int cost = -1;
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
