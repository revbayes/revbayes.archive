#ifndef AVMVNProposal_H
#define AVMVNProposal_H

#include "RbVector.h"
#include "MatrixReal.h"
#include "Proposal.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "Simplex.h"
#include "ContinuousStochasticNode.h"
#include "StochasticNode.h"

#include <ostream>
#include <vector>
#include <string>

namespace RevBayesCore {
    
    /**
     * @brief Up-Down Sliding Proposal for several parameters jointly.
     *
     *
     * This proposal randomly slides all a set of parameter up while the other set of parameters
     * is slided down by the same value. This should hopefully improve mixing in many cases.
     * The actual sliding factor is computed by delta = lambda * ( u - 0.5 ) )
     * where u ~ Uniform(0,1).
     *
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @copyright GPL version 3
     * @since 2015-05-25, version 1.0
     *
     */
    class AVMVNProposal : public Proposal {
        
    public:
        AVMVNProposal( double s, double e, double n0, double c0, double m );                                 //!< Constructor
        
        void                                        addCorrelationMatrix(StochasticNode<MatrixReal> *v);
        void                                        addUntransformedScalar(StochasticNode<double> *v);                                    //!< Add an up-scaling variable
        void                                        addLogScalar(StochasticNode<double> *v);                                    //!< Add an up-scaling variable
        void                                        addLogitScalar(ContinuousStochasticNode *v);                                    //!< Add an up-scaling variable
        void                                        addUntransformedVector(StochasticNode<RbVector<double> > *v);                         //!< Add an up-scaling variable
        void                                        addLogConstrainedSumVector(RevBayesCore::StochasticNode<RevBayesCore::Simplex> *v);                         //!< Add an up-scaling variable
        void                                        cleanProposal(void);                                                                //!< Clean up proposal
        AVMVNProposal*                              clone(void) const;                                                                  //!< Clone object
        double                                      doProposal(void);                                                                   //!< Perform proposal
        const std::string&                          getProposalName(void) const;                                                        //!< Get the name of the proposal for summary printing
        double                                      getProposalTuningParameter(void) const;
        void                                        printParameterSummary(std::ostream &o, bool name_only) const;                                       //!< Print the parameter summary
        void                                        prepareProposal(void);                                                              //!< Prepare the proposal
        void                                        removeCorrelationMatrix(StochasticNode<MatrixReal> *v);
        void                                        removeUntransformedScalar(StochasticNode<double> *v);                                    //!< Add an up-scaling variable
        void                                        removeLogScalar(StochasticNode<double> *v);                                    //!< Add an up-scaling variable
        void                                        removeLogitScalar(ContinuousStochasticNode *v);                                    //!< Add an up-scaling variable
        void                                        removeUntransformedVector(StochasticNode<RbVector<double> > *v);                         //!< Add an up-scaling variable
        void                                        removeLogConstrainedSumVector(RevBayesCore::StochasticNode<RevBayesCore::Simplex> *v);                         //!< Add an up-scaling variable
        void                                        setProposalTuningParameter(double tp);
        void                                        tune(double r);                                                                     //!< Tune the proposal to achieve a better acceptance/rejection ratio
        void                                        undoProposal(void);                                                                 //!< Reject the proposal
        
    protected:
        
        void                                        swapNodeInternal(DagNode *oldN, DagNode *newN);                                     //!< Swap the DAG nodes on which the Proposal is working on
        
        
    private:
        // parameters
        
        std::vector<StochasticNode<double> *>               noTransformScalarVariables;
        std::vector<StochasticNode<double> *>               logTransformScalarVariables;
        std::vector<ContinuousStochasticNode *>             logitTransformScalarVariables;
        std::vector<StochasticNode<RbVector<double> > *>    noTransformVectorVariables;
        std::vector<StochasticNode<Simplex> *>              logConstrainedSumTransformVectorVariables;
        std::vector<StochasticNode<MatrixReal> *>           correlationMatrixVariables;
        
        MatrixReal                                          C_emp;                                                   //!< The empirical covariance matrix of the samples
        MatrixReal                                          AVMVN_cholesky_L;                                       //!< Lower diagonal of Cholesky decomposition of

        size_t                                              waitBeforeLearning;                                      //!< How long to wait before tracking empirical covariances
        size_t                                              waitBeforeUsing;                                         //!< How long to wait before using the empirical covariances
        size_t                                              nTried;                                                  //!< How many times has this move been used?
        size_t                                              updates;                                                 //!< How many updates have been tried?
        size_t                                              maxUpdates;                                              //!< How many updates until we stop monitoring the covariances?
        
        double                                              sigma;                                                  //!< Variance of pre-learned (independent) normal proposal, also scales the MVN (proportional to variance, not SD)
        double                                              epsilon;                                                //!< Controls the weighting of the learned VCV and an Identity matrix ( eps * I + (1-eps) * empirical)
        size_t                                              dim;                                                    //!< Dimension of proposal
        double                                              lnHastingsratio;                                        //!< The Hastings ratio, so that helper functions can adjust as needed
        
        std::vector<double>                                 storedValues;                                           //!< The values before proposing the move, for resetting
        std::vector<double>                                 proposedValues;                                         //!< The values proposed by the move
        std::vector<double>                                 x_bar;                                                  //!< The averages in transformed space

        // functions
        void                                                getAVMVNMemberVariableValues(std::vector<double> *x);
        void                                                setAVMVNMemberVariableValues(std::vector<double> x_prime, std::vector<double> x);
        std::vector<double>                                 rMVNCholesky(std::vector<double> mu, MatrixReal L, RandomNumberGenerator& rng, double scale);
        
        
    };
    
}

#endif

