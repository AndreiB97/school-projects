Programul il citeste pe b, numarul de linii si numarul de coloane.

Programul verifica fiecare numare care trebuie citit astfel incat daca acesta este mai mare sau egal cu 2 la puterea b il va imparti la 2, altfel il transforma in baza 2 si il adauga in coada vectorului v. Dupa ce toate numerele au fost citite va afisa sirul.

Programul verifica daca coordonatele indeplinesc conditile si le modifica unde este necesar, apoi construieste numarul in baza pe baza intervalului [x,y], il afiseaza si ii marcheaza pozitia in matrice cu 1.

Pentru a determina numarul maxim de reuniuni programul foloseste un algoritm similar cu cel de fill astfel:
	- cauta in matrice prima pozitie cu valoarea 1
	- o marcheaza cu 2 si o contorizeaza
	- reia algoritmul pentru pozitile vecine

Dupa determinarea numarului maxim de reuniuni programul reia matricea si afiseaza coordonatele si numarul de baze al reuniunilor
