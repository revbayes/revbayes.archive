/**
 * @file
 * This file contains the declaration of the RbIterator class. 
 * The RbIterator is our implementation of the stl-vector-iterator. 
 * See RbVector for more information
 *
 *
 * @brief Declaration of the RbIterator class
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-07-18
 *
 * $Id$
 */

#ifndef RbIterator_H
#define RbIterator_H

#include <vector>

namespace RevBayesCore {
    
    template <class valueType>
    class RbIterator {
        
    public:
        // constructor(s)
        RbIterator(const typename std::vector<valueType*>::iterator &i);
        RbIterator(const RbIterator<valueType> &v);
        
        // public (stl-like) vector functions
        RbIterator&                                         operator++(void);                                                               //!< Increment index (prefix)
        const RbIterator&                                   operator++(void) const;                                                               //!< Increment index (prefix)
        RbIterator&                                         operator--(void);                                                               //!< Decrement index (prefix)
        const RbIterator&                                   operator--(void) const;                                                               //!< Decrement index (prefix)
        bool                                                operator==(const RbIterator& x) const;                                   //!< Equals operator
        bool                                                operator!=(const RbIterator& x) const;                                   //!< Not-Equals operator
        bool                                                operator<(const RbIterator& x) const; 
        valueType&                                          operator*(void);
        const valueType&                                    operator*(void) const;
        valueType*                                          operator->(void);
        const valueType*                                    operator->(void) const;
        
    private:
        
        // private members
		typename std::vector<valueType*>::iterator          it;
    };
    
}


template <class valueType>
RevBayesCore::RbIterator<valueType>::RbIterator(const typename std::vector<valueType*>::iterator &i) : it( i ) {
    
}


template <typename valueType>
RevBayesCore::RbIterator<valueType>& RevBayesCore::RbIterator<valueType>::operator++(void) {
    
    ++it;
    
    return *this;
}


template <typename valueType>
const RevBayesCore::RbIterator<valueType>& RevBayesCore::RbIterator<valueType>::operator++(void) const {
    
    ++it;
    
    return *this;
}


template <typename valueType>
RevBayesCore::RbIterator<valueType>& RevBayesCore::RbIterator<valueType>::operator--(void) {
    
    --it;
    
    return *this;
}


template <typename valueType>
const RevBayesCore::RbIterator<valueType>& RevBayesCore::RbIterator<valueType>::operator--(void) const {
    
    --it;
    
    return *this;
}


template <typename valueType>
bool RevBayesCore::RbIterator<valueType>::operator==(const RbIterator<valueType> &x) const {
    
    return it == x.it;
}


template <typename valueType>
bool RevBayesCore::RbIterator<valueType>::operator!=(const RbIterator<valueType> &x) const {
    
    return it == x.it;
}


template <typename valueType>
bool RevBayesCore::RbIterator<valueType>::operator<(const RbIterator<valueType> &x) const {
    
    return it < x.it;
}


template <typename valueType>
valueType* RevBayesCore::RbIterator<valueType>::operator->(void) {
    
    return (*it);
}


template <typename valueType>
const valueType* RevBayesCore::RbIterator<valueType>::operator->(void) const {
    
    return (*it);
}

template <typename valueType>
const valueType& RevBayesCore::RbIterator<valueType>::operator*(void) const {
    
    return *(*it);
}

template <typename valueType>
valueType& RevBayesCore::RbIterator<valueType>::operator*(void) {
    
    return *(*it);
}


#endif

