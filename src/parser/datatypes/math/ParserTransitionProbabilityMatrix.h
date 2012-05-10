/**
 * @file
 * This file contains the declaration of ParserTransitionProbabilityMatrix, which is
 * class that holds a matrix of transition.
 *
 * @brief Declaration of ParserTransitionProbabilityMatrix
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef ParserTransitionProbabilityMatrix_H
#define ParserTransitionProbabilityMatrix_H

#include "MemberObject.h"
#include "TransitionProbabilityMatrix.h"


class ParserTransitionProbabilityMatrix : public MemberObject {
    
public:
    ParserTransitionProbabilityMatrix(void);                                                                                    //!< Default constructor (never call this except from the workspace once)
    
    // Basic utility functions
    ParserTransitionProbabilityMatrix*  clone(void) const;                                                                      //!< Clone object
    static const std::string&           getClassName(void);                                                                     //!< Get class name
    static const TypeSpec&              getClassTypeSpec(void);                                                                 //!< Get class type spec
    const TypeSpec&                     getTypeSpec(void) const;                                                                //!< Get language type of the object
    void                                printValue(std::ostream& o) const;                                                      //!< Print value for user
    
    // Member variable rules
    const MemberRules&                  getMemberRules(void) const;                                                             //!< Get member rules
    
    // Member method inits
    const MethodTable&                  getMethods(void) const;                                                                 //!< Get methods
    
protected:
    RbPtr<RbLanguageObject>             executeOperationSimple(const std::string& name, const std::vector<Argument>& args);     //!< Map method call to internal functions
    
private:
    TransitionProbabilityMatrix         m;
};




#endif