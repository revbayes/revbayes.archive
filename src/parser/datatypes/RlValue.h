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
    
    private:
    void                                printValue(std::ostream& o, int level, int &index) const;

};


#include "RbException.h"
#include "RbNullObject.h"


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
    // compute the number of elements we need to reserve the memory for
    size_t elements = 1;
    for (size_t i = 0; i < lengths.size(); ++i) {
        elements *= lengths[i];
    }
    
    // get the size for each value
    size_t s = value[0]->memorySize();
    
    // allocate the memory
    
    // create the c-style array of elements
    void* data = malloc( elements * s );
    
    // tmp length vector
    std::vector<size_t> tmp_lengths;
    
    // iterate over all elements and copy their values
    typename std::vector<RbPtr<valueType> >::const_iterator i;
    for ( i = value.begin(); i != value.end(); i++ ) {
        const RbObject *x = *i;
        RbValue<void*> elemVal = elemVal = x->getLeanValue();
        memcpy(data, elemVal.value,(*i)->memorySize());
        //        *(data) = *(*i)->getValue(lengths);
        char *tmp_ptr = (char*)data;
        tmp_ptr += (*i)->memorySize();
        data = tmp_ptr;
    }
    char *tmp_ptr = (char*)data;
    tmp_ptr -= s;
    RbValue<void *> v;
    v.value = tmp_ptr;
    v.lengths = lengths;
    
    return v;
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
    
    if (value[0] == NULL) {
        return RbNullObject::getInstance().getTypeSpec();
    }
    
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
    
    if ( value.size() < 1 || value[0] == NULL) {
        throw RbException("Trying to access NULL element!!!");
    }
    return value[0]->isTypeSpec( ts );
}


template <typename valueType>
void RlValue<valueType>::printValue( std::ostream &o ) const {
    
    if (lengths.size() == 0) {
        value[0]->printValue( o );
    }
    else {
        int index = 0;
        printValue(o, 0, index);
    }
}


template <typename valueType>
void RlValue<valueType>::printValue( std::ostream &o, int level, int &index ) const {
    
    size_t size = lengths[level];
    int nextLevel = level + 1;
    
    o << "[ ";
    for (size_t i = 0; i < size; ++i) {
        if ( i != 0 ) {
            o << ", ";
        }
        
        if (lengths.size() == nextLevel) {
            value[index]->printValue( o );
            index++;
        }
        else {
            printValue(o, nextLevel, index);
        }
    }
    
    o << "]";
}



#endif
