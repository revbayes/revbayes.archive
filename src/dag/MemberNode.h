/**
 * @file
 * This file contains the declaration of MemberNode, which is used to
 * hold RbComplex values, i.e., data types with member variables, in
 * model DAGs.
 *
 * @brief Declaration of MemberNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2010-01-12 21:51:54 +0100 (Tis, 12 Jan 2010) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since 2010-09-28
 *
 * $Id: MemberNode.h 221 2010-01-12 20:51:54Z Hoehna $
 */

#ifndef MemberNode_H
#define MemberNode_H

#include "DeterministicNode.h"

class MemberObject;
class RbString;
class TypeSpec;
class VectorString;


class MemberNode : public DeterministicNode {

    public:
                                MemberNode(const std::string& valType);                                     //!< Constructor of empty member node
                                MemberNode(MemberObject* val);                                              //!< Constructor from member object
        virtual                ~MemberNode(void);                                                           //!< Destructor

        // Basic utility functions
        MemberNode*             clone(void) const;                                                          //!< Clone the member node
        const VectorString&     getDAGClass(void) const;                                                    //!< Get DAG node class vector
        void                    printStruct(std::ostream& o) const;                                         //!< Print struct for user
        std::string             richInfo(void) const;                                                       //!< Complete info about object

        // Parser element access functions
        bool                    existsElement(VectorInteger& index);                                        //!< Does element exist?
        DAGNode*                getElement(VectorInteger& index);                                           //!< Get element
        DAGNode*                getElementRef(VectorNatural& index);                                        //!< Get element reference for setting it
        void                    setElement(VectorNatural& index, DAGNode* var);                             //!< Set element for parser if member object requests such a call

        // DAG functions
        MemberNode*             cloneDAG(std::map<DAGNode*, DAGNode*>& newNodes) const;                     //!< Clone entire graph
        bool                    isMutableTo(const DAGNode* newNode) const;                                  //!< Is node mutable to newNode?
        bool                    isParentMutableTo(const DAGNode* oldNode, const DAGNode* newNode) const;    //!< Is parent mutable to newNode?
        void                    mutateTo(DAGNode* newNode);                                                 //!< Mutate to new node
        void                    swapParentNode(DAGNode* oldP, DAGNode* newP);                               //!< Swap a parent node

        // MemberNode functions
        const TypeSpec&         getMemberTypeSpec(const RbString& name) const;                              //!< Get type spec of a named member variable
        MemberObject*           getMemberObject(void) { return memberObject; }                              //!< Get member object 

    protected:
        // Utility function
        void                    update(void);                                                               //!< Update value and storedValue

        // Member variable
        MemberObject*           memberObject;                                                               //!< The member object
};

#endif
