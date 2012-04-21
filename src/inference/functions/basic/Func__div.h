/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func__div, which is used to divide
 * two variables.
 *
 * @brief Declaration and implementation of Func__div
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func__div_H
#define Func__div_H

#include "AbstractInferenceFunction.h"

#include <map>
#include <string>


template <typename firstValType, typename secondValType, typename retType>
class Func__div :  public AbstractInferenceFunction {

    public:
                                    Func__div( void );
        
        // Basic utility functions
        Func__div*                  clone(void) const;                                          //!< Clone the object
protected:
    void                            setInternalArguments(const std::vector<RbValue<void*> >& args);         //!< Set the argument for the label. We collect the argument and delegate to setArgumentVariable()
    void                            executeSimple(std::vector<size_t> &offset);
    
private:
	
	// Arguments
	RbValue<firstValType*>      first;
	RbValue<secondValType*>     second;
    
	// function return value
	RbValue<retType*>           retValue;
};

#endif

/** default constructor */
template <typename firstValType, typename secondValType, typename retType>
Func__div<firstValType, secondValType, retType>::Func__div( void ) : AbstractInferenceFunction( ) {
	
}


/** Clone object */
template <typename firstValType, typename secondValType, typename retType>
Func__div<firstValType, secondValType, retType>* Func__div<firstValType, secondValType, retType>::clone( void ) const {
	
    return new Func__div( *this );
}


template <typename firstValType, typename secondValType, typename retType>
void Func__div<firstValType, secondValType, retType>::executeSimple(std::vector<size_t> &offset) {
    
    retValue.value[offset[2]] = first.value[offset[0]] / second.value[offset[1]];
    
}

template <>
void Func__div<int, int, double>::executeSimple(std::vector<size_t> &offset) {
	
    retValue.value[offset[2]] = double(first.value[offset[0]]) / double(second.value[offset[1]]);
    
}

/** We catch here the setting of the argument variables to store our parameters. */
template <typename firstValType, typename secondValType, typename retType>
void Func__div<firstValType, secondValType, retType>::setInternalArguments(const std::vector<RbValue<void*> >& args) {
    
    
    first.value         = static_cast<firstValType*>( args[0].value );
    first.lengths       = args[0].lengths;
    
    second.value        = static_cast<secondValType*>( args[1].value );
    second.lengths      = args[1].lengths;
    
    retValue.value      = static_cast<retType*>( args[2].value );
    retValue.lengths    = args[2].lengths;
}


