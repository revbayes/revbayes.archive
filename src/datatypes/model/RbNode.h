/**
 * @file
 * This file contains the declaration of the node data type.
 * The node corresponds to the object representing a node in the phylogenetic tree.
 * Every node can have a parent node and a set of children nodes. The nodes are the structural elements building the tree.
 * Moreover, a node is a container of other nodes. These other nodes are the children of this node.
 * The root not wont have any parent, whereas the leaves wont have any children.
 * Every noe also contains a vector of parameters belonging to this node, with parameters being e.g. the history, ...
 *
 * @brief Declaration of the data type RbNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-10-22 11:38:03 -0700 (Thu, 22 Oct 2009) $
 * @author The REvBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-21, version 1.0
 * @implements RbDataType
 *
 * $Id: RbNode.h 51 2009-10-22 18:38:03Z Hoehna $
 */

#ifndef RbNode_H
#define RbNode_H

#include <set>
#include <string>
#include <vector>
#include "RbComplex.h"
#include "RbObject.h"

class RbBitset;

class RbNode : public RbComplex{

public:
	                            RbNode(void);
	                            RbNode(const RbNode& n);
	                            RbNode(int idx);
	                            RbNode(RbNode* p, std::set<RbNode*>& c);

        RbObject*               clone(void) const;                                                  //!< Equals comparison
		const StringVector&     getClass(void) const;                                               //!< Get class vector
		void                    print(std::ostream& o) const;                                       //!< Print value for user
		void                    printValue(std::ostream& o) const;                                  //!< Print value for user
		std::string             toString(void) const;                                               //!< Complete info 

	// overloaded operators
	bool       equals(const RbNode* n) const;                          //!< Comparison

	// member functions
	const std::set<RbNode*>& getChildren(void) const { return children; }
	int getNumberChildren(void) const { return children.size(); }
	void clearChildren(void) { children.clear(); }

	void clean(void);
	int getIndex(void) const { return index; }
	void setIndex(int i) { index = i; }
	void	addChild(RbNode* c) { children.insert(c); }
	void	addParent(RbNode* p) { parent = p; }
	RbNode* getChild(int i) const;
	RbNode* getParent() const;                                  //!< retrieves the parent RbNode
//	RbNode* getChild();                                         //!< retrieves the child RbNode
	void 	removeChild(RbNode* c);
	void	removeParent(RbNode* p);
//	void setChild(RbNode* c);                                   //!< sets the child RbNode to c
	void setParent(RbNode* p);                                  //!< sets the parent RbNode to p
	bool isLeaf(void) const;                                  //!< checks if this RbNode is a leaf RbNode


    // overloaded operators
    RbObject&       operator=(const RbObject& o);
    RbNode&         operator=(const RbNode& ar);

private:
	std::set<RbNode*> children;
	RbNode* parent;
//	int partialIndex;                        //!< index used to address the memory for the partial likelihood
//	int storedPartialIndex;                  //!< stored index for the stored partial likelihood
	int index;
	RbBitset*			taxonBipartition;
};

#endif
