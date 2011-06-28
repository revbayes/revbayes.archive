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

#include <string>
#include <vector>

enum TagType { ROOT, HELP_ENTRY, NAME, SUCCINCT, VERBOSE, ARGUMENT, USAGE, THEORY, EXAMPLE, AUTHOR, REFERENCE }; //!< Enum specifying XML tag types

class HelpNode {

	public:
                                         HelpNode(void);                                          //!< Default constructor
                                         HelpNode(const HelpNode& h);                             //!< Copy constructor
        void                             addChild(HelpNode* c) { childrenNodes.push_back(c); }    //!< Add a child to the vector of children nodes
        std::string                      getHelpEntry(void) { return helpEntry; }                 //!< Get the entry for this help node
        size_t                           getNumChildren(void) { return childrenNodes.size(); }    //!< Get the number of children nodes
        HelpNode*                        getChildIndexed(size_t idx);                             //!< Get a child with index idx
        std::vector<HelpNode*>&          getChildren(void) { return childrenNodes; }              //!< Get a reference to the children nodes
        HelpNode*                        getParent(void) { return parentNode; }                   //!< Get a pointer to the parent
        TagType                          getTagType(void) { return tagType; }                     //!< Get the tag type
        void                             setHelpEntry(std::string s) { helpEntry = s; }           //!< Set the entry for this help node
        void                             setParent(HelpNode* p) { parentNode = p; }               //!< Set the parent node
        void                             setTagType(TagType t) { tagType = t; }

	private:
        TagType                          tagType;                                                 //!< The XML tag associated with this node
        std::string                      helpEntry;                                               //!< String containing the information for this help node
        HelpNode*                        parentNode;                                              //!< The parent of this node
        std::vector<HelpNode*>           childrenNodes;                                           //!< Vector of children nodes
};

#endif


