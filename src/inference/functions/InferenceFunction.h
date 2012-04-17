/**
 * @file
 * This file contains the declaration of InferenceFunction, which is
 * the interface and abstract base class for RevBayes functions.
 *
 * @brief Declaration of InferenceFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-16 17:27:08 -0700 (Mon, 16 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @interface InferenceFunction
 * @package functions
 * @since Version 1.0, 2012-04-16
 *
 * $Id: InferenceFunction.h 1378 2012-04-17 00:27:08Z hoehna $
 */

#ifndef InferenceFunction_H
#define InferenceFunction_H

#include "RbValue.h"

#include <vector>

class InferenceFunction {
    
public:
    virtual                                        ~InferenceFunction(void) {};                                     //!< Destructor
        
    // InferenceFunction functions you have to override
    virtual InferenceFunction*                      clone(void) const = 0;                                          //!< Create a clone of the function
    virtual void                                    execute(void) = 0;                                              //!< Execute function
    virtual void                                    setArguments(const std::vector<RbValue<void*> > args) = 0;      //!< Set the argument for the label. We collect the argument and delegate to setArgumentVariable()
    
protected:
    InferenceFunction(void) {};                                                                                     //!< Basic constructor
    
};

#endif

