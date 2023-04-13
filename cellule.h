/***********************************
 * Nom : Côté, Dany			       *
 * Date : 15/12/2008               *
 * Fichier : cellule.h             *
 * But : utiliser la liste dans un *
 *	logiciel qui résout des sudoku *
 **********************************/
#pragma once
#include <iostream>
using namespace std;

template <class TYPE> class liste;
//CLASSE CELLULE
template <class TYPE>
class cellule
{
	friend class liste<TYPE>;
	TYPE element;				//Contenu de la cellule
	cellule <TYPE> *precedent;	//Pointeur vers la cellule précédente
	cellule <TYPE> *suivant;	//Pointeur vers la cellule suivante
	cellule(const TYPE &e, cellule <TYPE> *p = NULL, cellule <TYPE> *s = NULL)
	{
		element = e;
		precedent = p;
		suivant = s;
	}
};