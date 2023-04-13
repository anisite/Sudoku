// DANY COTÉ
// 4218
//---------------------------
// file.h
// Expérimenter les files
//---------------------------
#pragma once
#include "assert.h"
#include "iostream"
using namespace std;

template <class TYPE>
class file
{
private:
   struct cellule
   {
      TYPE element;
      cellule* suivant;
      cellule(const TYPE& e,cellule* s=NULL)
	  {  
         element=e;
         suivant=s;
      };
   };
   file(const file&);                  //copieur désactivé
   cellule *TETE,*QUEUE;
public:
   file();                             //construit une file vide
  ~file();                             //détruit la file

   const file& operator=(file& f);	   //affectateur
   void enfiler(const TYPE&);          //ajoute à la QUEUE
   TYPE defiler();                     //enlève la tête
   bool estVide() const;               //vérifie si la file est vide
   void vider();                       //vide la file
   const TYPE& tete() const;           //retourne la tête

};

template <class TYPE>
file<TYPE>::file()
{
	TETE=QUEUE=NULL;
}

template <class TYPE>
file<TYPE>::~file()
{
		vider();
}


template <class TYPE>
const file<TYPE>& file<TYPE>::operator=(file<TYPE>& f)
{
	if(f.QUEUE!=QUEUE)
	{
		vider();
		if(f.estVide()==false)
		{
			file f_temp;
			cellule *temp=f.QUEUE->suivant;
			f_temp.enfiler(temp->element);
			temp = temp->suivant;
			while(temp != f.QUEUE->suivant)
			{
				f_temp.enfiler(temp->element);
				temp = temp->suivant;
			}
			while(!f_temp.estVide())
			{
				enfiler(f_temp.defiler());
			}
		}
	}
	return *this;
}


template <class TYPE>
void file<TYPE>::enfiler(const TYPE& a)
{
	if(QUEUE==NULL)
	{
	  QUEUE=new cellule(a);
	  QUEUE->suivant=QUEUE;
	}
	else
	  QUEUE=QUEUE->suivant=new cellule(a,QUEUE->suivant);
}

template <class TYPE>
TYPE  file<TYPE>::defiler()
{
   assert(estVide()==false);
   cellule *temp=QUEUE->suivant;
   TYPE val=temp->element;
   QUEUE->suivant=temp->suivant;
   if(temp==QUEUE)
	   QUEUE=NULL;
   delete temp;
   return val; 
}

template <class TYPE>
bool file<TYPE>::estVide()const
{
	if(QUEUE == NULL)
	{
		return true;
	}
	return false;
}

template <class TYPE>
void file<TYPE>::vider()
{
	while(!estVide())
	{
		defiler();
	}
}

template <class TYPE>
const TYPE& file<TYPE>::tete() const
{
	assert(!estVide());
	return QUEUE->suivant->element;
}
