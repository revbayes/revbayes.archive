#include "BiSSE.h"
#include "Clade.h"
#include "MultiRateBirthDeathProcess.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbMathCombinatorialFunctions.h"
#include "TopologyNode.h"
#include "Topology.h"

#include <algorithm>
#include <cmath>

#include <boost/numeric/odeint.hpp>

using namespace RevBayesCore;


/**
 * Constructor.
 *
 * The constructor connects the parameters of the birth-death process (DAG structure)
 * and initializes the probability density by computing the combinatorial constant of the tree structure.
 *
 * \param[in]    o         Origin or time of the process.
 * \param[in]    ra        Age or the root (=time of the process).
 * \param[in]    r         Sampling probability of a species at present.
 * \param[in]    ss        The sampling strategy (uniform/diversified).
 * \param[in]    cdt       The condition of the process (time/survival/nTaxa)
 * \param[in]    nTaxa     Number of taxa (used for initialization during simulation).
 * \param[in]    tn        Taxon names used during initialization.
 * \param[in]    c         Clade constraints.
 */
MultiRateBirthDeathProcess::MultiRateBirthDeathProcess(const TypedDagNode<double> *o,
                                                       const TypedDagNode<double> *ra,
                                                       const TypedDagNode<RbVector<double> > *l,
                                                       const TypedDagNode<RbVector<double> > *m,
                                                       const TypedDagNode<RateMatrix>* q,
                                                       const TypedDagNode< RbVector< double > >* p,
                                                       const TypedDagNode<double> *rh,
                                                       const std::string &cdt,
                                                       const std::vector<Taxon> &tn,
                                                       const std::vector<Clade> &c) : AbstractBirthDeathProcess( o, ra, cdt, tn, c ),
    lambda( l ),
    mu( m ),
    pi( p ),
    Q( q ),
    rho( rh ),
    activeLikelihood( std::vector<size_t>(2*tn.size()-1, 0) ),
    changedNodes( std::vector<bool>(2*tn.size()-1, false) ),
    dirtyNodes( std::vector<bool>(2*tn.size()-1, true) ),
    nodeStates( std::vector<std::vector<state_type> >(2*tn.size()-1, std::vector<state_type>(2,state_type())) )
{
    
    addParameter( lambda );
    addParameter( mu );
    addParameter( pi );
    addParameter( Q );
    addParameter( rho );
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself
 */
MultiRateBirthDeathProcess* MultiRateBirthDeathProcess::clone( void ) const
{
    return new MultiRateBirthDeathProcess( *this );
}


/**
 * Compute the log-transformed probability of the current value under the current parameter values.
 *
 * \return   The log-transformed probability density.
 */
double MultiRateBirthDeathProcess::computeLnProbabilityTimes( void ) const
{
    
    // variable declarations and initialization
    double lnProbTimes = 0;
    
    // present time
    double ra = value->getRoot().getAge();
    double presentTime = 0.0;
    
    // test that the time of the process is larger or equal to the present time
    if ( startsAtRoot == false )
    {
        double org = origin->getValue();
        presentTime = org;
        
    }
    else
    {
        presentTime = ra;
    }
    
    // add the survival of a second species if we condition on the MRCA
    size_t numInitialSpecies = 1;
    
    // if we started at the root then we square the survival prob
    if ( startsAtRoot == true )
    {
        ++numInitialSpecies;
        lnProbTimes *= 2.0;
    }
    
    
    
//    for (size_t i = (numInitialSpecies-1); i < numTaxa-1; ++i)
//    {
//        if ( lnProbTimes == RbConstants::Double::nan ||
//            lnProbTimes == RbConstants::Double::inf ||
//            lnProbTimes == RbConstants::Double::neginf )
//        {
//            delete times;
//            return RbConstants::Double::nan;
//        }
//        
//        
//        
////        lnProbTimes += lnSpeciationRate((*times)[i]) + lnP1((*times)[i],presentTime,samplingProbability);
//    }
    
    lnProbTimes += computeRootLikelihood();
    
    return lnProbTimes;
    
}


void MultiRateBirthDeathProcess::computeNodeProbability(const RevBayesCore::TopologyNode &node, size_t nodeIndex) const
{
    
    // check for recomputation
    if ( dirtyNodes[nodeIndex] || true )
    {
        // mark as computed
        dirtyNodes[nodeIndex] = false;
        
        state_type initialState;
        if ( node.isTip() )
        {
            // this is a tip node
            // compute the likelihood for the tip and we are done
//            computeTipLikelihood(node, nodeIndex);
            
            double samplingProbability = rho->getValue();
            size_t numCats = lambda->getValue().size();
            for (size_t i=0; i<numCats; ++i)
            {
                initialState[i] = 1.0 - samplingProbability;
                initialState[numCats+i] = samplingProbability;
            }

        }
        else
        {
            // this is an internal node
            const TopologyNode &left = node.getChild(0);
            size_t leftIndex = left.getIndex();
            computeNodeProbability( left, leftIndex );
            const TopologyNode &right = node.getChild(1);
            size_t rightIndex = right.getIndex();
            computeNodeProbability( right, rightIndex );
            
            // now compute the likelihoods of this internal node
            state_type leftStates = nodeStates[leftIndex][activeLikelihood[leftIndex]];
            state_type rightStates = nodeStates[rightIndex][activeLikelihood[rightIndex]];
            const RbVector<double> &birthRate = lambda->getValue();
            size_t numCats = birthRate.size();
            for (size_t i=0; i<numCats; ++i)
            {
                initialState[i] = leftStates[i];
                initialState[numCats+i] = leftStates[numCats+i]*rightStates[numCats+i]*birthRate[i];
            }
//            computeInternalNodeLikelihood(node,nodeIndex,leftIndex,rightIndex);

        }
        
        BiSSE ode = BiSSE(lambda->getValue(), mu->getValue(), &Q->getValue());
        double beginAge = node.getAge();
        double endAge = node.getParent().getAge();
        boost::numeric::odeint::integrate( ode , initialState , beginAge , endAge , 0.01 );
        
        // store the states
        nodeStates[nodeIndex][activeLikelihood[nodeIndex]] = initialState;
    }
    
}


double MultiRateBirthDeathProcess::computeRootLikelihood( void ) const
{
    
    const TopologyNode &root = value->getRoot();
    
    // fill the like
    const TopologyNode &left = root.getChild(0);
    size_t leftIndex = left.getIndex();
    computeNodeProbability( left, leftIndex );
    const TopologyNode &right = root.getChild(1);
    size_t rightIndex = right.getIndex();
    computeNodeProbability( right, rightIndex );
    
    
    // now compute the likelihoods of this internal node
    state_type leftStates = nodeStates[leftIndex][activeLikelihood[leftIndex]];
    state_type rightStates = nodeStates[rightIndex][activeLikelihood[rightIndex]];
    const RbVector<double> &birthRate = lambda->getValue();
    const RbVector<double> &freqs = pi->getValue();
    size_t numCats = birthRate.size();
    double prob = 0.0;
    for (size_t i=0; i<numCats; ++i)
    {
//        initialState[i] = leftStates[i];
//        initialState[numCats+i] = leftStates[numCats+i]*rightStates[numCats+i]*birthRate[i];
        prob += freqs[i]*leftStates[numCats+i]*rightStates[numCats+i];
    }
    
    return log(prob);
}



double MultiRateBirthDeathProcess::pSurvival(double start, double end) const
{
    
    double samplingProbability = rho->getValue();
    size_t numCats = lambda->getValue().size();
    state_type initialState;
    for (size_t i=0; i<numCats; ++i)
    {
        initialState[i] = 1.0 - samplingProbability;
        initialState[numCats+i] = samplingProbability;
    }
    
    BiSSE ode = BiSSE(lambda->getValue(), mu->getValue(), &Q->getValue());
    boost::numeric::odeint::integrate( ode , initialState , start , end , 0.01 );
    
    
    double prob = 0.0;
    const RbVector<double> &freqs = pi->getValue();
    for (size_t i=0; i<numCats; ++i)
    {
        //        initialState[i] = leftStates[i];
        //        initialState[numCats+i] = leftStates[numCats+i]*rightStates[numCats+i]*birthRate[i];
        prob += freqs[i]*initialState[i];
    }
    
    return 1.0-prob;
}



std::vector<double>* MultiRateBirthDeathProcess::simSpeciations(size_t n, double origin) const
{
    
//    if ( samplingStrategy == "uniform" )
//    {
//        return simSpeciations(n, origin, rho->getValue() );
//    }
//    else
//    {
//        std::vector<double>* all = simSpeciations(round(n/rho->getValue()), origin, 1.0 );
//        all->resize(n);
//        return all;
//    }
    
    return NULL;
}


/**
 * Swap the parameters held by this distribution.
 *
 *
 * \param[in]    oldP      Pointer to the old parameter.
 * \param[in]    newP      Pointer to the new parameter.
 */
void MultiRateBirthDeathProcess::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if ( oldP == lambda )
    {
        lambda = static_cast<const TypedDagNode<RbVector<double> >* >( newP );
    }
    if ( oldP == mu )
    {
        mu = static_cast<const TypedDagNode<RbVector<double> >* >( newP );
    }
    if ( oldP == Q )
    {
        Q = static_cast<const TypedDagNode<RateMatrix>* >( newP );
    }
    if ( oldP == pi )
    {
        pi = static_cast<const TypedDagNode<RbVector<double> >* >( newP );
    }
    
    if ( oldP == rho )
    {
        rho = static_cast<const TypedDagNode<double>* >( newP );
    }
    else
    {
        // delegate the super-class
        AbstractBirthDeathProcess::swapParameterInternal(oldP, newP);
    }
    
}
