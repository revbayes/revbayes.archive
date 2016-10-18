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
        
        TreeSummary(void);
        TreeSummary(const TraceTree &t);
        
        Tree*                                                                   ancestralStateTree(const Tree &inputTree, std::vector<AncestralStateTrace> &ancestralstate_traces, int burnin, std::string summary_stat, int site);
        void                                                                    annotate(Tree &inputTree);
        void                                                                    annotateTree(Tree &inputTree, bool c);
        void                                                                    annotateHPDAges(Tree &inputTree, double hpd);
        double                                                                  cladeProbability(const Clade &c) const;
        Tree*                                                                   cladoAncestralStateTree(const Tree &inputTree, std::vector<AncestralStateTrace> &ancestralstate_traces, int burnin, std::string summary_stat, int site);
        Tree*                                                                   conTree(double cutoff);
        std::string                                                             getSiteState( const std::string &site_sample, size_t site );
        int                                                                     getNumberSamples(void) const;
        std::vector<Tree>                                                       getUniqueTrees(double ci=0.95) const;
        double                                                                  getTopologyFrequency(const Tree &t) const;
        bool                                                                    isTreeContainedInCredibleInterval(const Tree &t, double size);
        Tree*                                                                   map(bool c);
        void                                                                    printTreeSummary(std::ostream& o, double ci=0.95);
        void                                                                    printCladeSummary(std::ostream& o, double minP=0.05);
        void                                                                    setBurnin(int b);
        void                                                                    summarizeClades(bool clock);
		void                                                                    summarizeConditionalClades(bool clock);
		void                                                                    summarizeTrees(void);
        void                                                                    summarizeCladesForTree(const Tree &reference_tree, bool c);

    private:
        void                                                                    annotateContinuous(Tree &inputTree, const std::string &n, size_t paramIndex, double hpd = 0.95, bool np=true );
        void                                                                    annotateDiscrete(Tree &inputTree, const std::string &n, size_t paramIndex, size_t num = 3, bool np=true );
        
		TopologyNode*															assembleConsensusTopology(std::vector<TopologyNode*> *nodes, std::vector<std::string> tipNames, std::vector<double> *pp, double cutoff);
		Clade                                                                   fillClades(const TopologyNode &n, std::vector<Clade> &c, bool clock);
		Clade                                                                   fillConditionalClades(const TopologyNode &n, std::vector<ConditionalClade> &cc, std::vector<Clade> &c, bool cl);
        Sample<Clade>&                                                          findCladeSample(const Clade &n);
		void                                                                    calculateMedianAges(TopologyNode* n, double parentAge, std::vector<double> *ages);
		void																	resolveConsensusBush(TopologyNode* root, std::vector<TopologyNode*> nodes, std::vector<std::string> tipNames, std::vector<double> pp, double cutoff);

		size_t                                                                  burnin;
        TraceTree                                                               trace;
        std::vector<Sample<std::string> >                                       treeSamples;
        std::vector<Sample<Clade> >                                             cladeSamples;
		std::map<Clade, std::vector<double> >                                   cladeAges;
        std::map<Clade, std::vector<double> >                                   cladeAgesOfBestTree;
		std::map<Clade, std::map<Clade, std::vector<double> > >                 conditionalCladeFrequencies;
    };
    
    
} //end namespace RevBayesCore


#endif
