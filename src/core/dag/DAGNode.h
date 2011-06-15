/**
 * @file
 * This file contains the declaration of DAGNode, which is the base
 * class for nodes in a model DAG.
 *
 * @brief Declaration of DAGNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2010-01-12 21:51:54 +0100 (Tis, 12 Jan 2010) $
 * @author The REvBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @interface DAGNode
 *
 * $Id: DAGNode.h 221 2010-01-12 20:51:54Z Hoehna $
 */

#ifndef DAGNode_H
#define DAGNode_H

#include "TypeSpec.h"

#include <list>
#include <map>
#include <set>
#include <string>

class ContainerNode;
class Frame;
class RbObject;
class VariableNode;
class VariableSlot;
class VectorInteger;
class VectorNatural;
class VectorString;


class DAGNode {

    friend class                        VariableSlot;                                                           //!< Give VariableSlot exclusive access to slot mgmt functions

    public:
        virtual                        ~DAGNode(void);                                                          //!< Virtual destructor

        // Basic utility functions you should not override
        std::string                     briefInfo(void) const;                                                  //!< Brief info about object
        ContainerNode*                  getContainer(void) const;                                               //!< Get container if container element
        const std::string&              getDAGType(void) const;                                                 //!< Get DAG node type
        const std::string               getName(void) const;                                                    //!< Get name from slot and children
        const TypeSpec                  getTypeSpec(void) const;                                                //!< Get language type specification for value
        std::set<VariableSlot*>&        getReferringSlots(void) { return referringSlots; }                      //!< Return referring slots
		VariableSlot*                   getSlot(void) const { return slot; }                                    //!< Return slot managing the variable
        const std::string&              getValueType(void) const { return valueType; }                          //!< Get value type
        bool                            isDAGType(const std::string& type) const;                               //!< Is DAG node of type?
        bool                            isImmutable(void) const;                                                //!< Is DAG node immutable?
        bool                            isPermanent(Frame* frame) const;                                        //!< Is the node a permanent variable in frame?
        bool                            isTypeSpec(const TypeSpec& typeSp) const;                               //!< Is DAG node of language type typeSpec?
        int                             numReferringSlots(void) const { return int(referringSlots.size()); }    //!< Number of referring slots
        int                             numRefs(void) const;                                                    //!< Number of references

        // Basic utility functions you may want to override
        virtual DAGNode*                getReference(void) { return this; }                                     //!< Get reference to variable, override if lookup or fxn
        virtual int                     getSize(void) const { return 1; }                                       //!< Total number of elements (default is 1, only different for ContainerNode)
        virtual int                     getDim(void) const { return 0; }                                        //!< Get dim (0 for scalar, 1 for vector, etc)
        virtual bool                    isConst(void) const;                                                    //!< Is DAG node const value?

        // Element set and get functions you may want to override
        virtual bool                    existsElement(VectorInteger& index);                                    //!< Does element exist?
        virtual DAGNode*                getElement(VectorInteger& index);                                       //!< Give the parser an element
        virtual DAGNode*                getElementOwner(VectorInteger& index);                                  //!< Give the parser the element owner
        virtual void                    setElement(const VectorNatural& index, DAGNode* var, bool convert=true);//!< Set element

        // Basic utility functions you have to override
        virtual DAGNode*                clone(void) const = 0;                                                  //!< Clone this node
        virtual const VectorString&     getDAGClass(void) const;                                                //!< Get DAG node class vector
        virtual const RbObject*         getStoredValue(void) = 0;                                               //!< Get stored value
//        virtual const RbObject*         getValue(void) = 0;                                                     //!< Get value
        virtual const RbObject*         getValue(void) const = 0;                                               //!< Get const value; throw an error or warn if in transient state @Fredrik: Why is there a constant function if there is also a non-constant function?! (Sebastian)
        virtual void                    printValue(std::ostream& o) const = 0;                                  //!< Print value for user
        virtual void                    printStruct(std::ostream& o) const = 0;                                 //!< Print struct for user
        virtual std::string             richInfo(void) const = 0;                                               //!< Complete info about object

        // DAG functions you should not have to override
        void                            addChildNode(VariableNode* c) { children.insert(c); }                   //!< Add child node
        std::set<VariableNode*>&        getChildren(void) { return children; }                                  //!< Return children
		std::set<DAGNode*>&             getParents(void) { return parents; }                                    //!< Return parents
        bool                            isParentInDAG(const DAGNode* x, std::list<DAGNode*>& done) const;       //!< Is node x a parent of the caller in the DAG?
        int                             numChildren(void) const { return int(children.size()); }                //!< Number of children
        int                             numParents(void) const { return int(parents.size()); }                  //!< Number of parents
        void                            printChildren(std::ostream& o) const;                                   //!< Print children DAG nodes
        void                            printParents(std::ostream& o) const;                                    //!< Print children DAG nodes
        void                            removeChildNode(VariableNode* c) { children.erase(c); }                 //!< Remove a child node

        // DAG function you may want to override

        // DAG function you have to override
        virtual DAGNode*                cloneDAG(std::map<const DAGNode*, DAGNode*>& newNodes) const = 0;       //!< Clone graph

    protected:
                                        DAGNode(RbObject* value);                                               //!< Constructor of filled node
                                        DAGNode(const std::string& valType);                                    //!< Constructor of empty node
                                        DAGNode(const DAGNode& x);                                              //!< Copy constructor

        // Slot reference management and mutation functions for VariableSlot friend class
        void                            addReferringSlot(VariableSlot* s) { referringSlots.insert(s); }         //!< Add referring slot (not managing this node)
        virtual bool                    isMutableTo(DAGNode* newNode) const;                                    //!< Is node mutable to newNode?
        virtual void                    mutateTo(DAGNode* newNode);                                             //!< Mutate to new node
        void                            removeSlot(const VariableSlot* s);                                      //!< Remove a managing or referring slot
		void                            setSlot(VariableSlot* s) { slot = s; }                                  //!< Set slot managing the variable

        // Member variables keeping track of references
        std::set<VariableNode*>         children;                                                               //!< Set of children nodes
        std::set<DAGNode*>              parents;                                                                //!< Set of parent nodes
        VariableSlot*                   slot;                                                                   //!< Slot owning the node
        std::set<VariableSlot*>         referringSlots;                                                         //!< Set of slots referring to the node

        // Member value variables
        const std::string&              valueType;                                                              //!< Type of value
        RbObject*                       value;                                                                  //!< Value
};

#endif
