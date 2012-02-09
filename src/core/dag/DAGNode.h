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

#include "RbLanguageObject.h"
#include "RbPtr.h"
#include "TypeSpec.h"
#include "Variable.h"


#include <list>
#include <map>
#include <set>
#include <string>

class StochasticNode;
class Variable;
class VariableNode;
class VectorNatural;
class VectorString;


const std::string DAGNode_name = "DAG Node";

class DAGNode : public RbLanguageObject {

    public:
        virtual                                            ~DAGNode(void);                                                          //!< Virtual destructor

        // Basic utility functions you should not override
        const std::string&                                  getValueType(void) const { return valueTypeSpec.getType(); }            //!< Get value type
        const TypeSpec&                                     getValueTypeSpec(void) const { return valueTypeSpec; }                  //!< Get value type
        bool                                                isValueOfTypeSpec(const TypeSpec& typeSp) const;                        //!< Is DAG node of language type typeSpec?

        // Basic utility functions you may want to override
        virtual bool                                        isConst(void) const;                                                    //!< Is DAG node const value?

        // Basic utility functions you have to override
        virtual DAGNode*                                    clone(void) const = 0;                                                  //!< Clone this node
        virtual const VectorString&                         getClass(void) const;                                                   //!< Get DAG node class vector
        virtual const RbLanguageObject&                     getStoredValue(void) const = 0;                                         //!< Get stored value
        virtual const RbLanguageObject&                     getValue(void) const = 0;                                               //!< Get value (const)
        virtual RbLanguageObject&                           getValue(void) = 0;                                                     //!< Get value (non-const)
        virtual const RbLanguageObject*                     getValuePtr(void) const = 0;                                            //!< Get value pointer
        virtual void                                        printStruct(std::ostream& o) const = 0;                                 //!< Print struct for user
        virtual void                                        printValue(std::ostream& o) const = 0;                                  //!< Print value for user
    
        // DAG function you have to override
        virtual RbPtr<DAGNode>                              cloneDAG(std::map<const DAGNode*, RbPtr<DAGNode> >& newNodes) const = 0;//!< Clone graph

        // DAG functions you should not have to override
        void                                                addChildNode(VariableNode *c);                                          //!< Add child node
        void                                                getAffectedNodes(std::set<StochasticNode* >& affected);                 //!< Mark and get affected nodes
        const std::set<VariableNode*>&                      getChildren(void) const { return children; }                            //!< Return children
        const RbObject&                                     getElement(size_t index) const;                                         //!< Get element at index (container function)
        RbObject&                                           getElement(size_t index);                                               //!< Get element at index (container function)
        const std::string&                                  getName(void) const;                                                    //!< get the name
        const std::set<RbPtr<DAGNode> >&                    getParents(void) const;                                                 //!< Return parents
        const Variable&                                     getVariable(void) const;                                                //!< Get the variable owning this node
        bool                                                isParentInDAG(const RbPtr<DAGNode>& x, std::list<DAGNode*>& done) const;//!< Is node x a parent of the caller in the DAG?
        void                                                keep(void);                                                             //!< Keep current state of this node and all affected nodes
        size_t                                              numberOfChildren(void) const { return children.size(); }                //!< Number of children
        size_t                                              numberOfParents(void) const { return parents.size(); }                  //!< Number of parents
        void                                                printChildren(std::ostream& o) const;                                   //!< Print children DAG nodes
        void                                                printParents(std::ostream& o) const;                                    //!< Print children DAG nodes
        void                                                removeChildNode(VariableNode *c);                                       //!< Remove a child node
        void                                                restore(void);                                                          //!< Restore value of this and affected nodes
        void                                                setName(const std::string &n) { name = n; }                             //!< Replace the name of the variable
        void                                                setVariable(Variable* var);                                             //!< Set the variable owning this node
        void                                                touch(void);                                                            //!< Tell affected nodes value is reset


    protected:
                                                            DAGNode(RbLanguageObject* value);                                       //!< Constructor of filled node
                                                            DAGNode(const std::string& valType);                                    //!< Constructor of empty node
                                                            DAGNode(const DAGNode& x);                                              //!< Copy constructor

        virtual void                                        getAffected(std::set<StochasticNode* >& affected) = 0;                  //!< Mark and get affected nodes
        virtual void                                        keepAffected(void);                                                     //!< Keep value of affected nodes
        virtual void                                        keepMe(void) = 0;                                                       //!< Keep value of myself
        virtual void                                        restoreMe(void) = 0;                                                    //!< Restore value of this nodes
        virtual void                                        restoreAffected(void);                                                  //!< Restore value of affected nodes recursively
        virtual void                                        touchAffected(void);                                                    //!< Touch affected nodes (flag for recalculation)
        virtual void                                        touchMe(void) = 0;                                                      //!< Touch myself (flag for recalculation)
        

        // Member variables keeping track of references
        std::set<VariableNode* >                            children;                                                               //!< Set of children nodes
        std::set<RbPtr<DAGNode> >                           parents;                                                                //!< Set of parent nodes
        Variable*                                           variable;                                                               //!< The variable owning this dag node

        // Member value variables
        TypeSpec                                            valueTypeSpec;                                                          //!< the TypeSpec of the value
        RbLanguageObject*                                   value;                                                                  //!< Value
        std::string                                         name;                                                                   //!< The name/identifier of the DAG node
};

#endif
