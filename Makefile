PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

# Paths
ARMA = /home/$(USER)/installs/armadillo-11.0
BOOST = /home/$(USER)/installs/boost_1_80_0
BLAS = /home/$(USER)/installs/OpenBLAS

# Compilation
PREPROC = -DARMA_DONT_USE_WRAPPER -DARMA_DONT_USE_HDF5 -DARMA_DONT_USE_OPENMP -DARMA_USE_LAPACK -DARMA_USE_BLAS -DARMA_USE_NEWARP -DARMA_NO_DEBUG
INCLUDE = -I$(BLAS)/include -I$(ARMA)/include -I$(ARMA)/include/armadillo_bits -I$(BOOST)/include
LIBS = -ldl -lrt -lm -lz -lopenblaso64 -lgfortran -lquadmath -lboost_system -lboost_mpi -lboost_serialization
USER_LIBS = -L/home/andoras/installs/OpenBLAS/lib -L/home/andoras/installs/boost_1_80_0/lib -L/usr/lib64/openmpi/lib
CFLAGS_BASIC = -std=c++17 -m64 -fmessage-length=0 -pthread -fopenmp -fopenmp-simd

OBJS = technical-assessment.o

ifeq ($(BUILD_MODE),run)
	CFLAGS += $(CFLAGS_BASIC) -O2
else ifeq ($(BUILD_MODE),linuxtools)
	CFLAGS += $(CFLAGS_BASIC) -g -pg -fprofile-arcs -ftest-coverage
	LDFLAGS += -pg -fprofile-arcs -ftest-coverage
	EXTRA_CLEAN += technical-assessment.gcda technical-assessment.gcno $(PROJECT_ROOT)gmon.out
	EXTRA_CMDS = rm -rf technical-assessment.gcda
else #ifeq ($(BUILD_MODE),debug)
	CFLAGS += $(CFLAGS_BASIC) -g
endif

#$(error Build mode $(BUILD_MODE) not supported by this Makefile)

all:	technical-assessment

technical-assessment:	$(OBJS)
	$(CXX) $(USER_LIBS) $(LDFLAGS) -o $@ $^ $(LIBS)
	$(EXTRA_CMDS) 

%.o:	$(PROJECT_ROOT)%.cpp
	$(CXX) -c $(PREPROC) $(INCLUDE) $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

%.o:	$(PROJECT_ROOT)%.c
	$(CC) -c $(PREPROC) $(INCLUDE) $(CFLAGS) $(CPPFLAGS) -o $@ $<

clean:
	rm -fr technical-assessment $(OBJS) $(EXTRA_CLEAN)
