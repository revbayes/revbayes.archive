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

namespace RevBayesCore {

    template<class treeType>
    class TreeSummary {
    
    public:
    
        TreeSummary(const TreeTrace<treeType> &t);
    
        treeType*                                                               map(int burnin=-1);
		treeType*                                                               mapAncestralState(std::vector<AncestralStateTrace> &ancestralstate_traces, int burnin=-1);
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
            BranchLengthTree& tree = trace.objectAt( i );
            const TopologyNode& root = tree.getRoot();
            for (size_t j = 0; j < nodes.size(); ++j)
            {
                if ( root.containsClade(nodes[j], true) )
                {
                    size_t cladeIndex = root.getCladeIndex( nodes[j] );
                    pp[j] += weight;
                    double bl = tree.getBranchLength( cladeIndex );
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
            TimeTree& tree = trace.objectAt( i );
            
            // add this root age to our variable
            meanRootAge += tree.getRoot().getAge();
            
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
treeType* RevBayesCore::TreeSummary<treeType>::mapAncestralState(std::vector<AncestralStateTrace> &ancestralstate_traces, int b )
{	
	treeType* mapTree = map( b );
	
	const std::vector<TopologyNode*> &nodes = mapTree->getNodes();
	
	// 2-d vectors [node][ancestral_state]
	std::vector<std::vector<double> > pp(nodes.size(),std::vector<double>());
	std::vector<std::vector<std::string> > states(nodes.size(),std::vector<std::string>());
	
	double weight = 1.0 / (trace.size()-burnin);
	
	// loop through all trees in tree trace
	for (size_t i = burnin; i < trace.size(); i++)
	{
		treeType& tree = trace.objectAt( i );
		const TopologyNode& root = tree.getRoot();
		// loop through all nodes in MAP tree
		for (size_t j = 0; j < nodes.size(); j++)
		{
			
			if ( root.containsClade(nodes[j], true) )
			{
				
				// if the node in the MAP tree is also in the tree from the tree trace
				// we get the ancestral character state from the ancestral state trace
				size_t cladeIndex = root.getCladeIndex( nodes[j] );
				
				// get AncestralStateTrace for this node
				AncestralStateTrace ancestralstate_trace = ancestralstate_traces[cladeIndex];
				
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
	//std::vector<std::string*> best_states(nodes.size(), "");
	std::vector<std::string*> best_states;
	for (int i = 0; i < nodes.size(); i++) {
		
		double state1_pp = 0.0; 
		double state2_pp = 0.0; 
		double state3_pp = 0.0;
		
		std::string state1 = ""; 
		std::string state2 = ""; 
		std::string state3 = "";
		
		// loop through all states for this node
		for (int j = 0; j < pp[i].size(); j++) {
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
		std::string final_state = "{" + state1 + "=" + boost::lexical_cast<std::string>(state1_pp);
		if (state2_pp > 0) {
			final_state += "," + state2 + "=" + boost::lexical_cast<std::string>(state2_pp);
		}
		if (state3_pp > 0) {
			final_state += "," + state3 + "=" + boost::lexical_cast<std::string>(state3_pp);
		}
		if ((1 - (state1_pp+state2_pp+state3_pp)) > 0) {
			double other_states = 1.0 - (state1_pp+state2_pp+state3_pp);
			final_state = final_state + "," + "other=" + boost::lexical_cast<std::string>(other_states);
		} 
		final_state += "}";
		
		// make parameter string for this node
		std::string *s = new std::string(final_state);
		best_states.push_back(s);
	}
	mapTree->addNodeParameter("ancestralstates",best_states,true);
	
	return mapTree;
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
        treeType& tree = trace.objectAt( i );
        std::string newick = TreeUtilities::uniqueNewickTopology( tree );
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
        fillConditionalClades(tree.getRoot(), condClades, clades);
        
        
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
