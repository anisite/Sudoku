/***********************************
 * Nom : Côté, Dany			       *
 * Date : 15/12/2008               *
 * Fichier : sudoku.h              *
 * But : utiliser la liste dans un *
 *	logiciel qui résout des sudoku *
 **********************************/
#include "iostream"
#include "liste.h"
#include "fstream"
#include <windows.h> 
#pragma once

using namespace std;

class sudoku
{
	HANDLE hConsole;
	struct point
	{
		point(){}
		point(int ix, int iy){x=ix;y=iy;}
		int x;
		int y;
	};
	sudoku();
	void copie(sudoku &S);
	ITliste<int> pos[9][9];
	bool checked[9][9];
	bool connus[9][9];
	bool display;
	bool stepbystep;
	bool debug;
	point Pdebug;
	int debugVal;
	int debugVal2;
	int nbRestant;
	void flush(int val, ITliste<int> & in);
	void clean_all(point P);
	void clean_col(point P);
	void clean_row(point P);
	point search(point P);
	bool oneLeft(ITliste<int> & in);
	void afficher(point P, bool tout = true, int x = 1, int y = 2);
	void afficherGrille(int x, int y);
	point coinSousGrille(point P);
	bool solutionneur();
	point bestPoint();
	void setcolor(unsigned int color);
	void curseur(point P);
	void curseur(int x, int y);
public:
	sudoku(const char *fichier);
	~sudoku();
	void jouer(bool affichage=true, bool PasaPas=false);
};