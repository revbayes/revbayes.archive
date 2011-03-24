/**
 * @file
 * This file contains the declaration of RbComplex, which is
 * the RevBayes abstract base class for complex objects.
 *
 * @brief Declaration of RbComplex
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2009-11-18 01:05:57 +0100 (Ons, 18 Nov 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2009-09-09
 * @extends RbObject
 *
 * $Id: RbComplex.h 63 2009-11-18 00:05:57Z ronquist $
 */

#ifndef RbComplex_H
#define RbComplex_H

#include "RbObject.h"

#include <map>
#include <ostream>
#include <string>
#include <vector>

class Argument;
class ArgumentRule;
class ContainerIterator;
class DAGNode;
class DeterministicNode;
class Frame;
class VectorInteger;
class MethodTable;
class MoveSchedule;
class VectorString;

typedef Frame                        MemberTable;                                                            //!< Member table type def, for convenience
typedef std::vector<ArgumentRule*>   MemberRules;                                                            //!< Member rules type def, for convenience

class RbComplex : public RbObject {

    public:
		virtual                     ~RbComplex(void) {}                                                      //!< Virtual destructor 

        // Basic utility functions you have to override
        virtual RbComplex*           clone(void) const = 0;                                                  //!< Clone object
        virtual bool                 equals(const RbObject* obj) const = 0;                                  //!< Equals comparison
		virtual const VectorString&  getClass(void) const;                                                   //!< Get class vector
		virtual void                 printValue(std::ostream& o) const = 0;                                  //!< Print value for user
		virtual std::string          richInfo(void) const = 0;                                               //!< Complete info 

        // Member variable functions: override if object contains member variables
        virtual const MemberRules&   getMemberRules(void) const;                                             //!< Get member rules
        virtual const MemberTable&   getMembers(void) const;                                                 //!< Get members
        virtual const RbObject*      getValue(const std::string& name) const;                                //!< Get member value
        virtual const DAGNode*       getVariable(const std::string& name) const;                             //!< Get member variable
        virtual void                 setValue(const std::string& name, RbObject* val);                       //!< Set member value
        virtual void                 setVariable(const std::string& name, DAGNode* var);                     //!< Set member variable

        // Member method functions: override if object contains member functions
        virtual DAGNode*             executeMethod(const std::string& name, int funcId);                     //!< Execute method with preprocessed args
        virtual DAGNode*             executeMethod(const std::string& name, std::vector<Argument>& args);    //!< Execute method
        virtual const MethodTable&   getMethods(void) const;                                                 //!< Get method descriptions
        virtual int                  setArguments(const std::string& name, std::vector<Argument>& args);     //!< Set arguments of method 

		// Element access functions: override if object contains elements
        ContainerIterator            begin(void) const;                                                      //!< Begin iterator
        ContainerIterator            end(void) const;                                                        //!< End iterator
        virtual int                  getDim(void) { return 0; }                                              //!< Get subscript dimensions
        virtual RbObject*            getElement(const VectorInteger& index) const;                           //!< Get element (copy!)
        virtual DAGNode*             getElementRef(const VectorInteger& index);                              //!< Get reference to element (for modifying value)
        virtual const VectorInteger& getLength(void) const;                                                  //!< Get length in each dim
        virtual void                 resize(const VectorInteger& len);                                       //!< Resize
        virtual void                 setElement(const VectorInteger& index, RbObject* val);                  //!< Set element
        virtual void                 setLength(const VectorInteger& len);                                    //!< Set length in each dim

        // Default moves for types that contain sets of stochastic nodes
        MoveSchedule*                getDefaultMoves(DeterministicNode* node) const { return NULL; }         //!< Get block moves applicable to the complex datatype

	protected:
                                     RbComplex(void) : RbObject() {}                                         //!< No objects of this class
};

#endif

