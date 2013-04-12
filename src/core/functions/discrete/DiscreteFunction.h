/**
 * @file
 * This file contains the declaration of the interface for all discrete functions. Thus,
 * it derives from the template Function<valueType>. See Function.h for more details.
 *
 * @brief Declaration of continous functions.
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-06-20 22:57:09 +0200 (Wed, 20 Jun 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-09-02, version 1.0
 *
 * $Id: Function.h 1643 2012-06-20 20:57:09Z hoehna $
 */

#ifndef DiscreteFunction_H
#define DiscreteFunction_H

#include "TypedFunction.h"

#include <iostream>

namespace RevBayesCore {
    
    template <class valueType>
    class DiscreteFunction : public TypedFunction<valueType> {
        
    public:
        // constructors and destructor
        virtual                            ~DiscreteFunction(void) {}
        
        // pure virtual public methors
        virtual DiscreteFunction*           clone(void) const = 0;                                  //!< Clone the function
        virtual void                        update(void) = 0;                                       //!< Clone the function
        
    protected:
        DiscreteFunction(valueType *v);
        
    };
    
}


template <class valueType>
RevBayesCore::DiscreteFunction<valueType>::DiscreteFunction( valueType *v ) : TypedFunction<valueType>( v ) {
    
}

#endif
