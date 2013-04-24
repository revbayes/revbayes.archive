/**
 * @file
 * This file contains the declaration of Alignment, which is
 * class that holds a character matrix in RevBayes.
 *
 * @brief Declaration of Alignment
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef RlAbstractCharacterData_H
#define RlAbstractCharacterData_H

#include "AbstractCharacterData.h"
#include "RlModelVariableWrapper.h"

#include <set>
#include <string>
#include <vector>


namespace RevLanguage {
    
    
    class AbstractCharacterData : public RlModelVariableWrapper<RevBayesCore::AbstractCharacterData> {
        
    public:
        AbstractCharacterData(void);                                                                                                                    //!< Constructor requires character type
        AbstractCharacterData(RevBayesCore::AbstractCharacterData *v);                                                          //!< Constructor requires character type
        AbstractCharacterData(RevBayesCore::TypedDagNode<RevBayesCore::AbstractCharacterData>*d);                              //!< Constructor requires character type
        AbstractCharacterData(const AbstractCharacterData& d);                                                                                                  //!< Constructor requires character type
        
        typedef RevBayesCore::AbstractCharacterData valueType;
        
        // Basic utility functions
        AbstractCharacterData*              clone(void) const;                                                                                  //!< Clone object
        static const std::string&           getClassName(void);                                                                                 //!< Get class name
        static const TypeSpec&              getClassTypeSpec(void);                                                                             //!< Get class type spec
        const TypeSpec&                     getTypeSpec(void) const;                                                                            //!< Get language type of the object
        
        // Member method inits
        const MethodTable&                  getMethods(void) const;                                             //!< Get methods
        RbLanguageObject*                   executeMethod(const std::string& name, const std::vector<Argument>& args);  //!< Override to map member methods to internal functions
        
    };
    
}


#endif

