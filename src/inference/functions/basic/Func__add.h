/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func__add, which is used to add two
 * variables.
 *
 * @brief Declaration and implementation of Func__add
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func__add_H
#define Func__add_H

#include "InferenceFunction.h"

#include <map>
#include <string>

template <typename firstValType, typename secondValType, typename retType>
class Func__add :  public InferenceFunction {

    public:
                                    Func__add( void );
    
        // Basic utility functions
        Func__add*                  clone(void) const;                                          //!< Clone the object
        void                        execute(void);                                              //!< Execute function
        void                        setArguments(const std::vector<RbValue<void*> >& args);     //!< Set the argument for the label. We collect the argument and delegate to setArgumentVariable()
    
private:
        void                        execute(size_t result_offset, size_t first_offset, size_t second_offset, size_t level);

        // Arguments
        RbValue<firstValType*>      first;
        RbValue<secondValType*>     second;
    
        // function return value
        RbValue<retType*>           retValue;
};

#endif


/** default constructor */
template <typename firstValType, typename secondValType, typename retType>
Func__add<firstValType, secondValType, retType>::Func__add( void ) : InferenceFunction( ) {

}


/** Clone object */
template <typename firstValType, typename secondValType, typename retType>
Func__add<firstValType, secondValType, retType>* Func__add<firstValType, secondValType, retType>::clone( void ) const {

    return new Func__add( *this );
}


/** Execute function */
template <typename firstValType, typename secondValType, typename retType>
void Func__add<firstValType, secondValType, retType>::execute( void ) {
    
    // *result.value = exp( *lambda.value );
    
    execute(0,0,0,0);
}


template <typename firstValType, typename secondValType, typename retType>
void Func__add<firstValType, secondValType, retType>::execute(size_t result_offset, size_t first_offset, size_t second_offset, size_t level) {
    
    // first, we test if we have another dimension
    if ( retValue.lengths.size() == level ) {
        // no, we execute the actual function here
        retValue.value[result_offset] = first.value[first_offset] + second.value[second_offset];
    }
    else {
        // next, we compute the number of elements we need to compute
        size_t result_elements = 1;
        for (size_t i = level+1; i < retValue.lengths.size(); ++i) {
            result_elements *= retValue.lengths[i];
        }
        
        // and for the parameters
        size_t first_elements = 1;
        for (size_t i = level+1; i < first.lengths.size(); ++i) {
            first_elements *= first.lengths[i];
        }
        size_t second_elements = 1;
        for (size_t i = level+1; i < second.lengths.size(); ++i) {
            second_elements *= second.lengths[i];
        }
        
        // now, we iterate over the elements in this dimension
        for (size_t i = 0; i < retValue.lengths[level]; ++i) {
            // we recompute the offset
            size_t nested_offset_result = result_offset + result_elements * i;
            
            size_t nested_offset_first = first_offset;
            if ( level < first.lengths.size() ) {
                nested_offset_first += first_elements * i;
            }
            
            size_t nested_offset_second = second_offset;
            if ( level < second.lengths.size() ) {
                nested_offset_second += second_elements * i;
            }
            // and call the function for one level deeper
            execute(nested_offset_result, nested_offset_first, nested_offset_second, level+1);
        }
    }
}



/** We catch here the setting of the argument variables to store our parameters. */
template <typename firstValType, typename secondValType, typename retType>
void Func__add<firstValType, secondValType, retType>::setArguments(const std::vector<RbValue<void*> >& args) {
    
    
    first.value         = ( static_cast<firstValType*>( args[0].value ) );
    first.lengths       = args[0].lengths;
    
    second.value        = static_cast<secondValType*>( args[1].value );
    second.lengths      = args[1].lengths;
    
    retValue.value      = static_cast<retType*>( args[2].value );
    retValue.lengths    = args[2].lengths;
}

