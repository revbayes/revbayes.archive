/**
 * @file
 * This file contains the declaration of ContainerNode, which
 * is used to hold value or variable containers. In principle,
 * a value container could be kept in a ConstantNode, but the
 * user might through the parser change a value container to
 * a variable container at any time, so it makes sense to always
 * store a container in a ContainerNode. When the parser tries
 * to set an element of the container to a variable node, then
 * the ContainerNode object sees this and can convert the value
 * container to a variable container. If the ContainerNode contains
 * just a value container, then its parent node set is empty.
 *
 * @brief Declaration of ContainerNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-05, version 1.0
 *
 * $Id$
 */

#ifndef ContainerNode_H
#define ContainerNode_H

#include "DeterministicNode.h"
#include "VectorInteger.h"

#include <ostream>
#include <vector>

class VectorString;


class ContainerNode : public DeterministicNode {

    public:
                                ContainerNode(const std::string& valType, size_t dim);                      //!< Constructor of empty container node
                                ContainerNode(Container* val);                                              //!< Constructor from container object
                                ContainerNode(const ContainerNode& x);                                      //!< Copy constructor
        virtual                ~ContainerNode(void);                                                        //!< Virtual destructor

        // Assignment operator
        ContainerNode&          operator=(const ContainerNode& x);                                          //!< Assignment operator

        // Basic utility functions
        ContainerNode*          clone(void) const;                                                          //!< Clone the member node
        const VectorString&     getDAGClass(void) const;                                                    //!< Get DAG node class vector
        size_t                  getDim() const { return valueDim; }                                         //!< Get dimensions of value
        bool                    isConst(void) const;                                                        //!< Is this node a const value?
        void                    printStruct(std::ostream& o) const;                                         //!< Print struct for user
        std::string             richInfo(void) const;                                                       //!< Complete info about object

        // Parser element access, test and convert functions
        RbObject*               convertValElement(const VectorInteger& index, RbObject* val ) const;                    //!< Convert value element
        DAGNode*                convertVarElement(const VectorInteger& index, DAGNode* var, bool convert=true ) const;  //!< Convert value element
        bool                    existsElement(VectorInteger& index) const;                                              //!< Does element exist?
        DAGNode*                getElement(VectorInteger& index);                                                       //!< Get element
        DAGNode*                getElementOwner(VectorInteger& index);                                                  //!< Get element owner
        bool                    isValidElement(const VectorInteger& index, DAGNode* var, bool convert) const;           //!< Check validity of variable element
        bool                    isValidElement(const VectorInteger& index, const RbObject* val) const;                  //!< Check validity of value element
        void                    setElement(const VectorInteger& index, DAGNode* var, bool convert=true);                //!< Set element

        // DAG functions
        ContainerNode*          cloneDAG(std::map<const DAGNode*, DAGNode*>& newNodes) const;                           //!< Clone entire graph
        bool                    isMutableTo(const std::string& valType, size_t dim) const;                              //!< Is node mutable to valType and dim?
        bool                    isParentMutableTo(const DAGNode* oldNode, const DAGNode* newNode) const;                //!< Is parent mutable to newNode?
        ContainerNode*          mutateTo(const std::string& valType, size_t dim) const;                                 //!< Mutate to valType and dim
        void                    swapParentNode(DAGNode* oldNode, DAGNode* newNode);                                     //!< Swap a parent node

        // Container functions
        DAGNode*                getElement(const size_t i);                                                             //!< Convenient vector access
        VectorNatural           getIndex(const DAGNode* element) const;                                                 //!< Get index of element
        size_t                  size(void) const;                                                                       //!< Return container size

    private:
        // Utility function
        void                    update(void);                                                                           //!< Update value and storedValue

        // Member variables
        VariableContainer*      container;                                                                              //!< The variable container object (NULL if constant)
        size_t                  valueDim;                                                                               //!< Dimensions of value
};

#endif
