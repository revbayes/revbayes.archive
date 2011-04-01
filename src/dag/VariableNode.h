/**
 * @file
 * This file contains the declaration of VariableNode, which is the base
 * class for variable nodes in a model DAG.
 *
 * @brief Declaration of VariableNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-11 16:58:43 -0800 (Fre, 11 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 *
 * $Id: DAGNode.h 187 2009-12-12 00:58:43Z jhuelsenbeck $
 */

#ifndef VariableNode_H
#define VariableNode_H

#include "DAGNode.h"
#include "Monitor.h"

class Monitor;
class Move;
class MoveSchedule;
class StochasticNode;
class VectorString;

#include <set>

class VariableNode : public DAGNode {

    public:
        virtual                        ~VariableNode(void) {}                                        //!< Virtual destructor

        // Utility functions you have to override
        virtual VariableNode*           clone(void) const = 0;                                       //!< Clone this node
        virtual const VectorString&     getDAGClass(void) const;                                     //!< Get DAG node class vector
        virtual const RbObject*         getStoredValue(void) = 0;                                    //!< Get stored value
        virtual const RbObject*         getValue(void) = 0;                                          //!< Get value
        virtual void                    printStruct(std::ostream& o) const = 0;                      //!< Print struct for user
        virtual void                    printValue(std::ostream& o) const = 0;                       //!< Print value for user
        virtual std::string             richInfo(void) const = 0;                                    //!< Complete info about object

        // DAG functions you should not override
        bool                            isTouched(void) const { return touched; }                    //!< Is node touched by move or parser?

        // DAG functions you have to override
        virtual VariableNode*           cloneDAG(std::map<DAGNode*, DAGNode*>& newNodes) const = 0;                     //!< Clone entire graph
        virtual void    	            getAffected(std::set<StochasticNode*>& affected) = 0;                           //!< Mark and get affected nodes
        virtual bool                    isMutableTo(const DAGNode* newNode) const = 0;                                  //!< Is node mutable to newNode?
        virtual bool                    isParentMutableTo(const DAGNode* oldNode, const DAGNode* newNode) const = 0;    //!< Is parent mutable to newNode?
        virtual void    	            keepAffected(void) = 0;                                                         //!< Keep value of affected nodes
        virtual void                    mutateTo(DAGNode* newNode) = 0;                                                 //!< Mutate to new node
        virtual void                    restoreAffected(void) = 0;                                                      //!< Restore value of affected nodes
        virtual void                    swapParentNode(DAGNode* oldP, DAGNode* newP) = 0;                               //!< Swap a parent node
        virtual void                    touchAffected(void) = 0;                                                        //!< Tell affected nodes value is reset

        // Default monitors and move functions
        std::vector<Monitor*>           getDefaultMonitors(void);                                                       //!< Return default monitors
        virtual MoveSchedule*           getDefaultMoves(void) = 0;                                                      //!< Return default moves

    protected:
                                        VariableNode(const std::string& valType);                                       //!< Constructor of empty node

        // Member variables
        bool                            touched;                                                                        //!< Is touched by move?
        RbObject*                       storedValue;                                                                    //!< Stored value
};

#endif

