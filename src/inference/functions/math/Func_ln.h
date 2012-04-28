/**
 * @file
 * This file contains the declaration of Func_ln, which 
 * calculates 'e' to the power of 'b'.
 *
 * @brief Declaration of Func_ln
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_ln_H
#define Func_ln_H

#include "AbstractInferenceFunction.h"
#include "RbValue.h"

class Func_ln : public AbstractInferenceFunction {
    
public:
    Func_ln(void);                                                                                     //!< default constructor
    
    // Basic utility functions
    Func_ln*                    clone(void) const;                                                      //!< Clone the object
    
protected:
    void                        setInternalArguments(const std::vector<RbValue<void*> >& args);         //!< Set the argument for the label. We collect the argument and delegate to setArgumentVariable()
    void                        executeSimple(std::vector<size_t> &offsets);                            //!< Execute function
    
private:
    
    RbValue<double*>            x;
    RbValue<double*>            result;
};

#endif

