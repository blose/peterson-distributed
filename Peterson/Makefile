mpicxx = /usr/lib64/openmpi/bin/mpicxx
#CXX_FLAGS = -DNO_MPI -O2 -march=haswell -mfpmath=sse -g0 
CXX_FLAGS =  -O2 -march=haswell -mfpmath=sse -g0 
#-funsafe-math-optimizations -ftree-vectorize
peterson: main.o Method.o Peterson.o Propagator.o Vacuum.o
	$(mpicxx) -lm -o peterson main.o Method.o Peterson.o Propagator.o Vacuum.o
main.o: main.cpp definitions.h Method.h Peterson.h Propagator.h Vacuum.h
	$(mpicxx) $(CXX_FLAGS) -c main.cpp
Method.o: Method.cpp Method.h definitions.h Propagator.h
	$(mpicxx) $(CXX_FLAGS) -c Method.cpp
Peterson.o:  Peterson.cpp Peterson.h Method.h definitions.h Propagator.h
	$(mpicxx) $(CXX_FLAGS) -c Peterson.cpp
Propagator.o: Propagator.cpp Propagator.h definitions.h
	$(mpicxx) $(CXX_FLAGS) -c Propagator.cpp
Vacuum.o: Vacuum.cpp Vacuum.h Propagator.h definitions.h
	$(mpicxx) $(CXX_FLAGS) -c Vacuum.cpp
clean:
	rm -f *.o peterson
