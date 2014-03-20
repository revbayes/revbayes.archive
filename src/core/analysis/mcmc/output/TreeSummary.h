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

#include <algorithm>
#include <map>

namespace RevBayesCore {

    template<class treeType>
    class TreeSummary {
    
    public:
    
        TreeSummary(const TreeTrace<treeType> &t);
    
        treeType*                                                               map(int burnin=-1);
        void                                                                    summarize(int burnin=-1);
        void                                                                    printTreeSummary(std::ostream& o);
        void                                                                    printCladeSummary(void);
    
    private:
        
        Clade                                                                   fillConditionalClades(const TopologyNode &n, std::vector<ConditionalClade> &cc, std::vector<Clade> &c);
    
        size_t                                                                  burnin;
        TreeTrace<treeType>                                                     trace;
        std::map<std::string, unsigned int>                                     treeFrequencies;
        std::map<std::string, unsigned int>                                     cladeFrequencies;
        std::map<std::string, std::map<std::string, std::vector<double> > >     conditionalCladeFrequencies;
    };
    
    
    template <>
    inline BranchLengthTree* TreeSummary<BranchLengthTree>::map( int b ) {
        treeFrequencies.clear();
        
        if (b == -1) {
            burnin = trace.size() / 4;
        }
        else {
            burnin = b;
        }
        
        for (size_t i = burnin; i < trace.size(); ++i) {
            BranchLengthTree& tree = trace.objectAt( i );
            std::string newick = TreeUtilities::uniqueNewickTopology( tree );
            const std::map<std::string, unsigned int>::iterator& entry = treeFrequencies.find( newick );
            if ( entry == treeFrequencies.end() ) {
                treeFrequencies.insert( std::pair<std::string, unsigned int>(newick, 1));
            } else {
                entry->second++;
            }
        }
        
        // collect the samples
        std::vector<Sample<std::string> > samples;
        for (std::map<std::string, unsigned int>::const_iterator it = this->treeFrequencies.begin(); it != this->treeFrequencies.end(); ++it) {
            samples.push_back( Sample<std::string>(it->first, it->second) );
        }
        
        // sort the samples by frequency
        sort(samples.begin(), samples.end());
        
        std::string bestNewick = samples.rbegin()->getValue();
        NewickConverter converter;
        BranchLengthTree* bestTree = converter.convertFromNewick( bestNewick );
        size_t numTaxa = bestTree->getNumberOfTips();
        
        const std::vector<TopologyNode*> &nodes = bestTree->getNodes();
        std::vector<double> pp(nodes.size(),0.0);
        std::vector<std::vector<double> > ages(nodes.size(),std::vector<double>());
        double weight = 1.0 / (trace.size()-burnin);
        
        for (size_t i = burnin; i < trace.size(); ++i) {
            BranchLengthTree& tree = trace.objectAt( i );
            const TopologyNode& root = tree.getRoot();
            for (size_t j = numTaxa; j < nodes.size(); ++j) {
                if ( root.containsClade(nodes[j], true) ) {
                    pp[j] += weight;
                    double tmrca = tree.getTmrca(*nodes[j]);
                    //                double tmrca = 1.0;
                    ages[j].push_back(tmrca);
                }
            }
        }
        
        std::vector<double> meanBranchLengths;
        for (std::vector<std::vector<double> >::iterator it = ages.begin(); it != ages.end(); ++it) {
            double meanBranchLength = 0;
            const std::vector<double> &bl_samples = *it;
            for (std::vector<double>::const_iterator j = bl_samples.begin(); j != bl_samples.end(); ++j) {
                meanBranchLength += *j;
            }
            if ( bl_samples.size() > 0 ) {
                meanBranchLength /= double(bl_samples.size());
            }
            meanBranchLengths.push_back( meanBranchLength );
        }
        
        for (size_t i = 0; i < nodes.size(); ++i) {
            bestTree->setBranchLength(i, meanBranchLengths[i]);
        }
        
        bestTree->addNodeParameter("posterior",pp,true);
        
        return bestTree;
    }
    
    template <>
    inline TimeTree* TreeSummary<TimeTree>::map( int b ) 
    {
        treeFrequencies.clear();
        
        if (b == -1) 
        {
            burnin = trace.size() / 4;
        }
        else 
        {
            burnin = b;
        }
        
        double sampleSize = trace.size() - burnin;
        
        
        double meanRootAge;
        for (size_t i = burnin; i < trace.size(); ++i) 
        {
            // get the sampled tree
            TimeTree& tree = trace.objectAt( i );
            
            // add this root age to our variable
            meanRootAge += tree.getRoot().getAge();
            
            // construct a unique newick string that we will use as an identifier
            std::string newick = TreeUtilities::uniqueNewickTopology( tree );
            
            // increment the tree sample counter for this tree
            const std::map<std::string, unsigned int>::iterator& entry = treeFrequencies.find( newick );
            if ( entry == treeFrequencies.end() ) 
            {
                treeFrequencies.insert( std::pair<std::string, unsigned int>(newick, 1));
            } 
            else 
            {
                entry->second++;
            }
            
            // get the conditional clades for this
            std::vector<ConditionalClade> condClades;
            std::vector<Clade> clades;
            fillConditionalClades(tree.getRoot(), condClades, clades);
            
            
            // first increment the clade frequency countera
            // there need to be two loops because otherwise we count the the parent clade twice
            for (size_t i = 0; i < clades.size(); ++i) 
            {
                const Clade & c = clades[i];
                std::string parentString = c.toString();
                const std::map<std::string, unsigned int>::iterator& entry = cladeFrequencies.find( parentString );
                if ( entry == cladeFrequencies.end() ) 
                {
                    cladeFrequencies.insert( std::pair<std::string, unsigned int>(parentString, 1));
                    conditionalCladeFrequencies.insert( std::pair<std::string, std::map<std::string, std::vector<double> > >(parentString, std::map<std::string, std::vector<double> >()) );
                } 
                else 
                {
                    entry->second++;
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
        std::vector<Sample<std::string> > samples;
        for (std::map<std::string, unsigned int>::const_iterator it = this->treeFrequencies.begin(); it != this->treeFrequencies.end(); ++it) 
        {
            samples.push_back( Sample<std::string>(it->first, it->second) );
        }
        
        // sort the samples by frequency
        sort(samples.begin(), samples.end());
        
        std::string bestNewick = samples.rbegin()->getValue();
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
                Clade c( n->getTaxaStringVector(), 0.0 );

                double cladeFreq = cladeFrequencies[c.toString()];
                double pp = cladeFreq / sampleSize;
                n->addNodeParameter("posterior",pp);
                
                // second we compute the conditional clade probability
                double ccp = 1.0;
                double age = 0.0;
                if ( !n->isRoot() ) 
                {
                    Clade parent( n->getParent().getTaxaStringVector(), 0.0 );
                    std::map<std::string, std::vector<double> >& condCladeFreqs = conditionalCladeFrequencies[parent.toString()];
                    double parentCladeFreq = cladeFrequencies[parent.toString()];
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
                n->addNodeParameter("CCP",ccp);
                
                // finally, we compute the mean conditional age
                bestTimeTree->setAge(i, age);
            }
        }
        
        return bestTimeTree;
    }

}

#include "TopologyNode.h"

#include <vector>

template <class treeType>
RevBayesCore::TreeSummary<treeType>::TreeSummary( const TreeTrace<treeType> &t) : 
    burnin( 0 ), 
    trace( t ) 
{
    
}



/**
 *
 *
 */
template <class treeType>
RevBayesCore::Clade RevBayesCore::TreeSummary<treeType>::fillConditionalClades(const RevBayesCore::TopologyNode &n, std::vector<RevBayesCore::ConditionalClade> &condClades, std::vector<RevBayesCore::Clade> &clades)
{
 
    Clade parent( n.getTaxaStringVector(), n.getAge() );
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
void RevBayesCore::TreeSummary<treeType>::summarize( int b ) 
{
    treeFrequencies.clear();
    
    if (b == -1) 
    {
        burnin = trace.size() / 4;
    }
    else 
    {
        burnin = b;
    }
    
    for (size_t i = burnin; i < trace.size(); ++i) 
    {
        treeType& tree = trace.objectAt( i );
        std::string newick = TreeUtilities::uniqueNewickTopology( tree );
        const std::map<std::string, unsigned int>::iterator& entry = treeFrequencies.find( newick );
        if ( entry == treeFrequencies.end() ) 
        {
            treeFrequencies.insert( std::pair<std::string, unsigned int>(newick, 1));
        } 
        else
        {
            entry->second++;
        }
    }
}



template <class treeType>
void RevBayesCore::TreeSummary<treeType>::printTreeSummary(std::ostream &o) {
    
    // collect the samples
    std::vector<Sample<std::string> > samples;
    for (std::map<std::string, unsigned int>::const_iterator it = this->treeFrequencies.begin(); it != this->treeFrequencies.end(); ++it) 
    {
        samples.push_back( Sample<std::string>(it->first, it->second) );
    }
    
    // sort the samples by frequency
    sort(samples.begin(), samples.end());
    
    // now the printing
    o << "Samples\t\tProbability\t\tTree" << std::endl;
    double totalSamples = trace.size();
    for (std::vector<Sample<std::string> >::reverse_iterator it = samples.rbegin(); it != samples.rend(); ++it) 
    {
        o << it->getFrequency() << "\t\t\t" << it->getFrequency()/(totalSamples-burnin) << "\t\t\t" << it->getValue() << std::endl;
    }

}


#endif
