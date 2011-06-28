//
//  HelpNode.cpp
//  RevBayesCore
//
//  Created by John Huelsenbeck on 6/26/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "HelpNode.h"



HelpNode::HelpNode(void) {

}


HelpNode::HelpNode(const HelpNode& h) {

}


HelpNode* HelpNode::getChildIndexed(size_t idx) {

    if (idx >= childrenNodes.size())
        return NULL;
    return childrenNodes[idx];
}