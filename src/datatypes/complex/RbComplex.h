/**
 * @file
 * This file contains the declaration of RbComplex, which is
 * the RevBayes abstract base class for complex objects.
 *
 * @brief Declaration of RbComplex
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2009-11-18 01:05:57 +0100 (Ons, 18 Nov 2009) $
 * @author The REvBayes development core team
 * @license GPL version 3
 * @since Version 1.0, 2009-09-09
 * @extends RbObject
 *
 * $Id: RbComplex.h 63 2009-11-18 00:05:57Z ronquist $
 */

#ifndef RbComplex_H
#define RbComplex_H

#include "ContainerIterator.h"
#include "RbObject.h"

#include <map>
#include <ostream>
#include <string>
#include <vector>

class Argument;
class ArgumentRule;
class DAGNode;
class DeterministicNode;
class IntVector;
class MemberFunction;
class MoveSchedule;
class StringVector;

typedef std::map<std::string, DAGNode*>         MemberTable;        //!< Member table type def, for convenience
typedef std::map<std::string, MemberFunction*>  MethodTable;        //!< Method table type def, for convenience

class RbComplex : public RbObject {

    public:
		virtual                         ~RbComplex(void) {}                                                     //!< Virtual destructor 

        // Basic utility functions you have to override
        virtual RbObject*               clone(void) const = 0;                                                  //!< Clone object
        virtual bool                    equals(const RbObject* obj) const = 0;                                  //!< Equals comparison
		virtual const StringVector&     getClass(void) const;                                                   //!< Get class vector
		virtual void                    printValue(std::ostream& o) const = 0;                                  //!< Print value for user
		virtual std::string             toString(void) const = 0;                                               //!< Complete info 

        // Member variable functions: override if object contains member variables
        virtual const ArgumentRule**    getMemberRules(void) const;                                             //!< Get member rules
        virtual const MemberTable&      getMembers(void) const;                                                 //!< Get members
        virtual const RbObject*         getValue(const std::string& name) const;                                //!< Get member value
        virtual const DAGNode*          getVariable(const std::string& name) const;                             //!< Get member variable
        virtual void                    setValue(const std::string& name, RbObject* val);                       //!< Set member value
        virtual void                    setVariable(const std::string& name, DAGNode* var);                     //!< Set member variable

        // Member method functions: override if object contains member functions
        virtual const RbObject*         executeMethod(const std::string& name);                                 //!< Execute method with preprocessed args
        virtual const RbObject*         executeMethod(const std::string& name, std::vector<Argument>& args);    //!< Execute method
        virtual const MethodTable&      getMethods(void) const;                                                 //!< Get method descriptions
        virtual void                    setArguments(const std::string& name, std::vector<Argument>& args);     //!< Set arguments of method 

		// Element access functions: override if object contains elements
        ContainerIterator               begin() const { return ContainerIterator(getLength()); }                //!< First index
        ContainerIterator               end() const;                                                            //!< Last index + 1
        virtual int                     getDim(void) { return 0; }                                              //!< Get subscript dimensions
        virtual const StringVector&     getElementClass(void) const { return getClass() ; }                     //!< Get element class
        virtual const RbObject*         getElement(const IntVector& index) const;                               //!< Get element (read-only)
        virtual const IntVector&        getLength(void) const;                                                  //!< Get length in each dim
        virtual void                    resize(const IntVector& len);                                           //!< Resize
        virtual void                    setElement(const IntVector& index, RbObject* val);                      //!< Set element
        virtual void                    setLength(const IntVector& len);                                        //!< Set length in each dim

        // Default moves for types that contain sets of stochastic nodes
        MoveSchedule*                   getDefaultMoves(DeterministicNode* node) const { return NULL; }         //!< Get block moves applicable to the complex datatype

    protected:
                                        RbComplex(void) : RbObject() {}                                         //!< No objects of this class

        // Override these functions to provide friend classes with modify access to members or elements
        virtual RbObject*               getMemberPtr(const std::string& name);                                  //!< Allow modify access to member
        virtual RbObject*               getElementPtr(const IntVector& index);                                  //!< Allow modify access to element 

        // These are friends that may modify members or elements
        friend class                    SyntaxVariable;                                                         //!< The parser class dealing with variables 
};

#endif

