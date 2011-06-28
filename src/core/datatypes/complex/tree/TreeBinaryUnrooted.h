/**
 * @file
 * This file contains the declaration of TreeBinaryUnrooted, a member type
 * for an unrooted, binary tree.
 *
 * @brief Declaration of TreeBinaryUnrooted
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 * @extends RbComplex
 *
 * $Id:$
 */

#ifndef TreeBinaryUnrooted_H
#define TreeBinaryUnrooted_H

#include <string>
#include <vector>

#include "Tree.h"

class RandomNumberGenerator;


class TreeBinaryUnrooted : public Tree {
    
public:
    TreeBinaryUnrooted(void);
    TreeBinaryUnrooted(const TreeBinaryUnrooted& ut);
    TreeBinaryUnrooted(std::vector<std::string> names);
    TreeBinaryUnrooted(std::string newickStr);
    TreeBinaryUnrooted(int nTips);
    ~TreeBinaryUnrooted(void);
    
    
    // Basic utility functions
    TreeBinaryUnrooted*           clone(void) const;                                                            //!< Clone object
    bool                          equals(const RbObject* x) const;                                              //!< Equals comparison
    const VectorString&           getClass(void) const;                                                         //!< Get class vector
    void                          printValue(std::ostream& o) const;                                            //!< Print value for user
    std::string                   toString(void) const { return getNewickTree(); }                              //!< Complete info
    
    // Member variable functions
    const MemberRules&            getMemberRules(void) const;                                                   //!< Get member rules
    void                          setVariable(const std::string& name, DAGNode* var);                           //!< Set member variable
    void                          setValue(const std::string& name, RbObject* val);                             //!< Set member variable
    
    // Member method functions
    const MethodTable&            getMethods(void) const;                                                       //!< Get method specifications
    DAGNode*                      executeOperation(const std::string& name, ArgumentFrame& args);
    // Tree specific methods
    void                          buildRandomTree(int nTips, RandomNumberGenerator* rng);
    
    
private:
};

#endif


