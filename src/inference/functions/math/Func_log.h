/**
 * @file
 * This file contains the declaration of Func_log, which 
 * calculates logarithm of 'a' to the base 'b'.
 *
 * @brief Declaration of Func_log
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_log_H
#define Func_log_H


#include "AbstractInferenceFunction.h"
#include "RbValue.h"

class DAGNode;


class Func_log :  public AbstractInferenceFunction {
    
public:
    // Basic utility functions
    Func_log*                   clone(void) const;                                          //!< Clone the object
    
protected:
    void                        setInternalArguments(const std::vector<RbValue<void*> >& args);         //!< Set the argument for the label. We collect the argument and delegate to setArgumentVariable()
    void 					    executeSimple(std::vector<size_t> &offsets);                            //!< Execute function

private:
    RbValue<double*>            result;
    
    // arguments
    RbValue<double*>            x;
    RbValue<double*>            base;
    
};

#endif

