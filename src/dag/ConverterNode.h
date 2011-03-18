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
 * @author The RevBayes development core team
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
                                ConverterNode(const TypeSpec& toType);                                      //!< Constructor of pristine node
                                ConverterNode(DAGNode* origNode, const TypeSpec& toType);                   //!< Basic constructor

        // Basic utility functions
        ConverterNode*          clone(void) const;                                                          //!< Clone the converter node
        const VectorString&     getClass(void) const;                                                       //!< Get class vector
        void                    printStruct(std::ostream& o) const;                                         //!< Print struct for user
        std::string             toString(void) const;                                                       //!< Complete info about object

        // DAG functions
        ConverterNode*          cloneDAG(std::map<DAGNode*, DAGNode*>& newNodes) const;                     //!< Clone entire graph
        virtual bool            isMutableTo(const DAGNode* newNode) const;                                  //!< Is node mutable to newNode?
        virtual bool            isMutableTo(const VectorInteger& index, const RbObject* newValue) const;    //!< Is node mutable to contain newValue?
        virtual bool            isParentMutableTo(const DAGNode* oldNode, const DAGNode* newNode) const;    //!< Is parent mutable to newNode?
        virtual void            mutateTo(DAGNode* newNode);                                                 //!< Mutate to new node
        ConverterNode*          mutateTo(const VectorInteger& index, RbObject* newValue);                   //!< Mutate to contain newValue
        void                    swapParentNode(DAGNode* oldP, DAGNode* newP);                               //!< Swap a parent node

    protected:
        // Utility function
        void                    update(void);                                                               //!< Update value and storedValue
};

#endif

