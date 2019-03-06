#ifndef DuplicationLossProcess_H
#define DuplicationLossProcess_H

#include "RbVector.h"
#include "Tree.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

// If branching events are closer in time than 'EPS_COAL', they are considered
// the same event. This is important when assigning coalescent events to
// branching events on the gene tree.
const double EPS_COAL = 1E-10;

namespace RevBayesCore {
  
    class Clade;

    class DuplicationLossProcess : public TypedDistribution<Tree> {

    public:
        DuplicationLossProcess(const TypedDagNode<Tree> *it, const TypedDagNode<double> *org, const std::vector<Taxon> &t, const std::string &condition);

        // Virtual destructor.
        virtual                                             ~DuplicationLossProcess(void);

        // Public member functions.
        double                                              computeLnProbability(void);
        void                                                redrawValue(void);
        void                                                setDuplicationRate(TypedDagNode<RbVector<double> >* d);
        void                                                setDuplicationRate(TypedDagNode<double>* d);
        void                                                setLossRate(TypedDagNode<RbVector<double> >* l);
        void                                                setLossRate(TypedDagNode<double>* l);
        void                                                setGeneSamplingProbability(TypedDagNode<RbVector<double> >* s);
        // Set the current value, e.g. attach an observation (clamp).
        virtual void                                        setValue(Tree *v, bool f=false);

        // Pure virtual member functions.
        // Create an independent clone
        virtual DuplicationLossProcess*                     clone(void) const;

    protected:
        // Parameter management functions.
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP); //!< Swap a parameter

        // Helper functions.
        double                                              computeD(double dt, double e);
        double                                              computeE(double dt, double e);
        // k :: Number of genes at the bottom of the branch.
        // t :: ORDERED vector of duplication times (ascending order).
        // a :: Age recent (at the bottom of the branch).
        // b :: Age ancient (at the top of the branch).
        // n :: The topology node of the considered branch (the node at the bottom).
        // f :: Are we at the root?
        double                                              computeLnDuplicationLossProbability(size_t k, const std::vector<double> &t, double a, double b, const TopologyNode &n, bool f);
        double                                              recursivelyComputeLnProbability(const TopologyNode &n);
        void                                                resetTipAllocations(void);
        void                                                simulateTree(void);
        bool                                                simulateTreeRejectionSampling(void);
        void                                                recursivelySimulateTreeForward(double age_begin, const TopologyNode *i_node, std::vector<TopologyNode *> genes);

        // Members.
        std::vector<Taxon>                                  taxa;
        const TypedDagNode<Tree>*                           individual_tree;
        size_t                                              num_taxa;
        double                                              log_tree_topology_prob;

        const TypedDagNode<double>*                         origin;

      
        const TypedDagNode<RbVector<double> >*              duplication_rates;
        const TypedDagNode<double>*                         duplication_rate;
        const TypedDagNode<RbVector<double> >*              loss_rates;
        const TypedDagNode<double>*                         loss_rate;
        const TypedDagNode<RbVector<double> >*              gene_sampling_probabilities;

        std::vector< std::set< const TopologyNode* > >      genes_per_branch_recent;
        std::vector< std::set< const TopologyNode* > >      genes_per_branch_ancient;
        // Extinction probabilities at start (older/ancient end) of individual branch.
        std::vector< double >                               extinction_probs;
      
        std::string                                         condition;                                                                                          //!< The condition of the process (none/survival/#taxa).

  };
}
#endif
