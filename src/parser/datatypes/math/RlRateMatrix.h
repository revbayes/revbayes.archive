/**
 * @file
 * This file contains the declaration of RateMatrix, which is
 * class that holds a rate matrix for a continuous-time Markov model.
 *
 * @brief Declaration of RateMatrix
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef RlRateMatrix_H
#define RlRateMatrix_H

#include "RateMatrix.h"
#include "MemberObject.h"
#include "Natural.h"
#include "RbBoolean.h"
#include "RealPos.h"
#include <complex>
#include <vector>


class RlRateMatrix : public MemberObject {
    
public:
    RlRateMatrix(void);                                                                 //!< Default constructor (never call this except from the workspace once)
    ~RlRateMatrix(void);                                                                 //!< Destructor
    
    // overloaded operators
    RlRateMatrix&                       operator=(const RlRateMatrix& r);
    
    // Basic utility functions
    RlRateMatrix*                       clone(void) const;                                                                  //!< Clone object
    static const std::string&           getClassName(void);                                                                 //!< Get class name
    static const TypeSpec&              getClassTypeSpec(void);                                                             //!< Get class type spec
    const TypeSpec&                     getTypeSpec(void) const;                                                            //!< Get language type of the object   
    RbValue<void*>                      getLeanValue(void) const;                                                           //!< Transform the object into a basic element pointer for fast access.
    size_t                              memorySize() const;                                                                 //!< Get the size
    void                                printValue(std::ostream& o) const;                                                  //!< Print value for user
    
    // Member variable rules
    const MemberRules&                  getMemberRules(void) const;                                                         //!< Get member rules
    
    // Member method inits
    const MethodTable&                  getMethods(void) const;                                                             //!< Get methods
        
    RbPtr<RbLanguageObject>             executeSimpleMethod(const std::string& name, const std::vector<const RbObject *>& args);        //!< Override to map member methods to internal functions
    
private:

    RateMatrix                          rm;
};

#endif
