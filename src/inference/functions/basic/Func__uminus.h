/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func__uminus, which is used to perform
 * the unary minus operation.
 *
 * @brief Declaration and implementation of Func__uminus
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef Func__uminus_H
#define Func__uminus_H

#include "AbstractInferenceFunction.h"

#include <map>
#include <string>


template <typename firstValType, typename retType>
class Func__uminus :  public AbstractInferenceFunction {

    public:
                                    Func__uminus( void );
    
		// Basic utility functions
		Func__uminus*                  clone(void) const;                                                      //!< Clone the object

protected:
    void                            setInternalArguments(const std::vector<RbValue<void*> >& args);         //!< Set the argument for the label. We collect the argument and delegate to setArgumentVariable()
    void                            executeSimple(std::vector<size_t> &offset);
    
private:
	
	// Arguments
	RbValue<firstValType*>      first;
    
	// function return value
	RbValue<retType*>           retValue;
};

#endif

/** default constructor */
template <typename firstValType, typename retType>
Func__uminus<firstValType, retType>::Func__uminus( void ) : AbstractInferenceFunction( ) {
	
}

/** Clone object */
template <typename firstValType, typename retType>
Func__uminus<firstValType, retType>* Func__uminus<firstValType, retType>::clone( void ) const {
	
    return new Func__uminus( *this );
}


template <typename firstValType, typename retType>
void Func__uminus<firstValType, retType>::executeSimple(std::vector<size_t> &offset) {
    
    retValue.value[offset[1]] = -(first.value[offset[0]]);
    
}

/** We catch here the setting of the argument variables to store our parameters. */
template <typename firstValType, typename retType>
void Func__uminus<firstValType, retType>::setInternalArguments(const std::vector<RbValue<void*> >& args) {
    
    
    first.value         = static_cast<firstValType*>( args[0].value );
    first.lengths       = args[0].lengths;
        
    retValue.value      = static_cast<retType*>( args[1].value );
    retValue.lengths    = args[1].lengths;
}


