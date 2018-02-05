Am facut si bonusul.

Programul citeste coordonatele luminii, intensitatea luminii si numarul de copaci.

Coordonatele primilor doi copaci vor fi citite separat, iar coordonatele celui de al treilea vor fi citite separat intr-o instructiune repetitiva.

Verficarea daca un copac este ideal porneste de la al doilea copac astfel:

	Se calculeaza intensitatea luminii in varful sau

	Se va genera numarul format din ultima cifra a partii intregi a intensitatii luminii in varful copacului verificat, ultima cifra a inaltimii copacului din stanga, ultima cifra a inaltimii copacului verificat si ultima cifra a intaltimii copacului din dreapta.

	Numarul generat va fi verificat daca este numar magic. Daca este numar magic atunci 	indicele copacului, inaltimea sa si intensitatea in varful sau vor fi retinute.
	
	Coordonatele vecinilor vor fi retinute pentru aflarea unghiului

Dupa ce al doilea copac este verificat coordonatele primului copac vor deveni coordonatele celui de al doilea, coordonatele celui de al doilea vor deveni coordonatele celui de al treilea, in locul coordonatelor celui de al treilea copac vor fi citite coordonatele urmatorului copac si se va face din nou verificarea celui de al doilea copac.

Daca se va gasi un nou copac ideal atunci ii vom compara inaltimea cu cea a primului copac ideal gasit, daca inaltimea noului copac ideal gasit este mai mica atunci ii vom retine indicele, inaltimea, intensitatea in varful sau si coordonatele vecinilor. Nu este nevoie ca indicele sa fie verificat deoarece parcurgerea se face de la stanga la dreapta, astfel intotdeauna va fi gasit copacul ideal cel mai din stanga.

La final de executie se verifica daca am gasit un copac ideal si ii afisam indicele si intensitatea in varful sau, in caz contrar se afiseaza mesajul "There is no place for you in DwarfLand!".

Se afiseaza unghiul format de varful copacului ideal gasit si vecinii sai, iar daca acesta este obtuz se va afisa si mesajul "IT CAN BE EXTENDED!", in caz contrar va fi afisat mesajul "IT CAN NOT BE EXTENDED!".
