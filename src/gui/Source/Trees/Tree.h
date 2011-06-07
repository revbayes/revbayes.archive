//
//  Tree.h
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 4/6/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

/**
 * @file
 * This file contains the declaration of Vector, a complex type
 * that acts as a base class for all vectors.
 *
 * @brief Declaration of Vector
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 * @extends RbComplex
 *
 * $Id$
 */

#ifndef Tree_H
#define Tree_H

#include <vector>
#include <string>

//#include "MemberObject.h"

class RandomNumberGenerator;
class TreeNode;


class Tree {
    
public:
    Tree(void);    
    virtual                      ~Tree(void) { }
    
    // Basic utility functions
    
    
    // Tree specific methods
//    virtual void                  buildRandomTree(int nTips, RandomNumberGenerator* rng) = 0;
    std::string                     getNewickTree(void) const { return newick; }
    int                             getNumberOfNodes(void) { return (int)nodes.size(); }
    int                             getNumberOfTips(void) { return numberOfTips; }
    TreeNode*                       getRoot(void) { return root; }
//    void                          randomlyBreakTree(RandomNumberGenerator* rng) {}
//    void                          removeSubtreeFromNode(TreeNode* p) {}
//    void                          setNumberOfTips(int x) { numberOfTips = x; }
    
protected:
    void                            allocateNodes(int nNodes);
    void                            deleteNodes(void);
    std::string                     newick;                                                                       //!< The newick string, should be recalculated each time the tree has changed
    std::vector<TreeNode*>          nodes;
    int                             numberOfTips;
    TreeNode*                       root;
};

#endif
