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

#include "Cloneable.h"
#include "RbIterator.h"
#include "RbConstIterator.h"

#include <vector>
#include <iostream>

namespace RevBayesCore {
    
    template <class valueType>
    class RbVector : public Cloneable {
        
    public:
        // constructor(s)
        RbVector();
        RbVector(size_t n);
        RbVector(size_t n, const valueType &v);
        RbVector(const RbVector<valueType> &v);
        virtual                                            ~RbVector(void);
        
        typedef typename std::vector<valueType*>::iterator           iterator;
        typedef typename std::vector<valueType*>::const_iterator     const_iterator;

        
        // public member functions
        RbVector<valueType>*                                clone(void) const;                                                                      //!< Create an independent clone
        
        // public (stl-like) vector functions
        valueType&                                          operator[](size_t i);
        const valueType&                                    operator[](size_t i) const;
        RbIterator<valueType>                               begin(void);
        RbConstIterator<valueType>                          begin(void) const;
        void                                                clear(void);
        RbIterator<valueType>                               end(void);
        RbConstIterator<valueType>                          end(void) const;
        void                                                insert(size_t i, const valueType &v);
        void                                                insert(size_t i, valueType *v);
        void                                                push_back(const valueType &v);
        void                                                push_back(valueType *v);
        size_t                                              size(void) const;
        
    private:
        
        // private members
		std::vector<valueType*>								values;
    };
 
    
    template <class valueType>
    std::ostream&                       operator<<(std::ostream& o, const RbVector<valueType>& x);                            //!< Overloaded output operator

}



#include "Cloner.h"
#include "IsDerivedFrom.h"

template <class valueType>
RevBayesCore::RbVector<valueType>::RbVector() : values() {
    
}


template <class valueType>
RevBayesCore::RbVector<valueType>::RbVector(size_t n) : values(n, NULL) {
    
}


template <class valueType>
RevBayesCore::RbVector<valueType>::RbVector(size_t n, const valueType &v) : values() {
    
    // fill the vector with n independent copies
    for (size_t i = 0; i < n; ++i) 
    {
        values.push_back( Cloner<valueType, IsDerivedFrom<valueType, Cloneable>::Is >::createClone( v ) );
    }
    
}


template <class valueType>
RevBayesCore::RbVector<valueType>::RbVector( const RbVector<valueType> &v ) : values() {
    
    // fill the vector with n independent copies
    size_t n = v.size();
    for (size_t i = 0; i < n; ++i) 
    {
        values.push_back( Cloner<valueType, IsDerivedFrom<valueType, Cloneable>::Is >::createClone( v[i] ) );
    }
    
}


template <class valueType>
RevBayesCore::RbVector<valueType>::~RbVector( void ) {
    
    // delete the independent copies
    size_t n = values.size();
    for (size_t i = 0; i < n; ++i) 
    {
        valueType* v = values[i];
        delete v;
    }
    
    values.clear();

}


template <class valueType>
valueType& RevBayesCore::RbVector<valueType>::operator[](size_t i) {
    
    return *values[i];
}


template <class valueType>
const valueType& RevBayesCore::RbVector<valueType>::operator[](size_t i) const {
    
    return *values[i];
}


template <class valueType>
RevBayesCore::RbIterator<valueType> RevBayesCore::RbVector<valueType>::begin(void) {
    
    return RbIterator<valueType>( values.begin() );
}


template <class valueType>
RevBayesCore::RbConstIterator<valueType> RevBayesCore::RbVector<valueType>::begin(void) const {
    
    return RbConstIterator<valueType>( values.begin() );
}


template <class valueType>
void RevBayesCore::RbVector<valueType>::clear( void ) {
    
    // delete the independent copies
    size_t n = values.size();
    for (size_t i = 0; i < n; ++i) 
    {
        valueType* v = values[i];
        delete v;
    }
    
    values.clear();
    
}


template <class valueType>
RevBayesCore::RbVector<valueType>* RevBayesCore::RbVector<valueType>::clone(void) const {
    
    return new RbVector<valueType>( *this );
}


template <class valueType>
RevBayesCore::RbIterator<valueType> RevBayesCore::RbVector<valueType>::end(void) {
    
    return RbIterator<valueType>( values.end() );
}


template <class valueType>
RevBayesCore::RbConstIterator<valueType> RevBayesCore::RbVector<valueType>::end(void) const {
   
    return RbConstIterator<valueType>( values.end() );
}


template <class valueType>
void RevBayesCore::RbVector<valueType>::insert(size_t i, const valueType &v) {
    
    values[i] = Cloner<valueType, IsDerivedFrom<valueType, Cloneable>::Is >::createClone( v );

}


template <class valueType>
void RevBayesCore::RbVector<valueType>::insert(size_t i, valueType *v) {

    values[i] = v;

}


template <class valueType>
void RevBayesCore::RbVector<valueType>::push_back(const valueType &v) {
    
    values.push_back( Cloner<valueType, IsDerivedFrom<valueType, Cloneable>::Is >::createClone( v ) );

}


template <class valueType>
void RevBayesCore::RbVector<valueType>::push_back(valueType *v) {
    
    values.push_back( v );

}


template <class valueType>
size_t RevBayesCore::RbVector<valueType>::size(void) const {
    
    return values.size();
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

