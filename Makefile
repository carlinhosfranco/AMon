CPP = g++ -std=c++1y
FLAGS = -Wall -O3 
INCLUDE = ./upl/include/upl
SOURCE = apmon.cpp

all: 
	$(CPP) $(FLAGS) -I $(INCLUDE) -L ./upl/lib $(SOURCE) -o apmon -lupl -lm -pthread
clean:
	rm -rf apmon
