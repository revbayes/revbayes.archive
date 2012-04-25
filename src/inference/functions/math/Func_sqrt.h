/**
 * @file
 * This file contains the declaration of Func_sqrt, which 
 * calculates 'e' to the power of 'b'.
 *
 * @brief Declaration of Func_sqrt
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-19 11:54:31 -0700 (Thu, 19 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: Func_sqrt.h 1399 2012-04-19 18:54:31Z hoehna $
 */

#ifndef Func_sqrt_H
#define Func_sqrt_H

#include "AbstractInferenceFunction.h"
#include "RbValue.h"

class Func_sqrt : public AbstractInferenceFunction {
    
public:
    Func_sqrt(void);                                                                                     //!< default constructor
    
    // Basic utility functions
    Func_sqrt*                   clone(void) const;                                                      //!< Clone the object
	
protected:
    void                        setInternalArguments(const std::vector<RbValue<void*> >& args);         //!< Set the argument for the label. We collect the argument and delegate to setArgumentVariable()
    void                        executeSimple(std::vector<size_t> &result_offsets);                     //!< Execute function
	
private:
    
    RbValue<double*>            d;
    RbValue<double*>            result;
};

#endif

