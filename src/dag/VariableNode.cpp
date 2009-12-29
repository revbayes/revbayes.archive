/**
 * @file
 * This file contains the implementation of VariableNode, which is the base
 * class for variable nodes in the model DAG.
 *
 * @brief Implementation of VariableNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-13 15:50:43 +0100 (Sön, 13 Dec 2009) $
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 *
 * $Id: DAGNode.cpp 188 2009-12-13 14:50:43Z ronquist $
 */

#include "IntVector.h"
#include "Monitor.h"
#include "Move.h"
#include "MoveSchedule.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbObject.h"
#include "StringVector.h"
#include "VariableNode.h"


/** Constructor */
VariableNode::VariableNode(const std::string& valType)
    : DAGNode(valType) {

    touched  = false;
    moves    = NULL;
    monitors = NULL;
}


/** Copy constructor */
VariableNode::VariableNode(const VariableNode& x)
    : DAGNode(x.valueType) {

    touched = x.touched;

    if (x.moves != NULL)
        moves = (MoveSchedule*)(x.moves->clone());
    else
        moves = NULL;

    if (x.monitors != NULL) {
        monitors = new std::vector<Monitor*>();
        for (std::vector<Monitor*>::iterator i=x.monitors->begin(); i!=x.monitors->end(); i++)
            monitors->push_back(*i);
    }
    else
        monitors = NULL;
}


/** Destructor */
VariableNode::~VariableNode(void) {

    if (moves != NULL)
        delete moves;

    if (monitors != NULL) {
        for (std::vector<Monitor*>::iterator i=monitors->begin(); i!=monitors->end(); i++)
            delete (*i);
        delete monitors;
    }
}


/** Assignment operator */
VariableNode& VariableNode::operator=(const VariableNode& x) {

    if (this != &x) {

        touched = x.touched;

        if (moves != NULL)
            delete moves;
        if (x.moves != NULL)
            moves = (MoveSchedule*)(x.moves->clone());
        else
            moves = NULL;

        if (monitors != NULL) {
            for (std::vector<Monitor*>::iterator i=monitors->begin(); i!=monitors->end(); i++)
                delete (*i);
            delete monitors;
        }
        if (x.monitors != NULL) {
            monitors = new std::vector<Monitor*>();
            for (std::vector<Monitor*>::iterator i=x.monitors->begin(); i!=x.monitors->end(); i++)
                monitors->push_back(*i);
        }
        else
            monitors = NULL;
    }

    return (*this);
}


/** Add a monitor */
void VariableNode::addMonitor(Monitor* monitor) {

    monitors->push_back(monitor);
}


/** Add a move */
void VariableNode::addMove(Move* move) {

    if (moves == NULL)
        moves = new MoveSchedule(this, 1.0);
    moves->addMove(move);
}


/** Erase a monitor */
void VariableNode::eraseMonitor(Monitor* monitor) {

    std::vector<Monitor*>::iterator it;
    it = find(monitors->begin(), monitors->end(), monitor);
    if (it == monitors->end())
        throw RbException("Could not find monitor");
    else
        monitors->erase(it);
}


/** Erase a move */
void VariableNode::eraseMove(Move* move) {

    if (moves == NULL)
        throw RbException("No moves to erase");
    moves->eraseMove(move);
}


/** Get class vector describing type of object */
const StringVector& VariableNode::getClass() const {

    static StringVector rbClass = StringVector(VariableNode_name) + DAGNode::getClass();
    return rbClass;
}


/** Get update weight */
double VariableNode::getUpdateWeight(void) const {

    if (moves == NULL) 
        return 0.0;
    else
        return moves->getUpdateWeight();
}


/** Set update weight */
void VariableNode::setUpdateWeight(double weight) {

    if (weight < 0.0)
        throw RbException("Negative update weight");

    if (moves == NULL)
        moves = new MoveSchedule(this, weight);
    else
        moves->setUpdateWeight(weight);
}


