#include "RbNode.h"
#include "RbComplex.h"
#include "RbObject.h"

#include <string>


/**
 * @brief default constructor
 *
 * This is the default constructor
 */
RbNode::RbNode(void) : RbComplex() {
	parent = NULL;
}

/**
 * @brief copy constructor
 *
 * This is the copy constructor
 *
 * @param n      the RbNode to copy
 */
RbNode::RbNode(const RbNode &n) : RbComplex() {
	// initialize with default values
	parent = NULL;

	// make a deep copy of set values
	if (n.parent != NULL){
		parent = new RbNode(*n.parent);
	}

	// deep copy for all children
	for (int i=0; i<n.getNumberChildren(); i++){
	    RbNode* c = n.getChild(i);
		children.push_back((RbNode*) c->clone());
	}

}

RbNode::RbNode(int idx) : RbComplex() {
	// initialize with default values
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
RbNode::RbNode(RbNode* p, std::vector<RbNode*> c) : RbComplex() {
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

	c << name << "(";

	// deep print for all children

	for (int i=0 ; i < children.size(); i++ ){
		children[i]->print(c);
	}

	c << std::endl;
}

/**
 * @brief dump function
 *
 * This function dumps this object.
 *
 * @see RbObject.dump()
 * @param c           the stream where to dump to
 *
 */
void RbNode::dump(std::ostream& c){

}

/**
 * @brief resurrect function
 *
 * This function resurrects this object.
 *
 * @see RbObject.resurrect()
 * @param x           the object from which to resurrect
 *
 */
void RbNode::resurrect(const RbDumpState& x){

}

/**
 * @brief overloaded == operators
 *
 * This function compares this object
 *
 * @param o           the object to compare to
 *
 */
bool RbNode::operator ==(const RbObject& o) const {

	if (typeid(RbNode) == typeid(o)){
		// we are from the same type, which is perfect :)
		RbNode& tmp = ((RbNode&)o);
		return (*this) == tmp;
	}

	return false;
}

/**
 * @brief overloaded == operators
 *
 * This function compares this object
 *
 * @param o           the object to compare to
 *
 */
bool RbNode::operator ==(const RbNode& n) const {
	// check the parents
	if (!((*parent) == (*n.getParent()))){
		return false;
	}

	// check the number of children and parameters
	if (parameters.size() != n.getNumberParameter() || children.size() != n.getNumberChildren()){
		return false;
	}

	for (int i=0; i < children.size(); i++ ){
		RbNode& my_i = *children[i];
		if (!(my_i == (*n.getChild(i)))){
			return false;
		}
	}

	// deep print for all parameters
	for (int i=0 ; i < parameters.size(); i++ ){
		RbDataType& my_i = *parameters[i];
		if (!(my_i == (*n.getParameter(i)))){
			return false;
		}
	}

	return true;
}

/**
 * @brief Get parameters at position index
 *
 * This function searches the vector of parameters of the RbNode and returns the element at position index.
 *
 * @param index      the position of the parameter to return
 * @returns          the parameter at the position index. If no such index exists, it return NULL.
 *
 */
RbDataType* RbNode::getParameter(int index) const {
	if (parameters.size() <= index){
		return NULL;
	}

	return parameters[index];
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

	children.remove(c);
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
