/**
 * @file
 * This file contains the declaration of FunctionNode, which is derived
 * from DeterministicNode. FunctionNode is used for a DAG node associated with
 * an explicit function call that determines its value.
 *
 * @brief Declaration of FunctionNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2010-01-12 21:51:54 +0100 (Tis, 12 Jan 2010) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id: FunctionNode.h 221 2010-01-12 20:51:54Z Hoehna $
 */

#ifndef FunctionNode_H
#define FunctionNode_H

#include "DeterministicNode.h"

#include <string>
#include <vector>

class RbFunction;

class FunctionNode : public DeterministicNode {

    public:
                                    FunctionNode(const TypeSpec& valType);                                      //!< Constructor of empty node
                                    FunctionNode(RbFunction* func);                                             //!< Constructor from function
                                    FunctionNode(const FunctionNode& d);                                        //!< Copy constructor
        virtual                    ~FunctionNode(void);                                                         //!< Virtual destructor

        // Assignment operator
        FunctionNode&               operator=(const FunctionNode& x);                                           //!< Assignment operator

        // Basic utility functions
        virtual FunctionNode*       clone(void) const;                                                          //!< Clone the function node
        virtual const VectorString& getDAGClass(void) const;                                                    //!< Get DAG node class vector
        int                         getDim(void) const { return valueDim; }                                     //!< Get dim of return value
        const RbFunction*           getFunction(void) const { return function; }                                //!< Get function
        virtual void                printStruct(std::ostream& o) const;                                         //!< Print struct for user
        virtual std::string         toString(void) const;                                                       //!< Complete info about object

        // DAG functions
        FunctionNode*               cloneDAG(std::map<DAGNode*, DAGNode*>& newNodes) const;                     //!< Clone entire graph
        bool                        isMutableTo(const DAGNode* newNode) const;                                  //!< Is node mutable to newNode?
        bool                        isMutableTo(const VectorInteger& index, const RbObject* newValue) const;    //!< Is node mutable to contain newValue?
        bool                        isParentMutableTo(const DAGNode* oldNode, const DAGNode* newNode) const;    //!< Is parent mutable to newNode?
        void                        mutateTo(DAGNode* newNode);                                                 //!< Mutate to new node
        FunctionNode*               mutateTo(const VectorInteger& index, RbObject* newValue);                   //!< Mutate to contain newValue
        void                        swapParentNode(DAGNode* oldP, DAGNode* newP);                               //!< Swap a parent node

    protected:
        // Utility function
        virtual void                update(void);                                                               //!< Update value and storedValue

        // Member variables
        RbFunction*                 function;                                                                   //!< True when value updated after touch
        int                         valueDim;                                                                   //!< Dimensions of function return value (type in valueType)
};

#endif

