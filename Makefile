
all: testpng
testpng: testpng.cpp
	g++ testpng.cpp -lcairo -o testpng

