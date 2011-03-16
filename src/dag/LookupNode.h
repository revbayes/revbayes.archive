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
#include "VariableNode.h"

#include <string>
#include <vector>

typedef std::vector<Argument> Arguments;

class RbString;
class VectorString;


class LookupNode : public VariableNode {

    public: 
                            LookupNode();                                      //!< Constructor of empty node
                            LookupNode( DAGNode*    var,
                                        Arguments&  indexArgs);                //!< Lookup of regular variable
                            LookupNode( DAGNode*    baseVar,
                                        RbString*   varName,
                                        Arguments&  indexArgs);                //!< Lookup of member variable
                            LookupNode(const LookupNode& x);                   //!< Copy constructor
        virtual            ~LookupNode(void);                                  //!< Virtual destructor

        // Assignment operator
        LookupNode&         operator=(const DeterministicNode& x);              //!< Assignment operator

        // Basic utility functions
        LookupNode*         clone(void) const;                                  //!< Clone the lookup node
        const VectorString& getClass(void) const;                               //!< Get class vector
        DAGNode*            getStoredValue(void);                               //!< Get stored value intelligently
        const DAGNode*      getValue(void);                                     //!< Get current value intelligently
        const DAGNode*      getValue(void) const;                               //!< Get current value without changing the object if possible
        const VectorString& getValueClass(void) const;                          //!< Get value class
        void                printStruct(std::ostream& o) const;                             //!< Print struct for user
        void                printValue(std::ostream& o) const;                              //!< Print value for user
        void                setValue(RbObject* value);                                      //!< Set value
        std::string         toString(void) const;                                           //!< Complete info about object

        // Doubtful
        DAGNode*            getValElement(const VectorInteger& index) const;    //!< Get value element
        void                setElement(const VectorInteger& index, RbObject* value);        //!< Set value element

        // DAG functions
        LookupNode*         cloneDAG(std::map<DAGNode*, DAGNode*>& newNodes) const;         //!< Clone entire graph
        void   	            getAffected(std::set<StochasticNode*>& affected);               //!< Mark and get affected nodes
        bool                isMutableTo(const DAGNode* newNode) const;                      //!< Is node mutable to newNode?
        bool                isMutableTo(const VectorInteger& index, const RbObject* newValue) const;    //!< Is node mutable to contain newValue?
        bool                isParentMutableTo(const DAGNode* oldNode, const DAGNode* newNode) const;    //!< Is parent mutable to newNode?
        void                keepAffected(void);                                             //!< Keep value of affected nodes
        void                mutateTo(DAGNode* newNode);                                     //!< Mutate to new node
        LookupNode*         mutateTo(const VectorInteger& index, RbObject* newValue);       //!< Mutate to contain newValue
        void                restoreAffected(void);                                          //!< Restore value of affected nodes
        void                swapParentNode(DAGNode* oldP, DAGNode* newP);                   //!< Swap a parent node
        void                touchAffected(void);                                            //!< Tell affected nodes value is reset

        MoveSchedule*       getDefaultMoves(void);                                          //!< Return default moves

    protected:
        // Utility function
        void                update(void);                                                   //!< Update value and storedValue

        // Member variables
        Arguments           indexArgs;                                                      //!< Vector of index arguments
        DAGNode*            variable;                                                       //!< Ptr to variable or base variable
        RbString*           varName;                                                        //!< Name if member variable
};

#endif

