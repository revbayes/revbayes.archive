//
//  Tree.cpp
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 4/6/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <string>

#include "Tree.h"
#include "TreeNode.h"

/** Default constructor */
Tree::Tree(void) {
    
}



void Tree::allocateNodes(int nNodes) {
    
    for (int i=0; i<nNodes; i++)
        nodes.push_back( new TreeNode );
}

void Tree::deleteNodes(void) {
    
    for (std::vector<TreeNode*>::iterator it=nodes.begin(); it != nodes.end(); it++)
        delete (*it);
    nodes.clear();
}
