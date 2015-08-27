/**
 * @file
 * This file contains the declaration of the TreeBipartitions statistic, which simply computes
 * all bipartitions of a tree.
 *
 * @brief Declaration of the TreeBipartitions
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-07-05 16:47:08 +0200 (Thu, 05 Jul 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-04-13, version 1.0
 *
 * $Id: TreeBipartitions.h 1651 2012-07-05 14:47:08Z hoehna $
 */

#ifndef TreeBipartitions_H
#define TreeBipartitions_H

//#include "Statistic.h"
#include "TimeTree.h"
#include "Tree.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <boost/dynamic_bitset.hpp>


#include <vector>
#include <string>

namespace RevBayesCore {
    
    class TreeBipartitions : public TypedFunction< RbVector<boost::dynamic_bitset<> > > {
        
    public:
        TreeBipartitions(const TypedDagNode<TimeTree> *t);                                            //!< Default constructor
        virtual      ~TreeBipartitions(void);                                                         //!< Destructor
        
        // Basic utility functions
        TreeBipartitions*                           clone(void) const;                                   //!< Clone object
        void                                        update(void);                                     //!< Clone the function
        const std::vector<double>&                  getBipartitionAges(void);
        
    protected:
        void swapParameterInternal(const DagNode *oldP, const DagNode *newP);                         //!< Implementation of swaping parameters
        
    private:
        // members
        const TypedDagNode<TimeTree>*               tree;
        std::vector<boost::dynamic_bitset<> >       bipartitions;
        std::map<boost::dynamic_bitset<>, const TopologyNode*> bipartitionMap;
        std::vector<double> bipartitionAges;
        
        void computeBipartitions();
        void computeBipartitions(const TopologyNode* node, std::map <const TopologyNode*, unsigned long>& nodeToBitVectorIndex, const std::map<std::string, size_t>& nameToIndex );

    };
    
}

#endif

