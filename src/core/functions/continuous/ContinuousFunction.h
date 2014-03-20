/**
 * @file
 * This file contains the declaration of the interface for all continuous functions. Thus,
 * it derives from the template Function<double>. See Function.h for more details.
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

#ifndef ContinuousFunction_H
#define ContinuousFunction_H

#include "TypedFunction.h"

#include <iostream>

namespace RevBayesCore {
    
    class ContinuousFunction : public TypedFunction<double> {
        
    public:
        // constructors and destructor
        virtual                            ~ContinuousFunction(void) {}
                
        // pure virtual public methors
        virtual ContinuousFunction*         clone(void) const = 0;                                  //!< Clone the function
        virtual void                        update(void) = 0;                                       //!< Clone the function
        
    protected:
        ContinuousFunction(double *v, std::string revDecl = "");
        
    };
        
}

#endif
