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

#include "RbFunction.h"

class DAGNode;
class VectorString;



const std::string Func_gtr_name = "GTR function";

class Func_gtr :  public RbFunction {
    
    public:
        // Basic utility functions
        Func_gtr*                   clone(void) const;                                                       //!< Clone the object
        const VectorString&         getClass(void) const;                                                    //!< Get class vector
        const TypeSpec&             getTypeSpec(void) const;                                                //!< Get language type of the object
        
        // Regular functions
        const ArgumentRules*        getArgumentRules(void) const;                                            //!< Get argument rules
        const TypeSpec&             getReturnType(void) const;                                               //!< Get type of return value

    protected:
        RbLanguageObject*           executeFunction(void);                                                   //!< Execute function

    private:
        static const TypeSpec       typeSpec;
        static const TypeSpec       returnTypeSpec;

};

#endif

