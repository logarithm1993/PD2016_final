# PD Final Project MakeFile

CXX      := g++
CPPFLAGS := -c -os

SOURCES  := $(addprefix src/,                   \
           main.cpp                             \
           Element.cpp MPTreeSA.cpp             \
           MPTreeMgr.cpp MPTreeIo.cpp           \
           MPTreeTest.cpp utility.cpp           \
			  Temp_OAO.cpp Temp_NZ.cpp Temp_4U.cpp )

OBJECTS   := $(SOURCES:.cpp=.o)
EXECUTABLE:= ./bin/MPTree

# optimized version (DEFAULT)
all : $(SOURCES) $(EXECUTABLE)
	@echo -n ""

# debug version     
# (REMEMBER TO make clean FIRST!!)
dbg : CPPFLAGS = -c -g -DEBUG_ON_
dbg : $(SOURCES) $(EXECUTABLE)
	@echo "[debug_mode is on]"
	@echo "   remember to <make clean> before debug_mode is enabled"
	@echo -n ""

clean :
	rm ./src/*.o ./bin/*

$(EXECUTABLE) : $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ 

.cpp.o :
	$(CXX) $(CPPFLAGS) $< -o $@

