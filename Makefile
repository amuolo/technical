PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

OBJS = technical-assessment.o

ifeq ($(BUILD_MODE),run)
	CFLAGS += -O2
else ifeq ($(BUILD_MODE),linuxtools)
	CFLAGS += -g -pg -fprofile-arcs -ftest-coverage
	LDFLAGS += -pg -fprofile-arcs -ftest-coverage
	EXTRA_CLEAN += technical-assessment.gcda technical-assessment.gcno $(PROJECT_ROOT)gmon.out
	EXTRA_CMDS = rm -rf technical-assessment.gcda
else ifeq ($(BUILD_MODE),debug)
	CFLAGS += -g
else
    $(error Build mode $(BUILD_MODE) not supported by this Makefile)
endif

all:	technical-assessment

technical-assessment:	$(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^
	$(EXTRA_CMDS)

%.o:	$(PROJECT_ROOT)%.cpp
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

%.o:	$(PROJECT_ROOT)%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

clean:
	rm -fr technical-assessment $(OBJS) $(EXTRA_CLEAN)
