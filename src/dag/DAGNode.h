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


#include <list>
#include <map>
#include <set>
#include <string>

class IntVector;
class RbObject;
class VectorString;
class VariableNode;

class DAGNode {

    public:
        virtual                        ~DAGNode(void) {}                                                      //!< Destructor

        // Functions you have to override
        virtual DAGNode*                clone(void) const = 0;                                                //!< Clone this node
        virtual const VectorString&     getClass(void) const;                                                 //!< Get class vector
        virtual const RbObject*         getValElement(const IntVector& index) const = 0;                      //!< Get value element
        virtual const RbObject*         getStoredValue(void) = 0;                                             //!< Get stored value
        virtual const RbObject*         getValue(void) = 0;                                                   //!< Get value
        virtual const RbObject*         getValue(void) const = 0;                                             //!< Get value (const)
        virtual void                    printValue(std::ostream& o) const = 0;                                //!< Print value for user
        virtual void                    printStruct(std::ostream& o) const = 0;                               //!< Print struct for user
        virtual void                    setElement(const IntVector& index, RbObject*val) = 0;                 //!< Set value element
        virtual std::string             toString(void) const = 0;                                             //!< Complete info about object

        // Functions you may want to override
        virtual int                     getDim(void) const { return 0; }                                      //!< Get wrapper dim
        virtual const DAGNode*          getVarElement(const IntVector& index) const;                          //!< Get variable element
        virtual void                    setElement(const IntVector& index, DAGNode* var);                     //!< Set variable element
        virtual void                    setName(const std::string& id) { name=id; }                           //!< Set name

        // Functions you should not override
		const bool                      getIsDagExposed(void) { return isDagExposed; }                        //!< Get DAG exposure
        const std::string&              getName(void) const { return name; }                                  //!< Get name
        const std::string&              getType(void) const;                                                  //!< Get wrapper type
        const std::string&              getTypeOfValue(void) const { return valueType; }                      //!< Get value type
        const std::string&              getValueType(void) const { return valueType; }                        //!< Get value type
        bool                            isType(const std::string& type) const;                                //!< Is wrapper of type?

        // DAG functions
        void                            addChildNode(VariableNode* c) { children.insert(c); }                 //!< Add child node
        virtual DAGNode*                cloneDAG(std::map<DAGNode*, DAGNode*>& newNodes) const = 0;           //!< Clone graph
        virtual void                    convertValueTo(const std::string& type);                              //!< Convert the the value to the specific type. Should only be allowed for constant nodes.
        std::set<VariableNode*>&        getChildren(void) { return children; }                                //!< Return children
		std::set<DAGNode*>&             getParents(void) { return parents; }                                  //!< Return parents
        bool                            isParentInDAG(const DAGNode* x, std::list<DAGNode*>& done) const;     //!< Is node x a parent of the caller in the DAG?
        virtual bool                    isValueConvertibleTo(const std::string& type) const { return false; } //!< Can we convert the the value to the specific type? Should only be allowed for constant nodes.
        int                             numChildren(void) const { return int(children.size()); }              //!< Number of children
        int                             numParents(void) const { return int(parents.size()); }                //!< Number of parents
        int                             numRefs(void) const;                                                  //!< Number of references
        void                            printChildren(std::ostream& o) const;                                 //!< Print children DAG nodes
        void                            printParents(std::ostream& o) const;                                  //!< Print children DAG nodes
        void                            removeChildNode(VariableNode* c) { children.erase(c); }               //!< Remove a child node
        virtual void                    touchAffected(void) = 0;                                              //!< Tell affected nodes value is reset

    protected:
                                        DAGNode(const std::string& valType);                                  //!< Constructor
                                        DAGNode(const DAGNode& x);                                            //!< Copy constructor

        std::set<VariableNode*>         children;                                                             //!< Set of children nodes
        std::set<DAGNode*>              parents;                                                              //!< Set of parent nodes
        std::string                     name;                                                                 //!< Name, "" if not named
        std::string                     valueType;                                                            //!< Type of value
		bool                            isDagExposed;                                                         //!< Flag indicating whether DAG should be exposed to user

};

#endif

