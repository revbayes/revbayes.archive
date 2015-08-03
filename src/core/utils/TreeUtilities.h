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

#include "MatrixReal.h"
#include "TimeTree.h"
#include "Tree.h"
#include "TopologyNode.h"
#include <string>
#include <vector>

namespace RevBayesCore {

    namespace TreeUtilities {
    
        // these function are for public use
        TimeTree*       convertTree(const Tree &t);
//        AdmixtureTree*  convertToAdmixtureTree(const Tree &t, std::vector<std::string> names); // , const std::vector<std::string> names);
        void            rescaleSubtree(TimeTree *t, TopologyNode *n, double factor, bool v=false);
        void            rescaleTree(TimeTree *t, TopologyNode *n, double factor);
        void            getOldestTip(TimeTree* t, TopologyNode *n, double& oldest);
        void            getTaxaInSubtree(TopologyNode *n, std::vector<TopologyNode*> &taxa );
        std::string     uniqueNewickTopology(const Tree &t);
    
        // internal helper functions
        void            constructTimeTreeRecursively(TopologyNode *tn, const TopologyNode &n, std::vector<TopologyNode*> &nodes, std::vector<double> &ages, double depth);
//        void            constructAdmixtureTreeRecursively(AdmixtureNode *tn, const TopologyNode &n, std::vector<AdmixtureNode*> &nodes, std::vector<double> &ages);
        std::string     uniqueNewickTopologyRecursive(const TopologyNode &n);
		
		template<class treeType>
		MatrixReal* getDistanceMatrix(const treeType& tree);
		
		void processDistsInSubtree(const TopologyNode& node, MatrixReal& matrix, std::vector< std::pair<std::string, double> >& distsToNodeFather, const std::map< std::string, int >& namesToId);




    }
    
}


template<class treeType>
RevBayesCore::MatrixReal* RevBayesCore::TreeUtilities::getDistanceMatrix(const treeType& tree)
{
	
	RevBayesCore::MatrixReal* matrix = new RevBayesCore::MatrixReal( tree.getNumberOfTips() );
	
	std::vector<std::string> names = tree.getTipNames( ) ;
	
	
	
	std::map< std::string, int > namesToId;
	
	for(size_t i = 0; i < names.size(); ++i) {
		namesToId[ names[i] ] = i;
	}
	
	std::vector< std::pair<std::string, double> > distsToRoot;
	
	processDistsInSubtree( tree.getRoot() , *matrix, distsToRoot, namesToId);
	
	return matrix;
}



#endif
