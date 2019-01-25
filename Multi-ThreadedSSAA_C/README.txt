1. Description

	This is a C program that implements Multi-Threaded Super Sampling Anti Aliasing. The image is stored as an image array and each thread processes small areas of the original image and resizes it. The resulting image will have the same format as the original one, but it will be resize_factor times smaller. Even resize factors use the average value of the initial pixels as the value of the final one and when the resize factor is 3 then the resulting pixel is generated using the 3x3 guassian kernel.

2. Usage

	The program takes the following argumets, in this specific order:
	- the input file
	- the output file
	- the resize factor (must be even or 3)
	- the number of threads to run on

