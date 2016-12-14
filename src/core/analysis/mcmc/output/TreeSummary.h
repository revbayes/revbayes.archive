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

    /*
     * This functor is used as a comparator to identify equivalence classes
     * of unrooted splits, and rooted clades with or without sampled mrcas
     */
    struct CladeComparator
    {
        CladeComparator(bool r = true, const Clade& c = Clade() ) : clade(c), rooted(r) {};

        bool operator()(const Clade& a, const Clade& b) const;
        bool operator()(const Sample<Clade>& s) const;

        Clade clade;
        bool rooted;
    };
    
    struct AnnotationReport
    {
        bool ages             = true;
        bool cc_ages          = false;
        bool ccp              = true;
        bool tree_ages        = false;
        double hpd            = 0.95;
        bool map_parameters   = false;
        bool mean             = true;
        bool posterior        = true;
        bool sa               = true;
    };

    class TreeSummary : public Cloneable {
        
    public:
        
        TreeSummary(void);
        TreeSummary(const TraceTree &t);
        
        TreeSummary*                                                            clone(void) const;

        Tree*                                                                   ancestralStateTree(const Tree &inputTree, std::vector<AncestralStateTrace> &ancestralstate_traces, int burnin, std::string summary_stat, int site);
        void                                                                    annotateTree(Tree &inputTree, AnnotationReport report );
        double                                                                  cladeProbability(const Clade &c);
        Tree*                                                                   cladoAncestralStateTree(const Tree &inputTree, std::vector<AncestralStateTrace> &ancestralstate_traces, int burnin, std::string summary_stat, int site);
        int                                                                     getNumberSamples(void) const;
        std::vector<Tree>                                                       getUniqueTrees(double ci=0.95);
        int                                                                     getTopologyFrequency(const Tree &t);
        const TraceTree&                                                        getTreeTrace(void) const;
        bool                                                                    isTreeContainedInCredibleInterval(const Tree &t, double size);
        Tree*                                                                   mapTree(AnnotationReport report);
        Tree*                                                                   mccTree(AnnotationReport report);
        Tree*                                                                   mrTree(AnnotationReport report, double cutoff);
        void                                                                    printTreeSummary(std::ostream& o, double ci=0.95);
        void                                                                    printCladeSummary(std::ostream& o, double minP=0.05);
        void                                                                    setBurnin(int b);

    private:

        void                                                                    enforceNonnegativeBranchLengths(TopologyNode& tree) const;
        Clade                                                                   fillConditionalClades(const TopologyNode &n, std::map<Clade, std::set<Clade, CladeComparator>, CladeComparator> &cc);
        const Sample<Clade>&                                                    findCladeSample(const Clade &n) const;
        TopologyNode*                                                           findParentNode(TopologyNode&, const Clade &, std::vector<TopologyNode*>&, RbBitSet& ) const;
        std::string                                                             getSiteState( const std::string &site_sample, size_t site );
        void                                                                    mapContinuous(Tree &inputTree, const std::string &n, size_t paramIndex, double hpd = 0.95, bool np=true ) const;
        void                                                                    mapDiscrete(Tree &inputTree, const std::string &n, size_t paramIndex, size_t num = 3, bool np=true ) const;
        void                                                                    mapParameters(Tree &inputTree) const;
        void                                                                    summarize(void);

        size_t                                                                  burnin;
        bool                                                                    clock;
        bool                                                                    rooted;
        bool                                                                    summarized;
        TraceTree                                                               trace;
        bool                                                                    use_tree_trace;

        std::vector<Sample<Clade> >                                             cladeSamples;
        std::map<Taxon, Sample<Taxon> >                                         sampledAncestorSamples;
        std::vector<Sample<std::string> >                                       treeSamples;

        std::map<Clade, std::vector<double>, CladeComparator >                                    cladeAges;
        std::map<Clade, std::map<Clade, std::vector<double>, CladeComparator >, CladeComparator > conditionalCladeAges;
        std::map<std::string, std::map<Clade, std::vector<double>, CladeComparator > >            treeCladeAges;
    };
    

} //end namespace RevBayesCore


#endif
