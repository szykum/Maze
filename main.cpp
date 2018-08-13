#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <stdio.h>
#include "conio2.h"

#include <time.h>

#define SIZE    15																//SIZE i SIZE2 to wymiary labiryntu
#define SIZE2   21																//SIZE3 i SIZE4 to wymiary perspektywy
#define SIZE3   28
#define SIZE4   50



using namespace std;

struct wsp{																		//stuktura, ktora wykorzystam w funkcji
	int x;																		//zeby zwracala mi obecne polozenie gracza
	int y;																		//oraz czy wartosc licznika krokow
	int licz;
};

void rysuj_lab(char tab[SIZE][SIZE2]){											//funkcja rysujaca domyslny labirynt

	for (int i = 0; i < SIZE; i++){
		for (int j = 0; j < SIZE2; j++){
			if (tab[i][j] != '#') cout << " ";
			else{
				if (i >= 0 && i < 3) cout << (char)64;							//rysowanie roznych stylow scian
				else if (i >= 3 && i < 6) cout << (char)36;						//za pomoca kodow ASCII
				else if (i >= 6 && i < 9) cout << (char)37;
				else if (i >= 9 && i < 12) cout << (char)38;
				else cout << (char)35;
			}
		}
		cout << endl;
	}
}

char zmiana_zwrotu(int zn, char kursor){										 //funkcja zmieniajaca zwrot

	if (zn == 0x4b){															 //zmiana zwrotu kursora (strzalka w lewo)
		if (kursor == 'v') kursor = '>';
		else if (kursor == '>') kursor = '^';
		else if (kursor == '^') kursor = '<';
		else if (kursor == '<') kursor = 'v';
	}
	else if (zn == 0x4d){														  //zmiana zwrotu kursora (strzalka w prawo)
		if (kursor == 'v') kursor = '<';
		else if (kursor == '>') kursor = 'v';
		else if (kursor == '^') kursor = '>';
		else if (kursor == '<') kursor = '^';
	}
	return kursor;
}

wsp poruszanie(char tab[SIZE][SIZE2], int x, int y, char zn, char kurs){         //funkcja do poruszania

	wsp cords;
	cords.x = x;
	cords.y = y;
	cords.licz = 0;
	int xt = x, yt = y;
	if (zn == 0x48){															//ruch do przodu
		if (kurs == 'v'){	
			if (tab[y][x - 1] == '0' || tab[y][x - 1] == 'm') cords.y++;		//sprawdzanie czy nastapi kolizja
		}																		//jesli nie to zmieniamy polozenie
		else if (kurs == '>'){
			if (tab[y - 1][x] == '0' || tab[y - 1][x] == 'm') cords.x++;
		}
		else if (kurs == '^'){
			if (tab[y - 2][x - 1] == '0' || tab[y - 2][x - 1] == 'm')cords.y--;
		}
		else if (kurs == '<') {
			if (tab[y - 1][x - 2] == '0' || tab[y - 1][x - 2] == 'm')cords.x--;
		}
		if (xt != cords.x || yt != cords.y) cords.licz++;						 //zwiekszamy licznik gdy zmieni sie polozenie kursora
	}
	else if (zn == 0x50){														 //ruch do tylu
		if (kurs == 'v')
		{
			if (tab[y - 2][x - 1] == '0' || tab[y - 2][x - 1] == 'm'){
				if (x == 5 && y == 1){
					cords.x = 5;
					cords.y = 1;
				}
				else cords.y--;
			}
		}
		else if (kurs == '>'){
			if (tab[y - 1][x - 2] == '0' || tab[y - 1][x - 2] == 'm')cords.x--;
		}
		else if (kurs == '^'){
			if (tab[y][x - 1] == '0' || tab[y][x - 1] == 'm') cords.y++;
		}
		else if (kurs == '<'){
			if (tab[y - 1][x] == '0' || tab[y - 1][x] == 'm') cords.x++;
		}
		if (xt != cords.x || yt != cords.y) cords.licz++;
	}
	return cords;																//zwracamy nowe polozenie gracza 
}																				//i licznik krokow

double czas(double koniec, double start){									//funkcja liczaca roznice czasu od poczatku
	double roznica = 0;														//do konca pomiaru
	roznica = difftime(koniec, start);
	return roznica;
}

void tablica_domyslna(char tablica[SIZE][SIZE2]){							//funkcja ktora otwiera plik tekstowy z				
																			//przykladowym labiryntem i przypisuje jego
	char tab[SIZE][SIZE2];													//wartosci do tablicy dwuwymiarowej
	gotoxy(1, 1);
	FILE *plik = fopen("plik.txt", "r");
	if (plik == NULL)       printf("plik jest pusty, lub nie istnieje");
	else {
		for (int i = 0; i < SIZE; i++){
			for (int j = 0; j < SIZE2; j++){
				fscanf(plik, "%c", &tab[i][j]);
			}
		}
		fclose(plik);
	}
	for (int i = 0; i < SIZE; i++){
		for (int j = 0; j < SIZE2; j++) tablica[i][j] = tab[i][j];
	}

}

void tablica_nazwa(char tablica[SIZE][SIZE2], char nazwa[50]){				//funkcja, ktora otwiera plik o podanej przez
																			//gracza nazwie i przypisuje labirynt do
	char tab[SIZE][SIZE2];													//do tablicy dwuwymiarowej
	gotoxy(1, 1);
	FILE *plik = fopen(nazwa, "r");
	if (plik == NULL)       printf("plik jest pusty, lub nie istnieje");
	else {
		for (int i = 0; i < SIZE; i++){
			for (int j = 0; j < SIZE2; j++){
				fscanf(plik, "%c", &tab[i][j]);
			}
		}
		fclose(plik);
	}
	for (int i = 0; i < SIZE; i++){
		for (int j = 0; j < SIZE2; j++) tablica[i][j] = tab[i][j];
	}

}


void zapisanie_pliku(char tablica[SIZE][SIZE2], char nazwa[50]){			//funkcja, ktora zapisuje obecny labirynt
																			//do pliku o nazwie podanej przez gracza
																			
	FILE *plik = fopen(nazwa, "w");
		for (int i = 0; i < SIZE; i++){
			for (int j = 0; j < SIZE2; j++){
				fprintf(plik, "%c", tablica[i][j]);
			}
		}
		fclose(plik);
	
}

void menu(int licz_kr, double licz_czas){									//funkcja wyswietlajaca menu

	textcolor(7);
	gotoxy(55, 1);
	cputs("q = wyjscie");
	gotoxy(55, 2);
	cputs("strzalki = poruszanie");
	gotoxy(55, 3);
	cputs("spacja = zmiana koloru");
	gotoxy(25, 2);
	cout << "Wykonane kroki:\t" << licz_kr;
	gotoxy(25, 3);
	cout << "Czas:\t\t" << licz_czas;
	gotoxy(55, 4);
	cputs("i = wczytaj dom. lab.");
	gotoxy(55, 5);
	cputs("o = podaj nazwe pliku");
	gotoxy(55, 6);
	cputs("s = zapisz do pliku");
	gotoxy(55, 7);
	cputs("e = edytor labiryntu");
	gotoxy(55, 8);
	cputs("r = reset");
	gotoxy(5, 1);
}

int sprawdz_labirynt(char tab[SIZE][SIZE2]){							//funkcja sprawdzajaca czy formatowanie jest poprawne
	int bledy = 0;
	for (int i = 0; i < SIZE; i++){
		for (int j = 0; j < SIZE2; j++){
			if (tab[i + 1][j] == '0' && tab[i + 2][j] == '0' && tab[i + 1][j + 1] == '0' && tab[i + 2][j + 1] == '0')
				bledy++;
		}
	}																	//jezeli formatowanie jest poprawne to 
	return bledy;														//funkcja zwroci 0 czyli 0 bledow
}

void edytor(char tablica[SIZE][SIZE2]){										//edytor labiryntu
	
	int x = 1;
	int y = 1;
	int zn;
	do{
		clrscr();
		menu(0, 0);
		gotoxy(25, 6);
		cout << "d = dodaj/usun sciane";
		gotoxy(25, 7);
		cout << "e = wyjdz z edytora"; 
		gotoxy(25, 8);
		cout << "s = zapisz zmiany";
		if (sprawdz_labirynt(tablica) == 0){								//sprawdzamy czy labirynt jest
			gotoxy(5, 16);													//formatowany poprawnie
			cout << "Dobry Format";
		}
		else{
			gotoxy(5, 16);
			cout << "Nieprawidlowy Format";
		}

		gotoxy(1, 1);
		rysuj_lab(tablica);
		gotoxy(x, y);
		zn = getch();
		if (zn == 0x48) y--;												//poruszanie sie kursora po ekranie
		else if (zn == 0x50) y++;											//w edytorze
		else if (zn == 0x4b) x--;
		else if (zn == 0x4d) x++;
		else if (zn == 'd'){
			if (x < SIZE2 - 1 && y < SIZE && x>1 && y>1){					//po wcisnieciu d sciana albo sie pojawia albo ginie
				if (tablica[y - 1][x - 1] == '0')tablica[y - 1][x - 1] = '#';
				else if (tablica[y - 1][x - 1] == '#')tablica[y - 1][x - 1] = '0';
			}
		}
		else if (zn == 's'){												//zapisujemy wyniki do pliku edytor.txt
			if (sprawdz_labirynt(tablica) == 0) zapisanie_pliku(tablica, "edytor.txt");	//jezeli jest sformatowany poprawnie
			}	
		
	} while (zn != 'e');
	if (sprawdz_labirynt(tablica) != 0)tablica_domyslna(tablica);			//jezeli po wyjsciu z edytora formatowanie bylo
}																			//zle, ladujemy domyslny labirynt

int sprawdz_czy_istnieje(char tab[SIZE][SIZE2]){							//funkcja, ktora sprawdza czy ostatnio zostal 
																			//zapisany jakis labirynt po uzyciu edytora
	gotoxy(1, 1);
	FILE *plik = fopen("edytor.txt", "r");
	if (plik == NULL) {
		printf("Brak zapisanych labiryntow z edytora");
		return 0;
	}
	else {
		for (int i = 0; i < SIZE; i++){
			for (int j = 0; j < SIZE2; j++){
				fscanf(plik, "%c", &tab[i][j]);
			}
		}
		fclose(plik);
		return 1;
	}

}


int sprawdz_glebokosc(char labirynt[SIZE][SIZE2],char pers[SIZE3][SIZE4], char kursor,int x,int y){		//funkcja sprawdzajaca 
	int licz = 0;																						//odleglosc miedzy graczem
																										//a sciana z przodu
	if (kursor == 'v'){
		while (labirynt[y][x - 1] == 's' || labirynt[y][x - 1] == 'm' || labirynt[y][x - 1] == '0'){
			licz++;
			y++;
		}
	}
	else if (kursor == '>'){
		while (labirynt[y - 1][x] == '0' || labirynt[y - 1][x] == 'm' || labirynt[y - 1][x] == 's'){
			licz++;
			x++;
		}
	}
	else if (kursor == '<'){
		while (labirynt[y - 1][x - 2] == '0' || labirynt[y - 1][x - 2] == 'm' || labirynt[y - 1][x - 2] == 's'){
			licz++;
			x--;
		}
	}
	else if (kursor == '^'){
		while (labirynt[y - 2][x - 1] == '0' || labirynt[y - 2][x - 1] == 'm' || labirynt[y - 2][x - 1] == 's'){
			licz++;
			y--;
		}
	}
	return licz;
}

void rysuj_3d(char pers[SIZE3][SIZE4], char labirynt[SIZE][SIZE2], int x, int y){			//funkcja, ktora otrzymuje i rysuje
	int x1 = 23, y1 = 9;																	//tablice perspektywy
	for (int i = 0; i < SIZE3; i++){
		gotoxy(x1, y1);
		for (int j = 0; j < SIZE4; j++){
			if(pers[i][j]=='0') cout << " " ;
			else cout << pers[i][j];
		}
		y1++;
	}
}

int sciana_lewa(char labirynt[SIZE][SIZE2], char kursor, int x, int y, int z){			//funkcja, ktora sprawdza podczas rysowania
	int wartosc;
	if (kursor == 'v'){																	//perspektywy czy w danym momencie po
		if (labirynt[y - 1 + z][x] != '#') wartosc = 1;									//lewej stronie znajduje sie przejscie
		else wartosc =  0;																	//czy sciana
	}
	else if (kursor == '>'){
		if (labirynt[y - 2][x - 1 + z] != '#') wartosc =  1;
		else wartosc = 0;
	}
	else if (kursor == '<'){
		if (labirynt[y][x - 1 - z] != '#') wartosc = 1;
		else wartosc = 0;
	}
	else if (kursor == '^'){
		if (labirynt[y - 1 - z][x - 2] != '#') wartosc = 1;
		else wartosc = 0;
	}
	return wartosc;
}

int sciana_prawa(char labirynt[SIZE][SIZE2], char kursor, int x, int y, int z){		//funkcja robi to samo co funkcja
	int wartosc;
	if (kursor == 'v'){																//wyzej tylko dla sciany prawej
		if (labirynt[y - 1 + z][x - 2] != '#') wartosc = 1;
		else wartosc =  0;
	}
	else if (kursor == '>'){
		if (labirynt[y][x - 1 + z] != '#') wartosc = 1;
		else wartosc =  0;
	}
	else if (kursor == '<'){
		if (labirynt[y - 2][x - 1 - z] != '#') wartosc = 1;
		else wartosc = 0;
	}
	else if (kursor == '^'){
		if (labirynt[y - 1 - z][x] != '#') wartosc = 1;
		else wartosc = 0;
	}


	return wartosc;
}

void wartosci_pers(char pers[SIZE3][SIZE4], char labirynt[SIZE][SIZE2], char kursor, int x, int y){
	int z = 1;
	int i = 0, j = 0;
	for (int i = 0; i < SIZE3; i++)												//przypisujemy kazdej wartosci tablicy 
		for (int j = 0; j < SIZE4; j++) pers[i][j] = '0';						//perspektywy '0' zeby przypadkiem nie znalazly
																				//sie tam jakies "smieci"
	int glebokosc = sprawdz_glebokosc(labirynt, pers, kursor, x, y);			//sprawdzamy odleglosc do sciany z przodu

	if (glebokosc == 0){														//rysujemy jedna sciane gdy odleglosc jest 0

		for (int i = 0; i < SIZE3; i++){
			for (int j = 0; j < SIZE4; j++){
				if (i == 0 || i == (SIZE3 - 1)) pers[i][j] = '-';
				else if (j == 0 || j == (SIZE4 - 1)) pers[i][j] = '|';
			}
		}
	}
	
	else{

		while (z <= glebokosc){
			if (sciana_lewa(labirynt, kursor, x, y, z) == 0){						//rysujemy w petli najpierw lewa sciane
				for (int b = 0; b < 3; b++){										//petla zakonczy sie gdy z bedzie rownac 
					pers[i][j + 1] = '\\';											//sie odleglosci
					pers[i + 1][j + 2] = '\\';
					pers[SIZE3 - 1 - i][j + 1] = '/';
					pers[SIZE3 - 2 - i][j + 2] = '/';
					for (int c = i + 2; c < SIZE3 - 2 - i; c++)pers[c][j + 2] = '|';
					for (int c = i; c < SIZE3 - i; c++)pers[c][j] = '|';
				}
			}
			else{
				pers[i + 1][j] = '-';												//rysujemy gdy na lewo jest przejscie
				pers[i + 1][j + 1] = '-';
				pers[i + 1][j + 2] = '-';
				pers[SIZE3 - i - 2][j] = '-';
				pers[SIZE3 - i - 2][j + 1] = '-';
				pers[SIZE3 - i - 2][j + 2] = '-';
			}
			if (sciana_prawa(labirynt, kursor, x, y, z) == 0){						//to samo co wyzej tylko sciana prawa
				for (int b = 0; b < 3; b++){
					pers[i][SIZE4 - 2 - j] = '/';
					pers[i + 1][SIZE4 - 3 - j] = '/';
					pers[SIZE3 - 1 - i][SIZE4 - 2 - j] = '\\';
					pers[SIZE3 - 2 - i][SIZE4 - 3 - j] = '\\';
					for (int c = i + 2; c < SIZE3 - 2 - i; c++)pers[c][SIZE4 - 3 - j] = '|';
					for (int c = i; c < SIZE3 - i; c++)pers[c][SIZE4 - 1 - j] = '|';
				}
			}
			else{
				pers[i + 1][SIZE4 - j - 1] = '-';
				pers[i + 1][SIZE4 - j - 2] = '-';
				pers[i + 1][SIZE4 - j - 3] = '-';
				pers[SIZE3 - i - 2][SIZE4 - j - 1] = '-';
				pers[SIZE3 - i - 2][SIZE4 - j - 2] = '-';
				pers[SIZE3 - i - 2][SIZE4 - j - 3] = '-';
			}
			i += 2;
			j += 3;
			if (glebokosc <= 7){
				if (z == glebokosc){
					for (int c = j; c < SIZE4 - j; c++){							//gdy odleglosc jest mniejsza lub rowna 
						pers[i-1][c] = '-';											//7 rysujemy na koncu sciane 
						pers[SIZE3-i][c] = '-';
					}
					for (int c = i; c < SIZE3 - i; c++){
						pers[c][j-1] = '|';
						pers[SIZE3-1 - c][SIZE4-j] = '|';
					}
				}
			}
			else{
				if (z == 7){
					for (int b = j; b < SIZE4 - j; b++)pers[(SIZE3 / 2) - 1][b] = '_';	//gdy odleglosc jest wieksza od 7
					z = glebokosc;														//nie rysujemy sciany na koncu bo dalej
				}																		//nie widzimy, zwiekszamy z do odleglosci
			}																			//i wychodzimy z petli while
			z++;
		}
	}
	rysuj_3d(pers, labirynt, x, y);													//rysujemy rezultaty
}



void perspektywa(char labirynt[SIZE][SIZE2], char kursor, int x, int y){		//funkcja, ktora uruchamia mechanizm perspektywy
	char pers[SIZE3][SIZE4];
	wartosci_pers(pers, labirynt, kursor, x, y);	
}

int main()
{
	double licznik_czas = 0;
	time_t start, koniec; 
	wsp punkt;
	int licz_kr = 0;
	char tab[SIZE][SIZE2];
	char nazwa_pliku[50] = { 0 };
	char nazwa_pliku_zapis[50] = { 0 };
	int x1 = 5, y1 = 1;
	int zn, x = 5, y = 1, attr = 7, spr = 0, spr2 = 0;;
	char kursor = 'v';
	if (sprawdz_czy_istnieje(tab) == 1)spr++;								//sprawdzanie czy istnieje jakis plik edytora
	settitle("Szymon Kummer 154738");
	textbackground(BLACK);

	do {
		clrscr();
		time(&start);														//zaczynamy mierzyc czas
		menu(licz_kr, licznik_czas);										//wypisujemy menu 
		
		
		if (spr != 0) {														//wczytanie domyslnego labiryntu po wcisnieciu 'i'
			gotoxy(1, 1);													//lub ostatnio zapisanego gdy taki jest
			rysuj_lab(tab);
			perspektywa(tab, kursor, x, y);
			gotoxy(x, y);
			
		}

		textcolor(attr);
		putch(kursor);

		if (tab[y - 1][x - 1] == 'm'){										//sprawdzanie czy gracz nie stoi na mecie                      
			gotoxy(30, 8);
			cout << "WYGRANA";
		}

		zn = getch();

		if (spr != 0 || spr2!=0){
			if (zn == 0x4b || zn == 0x4d){									//zmiana zwrotu kursora
				kursor = zmiana_zwrotu(zn, kursor);
			}
			else if (zn == 0x48 || zn == 0x50){								//poruszanie
				punkt = poruszanie(tab, x, y, zn, kursor);
				x = punkt.x;
				y = punkt.y;
				licz_kr += punkt.licz;
			}
		}
		if (zn == ' ') attr = (attr + 1) % 16;								//zmiana koloru

		else if (zn == 'i'){												//laduje domyslny labirynt zpliku "plik"
			tablica_domyslna(tab);
			spr++;
			licznik_czas = 0;
			x = x1;
			y = y1;
			kursor = 'v';
			licz_kr = 0;
		}


		else if (zn == 'r'){												//resetowanie pozycji i licznikow
			licz_kr = 0;
			x = x1;
			y = y1;
			kursor = 'v';
			licznik_czas = 0;
		}
		else if (zn == 'o'){												//wczytywanie labiryntu przez podanie nazwy
			gotoxy(55, 9);
			scanf("%49s", nazwa_pliku);
			tablica_nazwa(tab, nazwa_pliku);
			spr++;
			x = x1;
			y = y1;
			licznik_czas = 0;
			licz_kr = 0;
			kursor = 'v';
		}
		else if (zn == 's'){												//zapisywanie do pliku
			gotoxy(55, 9);
			scanf("%49s", nazwa_pliku_zapis);
			zapisanie_pliku(tab, nazwa_pliku_zapis);
		}
		else if (zn == 'e'){												//edytor aktualnego labiryntu
			edytor(tab);
		}

		time(&koniec);														//koniec pomiaru czasu
		if(spr!=0 || spr2!=0)licznik_czas += czas(koniec, start);			//liczymy czas gdy jest zaladowany jakis labirynt
	
	} while (zn != 'q');

	return 0;
}