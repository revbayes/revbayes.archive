#include "RbBitset.h"
#include "RbComplex.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbNode.h"
#include "StringVector.h"



/** default constructor */
RbNode::RbNode(void) : RbComplex() {

	parent = NULL;
}

/** copy constructor  */
RbNode::RbNode(const RbNode &n) : RbComplex() {

	parent = NULL;

	// make a deep copy of set values
	if (n.parent != NULL){
		parent = new RbNode(*n.parent);
	}

	// deep copy for all children
	for (int i=0; i<n.getNumberChildren(); i++){
	    RbNode* c = n.getChild(i);
		children.insert((RbNode*) c->clone());
	}

}

RbNode::RbNode(int idx) : RbComplex() {

	parent = NULL;
	index = idx;
}


/**
 * @brief copy constructor
 *
 * This is the copy constructor
 *
 * @param n      the RbNode to copy
 */
RbNode::RbNode(RbNode* p, std::set<RbNode*>& c) : RbComplex() {
	parent = p;
	children = c;
}

void RbNode::clean(void) {

	parent = NULL;
	children.clear();
}

/**
 * @brief Copy this object
 *
 * This is a call of the copy constructor used from the base class
 *
 * @return     return a deep copy of the object
 *
 */
RbObject* RbNode::clone() const {

	return new RbNode(*this);
}

/** Get string showing inheritance */
const StringVector& RbNode::getClass(void) const { 

    static StringVector rbClass = StringVector(RbNames::Node::name) + RbComplex::getClass();
	return rbClass;
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
std::string RbNode::toString(void) const {

	std::string tmp = RbNames::Node::name + "(";

	// print for all children

	for (std::set<RbNode*>::iterator it=children.begin(); it!=children.end(); it++ ){
	    tmp += (*it)->toString();
	}

	return tmp + ")";
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
void RbNode::printValue(std::ostream &c) const {

    c << RbNames::Node::name << "(" << index << ")" << std::endl;
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
void RbNode::print(std::ostream &c) const {

    c << RbNames::Node::name << "(";

    // deep print for all children

    for (std::set<RbNode*>::iterator it=children.begin(); it!=children.end(); it++ ){
        (*it)->print(c);
    }

    c << std::endl;
}

/**
 * @brief overloaded == operators
 *
 * This function compares this object
 *
 * @param o           the object to compare to
 *
 */
bool RbNode::equals(const RbObject* o) const {

	return this == o;
}

/**
 * @brief overloaded == operators
 *
 * This function compares this object
 *
 * @param o           the object to compare to
 *
 */
bool RbNode::equals(const RbNode* n) const {

	return n == this;
}

/**
 * @brief Get parent RbNode
 *
 * This function retrieves the parent RbNode of this RbNode
 *
 * @returns      the parent RbNode
 *
 */
RbNode* RbNode::getParent() const {
	return parent;
}

RbNode* RbNode::getChild(int i) const {
    int idx = 0;
    for (std::set<RbNode*>::iterator it=children.begin(); it!=children.end(); it++) {
        if (idx == i) {
            return (*it);
        }
        idx++;
    }
    return NULL;
}

/**
 * @brief Set the parent RbNode
 *
 * This function sets the parent RbNode of the RbNode,
 *
 * @param p          the new parent RbNode
 *
 */
void RbNode::setParent(RbNode* p){
	parent = p;
}


void RbNode::removeChild(RbNode* c) {

	children.erase(c);
}

void RbNode::removeParent(RbNode* p) {

}

/**
 * @brief check whether this RbNode is a leaf RbNode or not.
 *
 * This function checks the size of the children of this RbNode. If this RbNode has any children, then it is an internal RbNode. Otherwise it is leaf RbNode.
 *
 * @returns         true if there are no child RbNodes attached
 *
 */
bool RbNode::isLeaf() const {
//	return (children == NULL || children.size() == 0);
	return (children.size() == 0);
}

RbObject& RbNode::operator=(const RbObject& obj) {

    try {
        // Use built-in fast down-casting first
        const RbNode& x = dynamic_cast<const RbNode&> (obj);

        RbNode& sv = (*this);
        sv = x;
        return sv;
    } catch (std::bad_cast & bce) {
        try {
            // Try converting the value to an argumentRule
            const RbNode& x = dynamic_cast<const RbNode&> (*(obj.convertTo(RbNames::Node::name)));

            RbNode& sv = (*this);
            sv = x;
            return sv;
        } catch (std::bad_cast & bce) {
            RbException e("Not supported assignment of " + obj.getClass()[0] + " to " + RbNames::Node::name);
            throw e;
        }
    }

    // dummy return
    return (*this);
}

RbNode& RbNode::operator=(const RbNode& o) {

    *parent = *(o.parent);

    // assign all children
    children.clear();
    const std::set<RbNode*>& cs = o.getChildren();
    for (std::set<RbNode*>::iterator it=cs.begin(); it!=cs.end(); it++){
        children.insert((RbNode*)(*it)->clone());
    }

    index = o.index;
    *taxonBipartition = *(o.taxonBipartition);
    return (*this);
}
