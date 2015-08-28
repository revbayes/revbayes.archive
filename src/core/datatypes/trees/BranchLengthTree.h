/**
 * @file
 * This file contains the declaration of a time calibrated tree, a light-weight class
 * that holds the pointer to the root node of a tree and provides some convinience functions.
 *
 * @brief Declaration of the BranchLengthTree
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-07-05 16:47:08 +0200 (Thu, 05 Jul 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-04-13, version 1.0
 *
 * $Id: BranchLengthTree.h 1651 2012-07-05 14:47:08Z hoehna $
 */

#ifndef BranchLengthTree_H
#define BranchLengthTree_H

#include "Tree.h"

#include <vector>
#include <string>

namespace RevBayesCore {
    
    class Topology;
    
    class BranchLengthTree : public Tree {
        
    public:
        BranchLengthTree(void);                                                                                             //!< Default constructor
        virtual                                    ~BranchLengthTree(void);                                                                                                        //!< Destructor
        
        // Basic utility functions
        virtual BranchLengthTree*                   clone(void) const;                                                      //!< Clone object
        
        // BranchLengthTree functions
        double                                      getAge(size_t idx) const;
        double                                      getBranchLength(size_t idx) const;
        double                                      getTime(size_t idx) const;
        double                                      getTreeLength(void) const;
        void                                        reroot(const std::string &outgroup);    //!< Re-root the tree with the given outgroup
		void                                        reroot(TopologyNode &n); 
        void                                        setBranchLength(size_t idx, double bl);
        
    protected:
        void                                        resizeElementVectors(size_t n);
        void                                        reverseParentChild(TopologyNode &n);                                    //!< Reverse the parent child relationship.
        
    private:
        std::vector<double>                         branchLengths;
        
    };
    

    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const BranchLengthTree& x);                                         //!< Overloaded output operator
}

#endif

