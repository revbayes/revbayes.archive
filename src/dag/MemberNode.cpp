/**
 * @file
 * This file contains the partial implementation of MemberNode, an abstract
 * base class for complex objects with member methods and variables.
 *
 * @brief Partial implementation of MemberNode
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

#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "Frame.h"
#include "MemberFunction.h"
#include "MemberNode.h"
#include "RbException.h"
#include "RbNames.h"
#include "VectorString.h"
#include "VariableNode.h"


/** Constructor: we set member variables here from member rules */
MemberNode::MemberNode(const MemberRules& memberRules, const MethodTable& methodInits) : RbComplex(), members(), methods(methodInits) {

    /* Fill member table (frame) based on member rules */
    for (MemberRules::const_iterator i=memberRules.begin(); i!=memberRules.end(); i++) {

        std::string name = (*i)->getLabel();
        if ((*i)->isWrapperRule()) {
            if ((*i)->hasDefault() == false)
                members.addReference(name, (*i)->getValueType(), (*i)->getDim());
            else
                members.addReference(name, (*i)->getDefaultVariablePtr());
        }
        else {
            if ((*i)->hasDefault() == false)
                members.addVariable(name, (*i)->getValueType(), (*i)->getDim());
            else
                members.addVariable(name, (*i)->getDefaultVariable());
        }
    }
}


/** Pointer-based equals comparison */
bool MemberNode::equals(const RbObject* obj) const {

    if (this != obj) {

        if (getType() != obj->getType())
            return false;

        MemberNode* p = (MemberNode*)(obj);

        // It is impossible to look into the C++ code of methods, so return
        // false if there are any methods
        if (methods.size() != 0 || p->methods.size() != 0)
            return false;

        if (members != p->members)
            return false;
    }

    return true;
}


/** Get class vector describing type of object */
const VectorString& MemberNode::getClass(void) const {

    static VectorString rbClass = VectorString(MemberNode_name) + RbComplex::getClass();
    return rbClass;
}


/** Execute member function with preprocessed arguments */
DAGNode* MemberNode::executeMethod(const std::string& name, int funcId) {

    /* Get preprocessed arguments */
    std::vector<DAGNode*> arguments = methods.getProcessedArguments(funcId);

    /* Execute the operation */
    return executeOperation(name, arguments);
}


/** Execute member function */
DAGNode* MemberNode::executeMethod(const std::string& name, std::vector<Argument>& args) {

    /* Process the arguments */
    int funcId = methods.processArguments(name, args);

    /* Get the processed arguments */
    std::vector<DAGNode*> arguments = methods.getProcessedArguments(funcId);

    /* Execute the operation */
    return executeOperation(name, arguments);
}


/** Get value of a member variable */
const RbObject* MemberNode::getValue(const std::string& name) {

    return members.getValue(name);
}


/** Get value of a member variable (const) */
const RbObject* MemberNode::getValue(const std::string& name) const {

    return members.getVariable(name)->getValue();
}


/** Get a member variable */
const DAGNode* MemberNode::getVariable(const std::string& name) const {

    return members.getVariable(name);
}


/** Get a member variable (non-const, for derived classes) */
DAGNode* MemberNode::getVariable(const std::string& name) {

    return const_cast<DAGNode*>(members.getVariable(name));
}


/** Print value for user */
void MemberNode::printValue(std::ostream& o) const {

    const VariableTable& varTable = members.getVariableTable();
    for (VariableTable::const_iterator i=varTable.begin(); i!=varTable.end(); i++) {
        o << "." << (*i).first << std::endl;
        if ((*i).second.variable == NULL)
            o << "NULL";
        else
            (*i).second.variable->printValue(o);
        o << std::endl << std::endl;
    }
}


/** Set arguments of a member function */
int MemberNode::setArguments(const std::string& name, std::vector<Argument>& args) {

    /* Process the arguments and return function id */
    int funcId = methods.processArguments(name, args);
    return funcId;
}


/** Set base name of member variables */
void MemberNode::setName(const std::string& name) {

    members.setFrameName(name);
}


/** Set value of a member variable */
void MemberNode::setValue(const std::string& name, RbObject* val) {

    members.setValue(name, val);
}


/** Set a member variable */
void MemberNode::setVariable(const std::string& name, DAGNode* var) {

    members.setVariable(name, var);
}


/** Complete info about object */
std::string MemberNode::toString(void) const {

    std::ostringstream o;
    o << getType() << ":" << std::endl;
    printValue(o);

    return o.str();
}

