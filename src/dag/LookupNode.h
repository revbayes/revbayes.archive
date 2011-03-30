/**
 * @file
 * This file contains the declaration of LookupNode, which is used to
 * hold and perform lookups of variables.
 *
 * @brief Declaration of LookupNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2010-01-12 21:51:54 +0100 (Tis, 12 Jan 2010) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since 2009-09-28
 *
 * $Id: DeterministicNode.h 221 2010-01-12 20:51:54Z Hoehna $
 */

#ifndef LookupNode_H
#define LookupNode_H

#include "Argument.h"
#include "DeterministicNode.h"

#include <string>
#include <vector>

typedef std::vector<DAGNode*> IndexArgs;

class MemberNode;
class RbString;
class VectorString;


class LookupNode : public DeterministicNode {

    public: 
                            LookupNode(const std::string& valType);                                     //!< Constructor of empty node
                            LookupNode( DAGNode*    var,
                                        IndexArgs&  indxArgs);                                          //!< Lookup of regular variable
                            LookupNode( MemberNode*    baseVar,
                                        RbString*   membrName,
                                        IndexArgs&  indxArgs);                                          //!< Lookup of member variable
                            LookupNode( LookupNode*    baseVar,
                                        RbString*   membrName,
                                        IndexArgs&  indxArgs);                                          //!< Indirect lookup of member variable
                            LookupNode(const LookupNode& x);                                            //!< Copy constructor
        virtual            ~LookupNode(void);                                                           //!< Virtual destructor

        // Assignment operator
        LookupNode&         operator=(const LookupNode& x);                                             //!< Assignment operator

        // Basic utility functions
        LookupNode*         clone(void) const;                                                          //!< Clone the lookup node
        const VectorString& getDAGClass(void) const;                                                    //!< Get DAG node class vector
        int                 getDim(void) const { return valueDim; }                                     //!< Get dim of lookup target value (0 for scalar, 1 for vector, etc)
        DAGNode*            getReference(void) { return lookup(); }                                     //!< Look up the variable and get a reference to it
        void                printStruct(std::ostream& o) const;                                         //!< Print struct for user
        std::string         richInfo(void) const;                                                       //!< Complete info about object

        // DAG functions
        LookupNode*         cloneDAG(std::map<DAGNode*, DAGNode*>& newNodes) const;                     //!< Clone entire graph
        bool                isMutableTo(const DAGNode* newNode) const;                                  //!< Is node mutable to newNode?
        bool                isParentMutableTo(const DAGNode* oldNode, const DAGNode* newNode) const;    //!< Is parent mutable to newNode?
        void                mutateTo(DAGNode* newNode);                                                 //!< Mutate to new node
        void                swapParentNode(DAGNode* oldP, DAGNode* newP);                               //!< Swap a parent node

        // Parser element access functions
        bool                existsElement(VectorInteger& index);                                        //!< Does element exist?
        DAGNode*            getElement(VectorInteger& index);                                           //!< Get element
        DAGNode*            getElementRef(VectorNatural& index);                                        //!< Get element reference for setting it
        void                setElement(VectorNatural& index, DAGNode* var);                             //!< Set element

        // LookupNode functions
        const TypeSpec&     getMemberTypeSpec(const RbString& name) const;                              //!< Get type spec of a named member variable of lookup target

    protected:
        // Utility functions
        DAGNode*            lookup(void);                                                               //!< Look up the variable
        void                update(void);                                                               //!< Update value and storedValue

        // Member variables
        DAGNode*            baseVariable;                                                               //!< Ptr to base variable
        LookupNode*         baseLookup;                                                                 //!< Ptr to lookup of base variable (member node)
        RbString*           memberName;                                                                 //!< Name if member variable
        IndexArgs           indexArgs;                                                                  //!< Vector of index arguments (vector<DAGNode*>)
        int                 valueDim;                                                                   //!< Dimensions of lookup target value
};

#endif

