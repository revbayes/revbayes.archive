/**
 * @file
 * This file contains the declaration of MemberObject, an abstract base
 * class for complex objects with member variables and methods.
 *
 * @todo For now, we do not allow function overloading. Overriding is manual.
 *
 * @brief Declaration of MemberObject
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-14 12:43:32 +0100 (Mån, 14 Dec 2009) $
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id: MemberObject.h 194 2009-12-14 11:43:32Z ronquist $
 */

#ifndef MemberObject_H
#define MemberObject_H

#include "RbComplex.h"
#include "StringVector.h"

#include <set>
#include <string>

class ArgumentRule;
class DAGNode;
class StringVector;

class MemberObject: public RbComplex {

    public:
	    virtual                         ~MemberObject() {}                                  //!< Destructor

        // Basic utility functions
        virtual RbObject*               clone(void) const = 0;                              //!< Clone object
        virtual bool                    equals(const RbObject* x) const = 0;                //!< Equals comparison
        virtual const StringVector&     getClass(void) const;                               //!< Get class vector
        virtual void                    printValue(std::ostream& o) const = 0;              //!< Print value for user
        virtual std::string             toString(void) const = 0;                           //!< Complete info

        // Member variable functions
        const MemberTable&              getMembers(void) { return members; }                //!< Get members
        virtual const ArgumentRule**    getMemberRules(void) = 0;                           //!< Get member rules
        const RbObject*                 getValue(const std::string& name) const;            //!< Get member value
        const DAGNode*                  getVariable(const std::string& name) const;         //!< Get member variable
        void                            setValue(const std::string& name, RbObject* val);   //!< Set member value
        void                            setVariable(const std::string& name, DAGNode* var); //!< Set member variable

        // Member method functions
        const RbObject*                 executeMethod(const std::string& name);                                 //!< Execute method with preprocessed args
        const RbObject*                 executeMethod(const std::string& name, std::vector<Argument>& args);    //!< Execute method
        const MethodTable&              getMethods(void) const { return methods; }                              //!< Get method descriptions
        void                            setArguments(const std::string& name, std::vector<Argument>& args);     //!< Set arguments of method

    protected:
                                        MemberObject(const ArgumentRule** memberRules);                         //!< Constructor

        virtual const RbObject*         executeOperation(const std::string&     name,
                                                         std::vector<DAGNode*>& args) = 0;      //!< Execute method

        // Member variables
        MemberTable                     members;                        //!< Member variables
        MethodTable                     methods;                        //!< Member methods
};

#endif

