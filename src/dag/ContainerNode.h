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
                                ContainerNode(const std::string& valType, int dim);                         //!< Constructor of empty container node
                                ContainerNode(Container* val);                                              //!< Constructor from container object
                                ContainerNode(const ContainerNode& x);                                      //!< Copy constructor
        virtual                ~ContainerNode(void);                                                        //!< Virtual destructor

        // Assignment operator
        ContainerNode&          operator=(const ContainerNode& x);                                          //!< Assignment operator

        // Basic utility functions
        ContainerNode*          clone(void) const;                                                          //!< Clone the member node
        const VectorString&     getDAGClass(void) const;                                                    //!< Get DAG node class vector
        int                     getDim() const { return valueDim; }                                         //!< Get dimensions of value
        void                    printStruct(std::ostream& o) const;                                         //!< Print struct for user
        std::string             richInfo(void) const;                                                       //!< Complete info about object

        // DAG functions
        ContainerNode*          cloneDAG(std::map<DAGNode*, DAGNode*>& newNodes) const;                     //!< Clone entire graph
        bool                    isMutableTo(const DAGNode* newNode) const;                                  //!< Is node mutable to newNode?
        bool                    isMutableTo(const TypeSpec& typeSpec) const;                                //!< Is node mutable to language type typeSpec?
        bool                    isParentMutableTo(const DAGNode* oldNode, const DAGNode* newNode) const;    //!< Is parent mutable to newNode?
        void                    mutateTo(DAGNode* newNode);                                                 //!< Mutate to new node
        ContainerNode*          mutateTo(const TypeSpec& typeSpec);                                         //!< Mutate to language type typeSpec
        void                    swapParentNode(DAGNode* oldP, DAGNode* newP);                               //!< Swap a parent node

        // Container functions
        int                     getIndex(const DAGNode* element) const;                                     //!< Get index of element

    private:
        // Utility function
        void                    update(void);                                                               //!< Update value and storedValue

        // Member variables
        VariableContainer*      container;                                                                  //!< The variable container object (NULL if constant)
        int                     valueDim;                                                                   //!< Dimensions of value
};

#endif
