CXX      = g++
CXXFLAGS = -Wall -std=c++11
LDFLAGS  = -lm

EXECUTABLES = createConfigs.x computeCharge_MC.x computeCorrelation_MC.x 
CFILES      = parameters.cpp file.cpp timestep.cpp lattice.cpp cluster.cpp metropolis.cpp

all: $(EXECUTABLES)


# executable
createConfigs.x : createConfigs.o $(CFILES)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS) 

computeCharge_MC.x : computeCharge_MC.o $(CFILES)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

computeCorrelation_MC.x : computeCorrelation_MC.o $(CFILES)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# all files
%.o: %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) $< -c -o $@

clean : 
	rm -f *.o *.x

