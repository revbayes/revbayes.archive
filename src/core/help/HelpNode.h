/**
 * @file
 * This file contains the declaration of HelpNode, which is
 * a part of a tree-like structure that contains the help for
 * the program.
 *
 * @brief Declaration of HelpNode
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 *
 * $Id$
 */

#ifndef HelpNode_H
#define HelpNode_H

#include "RbPtr.h"

#include <string>
#include <vector>

class HelpNode {

	public:
                                            HelpNode(void);                                                //!< Default constructor
                                            HelpNode(const HelpNode& h);                                   //!< Copy constructor
        void                                addChild(RbPtr<HelpNode> c) { childrenNodes.push_back(c); }    //!< Add a child to the vector of children nodes
        std::string                         getHelpEntry(void) { return helpEntry; }                 //!< Get the entry for this help node
        size_t                              getNumChildren(void) { return childrenNodes.size(); }    //!< Get the number of children nodes
        RbPtr<HelpNode>                     getChildIndexed(size_t idx);                             //!< Get a child with index idx
        std::vector<RbPtr<HelpNode> >&      getChildren(void) { return childrenNodes; }              //!< Get a reference to the children nodes
        RbPtr<HelpNode>                     getChildWithTag(const std::string& ts);                  //!< Returns the first (only?) child of this node with the tag
        RbPtr<HelpNode>                     getChildWithTag(const std::string& ts, size_t idx);      //!< Returns the idx-th child of this node with the tag
        int                                 getIndex(void) { return index; }                         //!< Returns the node index
        size_t                              getNumChildrenWithTag(const std::string& st);            //!< Returns the number of children of the node with the tag
        RbPtr<HelpNode>                     getParent(void) { return parentNode; }                   //!< Get a pointer to the parent
        std::string                         getTagName(void) { return tagName; }                     //!< Get the tag type
        bool                                hasChildWithTag(const std::string& ts);                  //!< Returns whether any of the children of this node have a tag
        bool                                isLeaf(void) const;                                            //!< Is this help node at the tip (leaf) of the help tree?
        void                                print(void);                                             //!< Print node information
        void                                setIndex(int x) { index = x; }                           //!< Set the index
        void                                setHelpEntry(std::string s) { helpEntry = s; }           //!< Set the entry for this help node
        void                                setParent(RbPtr<HelpNode> p) { parentNode = p; }               //!< Set the parent node
        void                                setTagName(std::string t) { tagName = t; }               //!< Set the tag name
        void                                showNode(RbPtr<HelpNode> p, size_t indent);                    //!< Show the help information in an indented format

	private:
        std::vector<RbPtr<HelpNode> >       childrenNodes;                                           //!< Vector of children nodes
        int                                 index;                                                   //!< A unique index for the help node, used when printing the help tree
        std::string                         helpEntry;                                               //!< String containing the information for this help node
        RbPtr<HelpNode>                     parentNode;                                              //!< The parent of this node
        std::string                         tagName;                                                 //!< The XML tag associated with this node
};

#endif


