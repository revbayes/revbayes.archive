#ifndef UltrametricTreeDistribution_H
#define UltrametricTreeDistribution_H

#include "Tree.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    class Clade;
    
    /*
     * This struct represents a value/count pair that is sorted by count
     */
    template <class T>
    struct Sample : public std::pair<T, long>
    {
        Sample(T t, long l) : std::pair<T, long>(t,l) {}
        
        inline bool operator<(const Sample<T>& rhs) const
        {
            if (this->second == rhs.second)
                return this->first < rhs.first;
            else
                return this->second < rhs.second;
        }
    };
    
    /*
     * This struct represents a tree bipartition (split) that can be rooted or unrooted
     */
    struct Split : public std::pair<RbBitSet, std::set<Taxon> >
    {
        Split( RbBitSet b, std::set<Taxon> m) : std::pair<RbBitSet, std::set<Taxon> >( b[0] ? ~b : b, m) {}
        
        inline bool operator()(const Sample<Split>& s)
        {
            return (*this) == s.first;
        }
    };
    
    class UltrametricTreeDistribution : public TypedDistribution<Tree>, public MemberObject< RbVector<double> > {
        
    public:
        UltrametricTreeDistribution(TypedDistribution<Tree>* tp, TypedDistribution<double>* rp, TypedDagNode<double> *ra, const TraceTree &tt, Trace<double>* d = NULL);   //!< Constructor
        UltrametricTreeDistribution(const UltrametricTreeDistribution &d);                                              //!< Copy-constructor
        virtual                                            ~UltrametricTreeDistribution(void);                          //!< Virtual destructor
        
        
        UltrametricTreeDistribution&                        operator=(const UltrametricTreeDistribution &d);            //!< Assignment operator

        // public member functions
        UltrametricTreeDistribution*                        clone(void) const;                                          //!< Create an independent clone
        double                                              computeLnProbability(void);                                 //!< Compute ln prob of current value
        void                                                executeMethod(const std::string &n, const std::vector<const DagNode*> &args, RbVector<double> &rv) const;     //!< Map the member methods to internal function calls
        void                                                redrawValue(void);                                          //!< Draw a new random value from distribution
        
        
    protected:
        // Parameter management functions
        virtual void                                        getAffected(RbOrderedSet<DagNode *>& affected, DagNode* affecter);                                      //!< get affected nodes
        virtual void                                        keepSpecialization(DagNode* affecter);
        virtual void                                        restoreSpecialization(DagNode *restorer);
        virtual void                                        touchSpecialization(DagNode *toucher, bool touchAll);
        
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter
        
    private:
        
        // helper functions
        void                                                attachTimes(Tree *psi, std::vector<TopologyNode *> &tips, size_t index, const std::vector<double> &times, double T);
        void                                                buildRandomBinaryHistory(std::vector<TopologyNode *> &tips);
        Split                                               collectTreeSample(const TopologyNode& n, RbBitSet& in, std::map<Split, double>& bl);
        Split                                               collectSplits(const TopologyNode& n, RbBitSet& in, std::vector<Split>& s);
        double                                              computeBranchRateLnProbability(const Tree &x, const std::string &newick, const std::vector<Split> &s, size_t index);
        void                                                prepareTreeSamples(const std::vector<Tree>& trees);
        void                                                simulateTree(void);
                
        // members
        TypedDistribution<Tree>*                            tree_prior;
        TypedDistribution<double>*                          rate_prior;
        const TypedDagNode<double>*                         root_age;
        std::vector<Tree>                                   trees;
        Trace<double>*                                      sample_prior_density;
        
        std::vector<std::string>                            trees_newick;
        std::map<std::string, std::vector<size_t> >         topology_indices;
        std::vector<std::map<Split, double> >               tree_branch_lengths;
        std::string                                         outgroup;
        
        size_t                                              num_samples;
        size_t                                              sample_block_start;
        size_t                                              sample_block_end;
        size_t                                              sample_block_size;

#ifdef RB_MPI
        std::vector<size_t>                                 pid_per_sample;
#endif

        std::vector<double>                                 ln_probs;
//        size_t                                              num_taxa;
//        std::vector<Taxon>                                  taxa;
    };
    
}

#endif

