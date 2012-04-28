/**
 * @file
 * This file contains the declaration of Func_exp, which 
 * calculates 'e' to the power of 'b'.
 *
 * @brief Declaration of Func_exp
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_exp_H
#define Func_exp_H

#include "AbstractInferenceFunction.h"
#include "RbValue.h"

class Func_exp : public AbstractInferenceFunction {
    
public:
    Func_exp(void);                                                                                     //!< default constructor
    
    // Basic utility functions
    Func_exp*                   clone(void) const;                                                      //!< Clone the object

protected:
    void                        setInternalArguments(const std::vector<RbValue<void*> >& args);         //!< Set the argument for the label. We collect the argument and delegate to setArgumentVariable()
    void                        executeSimple(std::vector<size_t> &offsets);                            //!< Execute function

private:
    
    RbValue<double*>            lambda;
    RbValue<double*>            result;
};

#endif

