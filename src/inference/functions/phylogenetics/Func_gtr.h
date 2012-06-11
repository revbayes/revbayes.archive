/**
 * @file
 * This file contains the declaration of Func_gtr, which is used to construct
 * a rate matrix for the GTR model of nucleotide substitution
 * 
 * @brief Declaration of Func_gtr
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_gtr_H
#define Func_gtr_H

#include "AbstractInferenceFunction.h"
#include "RateMatrix.h"

class DAGNode;


class Func_gtr :  public AbstractInferenceFunction {
    
    public:
        // Basic utility functions
        Func_gtr*                   clone(void) const;                                                       //!< Clone the object

    protected:
        void                        setInternalArguments(const std::vector<RbValue<void*> >& args);         //!< Set the argument for the label. We collect the argument and delegate to setArgumentVariable()
        void                        executeSimple(std::vector<size_t> &result_offsets);                     //!< Execute function
	
    private:
    
        RbValue<std::vector<double>*>   frequencies;
        RbValue<std::vector<double>*>   rates;
        RbValue<RateMatrix*>        result;
    
};

#endif

