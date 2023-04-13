/***********************************
 * Nom : Côté, Dany			       *
 * Date : 15/12/2008               *
 * Fichier : TP4.cpp               *
 * But : utiliser la liste dans un *
 *	logiciel qui résout des sudoku *
 **********************************/
#include "stdafx.h"
#include "liste.h"
#include "sudoku.h"
#include <iostream>
#include <string>

void main()
{
	string strFichier;			//nom du fichier contenant une grille de sudoku à résoudre

	//sudoku Jeu("td.txt");
	cout << "Bienvenue dans ce solutionneur de SUDOKU !" << endl << endl;
	cout << "Les différentes grilles à solutionner sont dans les fichiers suivants :" << endl 
		 << "sudoku.txt" << endl 
		 << "sudokuDUR.txt" << endl 
		 << "sudokuEXPERT.txt" << endl 
		 << "sudokuEvil.txt" << endl;

	cout << "Entrer le nom du fichier : ";
	cin >> strFichier;
	system("cls");

	sudoku Jeu(strFichier.c_str());
	//sudoku Jeu("sudokuDUR.txt");
	//sudoku Jeu("sudokuEXPERT.txt");
	//sudoku Jeu("sudokuEvil.txt");

	//sudoku Jeu("sudokuEXPERT_erreur.txt");
	//sudoku Jeu("sudokulol.txt");
	//sudoku Jeu("sudokuDUR.txt");
	//sudoku Jeu("sudoku_philip.txt");
	//sudoku Jeu("demo.txt");
	//sudoku Jeu("c:\boot.ini");

	Jeu.jouer(true);
		
	system ("pause");
}