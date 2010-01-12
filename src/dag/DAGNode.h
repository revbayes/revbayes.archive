/**
 * @file
 * This file contains the declaration of DAGNode, which is the base
 * class for nodes in a model DAG.
 *
 * @brief Declaration of DAGNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The REvBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @interface DAGNode
 *
 * $Id$
 */

#ifndef DAGNode_H
#define DAGNode_H


#include <list>
#include <map>
#include <set>
#include <string>

class IntVector;
class RbObject;
class StringVector;
class VariableNode;

class DAGNode {

    public:
        virtual                         ~DAGNode(void);                                         //!< Destructor

        // Functions you have to override
        virtual DAGNode*                clone(void) const = 0;                                  //!< Clone this node
        virtual const StringVector&     getClass(void) const;                                   //!< Get class vector
        virtual const RbObject*         getStoredValue(void) = 0;                               //!< Get stored value
        virtual const RbObject*         getValElement(const IntVector& index) const = 0;        //!< Get value element
        virtual const RbObject*         getValue(void) = 0;                                     //!< Get value
        virtual const RbObject*         getValue(void) const = 0;                               //!< Get value (const)
        virtual bool                    isTouched(void) const = 0;                              //!< Is touched by a move?
        virtual void                    printValue(std::ostream& o) const = 0;                  //!< Print value for user
        virtual void                    printStruct(std::ostream& o) const = 0;                 //!< Print struct for user
        virtual void                    setElement(const IntVector& index, RbObject*val) = 0;   //!< Set value element
        virtual std::string             toString(void) const = 0;                               //!< Complete info about object

        // Functions you may want to override
        virtual int                     getDim(void) const { return 0; }                        //!< Get wrapper dim
        virtual const DAGNode*          getVarElement(const IntVector& index) const;            //!< Get variable element
        virtual void                    setElement(const IntVector& index, DAGNode* var);       //!< Set variable element

        // Functions you should not override
        const std::string&              getName(void) const { return name; }                    //!< Get name
        const std::string&              getType(void) const;                                    //!< Get wrapper type
        const std::string&              getValueType(void) const { return valueType; }          //!< Get value type
        bool                            isType(const std::string& type) const;                  //!< Is wrapper of type?
        void                            setName(const std::string& id) { name=id; }             //!< Set name

        // overloaded operators
        RbObject&                       operator=(const RbObject& o);
        DAGNode&                        operator=(const DAGNode& o);

        // DAG functions
        void                            addChildNode(VariableNode* c) { children.insert(c); }   //!< Add child node
        DAGNode*                        cloneDAG(std::map<DAGNode*, DAGNode*>& newNodes) const; //!< Clone graph
        std::set<VariableNode*>&        getChildrenNodes(void) { return children; }             //!< Get children nodes
        std::set<DAGNode*>&             getParentNodes(void) { return parents; }                //!< Get children nodes
        virtual bool                    hasAttachedMove(void) const { return false; }           //!< Checks if the node has a move attached
        bool                            isParentInDAG(const DAGNode* x, std::list<DAGNode*>& done) const;  //!< Is node x a parent of the caller in the DAG?
        int                             numChildren(void) const { return int(children.size()); }//!< Number of children
        int                             numParents(void) const { return int(parents.size()); }  //!< Number of parents
        int                             numRefs(void) const;                                    //!< Number of references
        void                            printChildren(std::ostream& o) const;                   //!< Print children DAG nodes
        void                            printParents(std::ostream& o) const;                    //!< Print children DAG nodes
        void                            removeChildNode(VariableNode* c) { children.erase(c); } //!< Remove a child node

    protected:
                                        DAGNode(const std::string& valType);                    //!< Constructor
 
        std::set<VariableNode*>         children;                                               //!< Set of children nodes
        std::set<DAGNode*>              parents;                                                //!< Set of parent nodes
        std::string                     name;                                                   //!< Name, "" if not named
        const std::string&              valueType;                                              //!< Type of value

};

#endif


