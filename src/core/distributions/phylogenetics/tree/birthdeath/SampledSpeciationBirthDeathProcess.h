//
//  SampledSpeciationBirthDeathProcess.h
//  revbayes-proj
//
//  Created by Michael Landis on 2/10/16.
//  Copyright © 2016 Michael Landis. All rights reserved.
//

#ifndef SampledSpeciationBirthDeathProcess_h
#define SampledSpeciationBirthDeathProcess_h

#include "AbstractCharacterHistoryBirthDeathProcess.h"
#include "CharacterHistory.h"
#include "MemberObject.h"
#include "Tree.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    class Clade;
    
    class SampledSpeciationBirthDeathProcess : public AbstractCharacterHistoryBirthDeathProcess, public MemberObject< RbVector<int> > {
        
    public:
        SampledSpeciationBirthDeathProcess(const TypedDagNode<double> *a, const TypedDagNode<double> *s, const TypedDagNode<double> *e, const TypedDagNode<double> *r, const std::vector<Taxon> &n);                                                                                  //!< Constructor
        
        virtual                                             ~SampledSpeciationBirthDeathProcess(void);                          //!< Virtual destructor
        
        // public member functions
        SampledSpeciationBirthDeathProcess*                 clone(void) const;                                          //!< Create an independent clone
        double                                              computeLnProbability(void);                                 //!< Compute ln prob of current value
        void                                                executeMethod(const std::string &n, const std::vector<const DagNode*> &args, RbVector<int> &rv) const;     //!< Map the member methods to internal function calls
        CharacterHistory&                                   getCharacterHistory(void);                                  //!< Get the character histories
        void                                                redrawValue(void);                                          //!< Draw a new random value from distribution
        void                                                setValue(Tree *v, bool force);
        
    protected:
        // Parameter management functions
        virtual void                                        getAffected(RbOrderedSet<DagNode *>& affected, DagNode* affecter);                                      //!< get affected nodes
        virtual void                                        keepSpecialization(DagNode* affecter);
        virtual void                                        restoreSpecialization(DagNode *restorer);
        virtual void                                        touchSpecialization(DagNode *toucher, bool touchAll);
        
        void                                                computeNodeProbability(const TopologyNode &n, size_t nIdx);
        double                                              computeRootLikelihood(void);
        
        
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter
        
    private:
        
        // helper functions
        void                                                attachTimes(Tree *psi, std::vector<TopologyNode *> &tips, size_t index, const std::vector<double> &times, double T);
        void                                                buildRandomBinaryHistory(std::vector<TopologyNode *> &tips);
        void                                                simulateTree(void);
        
        // members
        const TypedDagNode<double>*                         root_age;
        const TypedDagNode<double>*                         speciation;
        const TypedDagNode<double>*                         extinction;
        const TypedDagNode<double>*                         rho;                                                //!< Sampling probability of each species.
        
        CharacterHistory                                    branch_histories;
        
        size_t                                              num_taxa;
        std::vector<Taxon>                                  taxa;
        
        
        std::vector<size_t>                                 activeLikelihood;
        std::vector<std::vector<double> >                   storedLikelihood;
        mutable std::vector<bool>                           changedNodes;
        mutable std::vector<bool>                           dirtyNodes;
        mutable std::vector<std::vector<double> >           scalingFactors;
        mutable double                                      totalScaling;
        
        double                                              logTreeTopologyProb;                                                                                //!< Log-transformed tree topology probability (combinatorial constant).
        
    };
    
}

#endif /* SampledSpeciationBirthDeathProcess_h */
