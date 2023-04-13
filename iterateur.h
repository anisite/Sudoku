/***********************************
 * Nom : Côté, Dany			       *
 * Date : 15/12/2008               *
 * Fichier : iterateur.h           *
 * But : utiliser la liste dans un *
 *	logiciel qui résout des sudoku *
 **********************************/
#include "assert.h"
template <class TYPE>
class ITliste
{
friend class liste<TYPE>;			//les 2 classes sont amies
protected:
   	cellule<TYPE>* courant;		//adresse de la cellule courante
	const liste<TYPE>& laListe;		//référence à la liste
public:
   ITliste(const liste<TYPE>& L);		//ITliste I(L);
   ITliste();							//ITliste I;
   ITliste(const ITliste& I);			//copieur
   void link(const liste<TYPE>& L);		//assignateur
   ~ITliste();							// destructeur

   liste<int> L;					// Liste vierge

   const ITliste& operator=(const ITliste& I);//pour j=i
   const ITliste& operator=(int i);           //pour j=3
   
   ITliste debut();					//pour i.debut()
   ITliste debut()const;
   ITliste fin();  					//pour i.fin()

   bool operator==(const ITliste& I)const;
   bool operator!=(const ITliste& I)const;
   
   ITliste operator++(int i);               	//i++    
   ITliste operator++(int i)const;					//i++ 
   const ITliste& operator++();             	//++i    

   ITliste operator--(int i);               	//i--        
   const ITliste& operator--();             	//--i

   TYPE& operator*();                       	//accès à un élément
   const TYPE& operator*()const;            	//idem, en mode const

   ITliste operator+(int i)const;            	//i+3
   ITliste operator-(int i)const;            	//i-3
};

template <class TYPE>
ITliste<TYPE>::ITliste(): laListe(L)
{
	courant = NULL;
}

template <class TYPE>
ITliste<TYPE>::ITliste(const liste<TYPE>& L):laListe(L)
{
	courant = L.debut();
}

template <class TYPE>
void ITliste<TYPE>::link(const liste<TYPE>& L)
{
	courant = L.debut();
}

template <class TYPE>
ITliste<TYPE>::ITliste(const ITliste& I):laListe(I.laListe)
{
	courant = I.courant;
}

template <class TYPE>
ITliste<TYPE> ITliste<TYPE>::debut()
{
	courant = laListe.debut();
	return *this;
}

template <class TYPE>
ITliste<TYPE> ITliste<TYPE>::debut()const
{
	courant = laListe.debut();
	return *this;
}

template <class TYPE>
ITliste<TYPE> ITliste<TYPE>::fin()
{
	courant = laListe.fin();
	return *this;
}

template <class TYPE>
bool ITliste<TYPE>::operator==(const ITliste& I)const
{
	if (courant == I.courant && &laListe == &I.laListe)
		return true;
	return false;
}

template <class TYPE>
bool ITliste<TYPE>::operator!=(const ITliste& I)const
{
	return !(*this==I);
}

template <class TYPE>
ITliste<TYPE> ITliste<TYPE>::operator++(int i)//i++
{
	ITliste<TYPE> avant(*this);
	courant = laListe.suivant(courant);
	return avant;
}

template <class TYPE>
ITliste<TYPE> ITliste<TYPE>::operator++(int i)const //i++
{
	ITliste<TYPE> avant(*this);
	courant = laListe.suivant(courant);
	return avant;
}

template <class TYPE>
const ITliste<TYPE>& ITliste<TYPE>::operator++()//++i
{
	courant = laListe.suivant(courant);
	return *this;
}

template <class TYPE>
ITliste<TYPE> ITliste<TYPE>::operator--(int i)   //i--  
{
	ITliste<TYPE> avant(*this);
	courant = laListe.precedent(courant);
	return avant;
}

template <class TYPE>
const ITliste<TYPE>& ITliste<TYPE>::operator--() //--i
{
	courant = laListe.precedent(courant);
	return *this;
}

template <class TYPE>
TYPE& ITliste<TYPE>::operator*()                    //accès à un élément
{
	assert(laListe.estVide()==false);
	return laListe.valeur(courant);
}

template <class TYPE>
const TYPE& ITliste<TYPE>::operator*()const
{
	return laListe.valeur(courant);
}

template <class TYPE>
ITliste<TYPE> ITliste<TYPE>::operator+(int i)const            	//i+3
{
	ITliste temp = *this;
	for(i;i>0;i--)
		temp++;
	return temp;
}

template <class TYPE>
ITliste<TYPE> ITliste<TYPE>::operator-(int i)const            	//i+3
{
	ITliste temp = *this;
	for(i;i>0;i--)
		temp--;
	return temp;
}

// Destructeur de la classe ITliste
template <class TYPE>
ITliste<TYPE>::~ITliste()
{
	courant = NULL;
}