/**
 * @file
 * This file contains the implementation of the branch data type.
 *
 * @brief Implementation of the data type Branch
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The REvBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-21, version 1.0
 * @implements RbDataType
 *
 * $Id$
 */

#include "Branch.h"
#include "Node.h"
#include "../RbDataType.h"
#include "../RbAbstractDataType.h"

/**
 * @brief the name of this data type
 *
 * The name of this data type which is used for association or referencing.
 * Data types in REvBayes can be checked from which type they are by asking for its dataType.
 *
 */
const std::string Branch::dataType="Branch";

/**
 * @brief default constructor
 *
 * This is the default constructor
 */
Branch::Branch(void) : RbAbstractDataType("Branch") {

}

/**
 * @brief copy constructor
 *
 * This is the copy constructor
 *
 * @param b      the branch to copy
 */
Branch::Branch(Branch &b) : RbAbstractDataType("Branch") {
	// initialize with default values
	child = NULL;
	parent = NULL;

	// make a deep copy of set values
	if (b.child != NULL){
		child = new Node(*b.child);
	}

	if (b.parent != NULL){
		parent = new Node(*b.parent);
	}

	// deep copy for all
	for (std::vector<RbDataType*>::iterator it=b.parameters.begin() ; it < b.parameters.end(); it++ ){
		parameters.push_back((RbDataType*)(*it)->clone());
	}
}

/**
 * @brief constructor with the set of parameters attached to this branch
 *
 * This is the constructor with the set of parameters attached to this branch
 *
 * @param p      the vector containing the parameters
 */
Branch::Branch(std::vector<RbDataType*> p) : RbAbstractDataType("Branch") {
	// initialize with default values
	child = NULL;
	parent = NULL;
	parameters = p;
}


/**
 * @brief constructor with the two nodes attached to this branch
 *
 * This is the constructor with the two nodes attached to this branch
 *
 * @param p      the parent node of the branch
 * @param c      the child node of the branch
 */
Branch::Branch(Node* p, Node* c) : RbAbstractDataType("Branch") {
	// initialize with default values
	child = c;
	parent = p;
}

/**
 * @brief constructor with the two nodes and the set of parameters attached to this branch
 *
 * This is the constructor with the two nodes and the set of parameters attached to this branch
 *
 * @param p          the parent node of the branch
 * @param c          the child node of the branch
 * @param param      the vector of parameters
 */
Branch::Branch(Node* p, Node* c, std::vector<RbDataType*> param) : RbAbstractDataType("Branch") {
	// initialize with default values
	child = c;
	parent = p;
	parameters = param;
}

/**
 * @brief Destructor
 *
 * This is the default destructor
 */
Branch::~Branch(void) {

}

/**
 * @brief Copy this object
 *
 * This is a call of the copy constructor used from the base class
 *
 * @return     return a deep copy of the object
 *
 */
RbObject* Branch::clone(void) {
	return new Branch(*this);
}

std::string Branch::toString(){
	std::string s = getName() + '\n' + "Parameter: ";
	for (std::vector<RbDataType*>::iterator it=parameters.begin() ; it < parameters.end(); it++ ){
		s += (*it)->toString() + " ";
	}
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
void Branch::print(ostream &c) const {
	std::string s = getName() + '\n' + "Parameter: ";
	for (int i=0 ; i < parameters.size(); i++ ){
		s += parameters[i]->toString() + " ";
	}

	c << s << endl;
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
void Branch::dump(std::ostream& c){

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
void Branch::resurrect(const RbDumpState& x){

}

/**
 * @brief get name for this data type
 *
 * This function get name for this data type.
 * It is basically only a convinience function to access the static member dataType from a base class reference.
 *
 * @see RbDataType.getType()
 *
 */
const std::string&  Branch::getType(void) const{
	return dataType;
}

/**
 * @brief overloaded == operators
 *
 * This function compares this object
 *
 * @param o           the object to compare to
 *
 */
bool Branch::operator ==(RbObject& o) const {

	if (typeid(Branch) == typeid(o)){
		// we are from the same type, which is perfect :)
		Branch& tmp = ((Branch&)o);
		return (*this) == tmp;
	}
	else {
		RbDataType& dt = dynamic_cast<RbDataType&>(o);
		if ((&dt) != 0) {
			if (isConvertible(dt)){
				//can I convert myself into the type of o
				RbDataType* newType = convertTo(dt);
				return ((*newType) == dt);
			}
			else if (dt.isConvertible(dataType)){
				//try to convert o into my data type
				RbDataType* newType = dt.convertTo(dataType);
				Branch& tmp = ((Branch&)*newType);
				return (*this) == tmp;
			}
		}
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
bool Branch::operator ==(Branch& n) const {
	// check the parents
	if (!((*parent) == (*n.getParent()))){
		return false;
	}

	// check the children
		if (!((*child) == (*n.getChild()))){
			return false;
		}

	// check the number of children and parameters
	if (parameters.size() != n.getNumberOfParameter()){
		return false;
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
 * This function searches the vector of parameters of the branch and returns the element at position index.
 *
 * @param index      the position of the parameter to return
 * @returns          the parameter at the position index. If no such index exists, it return NULL.
 *
 */
RbDataType* Branch::getParameter(int index){
	if (parameters.size() <= index){
		return NULL;
	}

	return parameters[index];
}

/**
 * @brief Get the parameter with given name.
 *
 * This function searches for the parameter with identifier name.
 *
 * @param name       the identifier of the parameter
 * @returns          the first parameter matching name. If no parameter is found, it return NULL.
 *
 */
RbDataType* Branch::getParameter(std::string& name){
	RbDataType* dt = NULL;

	for (std::vector<RbDataType*>::iterator it=parameters.begin() ; it < parameters.end(); it++ ){
		if ((*it)->getName().compare(name) == 0){
			dt = *it;
			break;
		}
	}

	return dt;
}

/**
 * @brief Get the number of parameters
 *
 * This function returns the number of parameters.
 *
 * @returns          the number of parameter
 *
 */
int Branch::getNumberOfParameter(){
	return parameters.size();
}

/**
 * @brief Get parent node of branch
 *
 * This function retrieves the parent node of the branch
 *
 * @returns      the parent node of the branch
 *
 */
Node* Branch::getParent(){
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
Node* Branch::getChild(){
	return child;
}

/**
 * @brief Add a parameter to the branch
 *
 * This function adds a parameter at the end of the vector containing all other parameters for the branch.
 *
 * @param p      the parameter to be added
 * @returns      the index of the position the parameter was added to
 *
 */
int Branch::addParameter(RbDataType* p){
	parameters.push_back(p);
	return parameters.size()-1;
}

/**
 * @brief Add parameter at position i
 *
 * This function adds the parameter p at index and return the index of the position
 *
 * @param p          the parameter to add
 * @param index      the position where to add the parameter in the vector
 * @returns          the position where the parameter was added
 *
 */
int Branch::addParameter(RbDataType* p, int index){
	//TODO need to check if enough elements are in the vector before this position
	// otherwise could add at the end or create empty elements in between
	vector<RbDataType*>::iterator it = parameters.begin();
	parameters.insert(it+index, p);
	return index;
}

/**
 * @brief Set the child node
 *
 * This function sets the child node of the branch,
 *
 * @param c          the new child node
 *
 */
void Branch::setChild(Node* c){
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
void Branch::setParent(Node* p){
	parent = p;
}

/**
 * @brief Remove parameter at index.
 *
 * This function removes the parameter at the position index nd returns the removed element.
 *
 * @param index      the position of the parameter
 * @returns          the removed element, NULL if there was no element add the position
 *
 */
RbDataType* Branch::removeParameter(int index){
	if (parameters.size() <= index){
		return NULL;
	}

	RbDataType* dt = parameters[index];
	vector<RbDataType*>::iterator it = parameters.begin();
	parameters.erase(it+index);

	return dt;
}

/**
 * @brief Remove parameter with identifier name
 *
 * This function removes the parameter with the identifier name and returns the removed element.
 *
 * @param name       the name of the parameter
 * @returns          the removed element, NULL if there was no element with the identifier name
 *
 */
RbDataType* Branch::removeParameter(std::string &name){
	RbDataType* dt = NULL;

	for (std::vector<RbDataType*>::iterator it=parameters.begin() ; it < parameters.end(); it++ ){
		if ((*it)->getName().compare(name) == 0){
			dt = *it;
			parameters.erase(it);
			break;
		}
	}

	return dt;
}

/**
 * @brief Remove parameter.
 *
 * This function removes the parameter equals to p and returns the removed element.
 *
 * @param index      the parameter to be removed
 * @returns          the removed element, NULL if there was no such element in the vector
 *
 */
RbDataType* Branch::removeParameter(RbDataType* p){
	RbDataType* dt = NULL;

	for (std::vector<RbDataType*>::iterator it=parameters.begin() ; it < parameters.end(); it++ ){
		if (*it == p){
			dt = p;
			parameters.erase(it);
			break;
		}
	}

	return dt;
}

