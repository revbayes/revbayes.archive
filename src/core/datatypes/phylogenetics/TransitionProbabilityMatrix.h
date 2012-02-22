/**
 * @file
 * This file contains the declaration of TransitionProbabilityMatrix, which is
 * class that holds a matrix of transition.
 *
 * @brief Declaration of TransitionProbabilityMatrix
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef TransitionProbabilityMatrix_H
#define TransitionProbabilityMatrix_H

#include "ConstantMemberObject.h"
#include "Natural.h"

class ArgumentRule;
class DAGNode;
class MatrixReal;
class VectorReal;
class VectorString;


class TransitionProbabilityMatrix : public ConstantMemberObject {

    public:
                                            TransitionProbabilityMatrix(void);                                 //!< Default constructor (never call this except from the workspace once)
                                            TransitionProbabilityMatrix(const TransitionProbabilityMatrix& m); //!< Copy constructor
                                            TransitionProbabilityMatrix(size_t n);                             //!< Construct rate matrix with n states
                                           ~TransitionProbabilityMatrix(void);                                 //!< Destructor

        // overloaded operators
        TransitionProbabilityMatrix&        operator=(const TransitionProbabilityMatrix& m);
        VectorReal&                         operator[](size_t i);                                              //!< Subscript operator
        const VectorReal&                   operator[](size_t i) const;                                        //!< Subscript operator (const)
    
        // Basic utility functions
        TransitionProbabilityMatrix*        clone(void) const;                                                 //!< Clone object
        static const std::string&           getClassName(void);                                                         //!< Get class name
        static const TypeSpec&              getClassTypeSpec(void);                                                     //!< Get class type spec
        const TypeSpec&                     getTypeSpec(void) const;                                           //!< Get language type of the object
        void                                printValue(std::ostream& o) const;                                 //!< Print value for user

        // Member variable rules
        const MemberRules&                  getMemberRules(void) const;                                        //!< Get member rules

        // Member method inits
        const MethodTable&                  getMethods(void) const;                                            //!< Get methods

    protected:
        const RbLanguageObject&             executeOperationSimple(const std::string& name, const std::vector<Argument>& args);//!< Map method call to internal functions

    private:
        Natural                             numStates;                                                         //!< The number of character states
        MatrixReal*                         theMatrix;                                                         //!< Holds the transition probability matrix
    
};

#endif

