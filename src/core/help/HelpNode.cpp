/**
 * @file
 * This file contains the implementation of HelpNode, instances of 
 * which form a tree of help information which is originally specified
 * in XML files.
 *
 * @brief Implementation of HelpNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id$
 */

#include "HelpNode.h"
#include "StringUtilities.h"
#include <iostream>



/** Default constructor */
HelpNode::HelpNode(void) {

}


/** Copy constructor */
HelpNode::HelpNode(const HelpNode& h) {

}


/** Return the idx-th child of this node */
RbPtr<HelpNode> HelpNode::getChildIndexed(size_t idx) {

    if (idx >= childrenNodes.size())
        return RbPtr<HelpNode>::getNullPtr();
    return childrenNodes[idx];
}


/** Return the first (only) child of the node with the tag */
RbPtr<HelpNode> HelpNode::getChildWithTag(const std::string& ts) {

    std::string tn = ts;
    StringUtilities::toLower(tn);
    for (std::vector<RbPtr<HelpNode> >::iterator it = childrenNodes.begin(); it != childrenNodes.end(); it++)
        {
        if ( (*it)->getTagName() == tn )
            return (*it);
        }
    return RbPtr<HelpNode>::getNullPtr();
}


/** Return the idx-th child of this node with the tag (there may be multiple children with the same tag) */
RbPtr<HelpNode> HelpNode::getChildWithTag(const std::string& ts, size_t idx) {

    std::string tn = ts;
    StringUtilities::toLower(tn);
    size_t nMatches = 0;
    for (std::vector<RbPtr<HelpNode> >::iterator it = childrenNodes.begin(); it != childrenNodes.end(); it++)
        {
        if ( (*it)->getTagName() == tn )
            {
            if (nMatches == idx)
                return (*it);
            nMatches++;
            }
        }
    return RbPtr<HelpNode>::getNullPtr();
}


/** Return the number of children of the node with the tag */
size_t HelpNode::getNumChildrenWithTag(const std::string& ts) {

    std::string tn = ts;
    StringUtilities::toLower(tn);
    size_t nMatches = 0;
    for (std::vector<RbPtr<HelpNode> >::iterator it = childrenNodes.begin(); it != childrenNodes.end(); it++)
        {
        if ( (*it)->getTagName() == tn )
            nMatches++;
        }
    return nMatches;
}


/** Returns whether the node has a child with the tag */
bool HelpNode::hasChildWithTag(const std::string& ts) {

    std::string tn = ts;
    StringUtilities::toLower(tn);
    for (std::vector<RbPtr<HelpNode> >::iterator it = childrenNodes.begin(); it != childrenNodes.end(); it++)
        {
        if ( (*it)->getTagName() == tn )
            return true;
        }
    return false;
}


/** Returns whether the node is a leaf node (tip node) */
bool HelpNode::isLeaf(void) const {

    if ( childrenNodes.size() == 0 )
        return true;
    return false;
}


/** Simple print function for the node, useful for debugging */
void HelpNode::print(void) {

    std::cout << "Node Name: \"" << tagName << "\"" << std::endl;
    std::cout << "Node Entry: \"" << helpEntry << "\"" << std::endl;
}


/** Used for printing the help tree in a nicely indented manner */
void HelpNode::showNode(RbPtr<HelpNode> p, size_t indent) {

    if (p.get() != NULL)
        {
        for (size_t i=0; i<indent; i++)
            std::cout << " ";
        std::cout << tagName << " \"" << helpEntry << "\"" << std::endl;
        for (std::vector<RbPtr<HelpNode> >::iterator it = childrenNodes.begin(); it != childrenNodes.end(); it++)
            {
            (*it)->showNode(*it, indent+3);
            }
        }
}
