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

#include "BranchLengthTree.h"
#include "Clade.h"
#include "ConditionalClade.h"
#include "NewickConverter.h"
#include "RlUserInterface.h"
#include "Sample.h"
#include "Topology.h"
#include "TreeTrace.h"
#include "TreeUtilities.h"

#include <algorithm>
#include <map>

namespace RevBayesCore {
    
    template<class treeType>
    class TreeSummary {
        
    public:
        
        TreeSummary(const TreeTrace<treeType> &t);
        
        treeType*                                                               map(int burnin=-1);
		treeType*																conTree(double cutoff, int burnin = -1);
        void                                                                    printTreeSummary(std::ostream& o);
        void                                                                    printCladeSummary(void);
		void                                                                    summarizeClades(int burnin = -1);
		void                                                                    summarizeConditionalClades(int burnin = -1);
		void                                                                    summarizeTrees(int burnin = -1);
        
    private:
		TopologyNode*															assembleConsensusTopology(std::vector<TopologyNode*> *nodes, std::vector<std::string> tipNames, std::vector<double> *pp, double cutoff, double burnin);
		Clade                                                                   fillClades(const TopologyNode &n, std::vector<Clade> &c);
		Clade                                                                   fillConditionalClades(const TopologyNode &n, std::vector<ConditionalClade> &cc, std::vector<Clade> &c);
        Sample<std::string>&                                                    findCladeSample(const std::string &n);
		void                                                                    calculateMedianAges(TopologyNode* n, double parentAge, std::vector<double> *ages);
		void																	resolveConsensusBush(TopologyNode* root, std::vector<TopologyNode*> nodes, std::vector<std::string> tipNames, std::vector<double> pp, double cutoff, double burnin);
        
		size_t                                                                  burnin;
        TreeTrace<treeType>                                                     trace;
        //        std::map<std::string, unsigned int>                                     treeFrequencies;
        std::vector<Sample<std::string> >                                       treeSamples;
        std::vector<Sample<std::string> >                                       cladeSamples;
		std::map<std::string, std::vector<double> >                             cladeAges;
		std::map<std::string, std::map<std::string, std::vector<double> > >     conditionalCladeFrequencies;
    };
    
    
    template <>
    inline BranchLengthTree* TreeSummary<BranchLengthTree>::map( int b )
    {
        
        summarizeConditionalClades( b );
		summarizeTrees( b );
        
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
        
        summarizeConditionalClades( b );
		summarizeTrees( b );
        
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
        
        TimeTree& firstTree = trace.objectAt( 0 );
        const std::vector<TopologyNode*> &firstNodes = firstTree.getNodes();
        for (size_t i = 0; i < numTaxa; i++)
        {
            if (firstNodes[i]->isTip()) {
                for (size_t j = 0; j < numTaxa; j++)
                {
                    if (firstNodes[i]->getName() == nodes[j]->getName())
                    {
                        bestTimeTree->setAge(j, firstNodes[i]->getAge());
                    }
                }
            }
        }
        
        return bestTimeTree;
    }
    
    
	template <>  //consensus tree function for BranchLengthTree
	inline BranchLengthTree* TreeSummary<BranchLengthTree>::conTree(double cutoff, int b) {
        
		//make sure burnin is proper
		if (b == -1)
		{
			burnin = trace.size() / 4;
		}
		else
		{
			burnin = (size_t)(b);
		}
		std::stringstream ss;
		ss << "Compiling consensus tree from " << trace.size() << " trees in tree trace, using a burnin of " << burnin << " trees.\n";
		RBOUT(ss.str());
        
		//fill in clades, use all above 50% to resolve the bush with the consensus partitions
		RBOUT("Calculating clade frequencies...\n");
		summarizeClades(b);		//fills std::vector<Sample<std::string> > cladeSamples, sorts them by descending freq
        
		//set up variables for consensus tree assembly
		BranchLengthTree& temptree = trace.objectAt(0);
		std::vector<std::string> tipNames = temptree.getTipNames();
		std::vector<double> pp;
		std::vector<TopologyNode*> nodes;
		
		if (cutoff < 0.0 || cutoff > 1.0) cutoff = 0.5;
        
		ss << "Assembling clades into a MR consensus tree, using a " << cutoff << " cutoff...\n";
		RBOUT(ss.str());
		TopologyNode* root = assembleConsensusTopology(&nodes, tipNames, &pp, cutoff, burnin);
		
		//calculate branch lengths
		RBOUT("Calculating median branch lengths...\n");
        
		std::vector<double> medianBranchLengths(nodes.size(), 0.0);
		
		for (size_t m = 0; m < nodes.size(); m++)
		{
			std::vector<std::string> taxa;
			nodes[m]->getTaxaStringVector(taxa);
			Clade c(taxa, 0.0); //clade age not used here
			std::vector<double> brlensVec = cladeAges.find(c.toString())->second;
			std::sort(brlensVec.begin(), brlensVec.end());
            
			size_t nbValues = brlensVec.size();
			if ((nbValues % 2) == 0)
			{
				medianBranchLengths[m] = (brlensVec[nbValues / 2] + brlensVec[(nbValues / 2) - 1]) / 2.0;
			}
			else
			{
				medianBranchLengths[m] = brlensVec[nbValues / 2];
			}
		}
        
		//now put the tree together
		Topology *tau = new Topology(root, nodes);
        
		BranchLengthTree* consensusTree = new BranchLengthTree();
		consensusTree->setTopology(tau, true);
        
		for (size_t p = 0; p < nodes.size(); ++p )
		{
			consensusTree->setBranchLength(p, medianBranchLengths[p]);
		}
		consensusTree->addNodeParameter("posterior", pp, true);
		RBOUT("Done.\n");
        
		return consensusTree;
	}
    
    
	template <>  //consensus tree function for TimeTree
	inline TimeTree* TreeSummary<TimeTree>::conTree(double cutoff, int b) {
        
		//make sure burnin is proper
		if (b == -1)
		{
			burnin = trace.size() / 4;
		}
		else
		{
			burnin = (size_t)(b);
		}
		std::stringstream ss;
		ss << "Compiling consensus tree from " << trace.size() << " trees in tree trace, using a burnin of " << burnin << " trees.\n";
		RBOUT(ss.str());
        
		//fill in clades, use all above 50% to resolve the bush with the consensus partitions
		RBOUT("Calculating clade frequencies...\n");
		summarizeClades(b);		//fills std::vector<Sample<std::string> > cladeSamples, sorts them by descending freq
        
		//set up variables for consensus tree assembly
		TimeTree& temptree = trace.objectAt(0);
		std::vector<std::string> tipNames = temptree.getTipNames();
		std::vector<double> pp;
		std::vector<TopologyNode*> nodes;
        
		if (cutoff < 0.0 || cutoff > 1.0) cutoff = 0.5;
        
		ss << "Assembling clades into a MR consensus tree, using a " << cutoff << " cutoff...\n";
		RBOUT(ss.str());
		TopologyNode* root = assembleConsensusTopology(&nodes, tipNames, &pp, cutoff, burnin);
        
		//now put the tree together
		Topology *tau = new Topology(root, nodes);
		TimeTree* consensusTree = new TimeTree();
		consensusTree->setTopology(tau, true);
		consensusTree->addNodeParameter("posterior", pp, true);
        
		//calculate node ages for consensus tree
		RBOUT("Calculating node ages...\n");
		double maxAge = 2147483647; //just a very high nb, so that age of root will be smaller
		std::vector<double> ages(nodes.size(), 0.0);
		calculateMedianAges(root, maxAge, &ages);
		
		for (size_t x = 0; x < ages.size(); x++)
		{
			consensusTree->setAge(x, ages[x]);
		}
        
		RBOUT("Done.\n");
        
		return consensusTree;
	}
    
	//filling in clades and clade ages - including tip nodes in clade sample - to get age for serially sampled tips in time trees
	template <>
	inline Clade TreeSummary<TimeTree>::fillClades(const RevBayesCore::TopologyNode &n, std::vector<RevBayesCore::Clade> &clades)
	{
		std::vector<std::string> taxa;
		n.getTaxaStringVector(taxa);
		Clade parentClade (taxa, n.getAge());
		clades.push_back(parentClade);
        
		if (!n.isTip())
		{
			for (size_t i = 0; i < n.getNumberOfChildren(); i++)
			{
				const TopologyNode &childNode = n.getChild(i);
				Clade ChildClade = fillClades(childNode, clades);
			}
		}
		return parentClade;
	}
    
	template <>
	inline Clade TreeSummary<BranchLengthTree>::fillClades(const RevBayesCore::TopologyNode &n, std::vector<RevBayesCore::Clade> &clades)
	{
		std::vector<std::string> taxa;
		n.getTaxaStringVector(taxa);
		Clade parentClade(taxa, n.getBranchLength());
		clades.push_back(parentClade);
        
		if (!n.isTip() )
		{
			for (size_t i = 0; i < n.getNumberOfChildren(); i++)
			{
				const TopologyNode &childNode = n.getChild(i);
				Clade ChildClade = fillClades(childNode, clades);
			}
		}
		return parentClade;
	}
    
    
} //end namespace RevBayesCore



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

/*
 *	recursively calculates the median node age from the root, and tests that it is not older than its parent node
 */
template <class treeType>
void RevBayesCore::TreeSummary<treeType>::calculateMedianAges(TopologyNode* n, double parentAge, std::vector<double> *ages)
{
	if (cladeAges.size() < 1)
		return;
	
	std::vector<std::string> taxa;
	n->getTaxaStringVector(taxa);
	Clade c (taxa, n->getAge());
	
	std::vector<double> ageVec = cladeAges.find(c.toString())->second;
	std::sort(ageVec.begin(), ageVec.end());
	double medianAge;
    
	size_t nbValues = ageVec.size();
	if ((nbValues % 2) == 0)
	{
		medianAge = (ageVec[nbValues / 2] + ageVec[(nbValues / 2) - 1]) / 2.0;
	}
	else
	{
		medianAge = ageVec[nbValues / 2];
	}
	if (parentAge < medianAge) medianAge = parentAge;
	ages->at( n->getIndex() ) = medianAge;
    
	if ( !n->isTip() )
	{
		for (size_t i = 0; i < n->getNumberOfChildren(); i++)
		{
			calculateMedianAges(&(n->getChild(i)), medianAge, ages);
		}
	}
}


template <class treeType>
RevBayesCore::Clade RevBayesCore::TreeSummary<treeType>::fillConditionalClades(const RevBayesCore::TopologyNode &n, std::vector<RevBayesCore::ConditionalClade> &condClades, std::vector<RevBayesCore::Clade> &clades)
{
    
	std::vector<std::string> taxa;
	n.getTaxaStringVector(taxa);
	Clade parent(taxa, n.getAge());
	clades.push_back(parent);
    
	for (size_t i = 0; i < n.getNumberOfChildren(); i++)	//SK: extended for multifurcating trees
	{
		const TopologyNode &childNode = n.getChild(i);
		if (!childNode.isTip())
		{
			Clade ChildClade = fillConditionalClades(childNode, condClades, clades);
			ConditionalClade cc = ConditionalClade(parent, ChildClade);
			condClades.push_back(cc);
		}
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
void RevBayesCore::TreeSummary<treeType>::summarizeClades(int b)
{
	if (b == -1)
	{
		burnin = trace.size() / 4;
	}
	else
	{
		burnin = size_t(b);
	}
    
    
	for (size_t i = burnin; i < trace.size(); ++i)
	{
		treeType& tree = trace.objectAt(i);
        
		// get the clades for this tree
		std::vector<Clade> clades;
		fillClades(tree.getRoot(), clades);
        
		// collect clade ages and increment the clade frequency counter
		for (size_t j = 0; j < clades.size(); ++j)
		{
			const Clade & c = clades[j];
			std::string cladeString = c.toString();
			Sample<std::string> thisSample = Sample<std::string>(cladeString, 0);
            
			if (cladeSamples.size() == 0) {
				cladeSamples.push_back(thisSample);
				cladeSamples[0].addObservation(true);
				std::vector<double> tempAgeVec(1, c.getAge());
				cladeAges.insert(std::pair<std::string, std::vector<double> >(cladeString, tempAgeVec));
			}
			else
			{
				bool found = false;
				for (size_t ii = 0; ii < cladeSamples.size(); ii++)
				{
					if (!cladeString.compare(cladeSamples[ii].getValue()) )
					{
						found = true;
						cladeSamples[ii].addObservation(true);
                        
						std::map<std::string, std::vector<double> >::iterator entry = cladeAges.find(cladeString);
						if (entry != cladeAges.end())
						{
							entry->second.push_back(c.getAge());
						}
						break;
					}
					else
					{
						cladeSamples[ii].addObservation(false);
					}
				}
                
				if (!found)
				{
					cladeSamples.push_back(thisSample);
					cladeSamples.rbegin()->addObservation(true);
					std::vector<double> tempAgeVec(1, c.getAge());
					cladeAges.insert(std::pair<std::string, std::vector<double> >(cladeString, tempAgeVec));
                    
				}
			}
		}
	}
    
	// sort the samples by frequency
	sort(cladeSamples.begin(), cladeSamples.end());
}

template <class treeType>
void RevBayesCore::TreeSummary<treeType>::summarizeConditionalClades( int b )
{
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
            const std::map<std::string, Sample<std::string> >::iterator& entry = cladeAbsencePresence.find( parentString );
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
void RevBayesCore::TreeSummary<treeType>::summarizeTrees(int b)
{
    
	std::map<std::string, Sample<std::string> > treeAbsencePresence;
    
	if (b == -1)
	{
		burnin = trace.size() / 4;
	}
	else
	{
		burnin = size_t(b);
	}
    
	for (size_t i = burnin; i < trace.size(); ++i)
	{
		treeType& tree = trace.objectAt(i);
		std::string newick = TreeUtilities::uniqueNewickTopology(tree);
		const std::map<std::string, Sample<std::string> >::iterator& entry = treeAbsencePresence.find(newick);
		if (entry == treeAbsencePresence.end())
		{
			Sample<std::string> treeSample = Sample<std::string>(newick, 0);
			if (i > burnin)
			{
				treeSample.setTrace(std::vector<double>(i - burnin, 0.0));
			}
			else
			{
				treeSample.setTrace(std::vector<double>());
			}
			treeAbsencePresence.insert(std::pair<std::string, Sample<std::string> >(newick, treeSample));
		}
        
		for (std::map<std::string, Sample<std::string> >::iterator it = treeAbsencePresence.begin(); it != treeAbsencePresence.end(); ++it)
		{
            
			if (it->first == newick)
			{
				it->second.addObservation(true);
			}
			else
			{
				it->second.addObservation(false);
			}
		}
	}
    
	// collect the samples
	treeSamples.clear();
	for (std::map<std::string, Sample<std::string> >::iterator it = treeAbsencePresence.begin(); it != treeAbsencePresence.end(); ++it)
	{
		it->second.computeStatistics();
		treeSamples.push_back(it->second);
	}
    
	// sort the samples by frequency
	sort(treeSamples.begin(), treeSamples.end());
    
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

template <class treeType>
RevBayesCore::TopologyNode* RevBayesCore::TreeSummary<treeType>::assembleConsensusTopology(std::vector<TopologyNode*> *nodes, std::vector<std::string> tipNames, std::vector<double> *pp, double cutoff, double burnin)
{
	//first create a bush
	TopologyNode* root = new TopologyNode(tipNames.size()); //construct root node with index = nb Tips
	root->setNodeType(false, true, true);
    
	for (size_t i = 0; i < tipNames.size(); i++)
	{
		TopologyNode* tipNode = new TopologyNode(tipNames.at(i), i); //Topology node constructor adding tip name and index=taxon nb
		tipNode->setNodeType(true, false, false);
        
		// set the parent-child relationship
		root->addChild(tipNode);
		tipNode->setParent(root);
		nodes->push_back(tipNode); //add the tip nodes to the node vector (they need to be first in the vector)
		pp->push_back(1.0);
	}
    
	nodes->push_back(root); //finally add the root node, is next index after the tips
	pp->push_back(1.0);
    
	size_t nIndex = tipNames.size();
    
	for (size_t j = 0; j < cladeSamples.size(); j++)
	{
		size_t rIndex = cladeSamples.size() - 1 - j;	//reverse pass through because clades are sorted in ascending frequency
		float cladeFreq = cladeSamples[rIndex].getFrequency() / (float)(trace.size() - burnin);
		if (cladeFreq < cutoff)  break;
        
		Clade clade = Clade(cladeSamples[rIndex].getValue(), 0.0);
		std::vector<std::string> intNodeTaxa = clade.getTaxonNames();
        
		//make sure we have an internal node
		if (intNodeTaxa.size() == 1 || intNodeTaxa.size() == tipNames.size())  continue;
        
		nIndex++;	//increment node index
		TopologyNode* intNode = new TopologyNode(nIndex); //Topology node constructor, with proper node index
		intNode->setNodeType(false, false, true);
        
		//find parent node
		std::string firstTaxon = intNodeTaxa.at(0);
		TopologyNode* parentNode = NULL;
		bool isCompatible = true;
        
		for (size_t l = 0; l < tipNames.size(); l++)
		{
			if (!firstTaxon.compare(nodes->at(l)->getName()))
			{
				parentNode = &nodes->at(l)->getParent();
				break;
			}
		}
		while (parentNode != NULL)
		{
			std::vector<std::string> subtendedTaxa;
			parentNode->getTaxaStringVector(subtendedTaxa);
			if (subtendedTaxa.size() >= intNodeTaxa.size())
			{
				//check if parent is compatible with new node
				size_t inBoth = 0;
				for (size_t ll = 0; ll < intNodeTaxa.size(); ll++)
				{
					for (size_t lll = 0; lll < subtendedTaxa.size(); lll++)
					{
						if (!intNodeTaxa[ll].compare(subtendedTaxa[lll]))
							inBoth++;
					}
				}
				if (inBoth != intNodeTaxa.size()) {
					isCompatible = false;
				}
                
				break;
			}
			parentNode = &parentNode->getParent();
		}
        
		if (!isCompatible)
		{
			delete intNode;
			nIndex--;
			continue;
		}
        
		//go through nodes subtended by parent, get children
		std::vector<TopologyNode*> children = parentNode->getChildren();
		std::vector<bool> isChild(children.size(), false);
		TopologyNode* childNode;
        
		for (size_t k = 0; k < children.size(); k++)
		{
			childNode = children[k];
			std::vector<std::string> childTaxa;
			int found = 0;
			childNode->getTaxaStringVector(childTaxa);
            
			for (size_t i = 0; i < childTaxa.size(); i++)
			{
				for (size_t ii = 0; ii < intNodeTaxa.size(); ii++)
				{
					if (!childTaxa[i].compare(intNodeTaxa[ii]))
					{
						found++;
					}
				}
			}
            
			if (found == childTaxa.size())
			{
				isChild[k] = true;
			}
			else if (found > 0 && found < childTaxa.size()) {
				isCompatible = false;
				break;
			}
		}
        
		if (!isCompatible)
		{
			delete intNode;
			nIndex--;
			continue;
		}
        
		for (size_t kk = 0; kk < children.size(); kk++)
		{
			if (isChild[kk])
			{
				childNode = children[kk];
				parentNode->removeChild(childNode);
				intNode->addChild(childNode, true);
				childNode->setParent(intNode);
			}
		}
		intNode->setParent(parentNode);
		parentNode->addChild(intNode);
		nodes->push_back(intNode); //add internal node to node vector
		pp->push_back(cladeFreq);
	}
    
	return(root);
}

#endif
