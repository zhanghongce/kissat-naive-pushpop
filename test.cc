#include "kissat_pushpop.h"

#include <iostream>

int main() {
  KissatSolver s;
  s.set_option("quiet", 1);

  unsigned total_clause = 0;
  unsigned num_sat = 0, num_unsat = 0;

  unsigned varnum = 10 + (rand() % 256);
  for (int t = 0; t<varnum; ++t)
    s.newVar();
    
  while(true) {
    // randomly decide if add new vars
    if (rand() % 7 == 0) {
      varnum ++;
      s.newVar();
    }


    s.push();

    unsigned num_clause = (rand() % 10) + 1;
    std::cout << "add " << num_clause << " clauses\n";

    for (int i = 0; i< num_clause; ++ i) {
      unsigned clsize = rand() % 10;
      for (unsigned i = 0; i < clsize; ++ i) {
        auto sign = rand() % 2;
        auto var = (rand() % varnum) + 1;
        int lit = sign ? -var : var;
        s.add(lit);
        std::cout << lit << " ";
      }
      s.add(0);
      std::cout << 0 << "\n";
    }

    auto res = s.solve();
    if (res == 20) { // unsat
      s.pop();
      std::cout << "unsat pop \n";
      num_unsat ++;
    } else {
      std::cout << "sat continue \n";
      total_clause += num_clause;
      num_sat ++;
    }
    std::cout << "c: " << total_clause << " v: " << varnum << "\n";
    std::cout << "sat/unsat = " << num_sat << "/" << num_unsat << "\n";

    if (total_clause >= 10000)
      break;

  } // end of while

  return 0;
}
