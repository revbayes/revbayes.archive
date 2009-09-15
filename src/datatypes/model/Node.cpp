#include "Node.h"


/**
 * @brief default constructor
 *
 * This is the default constructor
 */
Node::Node(void) : RbDataType("Node") {
	parent = NULL;
}

/**
 * @brief copy constructor
 *
 * This is the copy constructor
 *
 * @param n      the node to copy
 */
Node::Node(Node &n) : RbDataType("Node") {
	// initialize with default values
	parent = NULL;

	// make a deep copy of set values
	if (n.parent != NULL){
		parent = new Node(*n.parent);
	}

	// deep copy for all children
	for (std::vector<Node*>::iterator it=n.children.begin() ; it < n.children.end(); it++ ){
		parameters.push_back((Node*)(*it)->clone());
	}

	// deep copy for all parameters
	for (std::vector<RbDataType*>::iterator it=n.parameters.begin() ; it < n.parameters.end(); it++ ){
		parameters.push_back((RbDataType*)(*it)->clone());
	}
}

/**
 * @brief copy constructor
 *
 * This is the copy constructor
 *
 * @param n      the node to copy
 */
Node::Node(Node* p, std::vector<Node*> c) : RbDataType("Node") {
	parent = p;
	children = c;
}

/**
 * @brief copy constructor
 *
 * This is the copy constructor
 *
 * @param n      the node to copy
 */
Node::Node(Node* p, std::vector<Node*> c, std::vector<RbDataType*> param) : RbDataType("Node") {
	parent = p;
	children = c;
	parameters = param;
}

void Node::clean(void) {

	parent = NULL;
	children.clear();
	parameters.clear();
	setName("");
}

/**
 * @brief Copy this object
 *
 * This is a call of the copy constructor used from the base class
 *
 * @return     return a deep copy of the object
 *
 */
RbObject* Node::clone(){
	return new Node(*this);
}

/**
 * @brief Get parameters at position index
 *
 * This function searches the vector of parameters of the node and returns the element at position index.
 *
 * @param index      the position of the parameter to return
 * @returns          the parameter at the position index. If no such index exists, it return NULL.
 *
 */
RbDataType* Node::getParameter(int index){
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
RbDataType* Node::getParameter(std::string& name){
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
 * @brief Get parent node
 *
 * This function retrieves the parent node of this node
 *
 * @returns      the parent node
 *
 */
Node* Node::getParent(){
	return parent;
}

/**
 * @brief Add a parameter to the node
 *
 * This function adds a parameter at the end of the vector containing all other parameters for the node.
 *
 * @param p      the parameter to be added
 * @returns      the index of the position the parameter was added to
 *
 */
int Node::addParameter(RbDataType* p){
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
int Node::addParameter(RbDataType* p, int index){
	//TODO need to check if enough elements are in the vector before this position
	// otherwise could add at the end or create empty elements in between
	vector<RbDataType*>::iterator it = parameters.begin();
	parameters.insert(it+index, p);
	return index;
}

/**
 * @brief Set the parent node
 *
 * This function sets the parent node of the node,
 *
 * @param p          the new parent node
 *
 */
void Node::setParent(Node* p){
	parent = p;
}

/**
 * @brief Remove parameter at index.
 *
 * This function removes the parameter at the position index and returns the removed element.
 *
 * @param index      the position of the parameter
 * @returns          the removed element, NULL if there was no element add the position
 *
 */
RbDataType* Node::removeParameter(int index){
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
RbDataType* Node::removeParameter(std::string &name){
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
RbDataType* Node::removeParameter(RbDataType* p){
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

/**
 * @brief check whether this node is a leaf node or not.
 *
 * This function checks the size of the children of this node. If this node has any children, then it is an internal node. Otherwise it is leaf node.
 *
 * @returns         true if there are no child nodes attached
 *
 */
bool Node::isLeaf(){
	return (children == NULL || children.size() == 0);
}
