imageProc: 1.cpp
	g++ 1.cpp -o 1.out `pkg-config --cflags --libs opencv`
	./1.out sunflower.jpg rose.jpeg
