//
//  TreePairwiseDistances.h
//  revbayes_mlandis
//
//  Created by Bastien Boussau on 22/07/15.
//  Based on a Bio++ function.
//  Copyright (c) 2015 Bastien Boussau. All rights reserved.
//

#ifndef __revbayes_bboussau__TreePairwiseDistances__
#define __revbayes_bboussau__TreePairwiseDistances__

#include "Tree.h"
#include "TreeUtilities.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"
#include "MatrixReal.h"

#include <vector>
#include <string>

namespace RevBayesCore {
    
    template<class treeType>
	class TreePairwiseDistances : public TypedFunction<RevBayesCore::MatrixReal> {
        
    public:
        TreePairwiseDistances(const TypedDagNode<treeType> *t);                                                                                   //!< Default constructor
        TreePairwiseDistances(const TreePairwiseDistances& t);                                                                                      //!< Copy constructor
        virtual                                    ~TreePairwiseDistances(void);                                                                  //!< Destructor
        
        TreePairwiseDistances&                        operator=(const TreePairwiseDistances& t);
        
        // Basic utility functions
        TreePairwiseDistances*                        clone(void) const;                                                                          //!< Clone object
        void                                        update(void);                                                                               //!< Clone the function
        
    protected:
        void                                        swapParameterInternal(const DagNode *oldP, const DagNode *newP);                            //!< Implementation of swaping parameters
        
    private:
	/*	void processDistsInSubtree(const TopologyNode& node, RevBayesCore::MatrixReal& matrix, std::vector< std::pair<std::string, double> >& distsToNodeFather, const std::map< std::string, int >& namesToId);*/
		RevBayesCore::MatrixReal* getDistanceMatrix(const TypedDagNode<treeType>& tree);
		

        // members
        const TypedDagNode<treeType>*               tree;
        
    };
    
}

//#include "TreePairwiseDistances.h"

using namespace RevBayesCore;

template<class treeType>
TreePairwiseDistances<treeType>::TreePairwiseDistances(const TypedDagNode<treeType> *t) : TypedFunction<RevBayesCore::MatrixReal>( new RevBayesCore::MatrixReal() ), tree( t ) {
    // add the tree parameter as a parent
    addParameter( tree );
    
    update();
}

template<class treeType>
TreePairwiseDistances<treeType>::TreePairwiseDistances(const TreePairwiseDistances &n) : TypedFunction<RevBayesCore::MatrixReal>( n ), tree( n.tree ) {
    // no need to add parameters, happens automatically
}

template<class treeType>
TreePairwiseDistances<treeType>::~TreePairwiseDistances( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}


template<class treeType>
TreePairwiseDistances<treeType>* TreePairwiseDistances<treeType>::clone( void ) const {
    return new TreePairwiseDistances<treeType>( *this );
}

/*
template<class treeType>
void TreePairwiseDistances<treeType>::processDistsInSubtree(const TopologyNode& node, RevBayesCore::MatrixReal& matrix, std::vector< std::pair<std::string, double> >& distsToNodeFather, const std::map< std::string, int >& namesToId)
{
	distsToNodeFather.clear();

	// node-is-leaf case
	if ( node.isTip() )
	{
		distsToNodeFather.push_back(make_pair ( node.getName(), node.getBranchLength() ) );
		return;
	}

	// For all leaves in node's subtree, get leaf-to-node distances.
	// Leaves are ordered according to the children of node.
	std::map< size_t, std::vector< std::pair<std::string, double> > > leavesDists;
	for (size_t i = 0; i < node.getNumberOfChildren(); ++i)
	{
		const TopologyNode* child = &( node.getChild(i) );
		processDistsInSubtree(*child, matrix, leavesDists[child->getIndex()], namesToId); // recursivity
	}
	
	// Write leaf-leaf distances to the distance matrix.
	// Only pairs in which the two leaves belong to different
	// children are considered.
	for (size_t son1_loc = 0; son1_loc < node.getNumberOfChildren(); ++son1_loc)
	{
		for (size_t son2_loc = 0; son2_loc < son1_loc; ++son2_loc)
		{
			const TopologyNode* son1 = &(node.getChild(son1_loc) );
			const TopologyNode* son2 = &(node.getChild(son2_loc) );
			
			for (std::vector< std::pair<std::string, double> >::iterator son1_leaf = leavesDists[son1->getIndex()].begin();
				 son1_leaf != leavesDists[son1->getIndex()].end();
				 ++son1_leaf)
			{
				for (std::vector< std::pair<std::string, double> >::iterator son2_leaf = leavesDists[son2->getIndex()].begin();
					 son2_leaf != leavesDists[son2->getIndex()].end();
					 ++son2_leaf)
				{
					int son1_leaf_id = namesToId.at( son1_leaf->first );
					int son2_leaf_id = namesToId.at( son2_leaf->first );
					matrix[son1_leaf_id] [son2_leaf_id] =
					matrix[son2_leaf_id] [son1_leaf_id] =
					( son1_leaf->second + son2_leaf->second );
				}
			}
		}
	}

	// node-is-root case
	if (node.isRoot())
	{
		// node-is-root-and-leaf case
		if (node.isTip() )
		{
			std::string root_name = node.getName();
			for (std::vector< std::pair<std::string, double> >::iterator other_leaf = leavesDists[node.getChild(0).getIndex()].begin();
				 other_leaf != leavesDists[node.getChild(0).getIndex()].end();
				 ++other_leaf)
			{
				matrix [ namesToId.at(root_name) ] [ namesToId.at(other_leaf->first) ]= matrix[ namesToId.at(other_leaf->first) ] [ namesToId.at(root_name) ] = other_leaf->second;
			}
		}

		return;
	}

	
	// Get distances from node's parent to considered leaves
	distsToNodeFather.clear();
	double nodeToFather = node.getBranchLength();

	for (std::map<const size_t, std::vector<std::pair<std::string, double> > >::iterator son = leavesDists.begin(); son != leavesDists.end(); ++son)
	{
		for (std::vector< std::pair<std::string, double> >::iterator leaf = (son->second).begin(); leaf != (son->second).end(); ++leaf)
		{
			distsToNodeFather.push_back(make_pair(leaf->first, (leaf->second + nodeToFather)));
		}
	}

}*/


template<class treeType>
RevBayesCore::MatrixReal* TreePairwiseDistances<treeType>::getDistanceMatrix(const TypedDagNode<treeType>& tree)
{

	RevBayesCore::MatrixReal* matrix = TreeUtilities::getDistanceMatrix ( tree.getValue() );
	
	/*new RevBayesCore::MatrixReal( tree.getValue().getNumberOfTips() );

	std::vector<std::string> names = tree.getValue().getTipNames( ) ;
	


	std::map< std::string, int > namesToId;

	for(size_t i = 0; i < names.size(); ++i) {
		namesToId[ names[i] ] = i;
	}

	std::vector< std::pair<std::string, double> > distsToRoot;

	processDistsInSubtree( tree.getValue().getRoot() , *matrix, distsToRoot, namesToId);*/

	return matrix;
}




template<class treeType>
void TreePairwiseDistances<treeType>::update( void ) {
	
	RevBayesCore::MatrixReal matrix =	*(getDistanceMatrix( *tree ) );
    *value = matrix;
}


template<class treeType>
void TreePairwiseDistances<treeType>::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    if (oldP == tree) {
        tree = static_cast<const TypedDagNode<treeType>* >( newP );
    }
}

#endif /* defined(__revbayes_bboussau__TreePairwiseDistances__) */
