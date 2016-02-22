
all: testpng
testpng: testpng.cpp drawlib.cpp
	g++ testpng.cpp drawlib.cpp -lcairo -o testpng

