//
//  HelpNode.cpp
//  RevBayesCore
//
//  Created by John Huelsenbeck on 6/26/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "HelpNode.h"
#include "StringUtilities.h"
#include <iostream>



HelpNode::HelpNode(void) {

}


HelpNode::HelpNode(const HelpNode& h) {

}


HelpNode* HelpNode::getChildIndexed(size_t idx) {

    if (idx >= childrenNodes.size())
        return NULL;
    return childrenNodes[idx];
}


HelpNode* HelpNode::getChildWithTag(const std::string& ts) {

    std::string tn = ts;
    StringUtilities::toLower(tn);
    for (std::vector<HelpNode*>::iterator it = childrenNodes.begin(); it != childrenNodes.end(); it++)
        {
        if ( (*it)->getTagName() == tn )
            return (*it);
        }
    return NULL;
}


HelpNode* HelpNode::getChildWithTag(const std::string& ts, size_t idx) {

    std::string tn = ts;
    StringUtilities::toLower(tn);
    size_t nMatches = 0;
    for (std::vector<HelpNode*>::iterator it = childrenNodes.begin(); it != childrenNodes.end(); it++)
        {
        if ( (*it)->getTagName() == tn )
            {
            if (nMatches == idx)
                return (*it);
            nMatches++;
            }
        }
    return NULL;
}

size_t HelpNode::getNumChildrenWithTag(const std::string& ts) {

    std::string tn = ts;
    StringUtilities::toLower(tn);
    size_t nMatches = 0;
    for (std::vector<HelpNode*>::iterator it = childrenNodes.begin(); it != childrenNodes.end(); it++)
        {
        if ( (*it)->getTagName() == tn )
            nMatches++;
        }
    return nMatches;
}

bool HelpNode::hasChildWithTag(const std::string& ts) {

    std::string tn = ts;
    StringUtilities::toLower(tn);
    for (std::vector<HelpNode*>::iterator it = childrenNodes.begin(); it != childrenNodes.end(); it++)
        {
        if ( (*it)->getTagName() == tn )
            return true;
        }
    return false;
}


bool HelpNode::isLeaf(void) {

    if ( childrenNodes.size() == 0 )
        return true;
    return false;
}


void HelpNode::print(void) {

    std::cout << "Node Name: \"" << tagName << "\"" << std::endl;
    std::cout << "Node Entry: \"" << helpEntry << "\"" << std::endl;
}

void HelpNode::showNode(HelpNode* p, size_t indent) {

    if (p != NULL)
        {
        for (size_t i=0; i<indent; i++)
            std::cout << " ";
        std::cout << tagName << " \"" << helpEntry << "\"" << std::endl;
        for (std::vector<HelpNode*>::iterator it = childrenNodes.begin(); it != childrenNodes.end(); it++)
            {
            (*it)->showNode(*it, indent+3);
            }
        }
}
