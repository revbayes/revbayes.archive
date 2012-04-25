/**
 * @file
 * This file contains the declaration of AbstractInferenceFunction, which is
 * the interface and abstract base class for RevBayes functions.
 *
 * @brief Declaration of AbstractInferenceFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-16 17:27:08 -0700 (Mon, 16 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @interface AbstractInferenceFunction
 * @package functions
 * @since Version 1.0, 2012-04-16
 *
 * $Id: AbstractInferenceFunction.h 1378 2012-04-17 00:27:08Z hoehna $
 */

#ifndef AbstractInferenceFunction_H
#define AbstractInferenceFunction_H

#include "InferenceFunction.h"
#include "RbValue.h"

#include <vector>

class AbstractInferenceFunction : public InferenceFunction {
    
public:
    virtual                                        ~AbstractInferenceFunction(void) {};                                     //!< Destructor
    
    // functions you have to override
    virtual AbstractInferenceFunction*              clone(void) const = 0;                                                  //!< Create a clone of the function
    
    // functions you cannot overwrite
    void                                            execute(void);                                                          //!< Execute function
    void                                            setArguments(const std::vector<RbValue<void*> > &args);                 //!< Set the argument for the label. We collect the argument and delegate to setArgumentVariable()
        
protected:
    AbstractInferenceFunction(void) {};                                                                                     //!< Basic constructor
    
    // functions you have to overwrite
    virtual void                                    executeSimple(std::vector<size_t> &result_offsets) = 0;                 //!< Execute function
    virtual void                                    setInternalArguments(const std::vector<RbValue<void*> > &args) = 0;     //!< Set the argument for the label. We collect the argument and delegate to setArgumentVariable()
    
    // helper functions
    void                                            execute(std::vector<size_t> &result_offsets, size_t level);             //!< Execute repeated for each element in this dimension
    
    std::vector<RbValue<void*> >                    members;
};

#endif

