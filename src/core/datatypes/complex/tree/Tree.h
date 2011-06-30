/**
 * @file
 * This file contains the declaration of a Tree, the abstract class for all types of trees.
 * 
 * We consider trees as member objects because they can be associated with distribution and hence
 * need to be able to be wrapped into constant or stochastic dag nodes.
 *
 * A tree itself is considered as a model rather than a parameter. Considering a tree as a model instead of
 * a parameter has the effect that the tree is a collection of dag nodes with a certain structure, namely that
 * the dag nodes are ordered as a tree. Even when considering trees as models, we enable prior distribution on trees.
 * Priors on models has the statistical foundation of model comparison and hence can be incorporated into standard mcmc.
 *
 * The tree is a very light weith object which holds the tree structure and provides some convinience methods 
 * associated with trees (e.g. getTips(), getNewick()). The actual variables on the tree are associated with the TreeNodes.
 *
 * Internally we structure all trees as rooted trees for convinience, though there are derived classes which are rooted or unrooted.
 * Trees rely on the structure of the tree nodes, which know their parents and their children. The tree nodes are flexible enough 
 * to hold any type of descendancies, being either bifurcating or multifurcating.
 *
 * @brief Declaration of the Tree
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date:$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-04-13, version 1.0
 *
 * $Id$
 */

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
    
    // Tree specific methods
    virtual void                buildRandomTree(int nTips, RandomNumberGenerator* rng) = 0;                         //!< TODO: This function should go into one of the tree prior distributions (Sebastian)
    std::string                 getNewickTree(void) const { return newick; }
    std::vector<TreeNode*>      getNodes(void) const { return nodes; }
    int                         getNumberOfNodes(void) const { return int(nodes.size()); }
    int                         getNumberOfTips(void) const { return int(tips.size()); }
    TreeNode*                   getRoot(void) const { return root; }
    std::vector<TreeNode*>      getTips(void) const { return tips; }
    void                        randomlyBreakTree(RandomNumberGenerator* rng) {}
    void                        removeSubtreeFromNode(TreeNode* p) {}
    void                        setTopologyChanged(bool changed) { topologyChanged= changed; }
    
protected:
    Tree(void);
    Tree(const Tree& t);
    Tree(const MemberRules& memberRules);
    
    void                        computeNewickString(void);                                                          //!< recompute the newick string
    
    std::string                 newick;                                                                             //!< The newick string
    std::vector<TreeNode*>      nodes;                                                                              //!< vector of pointers to all nodes
    std::vector<TreeNode*>      tips;                                                                               //!< vector of pointers to the tips
    TreeNode*                   root;                                                                               //!< pointer to the root node
    
    // flags indicating recalculation
    bool                        topologyChanged;                                                                    //!< flag for topology changes. if the flag is set, then the newick string, nodes in traversal oders
    
    // Override this function to map member method calls to internal functions
    virtual DAGNode*            executeOperation(const std::string& name, ArgumentFrame& args);                     //!< Map member methods to internal functions
    
    // Members is the variable frame that stores member variables
    MemberFrame                 members;                                                                            //!< Member variables
};

#endif

