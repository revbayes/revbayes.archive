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

class RbString;
class VectorString;


class LookupNode : public DeterministicNode {

    public: 
                            LookupNode(const TypeSpec& valType);                //!< Constructor of empty node
                            LookupNode( DAGNode*    var,
                                        IndexArgs&  indxArgs);                  //!< Lookup of regular variable
                            LookupNode( LookupNode* baseVar,
                                        RbString*   membrName,
                                        IndexArgs&  indxArgs);                  //!< Lookup of member variable
                            LookupNode(const LookupNode& x);                    //!< Copy constructor
        virtual            ~LookupNode(void);                                   //!< Virtual destructor

        // Assignment operator
        LookupNode&         operator=(const LookupNode& x);                     //!< Assignment operator

        // Basic utility functions
        LookupNode*         clone(void) const;                                  //!< Clone the lookup node
        const VectorString& getClass(void) const;                               //!< Get class vector
        void                printStruct(std::ostream& o) const;                 //!< Print struct for user
        std::string         toString(void) const;                               //!< Complete info about object

        // DAG functions
        LookupNode*         cloneDAG(std::map<DAGNode*, DAGNode*>& newNodes) const;         //!< Clone entire graph
        bool                isMutableTo(const DAGNode* newNode) const;                      //!< Is node mutable to newNode?
        bool                isMutableTo(const VectorInteger& index, const RbObject* newValue) const;    //!< Is node mutable to contain newValue?
        bool                isParentMutableTo(const DAGNode* oldNode, const DAGNode* newNode) const;    //!< Is parent mutable to newNode?
        void                mutateTo(DAGNode* newNode);                                     //!< Mutate to new node
        LookupNode*         mutateTo(const VectorInteger& index, RbObject* newValue);       //!< Mutate to contain newValue
        void                swapParentNode(DAGNode* oldP, DAGNode* newP);                   //!< Swap a parent node

    protected:
        // Utility functions
        const TypeSpec&     getMemberType(const RbString* name) const;                      //!< Get type of a named member variable
        DAGNode*            getVariable(void);                                              //!< Look up the variable and get a reference to it
        RbObject*           lookup(void);                                                   //!< Look up the value
        void                update(void);                                                   //!< Update value and storedValue

        // Member variables
        DAGNode*            variable;                                                       //!< Ptr to variable (simple lookup)
        LookupNode*         baseVariable;                                                   //!< Ptr to base variable (member lookup)
        RbString*           memberName;                                                     //!< Name if member variable
        IndexArgs           indexArgs;                                                      //!< Vector of index arguments (vector<DAGNode*>)
};

#endif

