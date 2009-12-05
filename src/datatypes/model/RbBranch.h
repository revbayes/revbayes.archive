/**
 * @file
 * This file contains the declaration of the branch data type.
 * The branch corresponds to the object representing a branch in the phylogenetic tree.
 * Every branch has two nodes, a parent and a child node, attached two. The branch is the structural connection between these two nodes.
 * Every branch also contains a vector of parameters belonging to this branch, with parameters being e.g. length, clock rate, birth rate, death rate, ...
 *
 * @brief Declaration of the data type Branch
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-10-22 11:38:03 -0700 (Thu, 22 Oct 2009) $
 * @author The REvBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-21, version 1.0
 * @implements RbDataType
 *
 * $Id: Branch.h 51 2009-10-22 18:38:03Z Hoehna $
 */

#ifndef Branch_H
#define Branch_H

#include <vector>

#include "RbDataType.h"
#include "RbAbstractDataType.h"
#include "Node.h"

class Branch : public RbAbstractDataType {

public:
	// TODO not sure if I want to allow branch to be instantiated without the two nodes attached to it
	Branch(void);                                             //!< default constructor
	Branch(const Branch &b);                                        //!< copy constructor
	Branch(std::vector<RbDataType*> p);                       //!< constructor with the set of parameters attached to this branch
	Branch(Node* p, Node* c);                                 //!< constructor with the two nodes attached to this branch
	Branch(Node* p, Node* c, std::vector<RbDataType*> param); //!< constructor with the two nodes and the set of parameters attached to this branch
	~Branch(void);                                            //!< Destructor

	// implemented abstract/virtual functions from base classes
	RbObject*  clone(void) const;                                        //!< clone this object
	void       print(std::ostream &c) const;                            //!< Print the value to ostream c
	void       dump(std::ostream& c);                              //!< Dump to ostream c
	void       resurrect(const RbDumpState& x);                    //!< Resurrect from dumped state
	virtual std::string toString();

	//overloaded operators
	bool       operator==(const RbObject& o) const;                      //!< Comparison
    bool       operator==(const RbDataType& o) const;                    //!< Comparison
	bool       operator==(const Branch& o) const;                        //!< Comparison

	// assignment operator
	// += operator
	RbDataType* getParameter(int index) const;                //!< retrieves the parameter at index i
	RbDataType* getParameter(std::string& name) const;        //!< retrieves the parameter with given name
	int getNumberOfParameter() const;                         //!< get the number of parameters
	Node* getParent() const;                                  //!< retrieves the parent node
	Node* getChild() const;                                   //!< retrieves the child node
	int addParameter(RbDataType* p);                          //!< adds the parameter p at the end and return the index of the position
	int addParameter(RbDataType* p, int index);               //!< adds the parameter p at index and return the index of the position
	void setChild(Node* c);                                   //!< sets the child node to c
	void setParent(Node* p);                                  //!< sets the parent node to p
	RbDataType* removeParameter(int index);                   //!< removes the parameter at position index and returns the removed parameter
	RbDataType* removeParameter(std::string& name);           //!< removes the parameter with the name and returns the removed parameter
	RbDataType* removeParameter(RbDataType* p);               //!< removes the parameter p from the vector and returns the removes parameter

private:
	std::vector<RbDataType*> parameters;
	Node* parent;
	Node* child;
};

#endif
