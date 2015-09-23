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

#include "AncestralStateTrace.h"
#include "Clade.h"
#include "ConditionalClade.h"
#include "NewickConverter.h"
#include "RlUserInterface.h"
#include "Sample.h"
#include "Tree.h"
#include "TreeTrace.h"
#include "TreeUtilities.h"

#include <algorithm>
#include <map>
#include <string>

namespace RevBayesCore {
    
    class TreeSummary {
        
    public:
        
        TreeSummary(const TreeTrace &t);
        
        Tree*                                                                   map(int burnin = -1);
		Tree*                                                                   conTree(double cutoff, int burnin = -1);
        Tree*                                                                   ancestralStateTree(const Tree &inputTree, std::vector<AncestralStateTrace> &ancestralstate_traces, int burnin = -1);
        void                                                                    annotate(Tree &inputTree, int b = -1 );
        void                                                                    annotateHPDAges(Tree &inputTree, double hpd = 0.95, int b = -1 );
        void                                                                    printTreeSummary(std::ostream& o, double ci=0.95);
        void                                                                    printCladeSummary(std::ostream& o, double minP=0.05);
		void                                                                    summarizeClades(int burnin = -1);
		void                                                                    summarizeConditionalClades(int burnin = -1);
		void                                                                    summarizeTrees(int burnin = -1);
        
    private:
        void                                                                    annotateContinuous(Tree &inputTree, const std::string &n, size_t paramIndex, double hpd = 0.95, int b = -1, bool np=true );
        void                                                                    annotateDiscrete(Tree &inputTree, const std::string &n, size_t paramIndex, size_t num = 3, int b = -1, bool np=true );
        
		TopologyNode*															assembleConsensusTopology(std::vector<TopologyNode*> *nodes, std::vector<std::string> tipNames, std::vector<double> *pp, double cutoff, double burnin);
		Clade                                                                   fillClades(const TopologyNode &n, std::vector<Clade> &c);
		Clade                                                                   fillConditionalClades(const TopologyNode &n, std::vector<ConditionalClade> &cc, std::vector<Clade> &c);
        Sample<std::string>&                                                    findCladeSample(const std::string &n);
		void                                                                    calculateMedianAges(TopologyNode* n, double parentAge, std::vector<double> *ages);
		void																	resolveConsensusBush(TopologyNode* root, std::vector<TopologyNode*> nodes, std::vector<std::string> tipNames, std::vector<double> pp, double cutoff, double burnin);
		void                                                                    setBurnin(int b);
        
		size_t                                                                  burnin;
        TreeTrace                                                               trace;
        //        std::map<std::string, unsigned int>                                     treeFrequencies;
        std::vector<Sample<std::string> >                                       treeSamples;
        std::vector<Sample<std::string> >                                       cladeSamples;
		std::map<std::string, std::vector<double> >                             cladeAges;
		std::map<std::string, std::map<std::string, std::vector<double> > >     conditionalCladeFrequencies;
    };
    
    
} //end namespace RevBayesCore


#endif
