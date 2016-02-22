
all: testpng
testpng: testpng.cpp drawlib.cpp drawlibcairo.cpp
	g++ testpng.cpp drawlib.cpp drawlibcairo.cpp -lcairo -o testpng

