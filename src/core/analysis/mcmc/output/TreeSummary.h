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
        virtual                            ~TreeSummary();
    
        treeType*                           map(int burnin=-1);
        void                                summarize(int burnin=-1);
        void                                printTreeSummary(std::ostream& o);
        //    void                                printCladeSummary(void);
    
    private:
    
        size_t                              burnin;
        TreeTrace<treeType>                 trace;
        std::map<std::string, unsigned int> treeFrequencies;

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
    inline TimeTree* TreeSummary<TimeTree>::map( int b ) {
        treeFrequencies.clear();
        
        if (b == -1) {
            burnin = trace.size() / 4;
        }
        else {
            burnin = b;
        }
        
        for (size_t i = burnin; i < trace.size(); ++i) {
            TimeTree& tree = trace.objectAt( i );
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
        TimeTree* bestTimeTree = TreeUtilities::convertTree( *bestTree );
        size_t numTaxa = bestTree->getNumberOfTips();
        
        const std::vector<TopologyNode*> &nodes = bestTree->getNodes();
        std::vector<double> pp(nodes.size(),0.0);
        std::vector<std::vector<double> > ages(nodes.size(),std::vector<double>());
        double weight = 1.0 / (trace.size()-burnin);
        
        for (size_t i = burnin; i < trace.size(); ++i) {
            TimeTree& tree = trace.objectAt( i );
            const TopologyNode& root = tree.getRoot();
            for (size_t j = numTaxa; j < nodes.size(); ++j) {
                if ( root.containsClade(nodes[j],true) ) {
                    pp[j] += weight;
                    double tmrca = tree.getTmrca(*nodes[j]);
                    //                double tmrca = 1.0;
                    ages[j].push_back(tmrca);
                }
            }
        }
        
        std::vector<double> meanAges;
        for (std::vector<std::vector<double> >::iterator it = ages.begin(); it != ages.end(); ++it) {
            double meanAge = 0;
            const std::vector<double> &bl_samples = *it;
            for (std::vector<double>::const_iterator j = bl_samples.begin(); j != bl_samples.end(); ++j) {
                meanAge += *j;
            }
            if ( bl_samples.size() > 0 ) {
                meanAge /= double(bl_samples.size());
            }
            meanAges.push_back( meanAge );
        }
        
        for (size_t i = 0; i < nodes.size(); ++i) {
            bestTimeTree->setAge(i, meanAges[i]);
        }
        
        bestTimeTree->addNodeParameter("posterior",pp,true);
        
        return bestTimeTree;
    }

}

#include "TopologyNode.h"

#include <vector>

template <class treeType>
RevBayesCore::TreeSummary<treeType>::TreeSummary( const TreeTrace<treeType> &t) : burnin( 0 ), trace( t ) {
    
}



template <class treeType>
RevBayesCore::TreeSummary<treeType>::~TreeSummary<treeType>() {
    
}



template <class treeType>
void RevBayesCore::TreeSummary<treeType>::summarize( int b ) {
    treeFrequencies.clear();
    
    if (b == -1) {
        burnin = trace.size() / 4;
    }
    else {
        burnin = b;
    }
    
    for (size_t i = burnin; i < trace.size(); ++i) {
        treeType& tree = trace.objectAt( i );
        std::string newick = TreeUtilities::uniqueNewickTopology( tree );
        const std::map<std::string, unsigned int>::iterator& entry = treeFrequencies.find( newick );
        if ( entry == treeFrequencies.end() ) {
            treeFrequencies.insert( std::pair<std::string, unsigned int>(newick, 1));
        } else {
            entry->second++;
        }
    }
}



template <class treeType>
void RevBayesCore::TreeSummary<treeType>::printTreeSummary(std::ostream &o) {
    
    // collect the samples
    std::vector<Sample<std::string> > samples;
    for (std::map<std::string, unsigned int>::const_iterator it = this->treeFrequencies.begin(); it != this->treeFrequencies.end(); ++it) {
        samples.push_back( Sample<std::string>(it->first, it->second) );
    }
    
    // sort the samples by frequency
    sort(samples.begin(), samples.end());
    
    // now the printing
    o << "Samples\t\tProbability\t\tTree" << std::endl;
    double totalSamples = trace.size();
    for (std::vector<Sample<std::string> >::reverse_iterator it = samples.rbegin(); it != samples.rend(); ++it) {
        o << it->getFrequency() << "\t\t\t" << it->getFrequency()/(totalSamples-burnin) << "\t\t\t" << it->getValue() << std::endl;
    }

}


#endif
