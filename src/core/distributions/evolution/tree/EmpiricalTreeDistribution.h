 /**
 * @file
 * This file contains the declaration and implementation
 * of the EmpiricalTreeDistribution class. The distribution 
 * is constructed from an input tree trace.
 * 
 *
 * @brief Declaration and implementation of the EmpiricalTreeDistribution class
 *
 * (c) Copyright 2014-
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0
 *
 */

#ifndef EmpiricalTreeDistribution_H
#define EmpiricalTreeDistribution_H

#include "TreeTrace.h"
#include "TreeSummary.h"
#include "Sample.h"
#include "NewickConverter.h"

namespace RevBayesCore {
    
    template<typename treeType>
    class EmpiricalTreeDistribution : public TypedDistribution<treeType> {
        
    public:
		
		EmpiricalTreeDistribution(const TreeTrace<treeType> &t, int& b );
		
		virtual                                             ~EmpiricalTreeDistribution(void); 

		EmpiricalTreeDistribution<treeType>*                clone(void) const;                                          //!< Create an independent clone
		double                                              computeLnProbability(void);                                 //!< Compute ln prob of current value
		void                                                redrawValue(void);                                          //!< Draw a new random value from distribution
		
	protected:

		void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter
		
	private:
		
		double                                              probability;
		size_t                                              burnin;
		TreeTrace<treeType>                                 trace;
		TreeSummary<treeType>                               summary;
		
	};
	
	
	template <>
	inline void RevBayesCore::EmpiricalTreeDistribution<BranchLengthTree>::redrawValue( void ) {
		
		// get a random probability
		RandomNumberGenerator* rng = GLOBAL_RNG;
		double rand_prob = rng->uniform01();
		
		// get all the topology samples in the tree summary
		std::vector<Sample<std::string> > topo_samples = summary.getTreeSamples();
		size_t total_tree_samples = trace.size();
		size_t topo_index = 0;
		unsigned int frequency = 0;
		std::string newick = "";
		std::string outgroup = "";
		
		// loop through each topology sample
		while ( true ) {
			rand_prob -= topo_samples[topo_index].getFrequency() / (double)(total_tree_samples - burnin);
			if ( rand_prob > 0.0 ) {
				topo_index++;
				if ( topo_index == topo_samples.size() ) {
					topo_index = 0;
				}
			} else {
				// draw the current topology and calculate its probability
				newick = topo_samples[topo_index].getValue();
				RevBayesCore::NewickConverter c;
				// for BranchLengthTrees we need to reroot to compare topologies
				BranchLengthTree* tau = c.convertFromNewick( newick );
				outgroup = tau->getTipNode(0).getName();
				frequency = topo_samples[topo_index].getFrequency();
				probability = topo_samples[topo_index].getFrequency() / (double)(total_tree_samples - burnin);
				break;
			}
			
		}
		
		// find all the trees with the selected topology
		std::vector<size_t> trace_indexes;
		bool found = false;
		for (size_t i = burnin; i < total_tree_samples; i++) {
			BranchLengthTree tree = *trace.objectAt(i);
			tree.reroot( outgroup );
			if (newick == TreeUtilities::uniqueNewickTopology(tree)) {
				trace_indexes.push_back(i);
				found = true;
			}
		}
		if (!found)
		    throw RbException("EmpiricalTreeDistribution: error during redrawValue - could not find randomly drawn tree.");
		
		// now draw one of the trees with the selected topology
		if (trace_indexes.size() == 1) {
			this->value = new BranchLengthTree(*trace.objectAt(trace_indexes[0]));
		} else { 
			size_t random_tree = (size_t)( rng->uniform01() * (trace_indexes.size()-1) );
			this->value = new BranchLengthTree(*trace.objectAt(trace_indexes[random_tree]));
		}
	}
	
} 



#include "Tree.h"
#include "TreeTrace.h"
#include "TreeSummary.h"
#include "Sample.h"

using namespace RevBayesCore;



template <typename treeType>
RevBayesCore::EmpiricalTreeDistribution<treeType>::EmpiricalTreeDistribution(const TreeTrace<treeType> &t, int &b)
:  TypedDistribution<treeType>( new treeType() ),
probability( 0.0 ),
trace( t ),
burnin( b ),
summary( RevBayesCore::TreeSummary<treeType>( t ) )
{
	// make sure burnin is proper
	if (trace.size() <= burnin) {
		throw RbException("Burnin size is too large for the trace.");
	}
	if (burnin == -1) {
		burnin = trace.size() / 4;
	} else {
		burnin = (size_t)(b);
	}
	
	// summarize trees and draw the first value
	summary.summarizeTrees( b );
	redrawValue();
}


template <typename treeType>
RevBayesCore::EmpiricalTreeDistribution<treeType>::~EmpiricalTreeDistribution()
{
    
}


template <typename treeType>
RevBayesCore::EmpiricalTreeDistribution<treeType>* RevBayesCore::EmpiricalTreeDistribution<treeType>::clone( void ) const {
    
    return new RevBayesCore::EmpiricalTreeDistribution<treeType>( *this );
}


template <typename treeType>
double RevBayesCore::EmpiricalTreeDistribution<treeType>::computeLnProbability( void ) {
	
    return log( probability );
	
}


template <typename treeType>
void RevBayesCore::EmpiricalTreeDistribution<treeType>::redrawValue( void ) {
    
	// get a random probability
	RandomNumberGenerator* rng = GLOBAL_RNG;
	double rand_prob = rng->uniform01();
	
	// get all the topology samples in the tree summary
	std::vector<Sample<std::string> > topo_samples = summary.getTreeSamples();
	size_t total_tree_samples = trace.size();
	size_t topo_index = 0;
	unsigned int frequency = 0;
	std::string newick = "";
	
	// loop through each topology sample
	while ( true ) {
		rand_prob -= topo_samples[topo_index].getFrequency() / (double)(total_tree_samples - burnin);
		if ( rand_prob > 0.0 ) {
			topo_index++;
			if ( topo_index == topo_samples.size() ) {
				topo_index = 0;
			}
		} else {
			// draw the current topology and calculate its probability
			newick = topo_samples[topo_index].getValue();
			frequency = topo_samples[topo_index].getFrequency();
			probability = topo_samples[topo_index].getFrequency() / (double)(total_tree_samples - burnin);
			break;
		}
		
	}
	
	// find all the trees with the selected topology
	std::vector<size_t> trace_indexes;
	for (size_t i = burnin; i < total_tree_samples; i++) {
		treeType* tree = trace.objectAt(i);
		if (newick == TreeUtilities::uniqueNewickTopology(*tree))
			trace_indexes.push_back(i);
	}
	
	// now draw one of the trees with the selected topology
	if (trace_indexes.size() == 1) {
		this->value = new treeType(*trace.objectAt(trace_indexes[0]));
	} else { 
		size_t random_tree = (size_t)( rng->uniform01() * (trace_indexes.size()-1) );
		this->value = new treeType(*trace.objectAt(trace_indexes[random_tree]));
	}
}


template <typename treeType>
void RevBayesCore::EmpiricalTreeDistribution<treeType>::swapParameterInternal( const DagNode *oldP, const DagNode *newP )
{
}

#endif