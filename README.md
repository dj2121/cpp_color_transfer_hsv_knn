# cpp_color_transfer_hsv_knn
C++ implementation of dominant color transfer across images.

Uses KNN clustering across HSV (Hue Saturation based) Color Space to find dominant color.
Transfers the Hue across images.

Source Files: cpp_color_transfer_HSV_KNN.cpp

Compilation:
	g++ 1.cpp -o 1.out `pkg-config --cflags --libs opencv`

	Or use the makefile provided. (type "make" in terminal)
	(OpenCV Ubuntu installation: sudo apt-get install libopencv-dev)


Running:
	./1.out [(optional)	image1.jpg image2.jpg]
	(Or use the output filename given during compilation)

