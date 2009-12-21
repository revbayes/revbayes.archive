/**
 * @file
 * This file contains the implementation of the branch data type.
 *
 * @brief Implementation of the data type Branch
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-10-22 11:38:03 -0700 (Thu, 22 Oct 2009) $
 * @author The REvBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-21, version 1.0
 * @implements RbDataType
 *
 * $Id: Branch.cpp 51 2009-10-22 18:38:03Z Hoehna $
 */

#include "RbBranch.h"
#include "RbComplex.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbNode.h"
#include "RbTopology.h"
#include "StringVector.h"

/**
 * @brief default constructor
 *
 * This is the default constructor
 */
RbBranch::RbBranch(void) : RbComplex() {

}

/**
 * @brief copy constructor
 *
 * This is the copy constructor
 *
 * @param b      the branch to copy
 */
RbBranch::RbBranch(const RbBranch &b) : RbComplex() {
	// initialize with default values
	child = NULL;
	parent = NULL;

	// make a deep copy of set values
	if (b.child != NULL){
		child = new RbNode(*b.child);
	}

	if (b.parent != NULL){
		parent = new RbNode(*b.parent);
	}
}


/**
 * @brief constructor with the two nodes attached to this branch
 *
 * This is the constructor with the two nodes attached to this branch
 *
 * @param p      the parent node of the branch
 * @param c      the child node of the branch
 */
RbBranch::RbBranch(RbNode* p, RbNode* c) : RbComplex() {
	// initialize with default values
	child = c;
	parent = p;
}

/**
 * @brief Destructor
 *
 * This is the default destructor
 */
RbBranch::~RbBranch(void) {

}

/**
 * @brief Copy this object
 *
 * This is a call of the copy constructor used from the base class
 *
 * @return     return a deep copy of the object
 *
 */
RbObject* RbBranch::clone(void) const {
	return new RbBranch(*this);
}

std::string RbBranch::toString() const {
    std::string s = RbNames::Branch::name + " from " + parent->toString() + " to " + child->toString();

	return s;
}

/**
 * @brief print function
 *
 * This function prints this object.
 *
 * @see RbObject.print()
 * @param c           the stream where to print to
 *
 */
void RbBranch::printValue(std::ostream &c) const {
	std::string s = RbNames::Branch::name + " from " + parent->toString() + " to " + child->toString();

	c << s << std::endl;
}

/**
 * @brief print function
 *
 * This function prints this object.
 *
 * @see RbObject.print()
 * @param c           the stream where to print to
 *
 */
void RbBranch::print(std::ostream &c) const {
    std::string s = RbNames::Branch::name + " from " + parent->toString() + " to " + child->toString();

    c << s << std::endl;
}


/**
 * @brief overloaded == operators
 *
 * This function compares this object
 *
 * @param o           the object to compare to
 *
 */
bool RbBranch::equals(const RbObject* o) const {

	return o == this;
}

/**
 * @brief overloaded == operators
 *
 * This function compares this object
 *
 * @param o           the object to compare to
 *
 */
bool RbBranch::equals(const RbBranch* n) const {
	return n == this;
}

/** Get string showing inheritance */
const StringVector& RbBranch::getClass(void) const {

    static StringVector rbClass = StringVector(RbNames::Branch::name) + RbComplex::getClass();
    return rbClass;
}

/**
 * @brief Get parent node of branch
 *
 * This function retrieves the parent node of the branch
 *
 * @returns      the parent node of the branch
 *
 */
RbNode* RbBranch::getParent() const {
	return parent;
}

/**
 * @brief Get the child node.
 *
 * This function retrieves the child node of the branch.
 *
 * @returns      the child node
 *
 */
RbNode* RbBranch::getChild() const {
	return child;
}



/**
 * @brief Set the child node
 *
 * This function sets the child node of the branch,
 *
 * @param c          the new child node
 *
 */
void RbBranch::setChild(RbNode* c){
	child = c;
}

/**
 * @brief Set the parent node
 *
 * This function sets the parent node of the branch,
 *
 * @param p          the new parent node
 *
 */
void RbBranch::setParent(RbNode* p){
	parent = p;
}

RbObject& RbBranch::operator=(const RbObject& obj) {

    try {
        // Use built-in fast down-casting first
        const RbBranch& x = dynamic_cast<const RbBranch&> (obj);

        RbBranch& sv = (*this);
        sv = x;
        return sv;
    } catch (std::bad_cast & bce) {
        try {
            // Try converting the value to an argumentRule
            const RbBranch& x = dynamic_cast<const RbBranch&> (*(obj.convertTo(RbNames::Branch::name)));

            RbBranch& sv = (*this);
            sv = x;
            return sv;
        } catch (std::bad_cast & bce) {
            RbException e("Not supported assignment of " + obj.getClass()[0] + " to " + RbNames::Branch::name);
            throw e;
        }
    }

    // dummy return
    return (*this);
}

RbBranch& RbBranch::operator=(const RbBranch& o) {

    *parent = *(o.parent);
    *child = *(o.child);

    return (*this);
}
