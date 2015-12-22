#ifndef TreeSummary_H
#define TreeSummary_H

#include "AncestralStateTrace.h"
#include "Clade.h"
#include "ConditionalClade.h"
#include "NewickConverter.h"
#include "RlUserInterface.h"
#include "Sample.h"
#include "Tree.h"
#include "TraceTree.h"
#include "TreeUtilities.h"

#include <algorithm>
#include <map>
#include <string>

namespace RevBayesCore {
    
    class TreeSummary {
        
    public:
        
        TreeSummary(const TraceTree &t);
        
        Tree*                                                                   map(int burnin, bool c);
		Tree*                                                                   conTree(double cutoff, int burnin);
        Tree*                                                                   ancestralStateTree(const Tree &inputTree, std::vector<AncestralStateTrace> &ancestralstate_traces, int burnin);
        void                                                                    annotate(Tree &inputTree, int b);
        void                                                                    annotateHPDAges(Tree &inputTree, double hpd, int b);
        bool                                                                    isTreeContainedInCredibleInterval(const Tree &t, double size);
        void                                                                    printTreeSummary(std::ostream& o, double ci=0.95);
        void                                                                    printCladeSummary(std::ostream& o, double minP=0.05);
		void                                                                    summarizeClades(int burnin);
		void                                                                    summarizeConditionalClades(int burnin, bool c);
		void                                                                    summarizeTrees(int burnin);
        
    private:
        void                                                                    annotateContinuous(Tree &inputTree, const std::string &n, size_t paramIndex, double hpd = 0.95, int b = -1, bool np=true );
        void                                                                    annotateDiscrete(Tree &inputTree, const std::string &n, size_t paramIndex, size_t num = 3, int b = -1, bool np=true );
        
		TopologyNode*															assembleConsensusTopology(std::vector<TopologyNode*> *nodes, std::vector<std::string> tipNames, std::vector<double> *pp, double cutoff, double burnin);
		Clade                                                                   fillClades(const TopologyNode &n, std::vector<Clade> &c);
		Clade                                                                   fillConditionalClades(const TopologyNode &n, std::vector<ConditionalClade> &cc, std::vector<Clade> &c, bool cl);
        Sample<Clade>&                                                          findCladeSample(const Clade &n);
		void                                                                    calculateMedianAges(TopologyNode* n, double parentAge, std::vector<double> *ages);
		void																	resolveConsensusBush(TopologyNode* root, std::vector<TopologyNode*> nodes, std::vector<std::string> tipNames, std::vector<double> pp, double cutoff, double burnin);
		void                                                                    setBurnin(int b);
        void                                                                    summarizeCladesForTree(const Tree &reference_tree, int b);

		size_t                                                                  burnin;
        TraceTree                                                               trace;
        //        std::map<std::string, unsigned int>                                     treeFrequencies;
        std::vector<Sample<std::string> >                                       treeSamples;
        std::vector<Sample<Clade> >                                             cladeSamples;
		std::map<Clade, std::vector<double> >                                   cladeAges;
        std::map<Clade, std::vector<double> >                                   cladeAgesOfBestTree;
		std::map<Clade, std::map<Clade, std::vector<double> > >                 conditionalCladeFrequencies;
    };
    
    
} //end namespace RevBayesCore


#endif
