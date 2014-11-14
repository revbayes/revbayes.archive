/**
 * @file
 * This file contains the declaration of the TreeSummary class. 
 * The tree summary summarizes the output from an mcmc run (a trace).
 *
 *
 * @brief Declaration of the TreeSummay class
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date:$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-07-17
 *
 * $Id:$
 */


#ifndef TreeSummary_H
#define TreeSummary_H

#include "Clade.h"
#include "ConditionalClade.h"
#include "NewickConverter.h"
#include "Sample.h"
#include "TreeTrace.h"
#include "TreeUtilities.h"
#include "AncestralStateTrace.h"

#include <algorithm>
#include <map>
#include <boost/lexical_cast.hpp>
#include <string>

namespace RevBayesCore {

    template<class treeType>
    class TreeSummary {
    
    public:
    
        TreeSummary(const TreeTrace<treeType> &t);
    
        treeType*                                                               map(int burnin=-1);
		treeType*                                                               ancestralStateTree(const treeType &inputTree, std::vector<AncestralStateTrace> &ancestralstate_traces, int burnin=-1);
        void                                                                    summarize(int burnin=-1);
        void                                                                    printTreeSummary(std::ostream& o);
        void                                                                    printCladeSummary(void);
    
    private:
        
        Clade                                                                   fillConditionalClades(const TopologyNode &n, std::vector<ConditionalClade> &cc, std::vector<Clade> &c);
        Sample<std::string>&                                                    findCladeSample(const std::string &n);
        
        size_t                                                                  burnin;
        TreeTrace<treeType>                                                     trace;
//        std::map<std::string, unsigned int>                                     treeFrequencies;
        std::vector<Sample<std::string> >                                       treeSamples;
        std::vector<Sample<std::string> >                                       cladeSamples;
        std::map<std::string, std::map<std::string, std::vector<double> > >     conditionalCladeFrequencies;
    };
	
    
    template <>
    inline BranchLengthTree* TreeSummary<BranchLengthTree>::map( int b )
    {
        summarize( b );

        std::string bestNewick = treeSamples.rbegin()->getValue();
        NewickConverter converter;
        BranchLengthTree* bestTree = converter.convertFromNewick( bestNewick );

        const std::vector<TopologyNode*> &nodes = bestTree->getNodes();
        std::vector<double> pp(nodes.size(),0.0);
        std::vector<std::vector<double> > branchLengths(nodes.size(),std::vector<double>());
        double weight = 1.0 / (trace.size()-burnin);

        for (size_t i = burnin; i < trace.size(); ++i)
        {
            BranchLengthTree *tree = trace.objectAt( i );
			
            const TopologyNode& root = tree->getRoot();
            for (size_t j = 0; j < nodes.size(); ++j)
            {
                if ( root.containsClade(nodes[j], true) )
                {
                    size_t cladeIndex = root.getCladeIndex( nodes[j] );
                    pp[j] += weight;
                    double bl = tree->getBranchLength( cladeIndex );
                    branchLengths[j].push_back(bl);
                }
            }
        }

        std::vector<double> meanBranchLengths;
        for (std::vector<std::vector<double> >::iterator it = branchLengths.begin(); it != branchLengths.end(); ++it)
        {
            double meanBranchLength = 0;
            const std::vector<double> &bl_samples = *it;
            for (std::vector<double>::const_iterator j = bl_samples.begin(); j != bl_samples.end(); ++j)
            {
                meanBranchLength += *j;
            }
            if ( bl_samples.size() > 0 )
            {
                meanBranchLength /= double(bl_samples.size());
            }
            meanBranchLengths.push_back( meanBranchLength );
        }
        
        for (size_t i = 0; i < nodes.size(); ++i)
        {
            bestTree->setBranchLength(i, meanBranchLengths[i]);
        }
        
        bestTree->addNodeParameter("posterior",pp,true);
        
        return bestTree;
    }
    
    template <>
    inline TimeTree* TreeSummary<TimeTree>::map( int b ) 
    {
        
        summarize( b );
        
        double sampleSize = trace.size() - burnin;
        
        double meanRootAge = 0.0;
        for (size_t i = burnin; i < trace.size(); ++i) 
        {
            // get the sampled tree
            TimeTree *tree = trace.objectAt( i );
            
            // add this root age to our variable
            meanRootAge += tree->getRoot().getAge();
            
        }
        
        std::string bestNewick = treeSamples.rbegin()->getValue();
        NewickConverter converter;
        BranchLengthTree* bestTree = converter.convertFromNewick( bestNewick );
        TimeTree* bestTimeTree = TreeUtilities::convertTree( *bestTree );
        size_t numTaxa = bestTree->getNumberOfTips();
        
        const std::vector<TopologyNode*> &nodes = bestTimeTree->getNodes();        
        for (size_t i = numTaxa; i < nodes.size(); ++i) 
        {
            TopologyNode* n = nodes[i];
            if ( !n->isTip() ) 
            {
                // first we compute the posterior probability of the clade
                std::vector<std::string> taxa;
                n->getTaxaStringVector(taxa);
                Clade c( taxa, 0.0 );

                double cladeFreq = findCladeSample( c.toString() ).getFrequency();
                double pp = cladeFreq / sampleSize;
                n->addNodeParameter("posterior",pp);
                
                // second we compute the conditional clade probability
                double ccp = 1.0;
                double age = 0.0;
                if ( !n->isRoot() ) 
                {
                    std::vector<std::string> parentTaxa;
                    n->getParent().getTaxaStringVector(parentTaxa);
                    Clade parent( parentTaxa, 0.0 );
                    std::map<std::string, std::vector<double> >& condCladeFreqs = conditionalCladeFrequencies[parent.toString()];
                    double parentCladeFreq = findCladeSample( parent.toString() ).getFrequency();
                    const std::vector<double>& condCladeSamples = condCladeFreqs[c.toString()];
                    size_t condCladeSampleSize = condCladeSamples.size();
                    ccp = condCladeSampleSize / parentCladeFreq;
                    
                    // finally, we compute the mean conditional age
                    for (size_t i = 0; i<condCladeSampleSize; ++i) 
                    {
                        age += condCladeSamples[i];
                    }
                    age /= condCladeSampleSize;

                }
                else
                {
                    age = meanRootAge / sampleSize;
                }
                n->addNodeParameter("ccp",ccp);
                
                // finally, we compute the mean conditional age
                bestTimeTree->setAge(i, age);
            }
            
        }
        
        return bestTimeTree;
    }
	
	template <>
	inline BranchLengthTree* RevBayesCore::TreeSummary<BranchLengthTree>::ancestralStateTree(const BranchLengthTree &inputTree, std::vector<AncestralStateTrace> &ancestralstate_traces, int b )
	{	
		// this method calculates the MAP ancestral character states for the nodes on the input_tree 
		
		if (b == -1) {
			burnin = trace.size() / 4;
		}
		else {
			burnin = size_t( b );
		}
		
		BranchLengthTree &finalInputTree = const_cast<BranchLengthTree&>(inputTree);
		std::vector<TopologyNode*> input_nodes = finalInputTree.getNodes();
		bool root_found = false;
		
		// 2-d vectors to keep the data (posteriors and states) of the inputTree nodes: [node][data]
		std::vector<std::vector<double> > pp(input_nodes.size(),std::vector<double>());
		std::vector<std::vector<std::string> > states(input_nodes.size(),std::vector<std::string>());
		
		double weight = 1.0 / (trace.size()-burnin);
		
		// loop through all trees in tree trace
		for (size_t i = burnin; i < trace.size(); i++)
		{
			BranchLengthTree *sample_tree = trace.objectAt( i );
			
			const TopologyNode& sample_root = sample_tree->getRoot();
			
			if (!root_found) {
				// root the inputTree to the same node as the sample trees
				// so we can compare the two trees
				for (size_t j = 0; j < input_nodes.size(); j++) {
					
					// reroot a copy of the inputTree until we find the sample_root node
					BranchLengthTree* finalInputTreeCopy = new BranchLengthTree(finalInputTree);
					std::vector<TopologyNode*> input_nodes_copy = finalInputTreeCopy->getNodes();
					finalInputTreeCopy->reroot( *(input_nodes_copy[j]) );
					
					// check to see if the two roots have the same children
					std::vector<TopologyNode*> inputChildren = finalInputTreeCopy->getRoot().getChildren();
					std::vector<TopologyNode*> sampleChildren = sample_root.getChildren();
					// loop thru all the children of the inputTreeCopy root
					bool children_found = true;
					for (size_t k = 0; k < inputChildren.size(); k++) {
						bool child_found = false;
						// loop thru all the children of the sampleTree root
						for (size_t l = 0; l < sampleChildren.size(); l++) {
							if ( inputChildren[k]->containsClade(sampleChildren[l], true) ) {
								child_found = true;
								break;
							}
						}
						if (!child_found) {
							children_found = false;
							break;
						}
					}
					if (children_found) {
						// finally reroot the original inputTree
						finalInputTree.reroot( *(input_nodes[j]) );
						input_nodes = finalInputTree.getNodes();
						root_found = true;
						break;
					}
				}
				if (!root_found) {
					throw RbException("The input tree topology does not match the rooting constraints found in the AncestralStateTreeTrace.");
				}
			}
			
			// loop through all nodes in inputTree
			for (size_t j = 0; j < input_nodes.size(); j++)
			{
				if ( sample_root.containsClade(input_nodes[j], true) && !input_nodes[j]->isTip() )
				{
					// if the inputTree node is also in the sample tree
					// we get the ancestral character state from the ancestral state trace
					size_t sampleCladeIndex = sample_root.getCladeIndex( input_nodes[j] ); 
					size_t inputCladeIndex = finalInputTree.getRoot().getCladeIndex( input_nodes[j] );
					
					// get AncestralStateTrace for this node
					AncestralStateTrace ancestralstate_trace;
					for (size_t k = 0; k < ancestralstate_traces.size(); k++) {
						if (ancestralstate_traces[k].getParameterName() == boost::lexical_cast<std::string>(sampleCladeIndex)) {
							ancestralstate_trace = ancestralstate_traces[k];
							break;
						}
					}
					
					// get ancestral state vector for this iteration
					std::vector<std::string> ancestralstate_vector = ancestralstate_trace.getValues();
					std::string ancestralstate = ancestralstate_vector[i];
					
					bool state_found = false;
					int k = 0;
					for (; k < pp[inputCladeIndex].size(); k++) {
						if (states[inputCladeIndex][k] == ancestralstate) {
							state_found = true;
							break;
						}
					}
					// update the pp and states vectors
					if (!state_found) {
						pp[inputCladeIndex].push_back(weight);
						states[inputCladeIndex].push_back(ancestralstate);
					} else {
						pp[inputCladeIndex][k] += weight;
					}
				}
			}
		}
		// find the 3 most probable ancestral states for each node and add them to the tree as parameters
		std::vector<std::string*> best_states;
		std::vector<double> posteriors;
		for (int i = 0; i < input_nodes.size(); i++) {
			
			if ( input_nodes[i]->isTip() ) {
				
				std::string *s = new std::string("{}");
				best_states.push_back(s);
				posteriors.push_back(1.0);
				
			} else {

				double state1_pp = 0.0; 
				double state2_pp = 0.0; 
				double state3_pp = 0.0;
				double other_pp = 0.0;
				double total_node_pp = 0.0;
				
				std::string state1 = ""; 
				std::string state2 = ""; 
				std::string state3 = "";
				
				// loop through all states for this node
				for (int j = 0; j < pp[i].size(); j++) {
					total_node_pp += pp[i][j];
					if (pp[i][j] > state1_pp) {
						state3_pp = state2_pp;
						state2_pp = state1_pp;
						state1_pp = pp[i][j];
						state3 = state2;
						state2 = state1;
						state1 = states[i][j];
					} else if (pp[i][j] > state2_pp) {
						state3_pp = state2_pp;
						state2_pp = pp[i][j];
						state3 = state2;
						state2 = states[i][j];
					} else if (pp[i][j] > state3_pp) {
						state3_pp = pp[i][j];
						state3 = states[i][j];
					}
				}
				
				posteriors.push_back(total_node_pp);
				
				std::string final_state = "{" + state1 + "=" + boost::lexical_cast<std::string>(state1_pp+0.0000001).substr(0,6);
				if (state2_pp > 0.0001) {
					final_state += "," + state2 + "=" + boost::lexical_cast<std::string>(state2_pp+0.0000001).substr(0,6);
				}
				if (state3_pp > 0.0001) {
					final_state += "," + state3 + "=" + boost::lexical_cast<std::string>(state3_pp+0.0000001).substr(0,6);
				}
				other_pp = total_node_pp - (state1_pp+state2_pp+state3_pp);
				if (other_pp > 0.0001) {
					final_state += ",other=" + boost::lexical_cast<std::string>(other_pp+0.0000001).substr(0,6);
				} 
				if (1.0-total_node_pp > 0.0001) {
					final_state += ",node_doesnt_exist=" + boost::lexical_cast<std::string>(1.0-total_node_pp+0.0000001).substr(0,6);
				} 
				final_state += "}";
				
				// make parameter string for this node
				std::string *s = new std::string(final_state);
				best_states.push_back(s);
			}
		}
		finalInputTree.clearNodeParameters();
		finalInputTree.addNodeParameter("posterior",posteriors,true);
		finalInputTree.addNodeParameter("ancestralstates",best_states,true);
		return &finalInputTree;
	}

	
}

#include "StringUtilities.h"
#include "TopologyNode.h"

#include <iomanip>
#include <vector>

template <class treeType>
RevBayesCore::TreeSummary<treeType>::TreeSummary( const TreeTrace<treeType> &t) : 
    burnin( 0 ), 
    trace( t ) 
{

}


template <class treeType>
treeType* RevBayesCore::TreeSummary<treeType>::ancestralStateTree(const treeType &inputTree, std::vector<AncestralStateTrace> &ancestralstate_traces, int b )
{	
	// this method calculates the MAP ancestral character states for the nodes on the input_tree 
	
	if (b == -1) {
        burnin = trace.size() / 4;
    }
    else {
        burnin = size_t( b );
    }
	
	treeType &finalInputTree = const_cast<treeType&>(inputTree);
	const std::vector<TopologyNode*> &input_nodes = finalInputTree.getNodes();
	
	// 2-d vectors to keep the data (posteriors and states) of the inputTree nodes: [node][data]
	std::vector<std::vector<double> > pp(input_nodes.size(),std::vector<double>());
	std::vector<std::vector<std::string> > states(input_nodes.size(),std::vector<std::string>());
	
	double weight = 1.0 / (trace.size()-burnin);
	
	// loop through all trees in tree trace
	for (size_t i = burnin; i < trace.size(); i++)
	{
		treeType *sample_tree = trace.objectAt( i );
		const TopologyNode& sample_root = sample_tree->getRoot();
		
		// loop through all nodes in inputTree
		for (size_t j = 0; j < input_nodes.size(); j++)
		{
			if ( sample_root.containsClade(input_nodes[j], true) && !input_nodes[j]->isTip() )
			{
				// if the inputTree node is also in the sample tree
				// we get the ancestral character state from the ancestral state trace
				size_t sampleCladeIndex = sample_root.getCladeIndex( input_nodes[j] );
				
				// get AncestralStateTrace for this node
				AncestralStateTrace ancestralstate_trace;
				for (size_t k = 0; k < ancestralstate_traces.size(); k++) {
					if (ancestralstate_traces[k].getParameterName() == boost::lexical_cast<std::string>(sampleCladeIndex)) {
						ancestralstate_trace = ancestralstate_traces[k];
						break;
					}
				}
				
				// get ancestral state vector for this iteration
				std::vector<std::string> ancestralstate_vector = ancestralstate_trace.getValues();
				std::string ancestralstate = ancestralstate_vector[i];
				
				bool state_found = false;
				int k = 0;
				for (; k < pp[j].size(); k++) {
					if (states[j][k] == ancestralstate) {
						state_found = true;
						break;
					}
				}
				// update the pp and states vectors
				if (!state_found) {
					pp[j].push_back(weight);
					states[j].push_back(ancestralstate);
				} else {
					pp[j][k] += weight;
				}
			}
		}
	}
	// find the 3 most probable ancestral states for each node and add them to the tree as parameters
	std::vector<std::string*> best_states;
	std::vector<double> posteriors;
	for (int i = 0; i < input_nodes.size(); i++) {
		
		if ( input_nodes[i]->isTip() ) {
			
			std::string *s = new std::string("{}");
			best_states.push_back(s);
			posteriors.push_back(1.0);
			
		} else {
			
			double state1_pp = 0.0; 
			double state2_pp = 0.0; 
			double state3_pp = 0.0;
			double other_pp = 0.0;
			double total_node_pp = 0.0;
			
			std::string state1 = ""; 
			std::string state2 = ""; 
			std::string state3 = "";
			
			// loop through all states for this node
			for (int j = 0; j < pp[i].size(); j++) {
				total_node_pp += pp[i][j];
				if (pp[i][j] > state1_pp) {
					state3_pp = state2_pp;
					state2_pp = state1_pp;
					state1_pp = pp[i][j];
					state3 = state2;
					state2 = state1;
					state1 = states[i][j];
				} else if (pp[i][j] > state2_pp) {
					state3_pp = state2_pp;
					state2_pp = pp[i][j];
					state3 = state2;
					state2 = states[i][j];
				} else if (pp[i][j] > state3_pp) {
					state3_pp = pp[i][j];
					state3 = states[i][j];
				}
			}
			
			posteriors.push_back(total_node_pp);
			
			std::string final_state = "{" + state1 + "=" + boost::lexical_cast<std::string>(state1_pp+0.0000001).substr(0,6);
			if (state2_pp > 0.0001) {
				final_state += "," + state2 + "=" + boost::lexical_cast<std::string>(state2_pp+0.0000001).substr(0,6);
			}
			if (state3_pp > 0.0001) {
				final_state += "," + state3 + "=" + boost::lexical_cast<std::string>(state3_pp+0.0000001).substr(0,6);
			}
			other_pp = total_node_pp - (state1_pp+state2_pp+state3_pp);
			if (other_pp > 0.0001) {
				final_state += ",other=" + boost::lexical_cast<std::string>(other_pp+0.0000001).substr(0,6);
			} 
			if (1.0-total_node_pp > 0.0001) {
				final_state += ",node_doesnt_exist=" + boost::lexical_cast<std::string>(1.0-total_node_pp+0.0000001).substr(0,6);
			} 
			final_state += "}";
			
			// make parameter string for this node
			std::string *s = new std::string(final_state);
			best_states.push_back(s);
		}
	}
	finalInputTree.clearNodeParameters();
	finalInputTree.addNodeParameter("posterior",posteriors,true);
	finalInputTree.addNodeParameter("ancestralstates",best_states,true);
	return &finalInputTree;
}


/**
 *
 *
 */
template <class treeType>
RevBayesCore::Clade RevBayesCore::TreeSummary<treeType>::fillConditionalClades(const RevBayesCore::TopologyNode &n, std::vector<RevBayesCore::ConditionalClade> &condClades, std::vector<RevBayesCore::Clade> &clades)
{
 
    std::vector<std::string> taxa;
    n.getTaxaStringVector( taxa );
    Clade parent( taxa, n.getAge() );
    clades.push_back( parent );
    
    const TopologyNode &leftChild = n.getChild( 0 );
    const TopologyNode &rightChild = n.getChild( 1 );
    
    // only add non tip clades
    if ( !leftChild.isTip() )
    {
        Clade leftChildClade = fillConditionalClades(leftChild, condClades, clades);
        ConditionalClade cc = ConditionalClade(parent,leftChildClade);
        condClades.push_back( cc );
    }
    if ( !rightChild.isTip() )
    {
        Clade rightChildClade = fillConditionalClades(rightChild, condClades, clades);
        ConditionalClade cc = ConditionalClade(parent,rightChildClade);
        condClades.push_back( cc );
    }
    
    return parent;
}


template <class treeType>
RevBayesCore::Sample<std::string>& RevBayesCore::TreeSummary<treeType>::findCladeSample(const std::string &n)
{
    
    for (std::vector<Sample<std::string> >::iterator it=cladeSamples.begin(); it!= cladeSamples.end(); ++it)
    {
        if ( it->getValue() == n )
        {
            return *it;
        }
        
    }
    
    throw RbException("Couldn't find a clade with name '" + n + "'.");
}




template <class treeType>
void RevBayesCore::TreeSummary<treeType>::summarize( int b ) 
{
    
    std::map<std::string, Sample<std::string> > treeAbsencePresence;
    std::map<std::string, Sample<std::string> > cladeAbsencePresence;
    
    if (b == -1) 
    {
        burnin = trace.size() / 4;
    }
    else 
    {
        burnin = size_t( b );
    }
    
    for (size_t i = burnin; i < trace.size(); ++i) 
    {
        treeType *tree = trace.objectAt( i );
        std::string newick = TreeUtilities::uniqueNewickTopology( *tree );
        const std::map<std::string, Sample<std::string> >::iterator& entry = treeAbsencePresence.find( newick );
        if ( entry == treeAbsencePresence.end() )
        {
            Sample<std::string> treeSample = Sample<std::string>(newick,0);
            if ( i > burnin )
            {
                treeSample.setTrace( std::vector<double>(i - burnin,0.0) );
            }
            else
            {
                treeSample.setTrace( std::vector<double>() );
            }
            treeAbsencePresence.insert( std::pair<std::string, Sample<std::string> >(newick, treeSample));
        }
        
        for (std::map<std::string, Sample<std::string> >::iterator it=treeAbsencePresence.begin(); it!=treeAbsencePresence.end(); ++it )
        {
            
            if ( it->first == newick )
            {
                it->second.addObservation( true );
            }
            else
            {
                it->second.addObservation( false );
            }
            
        }
        
        // get the conditional clades for this
        std::vector<ConditionalClade> condClades;
        std::vector<Clade> clades;
        fillConditionalClades(tree->getRoot(), condClades, clades);
        
        
        // first increment the clade frequency counter
        // there need to be two loops because otherwise we count the the parent clade twice
        for (size_t i = 0; i < clades.size(); ++i)
        {
            const Clade & c = clades[i];
            std::string parentString = c.toString();
            const std::map<std::string, Sample<std::string> >::iterator& entry = cladeAbsencePresence.find( newick );
            if ( entry == cladeAbsencePresence.end() )
            {
                Sample<std::string> cladeSample = Sample<std::string>(parentString,0);
                if ( i > burnin )
                {
                    cladeSample.setTrace( std::vector<double>(i - burnin,0.0) );
                }
                else
                {
                    cladeSample.setTrace( std::vector<double>() );
                }
                cladeAbsencePresence.insert( std::pair<std::string, Sample<std::string> >(parentString, cladeSample));
                
                conditionalCladeFrequencies.insert( std::pair<std::string, std::map<std::string, std::vector<double> > >(parentString, std::map<std::string, std::vector<double> >()) );

            }
            
        }
        
        
        
        for (std::map<std::string, Sample<std::string> >::iterator it=cladeAbsencePresence.begin(); it!=cladeAbsencePresence.end(); ++it )
        {
            bool found = false;
            for (size_t i = 0; i < clades.size(); ++i)
            {
                std::string c = clades[i].toString();
                if ( it->first == c )
                {
                    found = true;
                    break;
                }
                
            }
            
            if ( found == true )
            {
                it->second.addObservation( true );
            }
            else
            {
                it->second.addObservation( false );
            }
            
        }
        
        for (size_t i = 0; i < condClades.size(); ++i)
        {
            const ConditionalClade & cc = condClades[i];
            const Clade &parent = cc.getParent();
            const Clade &child  = cc.getChild();
            
            // now increment the conditional clade frequency counter
            std::string parentString = parent.toString();
            std::string childString = child.toString();
            double childAge = child.getAge();
            std::map<std::string, std::vector<double> >& parentEntry = conditionalCladeFrequencies.find( parentString )->second;
            const std::map<std::string, std::vector<double> >::iterator& childEntry = parentEntry.find( childString );
            if ( childEntry == parentEntry.end() )
            {
                parentEntry.insert( std::pair<std::string, std::vector<double> >(childString, std::vector<double>(1,childAge) ));
            }
            else
            {
                std::vector<double> &samples = childEntry->second;
                samples.push_back( childAge );
            }
            
        }
    }
    
    // collect the samples
    treeSamples.clear();
    for (std::map<std::string, Sample<std::string> >::iterator it = treeAbsencePresence.begin(); it != treeAbsencePresence.end(); ++it)
    {
        it->second.computeStatistics();
        treeSamples.push_back( it->second );
    }
    
    // sort the samples by frequency
    sort(treeSamples.begin(), treeSamples.end());
    
    
    // collect the samples
    cladeSamples.clear();
    for (std::map<std::string, Sample<std::string> >::iterator it = cladeAbsencePresence.begin(); it != cladeAbsencePresence.end(); ++it)
    {
        it->second.computeStatistics();
        cladeSamples.push_back( it->second );
    }
    
    // sort the samples by frequency
    sort(cladeSamples.begin(), cladeSamples.end());
}



template <class treeType>
void RevBayesCore::TreeSummary<treeType>::printTreeSummary(std::ostream &o)
{
    
    std::stringstream ss;
    ss << std::fixed;
    ss << std::setprecision(4);

    o << "========================================" << std::endl;
    o << "Printing Posterior Distribution of Trees" << std::endl;
    o << "========================================" << std::endl;
    
    // now the printing
    std::string s = "Samples";
    StringUtilities::fillWithSpaces(s, 16, true);
    o << "\n" << s;
    s = "Posterior";
    StringUtilities::fillWithSpaces(s, 16, true);
    o << s;
    s = "ESS";
    StringUtilities::fillWithSpaces(s, 16, true);
    o << s;
    s = "Tree";
    StringUtilities::fillWithSpaces(s, 16, true);
    o << s;
    o << std::endl;
    o << "----------------------------------------------------------------" << std::endl;
    double totalSamples = trace.size();
    for (std::vector<Sample<std::string> >::reverse_iterator it = treeSamples.rbegin(); it != treeSamples.rend(); ++it)
    {
        ss.str(std::string());
        ss << it->getFrequency();
        s = ss.str();
        StringUtilities::fillWithSpaces(s, 16, true);
        o << s;
        
        ss.str(std::string());
        ss << it->getFrequency()/(totalSamples-burnin);
        s = ss.str();
        StringUtilities::fillWithSpaces(s, 16, true);
        o << s;
        
        ss.str(std::string());
        ss << it->getEss();
        s = ss.str();
        StringUtilities::fillWithSpaces(s, 16, true);
        o << s;
        
        o << it->getValue();
        o << std::endl;
        
    }
    
    
    o << "\n\n\n=========================================" << std::endl;
    o << "Printing Posterior Distribution of Clades" << std::endl;
    o << "=========================================" << std::endl;
    
    
    
    // now the printing
    s = "Samples";
    StringUtilities::fillWithSpaces(s, 16, true);
    o << "\n" << s;
    s = "Posterior";
    StringUtilities::fillWithSpaces(s, 16, true);
    o << s;
    s = "ESS";
    StringUtilities::fillWithSpaces(s, 16, true);
    o << s;
    s = "Clade";
    StringUtilities::fillWithSpaces(s, 16, true);
    o << s;
    o << std::endl;
    o << "--------------------------------------------------------------" << std::endl;
    for (std::vector<Sample<std::string> >::reverse_iterator it = cladeSamples.rbegin(); it != cladeSamples.rend(); ++it)
    {
        
        ss.str(std::string());
        ss << it->getFrequency();
        s = ss.str();
        StringUtilities::fillWithSpaces(s, 16, true);
        o << s;
        
        ss.str(std::string());
        ss << it->getFrequency()/(totalSamples-burnin);
        s = ss.str();
        StringUtilities::fillWithSpaces(s, 16, true);
        o << s;
        
        ss.str(std::string());
        if ( it->getFrequency() <  totalSamples && it->getFrequency() > 0 )
        {
            ss << it->getEss();
        }
        else
        {
            ss << " - ";
            
        }
        s = ss.str();
        StringUtilities::fillWithSpaces(s, 16, true);
        o << s;
        
        o << it->getValue();
        o << std::endl;
    }

}


#endif
