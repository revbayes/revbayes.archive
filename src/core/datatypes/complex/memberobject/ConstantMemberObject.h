/**
 * @file
 * This file contains the declaration of MutableMemberObject, an abstract base
 * class for all objects with member variables and methods which can add arbitrary named members.
 *
 * @todo For now, we do not allow function overloading. Overriding is manual.
 *
 * @brief Declaration of MutableMemberObject
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-07-27, version 1.0
 *
 * $Id$
 */

#ifndef ConstantMemberObject_H
#define ConstantMemberObject_H

#include "MemberObject.h"

#include <string>

class VectorString;


class ConstantMemberObject : public MemberObject {
    
public:
    virtual                    ~ConstantMemberObject(void) {}                                        //!< Destructor
    
    // Basic utility functions you have to override
    virtual ConstantMemberObject* clone(void) const = 0;                                             //!< Clone object
    virtual const VectorString& getClass(void) const;                                                //!< Get class vector
    
    // Basic utility functions you may want to override
    virtual RbLanguageObject*   convertTo(const std::string& type) const;                            //!< Convert to type (default throws an error)
    virtual bool                isConvertibleTo(const std::string& type, bool once) const;           //!< Is convertible to type? (default false)
    virtual void                printValue(std::ostream& o) const;                                   //!< Print value for user
    virtual std::string         richInfo(void) const;                                                //!< Complete info
    
    // Basic utility functions you do not have to override
    bool                        isConstant(void) const;                                              //!< Is the object, including all member variables and elements, constant?
    
protected:
    ConstantMemberObject(const MemberRules& memberRules);                                            //!< Standard constructor
    ConstantMemberObject(void);                                                                      //!< Default constructor; no members or methods
    
};

#endif

