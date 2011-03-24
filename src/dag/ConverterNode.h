/**
 * @file
 * This file contains the declaration of ConverterNode, which is derived
 * from DeterministicNode. ConverterNode is used for converting from one
 * value type to another in a DAG.
 *
 * @brief Declaration of ConverterNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2010-01-12 21:51:54 +0100 (Tis, 12 Jan 2010) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id: DeterministicNode.h 221 2010-01-12 20:51:54Z Hoehna $
 */

#ifndef ConverterNode_H
#define ConverterNode_H

#include "DeterministicNode.h"

#include <string>
#include <vector>

/**
 * @brief ConverterNode
 *
 * ConverterNode is a light-weight DAG node used to convert from one type to another. Unlike
 * a FunctionNode, it is created internally by the parser and it does not use an RbFunction
 * call to calculate its value.
 */
 class ConverterNode : public DeterministicNode {

    public:
                                ConverterNode(const TypeSpec& typeSpec);                                    //!< Constructor of pristine node
                                ConverterNode(DAGNode* origNode, const TypeSpec& typeSpec);                 //!< Basic constructor

        // Basic utility functions
        ConverterNode*          clone(void) const;                                                          //!< Clone the converter node
        const VectorString&     getDAGClass(void) const;                                                    //!< Get DAG node class vector
        int                     getDim(void) const { return valueDim; }                                     //!< Get dim of converted value (0 for scalar, 1 for vector, etc)
        void                    printStruct(std::ostream& o) const;                                         //!< Print struct for user
        std::string             richInfo(void) const;                                                       //!< Complete info about object

        // DAG functions
        ConverterNode*          cloneDAG(std::map<DAGNode*, DAGNode*>& newNodes) const;                     //!< Clone entire graph
        bool                    isMutableTo(const DAGNode* newNode) const;                                  //!< Is node mutable to newNode?
        bool                    isMutableTo(const TypeSpec& typeSpec) const;                                //!< Is node mutable to language type typeSpec?
        bool                    isParentMutableTo(const DAGNode* oldNode, const DAGNode* newNode) const;    //!< Is parent mutable to newNode?
        void                    mutateTo(DAGNode* newNode);                                                 //!< Mutate to new node
        ConverterNode*          mutateTo(const TypeSpec& typeSpec);                                         //!< Mutate to language type typeSpec
        void                    swapParentNode(DAGNode* oldP, DAGNode* newP);                               //!< Swap a parent node

    protected:
        // Utility function
        void                    update(void);                                                               //!< Update value and storedValue

        // Member variables
        int                     valueDim;                                                                   //!< Dimensions of converted value
};

#endif

