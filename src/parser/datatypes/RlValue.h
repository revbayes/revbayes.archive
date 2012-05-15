/**
 * @file
 * This file contains the declaration of RlValue, which is
 * the RevLanguage class for all values in the RevLanguage part of RevBayes.
 *
 * RlValue can contain values of any dimensionality (e.g. scalars, vectors, matrices ...).
 * The value is stored by a pointer to the first element and a vector of length for each dimension.
 *
 * @brief Declaration of RlValue
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-03-23 18:42:52 +0100 (Fri, 23 Mar 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-04-17
 *
 * $Id: RlValue.h 1358 2012-03-23 17:42:52Z hoehna $
 */

#ifndef RlValue_H
#define RlValue_H

#include "RbPtr.h"

#include <vector>

template<class valueType>
class RlValue {
    
public:
    RlValue(void);
    RlValue(const RbPtr<valueType> &v);
    RlValue(const std::vector<RbPtr<valueType> > &v, const std::vector<size_t> &l);
    RlValue(const RlValue<valueType> &x);
    virtual                            ~RlValue(void) {}                                                         //!< Virtual destructor
 
    // overloaded operators
    RlValue<valueType>&                 operator=(const RlValue<valueType> &x);
    
    // member functions
    RlValue<RbObject>                   clone(void) const;
    RlValue<RbObject>                   convertTo(const TypeSpec& type) const;                                  //!< Convert to type
    RbValue<void *>                     getLeanValue(void) const;
    const RbPtr<valueType>&             getSingleValue(void) const;
    const TypeSpec&                     getTypeSpec(void) const;
    bool                                isConvertibleTo(const TypeSpec& type) const;                            //!< Is convertible to type?
    bool                                isNULL(void) const;
    bool                                isTypeSpec(const TypeSpec &ts) const;
    void                                printValue(std::ostream& o) const;
    
    // public members
    std::vector<RbPtr<valueType> >      value;
    std::vector<size_t>                 lengths;
    
};


#include "RbException.h"


template <typename valueType>
RlValue<valueType>::RlValue( void ) {
    
}


template <typename valueType>
RlValue<valueType>::RlValue( const RbPtr<valueType> &val ) {
    value.push_back( val );
}

template <typename valueType>
RlValue<valueType>::RlValue( const std::vector<RbPtr<valueType> > &vals, const std::vector<size_t> &l ) {
    value = vals;
    lengths = l;
}


template <typename valueType>
RlValue<valueType>::RlValue( const RlValue<valueType> &x) {
    value = x.value;
    lengths = x.lengths;
}


template <typename valueType>
RlValue<valueType>& RlValue<valueType>::operator=(const RlValue<valueType> &x) {
    // check for self assignment
    if ( this != &x) {
        value = x.value;
        lengths = x.lengths;
    }
    
    return *this;
}


template <typename valueType>
RlValue<RbObject> RlValue<valueType>::clone( void ) const {
    
    std::vector<RbPtr<RbObject> > clones;
    typename std::vector<RbPtr<valueType> >::const_iterator i;
    for ( i = value.begin(); i != value.end(); ++i) {
        clones.push_back( (*i)->clone() );
    }
    
    return RlValue<RbObject>(clones, lengths);
}



template <typename valueType>
RlValue<RbObject> RlValue<valueType>::convertTo(const TypeSpec &type) const {
    
    
    std::vector<RbPtr<RbObject> > clones;
    typename std::vector<RbPtr<valueType> >::const_iterator i;
    for ( i = value.begin(); i != value.end(); ++i) {
        clones.push_back( (*i)->convertTo(type) );
    }
    
    return RlValue<RbObject>(clones, lengths);
}


template <typename valueType>
RbValue<void *> RlValue<valueType>::getLeanValue( void ) const {
    
}


template <typename valueType>
const RbPtr<valueType>& RlValue<valueType>::getSingleValue( void ) const {
    
    if ( lengths.size() > 0 ) {
        throw RbException("??? We shouldn't return a single value for a multidimensional RlValue!!!");
    }
    
    return value[0];
}


template <typename valueType>
const TypeSpec& RlValue<valueType>::getTypeSpec( void ) const {
    
    return value[0]->getTypeSpec();
}


template <typename valueType>
bool RlValue<valueType>::isConvertibleTo( const TypeSpec &type ) const {
    
    return value[0]->isConvertibleTo( type );
}


template <typename valueType>
bool RlValue<valueType>::isNULL( void ) const {
    
    return value.size() == 0 || value[0] == NULL;
}


template <typename valueType>
bool RlValue<valueType>::isTypeSpec( const TypeSpec &ts ) const {
    
    if ( value.size() < 1) {
        throw RbException("Trying to access NULL element!!!");
    }
    return value[0]->isTypeSpec( ts );
}


template <typename valueType>
void RlValue<valueType>::printValue( std::ostream &o ) const {
    
    typename std::vector<RbPtr<valueType> >::const_iterator i;
    for ( i = value.begin(); i != value.end(); ++i) {
        (*i)->printValue( o );
    }
}



#endif
