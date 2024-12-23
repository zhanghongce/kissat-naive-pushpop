# first go to subfoler kissat, and ./configure, then cd build, to make:
# libkissat.a

test: kissat/build/libkissat.a test.cc kissat_pushpop.h
	g++ -Ikissat/src test.cc kissat/build/libkissat.a -o test


