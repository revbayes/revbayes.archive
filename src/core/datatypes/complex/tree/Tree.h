//
//  Tree.h
//  RevBayes
//
//  Created by Sebastian Hoehna on 6/13/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef Tree_H
#define Tree_H

#include "MemberObject.h"


#include <set>
#include <string>

class ArgumentFrame;
class ArgumentRule;
class DAGNode;
class MethodTable;
class RandomNumberGenerator;
class TreeNode;
class VectorString;

typedef std::vector<ArgumentRule*>   MemberRules;                                                                       //!< Member rules type def, for convenience


class Tree: public MemberObject {
    
public:
    virtual                    ~Tree(void) {}                                                                       //!< Destructor
    
    // Basic utility functions you have to override
    virtual Tree*               clone(void) const = 0;                                                              //!< Clone object
    virtual const VectorString& getClass(void) const;                                                               //!< Get class vector
    
    // Basic utility functions you may want to override
    virtual void                printValue(std::ostream& o) const;                                                  //!< Print value for user
    virtual std::string         richInfo(void) const;                                                               //!< Complete info

    
    // Member variable functions; override getMemberRules to add member variables
    virtual const MemberRules&  getMemberRules(void) const;                                                         //!< Get member rules
    virtual void                setVariable(const std::string& name, DAGNode* var);                                 //!< Set member variable
    
    // Member method functions
    virtual const MethodTable&  getMethods(void) const;                                                             //!< Get member methods
    
    // Subscript operator functions
//    virtual bool                hasSubscript(void) { return false; }                                                //!< Does object support subscripting?
//    virtual DAGNode*            getSubelement(const size_t i);                                                      //!< Return subscript[](index) element
//    virtual size_t              getSubelementsSize(void) const { return 0; }                                        //!< Number of subscript elements
//    virtual void                setElement(VectorNatural& index, DAGNode* var, bool convert=true);                  //!< Set subelement
//    virtual void                setSubelement(size_t index, DAGNode* var, bool convert=true);                       //!< Set subelement
    
    // Tree specific methods
    virtual void                buildRandomTree(int nTips, RandomNumberGenerator* rng) = 0;
    std::string                 getNewickTree(void) const { return newick; }
    int                         getNumberOfNodes(void) { return int(nodes.size()); }
    int                         getNumberOfTips(void) { return numberOfTips; }
    TreeNode*                   getRoot(void) { return root; }
    void                        randomlyBreakTree(RandomNumberGenerator* rng) {}
    void                        removeSubtreeFromNode(TreeNode* p) {}
    void                        setNumberOfTips(int x) { numberOfTips = x; }
    
protected:
    Tree(void);
    Tree(const Tree& t);
    Tree(const MemberRules& memberRules);
    
    void                        allocateNodes(int nNodes);
    void                        deleteNodes(void);
    
    std::string                 newick;                                                                             //!< The newick string, should be recalculated each time the tree has changed
    std::vector<TreeNode*>      nodes;
    int                         numberOfTips;
    TreeNode*                   root;
    
    // Override this function to map member method calls to internal functions
    virtual DAGNode*            executeOperation(const std::string& name, ArgumentFrame& args);                     //!< Map member methods to internal functions
    
    // Members is the variable frame that stores member variables
    MemberFrame                 members;                                                                            //!< Member variables
};

#endif

