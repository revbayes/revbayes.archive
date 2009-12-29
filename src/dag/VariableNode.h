/**
 * @file
 * This file contains the declaration of VariableNode, which is the base
 * class for variable nodes in a model DAG.
 *
 * @brief Declaration of VariableNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-11 16:58:43 -0800 (Fre, 11 Dec 2009) $
 * @author The RevBayes core development team
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

class Move;
class MoveSchedule;
class StochasticNode;
class StringVector;

#include <set>

class VariableNode : public DAGNode {

    public:
        virtual                     ~VariableNode(void);                                    //!< Destructor

        // Functions you have to override
        virtual VariableNode*       clone(void) const = 0;                                  //!< Clone this node
        virtual const StringVector& getClass(void) const;                                   //!< Get class vector
        virtual const RbObject*     getStoredValue(void) = 0;                               //!< Get stored value
        virtual const RbObject*     getValElement(const IntVector& index) const = 0;        //!< Get value element
        virtual const RbObject*     getValue(void) = 0;                                     //!< Get value
        virtual const RbObject*     getValue(void) const = 0;                               //!< Get value (const)
        virtual void                printStruct(std::ostream& o) const = 0;                 //!< Print struct for user
        virtual void                printValue(std::ostream& o) const = 0;                  //!< Print value for user
        virtual void                setElement(const IntVector& index, RbObject* val) = 0;  //!< Set value element
        virtual void                setValue(RbObject* val) = 0;                            //!< Set value
        virtual std::string         toString(void) const = 0;                               //!< Complete info about object

        // Functions you should not override
        bool                        isTouched(void) const { return touched; }               //!< Is the node touched by a move?

        // Functions for updating part of a DAG
        virtual void    	        getAffected(std::set<StochasticNode*>& affected) = 0;   //!< Mark and get affected nodes
        virtual void    	        keepAffected(void) = 0;                                 //!< Keep value of affected nodes
        virtual void                restoreAffected(void) = 0;                              //!< Restore value of affected nodes

        // Monitor and move functions
        void                        addMonitor(Monitor* monitor);                           //!< Add a monitor
        void                        addMove(Move* move);                                    //!< Add a move
        void                        eraseMonitor(Monitor* monitor);                         //!< Remove a monitor
        void                        eraseMove(Move* move);                                  //!< Remove a move
        std::vector<Monitor*>*      getMonitors(void) { return monitors; }                  //!< Return moves
        MoveSchedule*               getMoves(void) { return moves; }                        //!< Return moves
        double                      getUpdateWeight(void) const;                            //!< Get update weight
        void                        setUpdateWeight(double weight);                         //!< Set update weight

    protected:
                                    VariableNode(const std::string& valType);               //!< Value class constructor
                                    VariableNode(const VariableNode& x);                    //!< Copy constructor

        // Assignment operator
        VariableNode&               operator=(const VariableNode& x);                       //!< Assignment operator

        // Member variables
        bool                        touched;                                                //!< Is touched by move?
        std::vector<Monitor*>*      monitors;                                               //!< Monitors
        MoveSchedule*               moves;                                                  //!< Moves
};

#endif


