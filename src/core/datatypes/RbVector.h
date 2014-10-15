/**
 * @file
 * This file contains the declaration of the RbVector class. 
 * The RbVector is our implementation of the stl vector, actually a wrapper class,
 * that internally stores the values in a stl-vector of pointers so that vectors
 * of abstract base classes can be used as well.
 *
 *
 * @brief Declaration of the RbVector class
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-07-18
 *
 * $Id$
 */

#ifndef RbVector_H
#define RbVector_H

#include "IsAbstract.h"
#include "RbVectorImpl.h"
#include "RbIterator.h"
#include "RbConstIterator.h"

#include <vector>
#include <iostream>

namespace RevBayesCore {
    
    template <class valueType>
    class RbVector : public RbVectorImpl<valueType, IsAbstract<valueType>::Is > {
        
    public:
        // constructor(s)
        RbVector();
        RbVector(size_t n);
        RbVector(size_t n, const valueType &v);
        RbVector(const typename RbVectorImpl<valueType, IsAbstract<valueType>::Is >::vectorType &v);
        RbVector(const RbVector<valueType> &v);
        virtual                                            ~RbVector(void);
        
//        typedef typename std::vector<valueType*>::iterator           iterator;
//        typedef typename std::vector<valueType*>::const_iterator     const_iterator;

        
        // public member functions
        RbVector<valueType>*                                clone(void) const;                                                                      //!< Create an independent clone
        void                                                printElement(std::ostream &o, size_t i) const;                                          //!< Print the i-th element
        
        // public (stl-like) vector functions
//        RbIterator<valueType>                               begin(void);
//        RbConstIterator<valueType>                          begin(void) const;
//        RbIterator<valueType>                               end(void);
//        RbConstIterator<valueType>                          end(void) const;
//        size_t                                              size(void) const;
        void                                                sort(void);
        
    private:
        
        // private methods
        int                                                 pivot(int l, int r);
        void                                                quicksort(int l, int r);
        void                                                swap( valueType& a, valueType& b);
        
        // private members

    };
 
    
    template <class valueType>
    std::ostream&                       operator<<(std::ostream& o, const RbVector<valueType>& x);                            //!< Overloaded output operator

}



#include "Cloner.h"
#include "IsAbstract.h"
#include "IsDerivedFrom.h"

template <class valueType>
RevBayesCore::RbVector<valueType>::RbVector() : RbVectorImpl<valueType, IsAbstract<valueType>::Is  >()
{
    
}


template <class valueType>
RevBayesCore::RbVector<valueType>::RbVector(size_t n) : RbVectorImpl<valueType, IsAbstract<valueType>::Is  >(n)
{
    
}


template <class valueType>
RevBayesCore::RbVector<valueType>::RbVector(size_t n, const valueType &v) : RbVectorImpl<valueType, IsAbstract<valueType>::Is  >(n,v)
{
    
}


template <class valueType>
RevBayesCore::RbVector<valueType>::RbVector( const typename RbVectorImpl<valueType, IsAbstract<valueType>::Is >::vectorType &v ) : RbVectorImpl<valueType, IsAbstract<valueType>::Is  >(v)
{
    
}


template <class valueType>
RevBayesCore::RbVector<valueType>::RbVector( const RbVector<valueType> &v ) : RbVectorImpl<valueType, IsAbstract<valueType>::Is  >(v)
{
    
}


template <class valueType>
RevBayesCore::RbVector<valueType>::~RbVector( void )
{

}


template <class valueType>
RevBayesCore::RbVector<valueType>* RevBayesCore::RbVector<valueType>::clone(void) const {
    
    return new RbVector<valueType>( *this );
}

/**
 * Find and return the index of pivot element.
 * @param a - The array.
 * @param first - The start of the sequence.
 * @param last - The end of the sequence.
 * @return - the pivot element
 */
template <class valueType>
int RevBayesCore::RbVector<valueType>::pivot(int first, int last)
{
    int  p = first;
    const valueType& pivotElement = this->operator[](first);
    
    for(int i = first+1 ; i <= last ; i++)
    {
        /* If you want to sort the list in the other order, change "<=" to ">" */
        if(this->operator[](i) <= pivotElement)
        {
            p++;
            
            valueType *temp = Cloner<valueType, IsDerivedFrom<valueType, Cloneable>::Is >::createClone( this->operator[](i) );
            this->operator[](i) = this->operator[](p);
            this->operator[](p) = *temp;
            
            delete temp;
        }
    }
    
    valueType *temp = Cloner<valueType, IsDerivedFrom<valueType, Cloneable>::Is >::createClone( this->operator[](p) );
    this->operator[](p) = this->operator[](first);
    this->operator[](first) = *temp;
    
    delete temp;
    
    return p;
}




template <class valueType>
void RevBayesCore::RbVector<valueType>::printElement(std::ostream& o, size_t i) const
{
    
    o << this->operator[](i);
    
}


template <class valueType>
void RevBayesCore::RbVector<valueType>::quicksort(int first, int last)
{
    
    if(first < last)
    {
        int pivotElement = pivot(first, last);
        quicksort(first, pivotElement-1);
        quicksort(pivotElement+1, last);
    }
}


template <class valueType>
void RevBayesCore::RbVector<valueType>::sort(void)
{
    // just delegate to our internal quicksort method.
    quicksort(0, int(this->size())-1);
}


/**
 * Swap the parameters.
 * @param a - The first parameter.
 * @param b - The second parameter.
 */
template <class valueType>
void RevBayesCore::RbVector<valueType>::swap( valueType& a, valueType& b)
{
    valueType *temp = Cloner<valueType, IsDerivedFrom<valueType, Cloneable>::Is >::createClone( a );
    a = b;
    b = *temp;
    
    delete temp;
}


template <class valueType>
std::ostream& RevBayesCore::operator<<(std::ostream& o, const RevBayesCore::RbVector<valueType>& x) {
    
    size_t lineLength = 75;
    
    o << "[ ";
    size_t curLength = 2;

    for (RbConstIterator<valueType> it = x.begin(); it != x.end(); ++it) 
    {
        if ( it != x.begin() ) 
        {
            o << ", ";
        }
        const valueType& v = *it;
        o << v;
    }
    if ( curLength + 2 > lineLength )
        o << std::endl << "]";
    else
        o << " ]";
    
    return o;
}


#endif

