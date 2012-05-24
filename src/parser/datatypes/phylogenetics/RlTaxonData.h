/**
 * @file
 * This file contains the declaration of RlTaxonData, an
 * abstract base class for different types of character vectors.
 *
 * @brief Declaration of RlTaxonData
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: RlTaxonData.h -1   $
 */

#ifndef RlTaxonData_H
#define RlTaxonData_H

#include "MemberObject.h"
#include "TaxonData.h"

#include <ostream>
#include <string>
#include <vector>

class Character;


class RlTaxonData : public MemberObject {
    
public:
    RlTaxonData(const std::string &tname="");                                                                    //!< Constructor with taxon name
    RlTaxonData(const TaxonData &td);                                                                           //!< Constructor with internal data element
    
    
    // Basic utility functions you have to override
    RlTaxonData*                            clone(void) const;                                                  //!< Clone object
    static const std::string&               getClassName(void);                                                 //!< Get class name
    static const TypeSpec&                  getClassTypeSpec(void);                                             //!< Get class type spec
    const TypeSpec&                         getTypeSpec(void) const;                                            //!< Get language type of the object
    void                                    printValue(std::ostream& o) const;                                  //!< Print value for user
    
    // Member object methods 
    RbPtr<RbLanguageObject>                 executeSimpleMethod(const std::string& name, const std::vector<const RbObject*>& args);            //!< Override to map member methods to internal functions
    const MemberRules&                      getMemberRules(void) const;                                         //!< Get member rules
    virtual const MethodTable&              getMethods(void) const;                                             //!< Get methods
    void                                    setSimpleMemberValue(const std::string &name, const RbPtr<const RbLanguageObject> &var);    //!< Set a member variable. We catch here setting of variable nodes
        
    const TaxonData&                        getTaxonData(void) const;
    
private:
    TaxonData                               taxon;                                                              //!< taxon element               
    
    
};

#endif