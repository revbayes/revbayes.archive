/**
 * @file
 * This file contains the declaration of TreeSimple, a standard implementation of a tree.
 * This simple tree has no constraints, i.e. it can be rooted or unrooted, bifurcating or multifurcating.
 * The TreeSimple should be used to import trees and then convert it to the required format if multiple formats
 * could have been read.
 *
 * @brief Declaration of TreeSimple
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 * @extends Tree
 *
 * $Id$
 */

#ifndef TreeSimple_H
#define TreeSimple_H

#include <string>
#include <vector>

#include "Tree.h"

class RandomNumberGenerator;


class TreeSimple : public Tree {
    
public:
    TreeSimple(void);
    TreeSimple(const TreeSimple& ut);
    TreeSimple(TreeNode *root);
    TreeSimple(std::vector<std::string> names);
    TreeSimple(std::string newickStr);
    TreeSimple(int nTips);
    ~TreeSimple(void);
    
    
    // Basic utility functions
    TreeSimple*                 clone(void) const;                                                              //!< Clone object
    bool                        equals(const RbObject* x) const;                                                //!< Equals comparison
    const VectorString&         getClass(void) const;                                                           //!< Get class vector
    void                        printValue(std::ostream& o) const;                                              //!< Print value for user
    std::string                 toString(void) const { return getNewickTree(); }                                //!< Complete info
    
    // Member variable functions
    const MemberRules&          getMemberRules(void) const;                                                     //!< Get member rules
    void                        setVariable(const std::string& name, DAGNode* var);                             //!< Set member variable
    void                        setValue(const std::string& name, RbObject* val);                               //!< Set member variable
    
    // Member method functions
    const MethodTable&          getMethods(void) const;                                                         //!< Get method specifications
    DAGNode*                    executeOperation(const std::string& name, ArgumentFrame& args);
    // Tree specific methods
    void                        buildRandomTree(int nTips, RandomNumberGenerator* rng);
    
    
private:
};

#endif


