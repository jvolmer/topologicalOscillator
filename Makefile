CXX      = g++
CXXFLAGS = -Wall -std=c++11
LDFLAGS  = -lm

EXECUTABLES = createConfigs.x computeCharge_MC.x computeCorrelation_MC.x 
OBJECTS     = parameters.o file.o timestep.o lattice.o cluster.o metropolis.o

all: $(EXECUTABLES)

# creating targets
createConfigs.x : createConfigs.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS) 

computeCharge_MC.x : computeCharge_MC.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

computeCorrelation_MC.x : computeCorrelation_MC.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# creating object files
createConfigs.o : parameters.hpp file.hpp timestep.hpp lattice.hpp latticeEquilibrationFactory.hpp cluster.hpp metropolis.hpp
parameters.o    : parameters.hpp
file.o 		: file.hpp parameters.hpp
timestep.o 	: timestep.hpp
lattice.o 	: lattice.hpp parameters.hpp file.hpp timestep.hpp lattice.hpp
cluster.o 	: cluster.hpp latticeEquilibration.hpp
metropolis.o 	: metropolis.hpp latticeEquilibration.hpp


clean : 
	rm -f $(EXECUTABLES) $(OBJECTS)

