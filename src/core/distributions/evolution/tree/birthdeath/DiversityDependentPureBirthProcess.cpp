#include "DistributionExponential.h"
#include "DiversityDependentPureBirthProcess.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"

#include <cmath>

using namespace RevBayesCore;


/**
 * Constructor. 
 * We delegate most parameters to the base class and initialize the members.
 *
 * \param[in]    o      Time of the origin/present/length of the process.
 * \param[in]    s      Initial speciation rate (lambda_0).
 * \param[in]    k      Carrying capacity.
 * \param[in]    cdt    Condition of the process (none/survival/#Taxa).
 * \param[in]    tn     Taxa.
 * \param[in]    c      Clades conditioned to be present.
 */
DiversityDependentPureBirthProcess::DiversityDependentPureBirthProcess(const TypedDagNode<double> *o, const TypedDagNode<double> *ra, const TypedDagNode<double> *s, const TypedDagNode<int> *k,
                                                                       const std::string &cdt, const std::vector<Taxon> &tn, const std::vector<Clade> &c) : AbstractBirthDeathProcess( o, ra, cdt, tn, c ),
        initialSpeciation( s ), 
        capacity( k ) 
{
    simulateTree();
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself 
 */
DiversityDependentPureBirthProcess* DiversityDependentPureBirthProcess::clone( void ) const
{
    
    return new DiversityDependentPureBirthProcess( *this );
}


/**
 * Compute the log-transformed probability of the current value under the current parameter values.
 *
 */
double DiversityDependentPureBirthProcess::computeLnProbabilityTimes( void ) const
{
    
    // variable declarations and initialization
    double lnProbTimes = 0;
    
    // present time 
    double tipTime = value->getTipNode(0).getTime();
    
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
    
    // test that the time of the process is larger or equal to the present time
    if ( tipTime > presentTime )
    {
        return RbConstants::Double::neginf;
    }
    
    
    // add the survival of a second species if we condition on the MRCA
    int numInitialSpecies = 1;
    
    // if we started at the root then we square the survival prob
    if ( startsAtRoot == true )
    {
        ++numInitialSpecies;
    }
    
    // retrieved the speciation times
    std::vector<double>* times = divergenceTimesSinceOrigin();
    
    int n = numInitialSpecies;
    double b = initialSpeciation->getValue();
    int k = capacity->getValue();
    double lastTime = 0.0;
    double speciationRate, timeInterval;
    for (size_t i = numInitialSpecies-1; i < numTaxa-1; ++i)
    {
        if ( lnProbTimes == RbConstants::Double::nan || 
            lnProbTimes == RbConstants::Double::inf || 
            lnProbTimes == RbConstants::Double::neginf ) 
        {
            return RbConstants::Double::nan;
        }
        
        speciationRate = (1.0 - double(n)/k) * b ;
        timeInterval = (*times)[i] - lastTime;
        lastTime = (*times)[i];
        
        lnProbTimes += log(speciationRate) - double(n) * speciationRate * timeInterval;
        ++n;
    }
    speciationRate = double(n) * (1.0 - double(n)/k) * b ;
    timeInterval = presentTime - lastTime;
    lnProbTimes -= speciationRate * timeInterval;
    
    return lnProbTimes;
    
}


/**
 * Compute the probability of survival if the process starts with one species at time start and ends at time end.
 * The general equation for the homogeneous birth-death process is
 *    P(N(T)>0|N(t)=1) = (1+\int{mu*exp[rateIntegral(t,s)]ds})^{-1}
 * Because this process has no extinction, the survival probability is simplified to
 *    P(N(T)>0|N(t)=1) = 1
 * 
 *
 * \param[in]    start      Start time of the process.
 * \param[in]    end        End/stopping time of the process.
 *
 * \return Speciation rate at time t. 
 */
double DiversityDependentPureBirthProcess::pSurvival(double start, double end) const
{
    
    return 1.0;
}


/**
 * Simulate new speciation times.
 */
std::vector<double>* DiversityDependentPureBirthProcess::simSpeciations(size_t n, double origin) const
{
    
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // get the parameters
    double lambda = initialSpeciation->getValue();
    double k = capacity->getValue();
    
    // \todo
    // draw the final event
    // this is not until actually an event happened but a uniform time before the next species would have been sampled.
    
    double lastEvent = 0.0;
    
    std::vector<double> *times = new std::vector<double>(n,0.0);
    for (size_t i = 0; i < n; i++ )
    {
        double rate = ( 1.0 - ((n-i+2)/k) ) * lambda;
        double t = lastEvent + RbStatistics::Exponential::rv(rate, *rng);
        lastEvent = t;
        (*times)[n-i-1] = t;
    }
	
    return times;
}



/** Get the parameters of the distribution */
std::set<const DagNode*> DiversityDependentPureBirthProcess::getParameters( void ) const
{
    std::set<const DagNode*> parameters = AbstractBirthDeathProcess::getParameters();
    
    parameters.insert( initialSpeciation );
    parameters.insert( capacity );
    
    parameters.erase( NULL );
    return parameters;
}


/**
 * Swap the parameters held by this distribution.
 *
 * 
 * \param[in]    oldP      Pointer to the old parameter.
 * \param[in]    newP      Pointer to the new parameter.
 */
void DiversityDependentPureBirthProcess::swapParameter(const DagNode *oldP, const DagNode *newP) 
{
    
    if (oldP == initialSpeciation) 
    {
        initialSpeciation = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == capacity) 
    {
        capacity = static_cast<const TypedDagNode<int>* >( newP );
    }
    else 
    {
        // delegate the super-class
        AbstractBirthDeathProcess::swapParameter(oldP, newP);
    }
    
}
