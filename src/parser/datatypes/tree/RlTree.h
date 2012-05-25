/**
 * @file
 * This file contains the declaration of RlTree,
 * the RevLanguage class holding a tree object.
 *
 * @brief Declaration of RlTree
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: RlTree.h -1   $
 */

#ifndef RlTree_H
#define RlTree_H

#include "MemberObject.h"
#include "TreePlate.h"

#include <ostream>
#include <string>
#include <vector>


class RlTree : public MemberObject {
    
public:
    RlTree(void);                                                                                               //!< Constructor with taxon name
    RlTree(const TreePlate &t);                                                                                 //!< Constructor with internal data element
    
    typedef TreePlate valueType;
    
    // Basic utility functions you have to override
    RlTree*                                 clone(void) const;                                                  //!< Clone object
    static const std::string&               getClassName(void);                                                 //!< Get class name
    static const TypeSpec&                  getClassTypeSpec(void);                                             //!< Get class type spec
    const TypeSpec&                         getTypeSpec(void) const;                                            //!< Get language type of the object
    void                                    printValue(std::ostream& o) const;                                  //!< Print value for user
    
    // Member object methods 
    RbPtr<RbLanguageObject>                 executeSimpleMethod(const std::string& name, const std::vector<const RbObject*>& args);            //!< Override to map member methods to internal functions
    const MemberRules&                      getMemberRules(void) const;                                         //!< Get member rules
    virtual const MethodTable&              getMethods(void) const;                                             //!< Get methods
    void                                    setSimpleMemberValue(const std::string &name, const RbPtr<const RbLanguageObject> &var);    //!< Set a member variable. We catch here setting of variable nodes
    
    const TreePlate&                        getValue(void) const;
    
private:
    TreePlate                               tree;                                                               //!< internal tree element               
    
    
};

#endif