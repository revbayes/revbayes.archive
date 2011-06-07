/**
 * @file
 * This file contains the declaration of Vector, a complex type
 * that acts as a base class for all vectors.
 *
 * @brief Declaration of Vector
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 * @extends RbComplex
 *
 * $Id$
 */

#ifndef Tree_H
#define Tree_H

#include <vector>

#include "MemberObject.h"

class RandomNumberGenerator;
class TreeNode;


class Tree : public MemberObject {

    public:
        virtual                      ~Tree(void) {}
    
        // Basic utility functions
        virtual MemberObject*         clone(void) const = 0;                                                        //!< Clone object
//        virtual bool                  equals(const RbObject* x) const;                                              //!< Equals comparison
        virtual const VectorString&   getClass(void) const;                                                         //!< Get class vector
//        virtual void                  printValue(std::ostream& o) const;                                            //!< Print value for user
//        virtual std::string           toString(void) const;                                                         //!< Complete info

        // Member variable functions
        virtual const MemberRules&    getMemberRules(void) const;                                                   //!< Get member rules
        virtual void                  setVariable(const std::string& name, DAGNode* var);                           //!< Set member variable
        virtual void                  setValue(const std::string& name, RbObject* val);                             //!< Set member variable

        // Member method functions
        virtual const MethodTable&    getMethodInits(void) const;                                                   //!< Get method specifications
        virtual DAGNode*             executeOperation(const std::string& name, std::vector<DAGNode*>& args);

    
        // Tree specific methods
        virtual void                  buildRandomTree(int nTips, RandomNumberGenerator* rng) = 0;
        std::string                   getNewickTree(void) const { return newick; }
        int                           getNumberOfNodes(void) { return nodes.size(); }
        int                           getNumberOfTips(void) { return numberOfTips; }
        TreeNode*                     getRoot(void) { return root; }
        void                          randomlyBreakTree(RandomNumberGenerator* rng) {}
        void                          removeSubtreeFromNode(TreeNode* p) {}
        void                          setNumberOfTips(int x) { numberOfTips = x; }

    protected:
        void                          allocateNodes(int nNodes);
        void                          deleteNodes(void);
                                      Tree(void);
                                      Tree(const MemberRules& memberRules);
        std::string                   newick;                                                                       //!< The newick string, should be recalculated each time the tree has changed
        std::vector<TreeNode*>        nodes;
        int                           numberOfTips;
        TreeNode*                     root;
};

#endif


