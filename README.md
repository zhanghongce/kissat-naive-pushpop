# kissat-naive-pushpop

Because currently kissat does not support incremental solving,
this is a wrapper around kissat to store CNF, and support push/pop.
When solving, it just inserts stored CNF dynamically into a new kissat solver.

In the future, will try to extract learnt clauses from the previous run.

# usage
See 'test.cc'
