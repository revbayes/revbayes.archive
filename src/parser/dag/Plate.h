/**
 * @file
 * This file contains the declaration of Plate.
 *
 * @brief Declaration of Plate
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-03-23 18:42:52 +0100 (Fri, 23 Mar 2012) $
 * @author The REvBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-04-13, version 1.0
 * @interface DAGNode
 *
 * $Id: DAGNode.h 1358 2012-03-23 17:42:52Z hoehna $
 */

#ifndef Plate_H
#define Plate_H

#include "MemberObject.h"


class Plate : public MemberObject {
    
public:
    Plate();
    
    // Basic utility functions you have to override (also getClassTypeSpec()!)
    virtual Plate*                      clone(void) const;                                                                                  //!< Clone object
    static const std::string&           getClassName(void);                                                                                 //!< Get class name
    static const TypeSpec&              getClassTypeSpec(void);                                                                             //!< Get class type spec
    virtual const TypeSpec&             getTypeSpec(void) const;                                                                            //!< Get the type spec of the instance
    virtual void                        printValue(std::ostream& o) const;                                                                  //!< Print value for user
    
    // Member variable functions
    const MemberRules&                  getMemberRules(void) const;                                                                         //!< Get member rules
    void                                setSimpleMemberValue(const std::string& name, const RbPtr<const RbLanguageObject> &var);            //!< Catch setting of the topology
    
    // Member method inits
    const MethodTable&                  getMethods(void) const;                                                                             //!< Get methods
    RlValue<RbLanguageObject>           executeMethod(const std::string& name, const std::vector<RbPtr<Argument> >& args);                  //!< Execute method
    
    const RbPtr<const Plate>&           getParentPlate(void) const;                                                                         //!< Get the parent plate of this plate.
    std::vector<size_t>                 getPlateLengths(void) const;                                                                        //!< Get the length for this plate and all parent plates
    size_t                              getLength(void) const;                                                                              //!< Get the length for this plate
    
    
private:
    
    RbPtr<const Plate>                  parent;
    size_t                              size;
};

#endif