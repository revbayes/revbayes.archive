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

#include "MemberFrame.h"
#include "MethodTable.h"
#include "RbComplex.h"

#include <set>
#include <string>

class ArgumentFrame;
class ArgumentRule;
class DAGNode;
class MethodTable;
class VectorString;

typedef std::vector<ArgumentRule*>   MemberRules;                                                                       //!< Member rules type def, for convenience


class MemberObject: public RbComplex {

    public:
        virtual                    ~MemberObject(void) {}                                                               //!< Destructor

        // Basic utility functions you have to override
        virtual MemberObject*       clone(void) const = 0;                                                              //!< Clone object
        virtual MemberObject*       cloneAsConstant(void) const;                                                        //!< Make a clone with only constant members and elements
        virtual const VectorString& getClass(void) const;                                                               //!< Get class vector
        
        // Basic utility functions you may want to override
        virtual RbObject*           convertTo(const std::string& type, size_t dim) const;                               //!< Convert to type and dim (default throws an error)
        virtual bool                isConvertibleTo(const std::string& type, size_t dim, bool once) const;              //!< Is convertible to type and dim? (default false)
        virtual void                printValue(std::ostream& o) const;                                                  //!< Print value for user
        virtual std::string         richInfo(void) const;                                                               //!< Complete info

        // Basic utility functions you do not have to override
        bool                        isConstant(void) const;                                                             //!< Is the object, including all member variables and elements, constant?
        DAGNode*                    wrapIntoVariable(void);                                                             //!< Wrap value into variable

        // Member variable functions you do not have to override
        const MemberFrame&          getMembers(void) const { return members; }                                          //!< Get members
        const TypeSpec&             getMemberTypeSpec(const std::string& name) const;                                   //!< Get type spec for a member variable
        const RbObject*             getValue(const std::string& name) const;                                            //!< Get member value (const)
        const DAGNode*              getVariable(const std::string& name) const;                                         //!< Get member variable @Fredrik: Why is there a const getVariable() if there is also a non-const getVariable()? (Sebastian)
        DAGNode*                    getVariable(const std::string& name);                                               //!< Get member variable (non-const ptr)
        void                        setValue(const std::string& name, RbObject* val);                                   //!< Set member value

        // Member variable functions you may wish to override
        virtual const MemberRules&  getMemberRules(void) const;                                                         //!< Get member rules
        virtual void                setVariable(const std::string& name, DAGNode* var);                                 //!< Set member variable

        // Member method functions
        DAGNode*                    executeMethod(const std::string& name, const std::vector<Argument>& args);          //!< Direct call of member method
        virtual DAGNode*            executeOperation(const std::string& name, ArgumentFrame& args);                     //!< Override to map member methods to internal functions
        virtual const MethodTable&  getMethods(void) const;                                                             //!< Get member methods


        // Direct string index functions you do not need to override
        DAGNode*                    getElement(std::string& elemName);                                                  //!< Get element from string index
        void                        setElement(std::string& elemName, DAGNode* var, bool convert=true);                 //!< Set element
    
        // Direct index functions you have to override to support basic indexing in the parser
        virtual DAGNode*            getElement(size_t index);                                                           //!< Get element from size_t index
        virtual size_t              getElementsSize(void) const { return 0; }                                           //!< Number of subscript elements
        virtual void                setElement(size_t index, DAGNode* var, bool convert=true);                          //!< Set element
        virtual bool                supportsIndex(void) const { return false; }                                         //!< Does object support index operator?

        // Function you need to override to support string indexing
        virtual size_t              getElementIndex(std::string& elemName) const;                                       //!< Override to support string indexing
        
        // Recursive index functions - override to support empty indices or more elaborate checking
        virtual DAGNode*            getElement(VectorIndex& index);                                                     //!< Return element (for parser)
        virtual void                setElement(VectorIndex& index, DAGNode* var, bool convert=true);                    //!< Set element or subelement

    protected:
									MemberObject(const MemberRules& memberRules);                                       //!< Standard constructor
                                    MemberObject(void){}                                                                //!< Default constructor; no members or methods
 
        // Members is the variable frame that stores member variables
        MemberFrame                 members;                                                                            //!< Member variables

        // Friend classes
        friend class                DistributionFunction;                                                               //!< Give DistributionFunction access
        friend class                MemberNode;                                                                         //!< Give MemberNode access
};

#endif

