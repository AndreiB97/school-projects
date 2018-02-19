1. Description 
	This is a game where heroes of different classes fight. All the heroes are placed on the map, then each round every heroes can move one unit on the map. When two heroes meet they fight using their two abilities. After the round is over the heroes can leave or continue fighting. If one of the heroes dies, then his opponent receives XP based on the level difference between them. There are four types of terrain on the map: land, volcanic, dessert and woods. Every hero type has a preferred terrain type. There are also four types of heroes: Knight, Pyromancer, Wizard and Rogue. Hero abilities deal more or less damage depending on the type of enemy they fight.

2. Usage
	The program requires an input file as the first argument of the program and an output file as the second argument.
	The input must have two numbers on the first line: the width and the height of the map, followed by the map layout (L for Land, V for Volcanic, D for Dessert, W for Woods), then the number of heroes and each of their type (K for Knight, R for Rogue, P for Pyromancer, W for Wizard) and their coordinates on the map, the number of rounds is next followed by each heroes movement (U for Up, D for Down, L for Left, R for Right or _ to skip movement).
	The output file has the following format: Type Level XP Row Col or Type dead if the hero is dead.
	
3. Example
	For the following input:
	
	1 1
	L
	2
	W 0 0
	R 0 0
	2
	__
	__
	
	The program outputs:
	>W dead
	>R 0 200 340 0 0