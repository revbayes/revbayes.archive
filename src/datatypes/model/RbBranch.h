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

#include "RbComplex.h"
#include "RbNode.h"

class RbBranch : public RbComplex {

public:
        static const StringVector   rbClass;            //!< Static class attribute

	// TODO not sure if I want to allow branch to be instantiated without the two nodes attached to it
        RbBranch(void);                                             //!< default constructor
        RbBranch(const RbBranch &b);                                        //!< copy constructor
        RbBranch(RbNode* p, RbNode* c);                                 //!< constructor with the two nodes attached to this branch
        ~RbBranch(void);                                            //!< Destructor

	// implemented abstract/virtual functions from base classes
	RbObject*  clone(void) const;                                        //!< clone this object
	const StringVector&        getClass() const { return rbClass; }            //!< Get class
	void       print(std::ostream &c) const;                            //!< Print the value to ostream c
	void       dump(std::ostream& c);                              //!< Dump to ostream c
	void       resurrect(const RbDumpState& x);                    //!< Resurrect from dumped state
	std::string toString() const ;

	//overloaded operators
	bool       equals(const RbObject* o) const;                      //!< Comparison
    bool       equals(const RbBranch* o) const;                        //!< Comparison

	// assignment operator
	// += operator
	RbNode* getParent() const;                                  //!< retrieves the parent node
	RbNode* getChild() const;                                   //!< retrieves the child node
	void setChild(RbNode* c);                                   //!< sets the child node to c
	void setParent(RbNode* p);                                  //!< sets the parent node to p

    // overloaded operators
    RbObject&         operator=(const RbObject& o);
    RbBranch&         operator=(const RbBranch& o);
private:
	RbNode* parent;
	RbNode* child;
};

#endif
