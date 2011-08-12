/**
 * @file
 * This file contains the implementation of MoveSchedule, which is a
 * complex type holding an update weight and a set of moves for either
 * a stochastic DAG node or a set of stochastic DAG nodes retrieved
 * through a complex object like a tree.
 *
 * @brief Declaration of MoveSchedule
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id$
 */

#include "Move.h"
#include "MoveSchedule.h"
#include "RbException.h"
#include "RbNames.h"
#include "VectorString.h"
#include "VariableNode.h"

#include <algorithm>


/** Constructor */
MoveSchedule::MoveSchedule(VariableNode* node, double weight)
    : ConstantMemberObject() {

    theNode = node;
    nodeUpdateWeight = weight;
}


/** Copy constructor: copy moves */
MoveSchedule::MoveSchedule(const MoveSchedule& x) {

    theNode          = x.theNode;
    nodeUpdateWeight = x.nodeUpdateWeight;

    for (std::vector<Move*>::const_iterator i=x.schedule.begin(); i!=x.schedule.end(); i++)
        schedule.push_back((Move*)((*i)->clone()));
}


/** Destructor: delete moves */
MoveSchedule::~MoveSchedule(void) {

    for (std::vector<Move*>::iterator i=schedule.begin(); i!=schedule.end(); i++)
        delete (*i);
}


/** Assignment operator */
MoveSchedule& MoveSchedule::operator=(const MoveSchedule& x) {

    if (this != &x) {
        theNode          = x.theNode;
        nodeUpdateWeight = x.nodeUpdateWeight;

        for (std::vector<Move*>::iterator i=schedule.begin(); i!=schedule.end(); i++)
            delete (*i);

        schedule.clear();

        for (std::vector<Move*>::const_iterator i=x.schedule.begin(); i!=x.schedule.end(); i++)
            schedule.push_back((Move*)((*i)->clone()));
    }

    return (*this);
}


/** Add move */
void MoveSchedule::addMove(Move* move) {

    schedule.push_back(move);
}


/** Clone object */
MoveSchedule* MoveSchedule::clone(void) const {

    return new MoveSchedule(*this);
}


/** Get class vector describing type of object */
const VectorString& MoveSchedule::getClass(void) const { 

    static VectorString rbClass = VectorString(MoveSchedule_name) + ConstantMemberObject::getClass();
	return rbClass;
}


/** Erase move */
void MoveSchedule::eraseMove(const Move* move) {

    std::vector<Move*>::iterator i = find(schedule.begin(), schedule.end(), move);
    if (i == schedule.end())
        throw RbException("Move does not exist in move schedule");

    schedule.erase(i);
}


/** Print value for user */
void MoveSchedule::printValue(std::ostream& o) const {

    o << "Move schedule with " << schedule.size() << " moves; update weight = " << nodeUpdateWeight << std::endl;
}


/** Complete info about object */
std::string MoveSchedule::richInfo(void) const {

    std::ostringstream o;
    o << "MoveSchedule:" << std::endl;
    o << "theNode          = " << theNode->getValue() << std::endl;
    o << "schedule         = Vector with " << schedule.size() << " moves" << std::endl;
    o << "nodeUpdateWeight = " << nodeUpdateWeight;

    return o.str();
}


/** Set update weight */
void MoveSchedule::setUpdateWeight(double weight) {

    if (weight < 0.0)
        throw RbException("Invalid assignment: negative update weight");

    nodeUpdateWeight = weight;
}



