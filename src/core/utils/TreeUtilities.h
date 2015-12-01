/**
 * @file
 * This file contains helper functions for manipulating trees in RevBayes.
 *
 * @brief Namespace containing helper functions for trees
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-07-05 16:47:08 +0200 (Thu, 05 Jul 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: RlStringUtilities.h 1651 2012-07-05 14:47:08Z hoehna $
 */

#ifndef TreeUtilities_H
#define TreeUtilities_H

#include "DistanceMatrix.h"
#include "MatrixReal.h"
#include "Tree.h"
#include "Tree.h"
#include "TopologyNode.h"
#include <string>
#include <vector>

namespace RevBayesCore {

    namespace TreeUtilities {
    
        // these function are for public use
        Tree*           convertTree(const Tree &t);
//        AdmixtureTree*  convertToAdmixtureTree(const Tree &t, std::vector<std::string> names); // , const std::vector<std::string> names);
        DistanceMatrix* getDistanceMatrix(const Tree& tree);
        void            getOldestTip(Tree* t, TopologyNode *n, double& oldest);
        void            getTaxaInSubtree(TopologyNode *n, std::vector<TopologyNode*> &taxa );
        void            rescaleSubtree(Tree *t, TopologyNode *n, double factor, bool v=false);
        void            rescaleTree(Tree *t, TopologyNode *n, double factor);
        std::string     uniqueNewickTopology(const Tree &t);
    
        // internal helper functions
        void            constructTimeTreeRecursively(TopologyNode *tn, const TopologyNode &n, std::vector<TopologyNode*> &nodes, std::vector<double> &ages, double depth);
//        void            constructAdmixtureTreeRecursively(AdmixtureNode *tn, const TopologyNode &n, std::vector<AdmixtureNode*> &nodes, std::vector<double> &ages);
        std::string     uniqueNewickTopologyRecursive(const TopologyNode &n);
		
		
		void            processDistsInSubtree(const TopologyNode& node, MatrixReal& matrix, std::vector< std::pair<std::string, double> >& distsToNodeFather, const std::map< std::string, int >& namesToId);




    }
    
}


#endif
