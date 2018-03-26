#ifndef UltrametricTreeDistribution_H
#define UltrametricTreeDistribution_H

#include "Tree.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    class Clade;
    
    class UltrametricTreeDistribution : public TypedDistribution<Tree>, public MemberObject< RbVector<double> > {
        
    public:
        UltrametricTreeDistribution(TypedDistribution<Tree>* tp, TypedDistribution<double>* rp, TypedDagNode<double> *ra, const TraceTree &tt);   //!< Constructor
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
        double                                              computeBranchRateLnProbability(const Tree &t);
        void                                                simulateTree(void);
        
        // members
        TypedDistribution<Tree>*                            tree_prior;
        TypedDistribution<double>*                          rate_prior;
        const TypedDagNode<double>*                         root_age;
        std::vector<Tree>                                   trees;
        
        size_t                                              num_samples;
        size_t                                              sample_block_start;
        size_t                                              sample_block_end;
        size_t                                              sample_block_size;
        
        std::vector<double>                                 ln_probs;
//        size_t                                              num_taxa;
//        std::vector<Taxon>                                  taxa;
    };
    
}

#endif

