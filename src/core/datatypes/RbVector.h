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
        RbVector( const typename RbVectorImpl<valueType, IsAbstract<valueType>::Is >::vectorType &v);
        RbVector(const RbVector<valueType> &v);
        virtual                                            ~RbVector(void);
        
//        typedef typename std::vector<valueType*>::iterator           iterator;
//        typedef typename std::vector<valueType*>::const_iterator     const_iterator;

        
        // public member functions
        RbVector<valueType>*                                clone(void) const;                                                                      //!< Create an independent clone
        
        // public (stl-like) vector functions
//        RbIterator<valueType>                               begin(void);
//        RbConstIterator<valueType>                          begin(void) const;
//        RbIterator<valueType>                               end(void);
//        RbConstIterator<valueType>                          end(void) const;
//        size_t                                              size(void) const;
        
    private:
        
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


template <class valueType>
std::ostream& RevBayesCore::operator<<(std::ostream& o, const RevBayesCore::RbVector<valueType>& x) {
    
    o << "(";
    for (RbConstIterator<valueType> it = x.begin(); it != x.end(); ++it) 
    {
        if ( it != x.begin() ) 
        {
            o << ", ";
        }
        o << *it;
    }
    o << ")";
    
    return o;
}


#endif

