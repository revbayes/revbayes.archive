/**
 * @file
 * This file contains the declaration of the node data type.
 * The node corresponds to the object representing a node in the phylogenetic tree.
 * Every node can have a parent node and a set of children nodes. The nodes are the structural elements building the tree.
 * Moreover, a node is a container of other nodes. These other nodes are the children of this node.
 * The root not wont have any parent, whereas the leaves wont have any children.
 * Every noe also contains a vector of parameters belonging to this node, with parameters being e.g. the history, ...
 *
 * @brief Declaration of the data type Node
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

#ifndef Node_H
#define Node_H

#include <string>
#include <vector>
#include "RbAbstractDataType.h"
#include "RbDataType.h"
#include "RbObject.h"

class Node : public RbAbstractDataType{

public:

	Node(void);
	Node(const Node& n);
	Node(Node* p, std::vector<Node*> c);
	Node(Node* p, std::vector<Node*> c, std::vector<RbDataType*> param);

    // implemented abstract/virtual functions from base classes
	RbObject*  clone() const;
	void       print(std::ostream &c) const;                            //!< Print the value to ostream c
	void       dump(std::ostream& c);                              //!< Dump to ostream c
	void       resurrect(const RbDumpState& x);                    //!< Resurrect from dumped state

	// overloaded operators
	bool       operator==(const RbObject& o) const;                      //!< Comparison
	bool       operator==(const RbDataType& n) const;                    //!< Comparison
	bool       operator==(const Node& n) const;                          //!< Comparison

	// member functions
	Node* getChild(int i) const { return children[i]; }
	std::vector<Node*> &getChildren(void) { return children; }
	int getNumberChildren(void) const { return children.size(); }
	void clearChildren(void) { children.clear(); }
	std::vector<RbDataType*> &getParameter(void) { return parameters; }
	int getNumberParameter(void) const { return parameters.size(); }

	void clean(void);
	int getIndex(void) const { return index; }
	void setIndex(int i) { index = i; }
	RbDataType* getParameter(int index) const;                //!< retrieves the parameter at index i
	RbDataType* getParameter(std::string& name) const;        //!< retrieves the parameter with given name
	Node* getParent() const;                                  //!< retrieves the parent node
//	Node* getChild();                                         //!< retrieves the child node
	int addParameter(RbDataType* p);                          //!< adds the parameter p at the end and return the index of the position
	int addParameter(RbDataType* p, int index);               //!< adds the parameter p at index and return the index of the position
//	void setChild(Node* c);                                   //!< sets the child node to c
	void setParent(Node* p);                                  //!< sets the parent node to p
	RbDataType* removeParameter(int index);                   //!< removes the parameter at position index and returns the removed parameter
	RbDataType* removeParameter(std::string& name);           //!< removes the parameter with the name and returns the removed parameter
	RbDataType* removeParameter(RbDataType* p);               //!< removes the parameter p from the vector and returns the removes parameter
	bool isLeaf(void) const;                                  //!< checks if this node is a leaf node


private:
	std::vector<Node*> children;
	Node* parent;
	std::vector<RbDataType*> parameters;      //!< node parameters as history
	int partialIndex;                        //!< index used to address the memory for the partial likelihood
	int storedPartialIndex;                  //!< stored index for the stored partial likelihood
	int index;
};

#endif
