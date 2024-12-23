#pragma once

extern "C" {
  #include "kissat.h"
}

#include <vector>
#include <string>
#include <unordered_map>
#include <cassert>

#ifndef ABS
#define ABS(A) ((A) < 0 ? -(A) : (A))

class KissatSolver {
  struct StackItem
  {
    unsigned litptr;
    unsigned nextvar;
    StackItem(unsigned l, unsigned nv) : litptr(l), nextvar(nv) {}
  };
  
protected:
  kissat * solver_;
  unsigned nextvar;
  std::vector<int> lits;
  bool stale_solver_;
  std::vector<StackItem> stack_;
  std::unordered_map<std::string, int> options_;

public:
  KissatSolver() : nextvar(1), stale_solver_(false) {
    solver_ = kissat_init();
  }

  void set_option(const std::string & name, int v) {
    kissat_set_option(solver_, name.c_str(), v);
    options_.emplace(name,v);
  }

  ~KissatSolver() {
    kissat_release(solver_);
  }
  
  unsigned newVar() { return nextvar++; }

  // add to local storage
  void add (int lit) {
    assert(ABS(lit) < nextvar);
    lits.push_back(lit);
  }

  int solve () {
    if (stale_solver_) {
      // recreate a new one
      kissat_release(solver_);
      solver_ = kissat_init();
      for (const auto & opv: options_)
        kissat_set_option(solver_, opv.first.c_str(), opv.second);
    }
    stale_solver_ = true;
    kissat_reserve(solver_, nextvar);

    for (auto lit : lits)
      kissat_add(solver_, lit);
    return kissat_solve(solver_);
  }

  int value (int lit) {
    assert(stale_solver_);
    assert(ABS(lit) < nextvar);
    return kissat_value(solver_, lit);
  }

  void push() {
    // Normally, you should not push if the current clause is not complete yet
    assert(lits.empty() || lits.back() == 0);
    stack_.push_back(StackItem(lits.size(), nextvar));
  }

  void pop() {
    auto old = stack_.back();
    stack_.pop_back();
    lits.resize(old.litptr);
    nextvar = old.nextvar;
  }

};

#undef ABS
#endif
