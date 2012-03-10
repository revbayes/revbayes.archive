/**
 * @file
 * This file contains the declaration of Func_distance, which is used to construct
 * a rate matrix for the GTR model of nucleotide substitution
 * 
 * @brief Declaration of Func_distance
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_distance_H
#define Func_distance_H

#include "RbFunction.h"
#include "DistanceMatrix.h"

class DAGNode;


class Func_distance :  public RbFunction {
    
    public:
        Func_distance(void);
    
        // Basic utility functions
        Func_distance*              clone(void) const;                                                       //!< Clone the object
        static const std::string&   getClassName(void);                                                      //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                                  //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                                 //!< Get language type of the object
        
        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;                                            //!< Get argument rules
        const TypeSpec&             getReturnType(void) const;                                               //!< Get type of return value

    protected:
        const RbLanguageObject&     executeFunction(void);                                                   //!< Execute function
        void                        setArgumentVariable(const std::string& name, const Variable* var);

    private:
        double                      distanceP(const TaxonData& td1, const TaxonData& td2);
        double                      distanceJC69(const TaxonData& td1, const TaxonData& td2);

        // Arguments
        RbConstVariablePtr          data;
        RbConstVariablePtr          model;
        RbConstVariablePtr          freqs;
        RbConstVariablePtr          asrv;
        RbConstVariablePtr          shape;
        RbConstVariablePtr          pinvar;
    
        // function return value
        DistanceMatrix              matrix;

};

#endif

