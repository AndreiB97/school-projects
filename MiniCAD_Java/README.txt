1. Description
	This is a Java program capable of drawing simple geometric shapes, such as lines, circles, triangles etc. The program takes an input file that contains the number of shapes that need to be drawn, the properties of the canvas on which the shapes are drawn (size in pixels and color in RGB+A format) and the shapes that need to be drawn along with their properties (location on canvas, size, outline color, fill color). The resulting image is saved in a PNG file.

2. Usage
	The program requires an input file as a CLI argument. It will automatically output to a file named "drawing.png". The input file must first contain the number of shapes that will be drawn (including the canvas), followed by the keyword CANVAS and the canvas properties (width, height, rgb and alpha), then the rest of the shapes.
	Input format for every possible shape:
	LINE x0-coord y0-coord x1-coord y1-coord rgb alpha
	SQUARE x-coord y-coord length outline-rgb outline-alpha fill-rgb fill-alpha
	CIRCLE x-coord y-coord radius outline-rgb outline-alpha fill-rgb fill-alpha
	DIAMOND x-coord y-coord horizontal-diagonal vertical-diagonal outline-rgb outline-alpha fill-rgb fill-alpha
	POLYGON number-of-points x0-coord y0-coord ... xn-coord yn-coord outline-rgb outline-alpha fill-rgb fill-alpha
	RECTANGLE x-coord y-coord height width outline-rgb outline-alpha fill-rgb fill-alpha
	TRIANGLE x0-coord y0-coord x1-coord y1-coord x2-coord y2-coord outline-rgb outline-alpha fill-rgb fill-alpha


3. Example
	Example of input file:

	21

	CANVAS 1080 720 #FFFFA1 100
	
  	TRIANGLE 200 180 200 380 300 280 #E01616 100 #19D332 100

	TRIANGLE 300 280 400 180 400 380 #E01616 100 #19D332 100

	DIAMOND 300 380 200 200 #2F3BB7 100 #E01616 100

	LINE 300 380 165 330 #000000 100

	LINE 300 380 165 370 #000000 100

	LINE 300 380 190 430 #000000 100

	LINE 300 380 435 330 #000000 100

	LINE 300 380 435 370 #000000 100

	LINE 300 380 410 430 #000000 100

	DIAMOND 350 555 250 250 #2F3BB7 100 #E01616 100

	TRIANGLE 350 430 350 780 520 605 #00FFFF 100 #19D332 100

	POLYGON 5 375 455 520 605 520 810 375 810 350 780 #f442ee 100 #f442ee 100

	POLYGON 5 400 480 520 605 520 810 375 810 350 780 #00FFFF 100 #19D332 100

	POLYGON 5 425 505 520 605 520 810 375 810 350 780 #f442ee 100 #f442ee 100

	POLYGON 5 450 530 520 605 520 810 375 810 350 780 #00FFFF 100 #19D332 100

	POLYGON 5 475 555 520 605 520 810 375 810 350 780 #f442ee 100 #f442ee 100

	POLYGON 5 500 580 520 605 520 810 375 810 350 780 #00FFFF 100 #19D332 100

	POLYGON 5 525 605 520 605 520 810 375 810 350 780 #f442ee 100 #f442ee 100

	TRIANGLE 375 810 350 780 290 810 #00FFFF 100 #19D332 100

	POLYGON 4 520 810 560 700 650 610 625 725 #f442ee 100 #f442ee 100 

	A PNG file named "drawing.png" will contain the resulting picture
	
