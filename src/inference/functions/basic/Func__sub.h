/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func__sub, which is used to subtract
 * a variable from another.
 *
 * @brief Declaration and implementation of Func__sub
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-19 12:03:23 -0700 (Thu, 19 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: Func__sub.h 1400 2012-04-19 19:03:23Z hoehna $
 */

#ifndef Func__sub_H
#define Func__sub_H

#include "AbstractInferenceFunction.h"

#include <map>
#include <string>

template <typename firstValType, typename secondValType, typename retType>
class Func__sub :  public AbstractInferenceFunction {
	
public:
	Func__sub( void );
    
	// Basic utility functions
	Func__sub*                  clone(void) const;                                                      //!< Clone the object
	
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
Func__sub<firstValType, secondValType, retType>::Func__sub( void ) : AbstractInferenceFunction( ) {
	
}


/** Clone object */
template <typename firstValType, typename secondValType, typename retType>
Func__sub<firstValType, secondValType, retType>* Func__sub<firstValType, secondValType, retType>::clone( void ) const {
	
    return new Func__sub( *this );
}


template <typename firstValType, typename secondValType, typename retType>
void Func__sub<firstValType, secondValType, retType>::executeSimple(std::vector<size_t> &offset) {
    
    retValue.value[offset[2]] = first.value[offset[0]] - second.value[offset[1]];
    
}



/** We catch here the setting of the argument variables to store our parameters. */
template <typename firstValType, typename secondValType, typename retType>
void Func__sub<firstValType, secondValType, retType>::setInternalArguments(const std::vector<RbValue<void*> >& args) {
    
    
    first.value         = ( static_cast<firstValType*>( args[0].value ) );
    first.lengths       = args[0].lengths;
    
    second.value        = static_cast<secondValType*>( args[1].value );
    second.lengths      = args[1].lengths;
    
    retValue.value      = static_cast<retType*>( args[2].value );
    retValue.lengths    = args[2].lengths;
}

