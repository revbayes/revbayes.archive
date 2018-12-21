#ifndef UnrootedSBN_H
#define UnrootedSBN_H

#include "MatrixReal.h"
#include "Subsplit.h"
#include "Taxon.h"
#include "Tree.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {

    class UnrootedSBN : public TypedDistribution<Tree> {

    public:
        // UnrootedSBN(const std::vector<Taxon> &tn, const std::vector<Subsplit> &ss, const std::vector<long> &rs, const std::vector<double> &rp, const std::vector<std::vector<std::vector<long> > > &sc, const std::vector<std::vector<std::vector<double> > > &sp);
        UnrootedSBN(const std::vector<Taxon> &tn, const std::vector<Subsplit> &ss, const std::vector<long> &rs, const std::vector<double> &rp, const RbVector<RbVector<RbVector<long> > > sc, const RbVector<RbVector<RbVector<double> > > sp);
		virtual                                            ~UnrootedSBN(void);                                                                    //!< Virtual destructor

        // public member functions
        UnrootedSBN*                        clone(void) const;                                                                                  //!< Create an independent clone
        double                                              computeLnProbability(void);
        double                                              computeLnProbabilityGivenRoot(void);
        void                                                redrawValue(void);
        virtual void                                        setValue(Tree *v, bool f=false);                                    //!< Set the current value, e.g. attach an observation (clamp)

    protected:
        // Parameter management functions
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter

    private:

        // helper functions
        void                                                simulateTree(void);

        // members
        size_t                                              num_taxa;
        std::vector<Taxon>                                  taxa;
        bool                                                rooted;
        std::vector<Subsplit>                               subsplits; // Subsplits list should contain trivial subsplits, so the children of A|B should be A and B (and the child of A should be A)
        std::vector<long>                                   root_splits;
        std::vector<double>                                 root_split_probabilities;
        RbVector<RbVector<RbVector<long> > >                subsplit_children; // This is a vector of vectors of vectors, where subsplit_children[i] is all children of subsplit[i], subsplit_children[i][0] is all subsplits of clade X of i, and subsplit_children[i][1][j] is a specific child of subsplit i's clade Y.
        RbVector<RbVector<RbVector<double> > >              subsplit_probabilities; // For every parent-child subsplit pair recorded in subsplit_children, this is the probability of that transition
        std::map<Subsplit,size_t>                           subsplit_bitset_map;
        std::map<Subsplit,double>                           root_split_probability_map; // For any root split, this tells us the probability of it
        std::map<std::pair<size_t,size_t>,double>           subsplit_parent_child_probability_pair_map; // For any parent-child subsplit pair (i,j) (parent being i), this tells us the probability of the transition
    };

}

#endif
