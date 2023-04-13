/***********************************
 * Nom : Côté, Dany			       *
 * Date : 15/12/2008               *
 * Fichier : liste.h               *
 * But : utiliser la liste dans un *
 *	logiciel qui résout des sudoku *
 **********************************/
#pragma once
#include "cellule.h"
#include "iterateur.h"
#include "assert.h"
using namespace std;

//CLASSE liste explicite
template <class TYPE>
class liste
{
friend class ITliste<TYPE>;			//les 2 classes sont amies
protected:
   cellule<TYPE>* pDebut;		// pointeur sur 1ere cellule
   cellule<TYPE>* pFin;			// pointeur sur 1ere cellule
   int size;					// taille de la liste

   liste (const liste&);		// copieur désactivé

   cellule<TYPE>* inserer(const TYPE& e, cellule<TYPE>* cell);
   cellule<TYPE>* retirer(cellule<TYPE>* cell);
   cellule<TYPE>* suivant(cellule<TYPE>* cell)const;
   cellule<TYPE>* precedent(cellule<TYPE>* cell)const;

   cellule<TYPE>* debut()const;
   cellule<TYPE>* fin()const;
   
   bool estDebut(cellule<TYPE>* cell)const;
   bool estFin(cellule<TYPE>* cell)const;

   TYPE& valeur(cellule<TYPE>* cell)const;
   
public:
	liste();
   ~liste();

   bool estVide()const;
   void vider();

   TYPE& operator[](const ITliste<TYPE>& I);
   const TYPE& operator[](const ITliste<TYPE>& I)const;

   const ITliste<TYPE>& inserer(const TYPE&, ITliste<TYPE>& I);
   ITliste<TYPE> inserer(const TYPE&,const ITliste<TYPE>& I);

   const ITliste<TYPE>& supprimer(ITliste<TYPE>& I);
   ITliste<TYPE> supprimer(const ITliste<TYPE>& I);

   int taille();
   int taille()const;
};

//---------------
// Vide la liste
//--------------
template <class TYPE>
void liste<TYPE>::vider()
{
	while(!estVide())
		retirer(fin());
}

//---------------
// Affectateur sans paramètres
//--------------
template <class TYPE>
liste<TYPE>::liste()
{
	pDebut = pFin = NULL;
	size=0;
}

//---------------
// Destructeur
//--------------
template <class TYPE>
liste<TYPE>::~liste()
{
	vider();
	pDebut = pFin = NULL;
}

//---------------
// Retourne true si la liste est vide
//--------------
template <class TYPE>
bool liste<TYPE>::estVide()const
{
	if ( (pDebut == NULL && pFin == NULL)|| size == 0 )
		return true;
	return false;
}

//---------------
// Insère un élément dans la liste
//--------------
template <class TYPE>
cellule<TYPE>* liste<TYPE>::inserer(const TYPE &e, cellule<TYPE>* cell)
{
	if(estVide())		// Si la liste est vide (aucun élément)
	{
		cell = pDebut = pFin = new cellule<TYPE>(e);
		assert(pDebut != NULL);	
	}
	else if(estFin(cell))//cell == pFin) // Si l'on insère à la fin
	{
		cell = pFin = pFin->suivant = new cellule<TYPE>(e,pFin);
	}
	else if (estDebut(cell))				// Si on insère ailleur
	{
		cell = pDebut = pDebut->precedent = new cellule<TYPE>(e,NULL,cell);
	}
	else
	{
		cell = cell->precedent->suivant = cell->precedent = new cellule<TYPE>(e,cell->precedent,cell);
	}
	size++;
	return cell;
}

//---------------
// Retire un élément de la liste
//--------------
template <class TYPE>
cellule<TYPE>* liste<TYPE>::retirer(cellule<TYPE>* cell)
{
	if(estVide())		// Si la liste est vide (aucun élément), on ne supprime rien
	{
		return NULL;
	}
	else if(size==1)	// Si il reste juste 1 élément
	{
		pFin = pDebut = cell = NULL;
	}
	else if(estFin(cell)) // Si l'on retire à la fin on met a jour en conséquence
	{
		pFin = cell->precedent;
		cell->precedent->suivant = NULL;
		delete cell;
		cell = pFin;
	}
	else if(estDebut(cell)) // Si l'on retire au début on met a jour en conséquence
	{
		pDebut = cell->suivant;
		cell->suivant->precedent = NULL;
		delete cell;
		cell = pDebut;
	}
	else				// Si l'on supprime au centre
	{
		cellule<TYPE>* pTemp = cell->precedent;
		cell->precedent->suivant=cell->suivant;
		cell->suivant->precedent=cell->precedent;
		delete cell;
		cell = pTemp;
	}
	size--;
	return cell;
}

//---------------
// Retourne l'élément suivant
//--------------
template <class TYPE>
cellule<TYPE>* liste<TYPE>::suivant(cellule<TYPE> *cell) const
{
	if(pDebut == NULL || cell == pFin || cell == NULL)
		return NULL;
	else
		return cell->suivant;
}

//---------------
// Retourne l'élément précédent
//--------------
template <class TYPE>
cellule<TYPE>* liste<TYPE>::precedent(cellule<TYPE> *cell) const
{
	if (cell == NULL || cell == pDebut)
		return NULL;
	else
		return cell->precedent;
}

//---------------
// Retourne la valeur d'une cellule
//--------------
template <class TYPE>
TYPE& liste<TYPE>::valeur(cellule<TYPE>* cell)const
{
	assert(!estVide()); // la cellule n'existe pas
	if ( cell == NULL )
		return fin()->element;
	else
		return cell->element;
}

//---------------
// Retourne la cellule du début
//--------------
template <class TYPE>
cellule<TYPE>* liste<TYPE>::debut()const
{
	return pDebut;
}

//---------------
// Retourne la cellule de la fin
//--------------
template <class TYPE>
cellule<TYPE>* liste<TYPE>::fin()const
{
	return pFin;
}

//---------------
// Retourne true si la cellule est la premiere
//--------------
template <class TYPE>
bool liste<TYPE>::estDebut(cellule<TYPE>* cell)const
{
	if(cell==pDebut)
		return true;
	return false;
}

//---------------
// Retourne true si la cellule est la derniere
//--------------
template <class TYPE>
bool liste<TYPE>::estFin(cellule<TYPE>* cell)const
{
	if(cell==NULL || cell->suivant == NULL)
		return true;
	return false;
}

//---------------
// Retourne la taille de la cellule
//--------------
template <class TYPE>
int liste<TYPE>::taille()
{
	return size;
}

//---------------
// Retourne la valeur de la cellule envoyée entre crochets
//--------------
template <class TYPE>
TYPE& liste<TYPE>::operator[](const ITliste<TYPE>& I)
{
   assert(this==&I.L);
   assert(!!I);                   //i n’est pas à la fin
   return valeur(I.courant);
}

//---------------
// Retourne la valeur de la cellule envoyée entre crochets
//--------------
template <class TYPE>
const TYPE& liste<TYPE>::operator[](const ITliste<TYPE>& I)const
{
   assert(this==&I.laListe);
   assert(!!I);                   //i n’est pas à la fin
   return valeur(I.courant);
}

//---------------
// Insère
//--------------
template <class TYPE> 
const ITliste<TYPE>& liste<TYPE>::inserer(const TYPE&x, ITliste<TYPE>&I)
{
   assert(this==&I.laListe);
   ITliste<TYPE> temp(I);
   temp.courant=inserer(x,temp.courant);
   I.courant=suivant(I.courant);
   return I; //avait temp
}


//---------------
// Insère
//--------------
template <class TYPE> 
ITliste<TYPE> liste<TYPE>::inserer(const TYPE&x, const ITliste<TYPE>&I)
{
   assert(this==&I.laListe);
   ITliste<TYPE> temp(I);
   temp.courant=inserer(x,temp.courant);
   return temp;
}


//---------------
// Supprime
//--------------
template <class TYPE> 
const ITliste<TYPE>& liste<TYPE>::supprimer(ITliste<TYPE>& I)
{
	assert(this==&I.laListe);
	I.courant = retirer(I.courant);
	return I;
}

//---------------
// Supprime
//--------------
template <class TYPE> 
ITliste<TYPE> liste<TYPE>::supprimer(const ITliste<TYPE>& I)
{
	assert(this==&I.laListe);
	I.courant = retirer(I.courant);
	return I;
}