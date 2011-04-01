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
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id: MemberObject.h 194 2009-12-14 11:43:32Z ronquist $
 */

#ifndef MemberObject_H
#define MemberObject_H

#include "Frame.h"
#include "MethodTable.h"
#include "RbComplex.h"

#include <set>
#include <string>

class ArgumentRule;
class DAGNode;
class MethodTable;
class VectorString;

typedef Frame                        MemberTable;                                                            //!< Member table type def, for convenience
typedef std::vector<ArgumentRule*>   MemberRules;                                                            //!< Member rules type def, for convenience


class MemberObject: public RbComplex {

    public:
        virtual                    ~MemberObject(void) {}                                                               //!< Destructor

        // Basic utility functions you have to override
        virtual MemberObject*       clone(void) const = 0;                                                              //!< Clone object
        virtual const VectorString& getClass(void) const;                                                               //!< Get class vector
        
        // Basic utility functions you may want to override
        virtual MemberObject*       convertTo(const std::string& type, int dim) const;                                  //!< Convert to language object of type and dim (default throws an error)
        virtual bool                isConvertibleTo(const std::string& type, int dim, bool once) const;                 //!< Is convertible to language object of type and dim? (default false)
        virtual void                printValue(std::ostream& o) const;                                                  //!< Print value for user
        virtual std::string         richInfo(void) const;                                                               //!< Complete info

        // DAG utility function you do not have to override
        MemberObject*               getConstValue(void) const;                                                          //!< Make a constant clone

        // Member variable functions; override getMemberRules to add member variables
        const MemberTable&          getMembers(void) const { return members; }                                          //!< Get members
        virtual const MemberRules&  getMemberRules(void) const;                                                         //!< Get member rules
        const TypeSpec&             getMemberTypeSpec(const std::string& name) const;                                   //!< Get type spec for a member variable
        const RbObject*             getValue(const std::string& name);                                                  //!< Get member value
        const RbObject*             getValue(const std::string& name) const;                                            //!< Get member value (const)
        const DAGNode*              getVariable(const std::string& name) const;                                         //!< Get member variable
        DAGNode*                    getVariable(const std::string& name);                                               //!< Get member variable (non-const ptr)
        void                        setValue(const std::string& name, RbObject* val);                                   //!< Set member value
        virtual void                setVariable(const std::string& name, DAGNode* var);                                 //!< Set member variable

        // Member method functions
        DAGNode*                    executeMethod(const std::string& name, int funcId);                                 //!< Execute method with preprocessed args (repeated evaluation)
        DAGNode*                    executeMethod(const std::string& name, std::vector<Argument>& args);                //!< Execute method (evaluate once)
        virtual const MethodTable&  getMethodInits(void) const;                                                         //!< Get method specifications
        const MethodTable&          getMethods(void) const { return methods; }                                          //!< Get methods

        // Subscript operator functions
        virtual bool                hasSubscript(void) { return false; }                                                //!< Does object support subscripting?
        virtual DAGNode*            getSubelement(const size_t i);                                                      //!< Return subscript[](index) element
        virtual size_t              getSubelementsSize(void) { return 0; }                                              //!< Number of subscript elements
        virtual void                setElement(VectorNatural& index, DAGNode* var);                                     //!< Set subelement, or elements of a subelement; only override if you want full control

    protected:
									MemberObject(const MemberRules& memberRules, const MethodTable& methodInits);       //!< Standard constructor
                                    MemberObject(void){}                                                                //!< Default constructor; no members or methods
        // Protected functions
        virtual DAGNode*            executeOperation(const std::string& name, const std::vector<VariableSlot>& args);   //!< Execute method

        // Members and methods keep track of variables and functions belonging to the object
        MemberTable                 members;                                                                            //!< Member variables
        MethodTable                 methods;                                                                            //!< Member methods

        // Friend class
        friend class                DistributionFunction;                                                               //!< Give distribution function access
};

#endif

