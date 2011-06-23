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
                                ConverterNode(const std::string& type, size_t dim);                         //!< Constructor of pristine node
                                ConverterNode(DAGNode* origNode, const std::string& type, size_t dim);      //!< Basic constructor
        virtual                ~ConverterNode(void);                                                        //!< Virtual destructor

        // Basic utility functions
        ConverterNode*          clone(void) const;                                                          //!< Clone the converter node
        const VectorString&     getDAGClass(void) const;                                                    //!< Get DAG node class vector
        size_t                  getDim(void) const { return valueDim; }                                     //!< Get dim of converted value (0 for scalar, 1 for vector, etc)
        void                    printStruct(std::ostream& o) const;                                         //!< Print struct for user
        std::string             richInfo(void) const;                                                       //!< Complete info about object

        // DAG functions
        ConverterNode*          cloneDAG(std::map<const DAGNode*, DAGNode*>& newNodes) const;               //!< Clone entire graph
        bool                    isParentMutableTo(const DAGNode* oldNode, const DAGNode* newNode) const;    //!< Is parent mutable to newNode?

    protected:
        // Utility function
        void                    update(void);                                                               //!< Update value and storedValue

        // Member variables
        size_t                  valueDim;                                                                   //!< Dimensions of converted value
};

#endif

