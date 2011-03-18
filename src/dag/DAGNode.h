/**
 * @file
 * This file contains the declaration of DAGNode, which is the base
 * class for nodes in a model DAG.
 *
 * @brief Declaration of DAGNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2010-01-12 21:51:54 +0100 (Tis, 12 Jan 2010) $
 * @author The REvBayes development core team
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

class RbObject;
class VariableNode;
class VariableSlot;
class VectorInteger;
class VectorString;

class DAGNode {

    public:
        virtual                        ~DAGNode(void) {}                                                    //!< Destructor

        // Functions you have to override
        virtual DAGNode*                clone(void) const = 0;                                              //!< Clone this node
        virtual const VectorString&     getClass(void) const;                                               //!< Get class vector
        virtual const RbObject*         getValElement(const VectorInteger& index) const = 0;                //!< Get value element
        virtual const RbObject*         getStoredValue(void) = 0;                                           //!< Get stored value
        virtual const RbObject*         getValue(void) = 0;                                                 //!< Get value
        virtual const RbObject*         getValue(void) const = 0;                                           //!< Get value (const)
        virtual void                    printValue(std::ostream& o) const = 0;                              //!< Print value for user
        virtual void                    printStruct(std::ostream& o) const = 0;                             //!< Print struct for user
        virtual void                    setElement(const VectorInteger& index, RbObject*val) = 0;           //!< Set value element
        virtual std::string             toString(void) const = 0;                                           //!< Complete info about object

        // Functions you may want to override
        virtual int                     getDim(void) const { return 0; }                                    //!< Get wrapper dim
        virtual const DAGNode*          getVarElement(const VectorInteger& index) const;                    //!< Get variable element
        virtual void                    setElement(const VectorInteger& index, DAGNode* var);               //!< Set variable element

        // Functions you should not override
        const std::string               getName(void) const;                                                //!< Get name from slot and children
        const std::string&              getType(void) const;                                                //!< Get wrapper type
        const TypeSpec&                 getValueType(void) const { return valueType; }                      //!< Get value type
        bool                            isType(const std::string& type) const;                              //!< Is wrapper of type?

        // DAG functions you should not override
        void                            addChildNode(VariableNode* c) { children.insert(c); }                   //!< Add child node
        void                            addReferringSlot(VariableSlot* s) { referringSlots.insert(s); }         //!< Add referring slot
        std::set<VariableNode*>&        getChildren(void) { return children; }                                  //!< Return children
		VariableSlot*                   getSlot(void) { return slot; }                                          //!< Return slot managing the variable
		std::set<DAGNode*>&             getParents(void) { return parents; }                                    //!< Return parents
		DAGNode*                        getReference(void) { return this; }                                     //!< Get reference (this node by default)
        std::set<VariableSlot*>&        getReferringSlots(void) { return referringSlots; }                      //!< Return referring slots
        bool                            isParentInDAG(const DAGNode* x, std::list<DAGNode*>& done) const;       //!< Is node x a parent of the caller in the DAG?
        int                             numChildren(void) const { return int(children.size()); }                //!< Number of children
        int                             numParents(void) const { return int(parents.size()); }                  //!< Number of parents
        int                             numReferringSlots(void) const { return int(referringSlots.size()); }    //!< Number of referring slots
        int                             numRefs(void) const;                                                    //!< Number of references
        void                            printChildren(std::ostream& o) const;                                   //!< Print children DAG nodes
        void                            printParents(std::ostream& o) const;                                    //!< Print children DAG nodes
        void                            removeChildNode(VariableNode* c) { children.erase(c); }                 //!< Remove a child node
        void                            removeReferringSlot(VariableSlot* s) { referringSlots.erase(s); }       //!< Remove a referring slot
		void                            setSlot(VariableSlot* s) { slot = s; }                                  //!< Set slot managing the variable
        void                            swapNodeTo(DAGNode* newNode);                                           //!< Swap node in DAG

        // DAG functions you have to override
        virtual DAGNode*                cloneDAG(std::map<DAGNode*, DAGNode*>& newNodes) const = 0;             //!< Clone graph
        virtual bool                    isMutableTo(const DAGNode* newNode) const = 0;                          //!< Is node mutable to newNode?
        virtual bool                    isMutableTo(const VectorInteger& index, const RbObject* newValue) const = 0;    //!< Is node mutable to contain newValue?
        virtual void                    mutateTo(DAGNode* newNode) = 0;                                         //!< Mutate to new node
        virtual DAGNode*                mutateTo(const VectorInteger& index, RbObject* newValue) = 0;           //!< Mutate to contain newValue
        virtual void                    touchAffected(void) = 0;                                                //!< Tell affected nodes value is reset

    protected:
                                        DAGNode(const TypeSpec& valType);                                       //!< Constructor
                                        DAGNode(const DAGNode& x);                                              //!< Copy constructor

        std::set<VariableNode*>         children;                                                               //!< Set of children nodes
        std::set<DAGNode*>              parents;                                                                //!< Set of parent nodes
        VariableSlot*                   slot;                                                                   //!< Slot owning the node
        std::set<VariableSlot*>         referringSlots;                                                         //!< Set of slots referring to the node
        TypeSpec                        valueType;                                                              //!< Type of value
};

#endif
