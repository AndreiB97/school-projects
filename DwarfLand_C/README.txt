1. Description
	This is a C program about finding the perfect tree for dwarves to live in. The tree is picked according to the amount of sunlight available from it, it's height and it's neighboring trees heights. Once the best tree is found it's index is printed or "There is no place for you in DwarfLand!" is printed if there is no suitable tree. If the tree was found then the dwarves move in and then they check if they can extend their home to the neighboring trees. The program checks if the angle formed by the three tree tops is obtuse, if the angle is obtuse then the home can be extended and an appropriate message is printed.

2. Usage
	The program takes the following input from the user:
	The light source's X (int) and Y (int) coordinates along with it's intensity (float)
	The number of trees (int)
	Each tree top's  X (int) and Y (int) coordinates