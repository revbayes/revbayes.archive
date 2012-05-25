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
 * $Id: RlCharacterData.h -1   $
 */

#ifndef RlCharacterData_H
#define RlCharacterData_H

#include "CharacterData.h"

#include <set>
#include <string>
#include <vector>


class RlCharacterData : public MemberObject {
    
public:
    RlCharacterData();                                                                                                                      //!< Constructor requires character type
    RlCharacterData(const CharacterData& d);                                                                                     //!< Constructor requires character type
        
    // Basic utility functions
    RlCharacterData*                    clone(void) const;                                                                                  //!< Clone object
    static const std::string&           getClassName(void);                                                                                 //!< Get class name
    static const TypeSpec&              getClassTypeSpec(void);                                                                             //!< Get class type spec
    const TypeSpec&                     getTypeSpec(void) const;                                                                            //!< Get language type of the object
    void                                printValue(std::ostream& o) const;                                                                  //!< Print value for user
    
    // Member variable functions
    const MemberRules&                  getMemberRules(void) const;                                                                         //!< Get member rules
    void                                setSimpleMemberValue(const std::string& name, const RbPtr<const RbLanguageObject> &var);            //!< Catch setting of the topology
    
    // Member method inits
    const MethodTable&                  getMethods(void) const;                                                                             //!< Get methods
    RbPtr<RbLanguageObject>             executeSimpleMethod(const std::string& name, const std::vector<const RbObject*>& args);             //!< Override to map member methods to internal functions
        
    const CharacterData&                getValue(void) const;
        
private:
    CharacterData                       data;
};

#endif

