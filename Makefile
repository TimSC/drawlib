
all: testpng
testpng: testpng.cpp drawlib.cpp drawlibcairo.cpp
	g++ -I/usr/include/pango-1.0 -I/usr/include/glib-2.0 -I/usr/lib/i386-linux-gnu/glib-2.0/include -I/usr/include/cairo testpng.cpp drawlib.cpp drawlibcairo.cpp -lcairo -lglib-2.0 -lgobject-2.0 -lpangocairo-1.0 -lpango-1.0 -o testpng

