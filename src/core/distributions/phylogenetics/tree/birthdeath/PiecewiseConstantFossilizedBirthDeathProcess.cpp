#include "DistributionExponential.h"
#include "PiecewiseConstantFossilizedBirthDeathProcess.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"

#include <algorithm>
#include <cmath>

using namespace RevBayesCore;

/**
 * Constructor. 
 * We delegate most parameters to the base class and initialize the members.
 *
 * \param[in]    o              Time of the origin/present/length of the process.
 * \param[in]    s              Speciation rates.
 * \param[in]    st             Speciation rate-change times.
 * \param[in]    e              Extinction rates.
 * \param[in]    et             Extinction rate-change times.
 * \param[in]    p              Fossil sampling rates.
 * \param[in]    pt             Fossil sampling rate-change times.
 * \param[in]    r              Instantaneous sampling probabilities.
 * \param[in]    rt             Instantaneous sampling times.
 * \param[in]    cdt            Condition of the process (none/survival/#Taxa).
 * \param[in]    tn             Taxa.
 * \param[in]    c              Clades conditioned to be present.
 */
PiecewiseConstantFossilizedBirthDeathProcess::PiecewiseConstantFossilizedBirthDeathProcess
(
 const TypedDagNode<double> *o, const TypedDagNode<double> *ra,
 const TypedDagNode< RbVector<double> > *s, const TypedDagNode< RbVector<double> > *st,
 const TypedDagNode< RbVector<double> > *e, const TypedDagNode< RbVector<double> > *et,
 const TypedDagNode< RbVector<double> > *p, const TypedDagNode< RbVector<double> > *pt,
 const TypedDagNode< RbVector<double> > *r, const TypedDagNode< RbVector<double> > *rt,
 const std::string &cdt, const std::vector<Taxon> &tn ): AbstractBirthDeathProcess( o, ra, cdt, tn ),
    lambda( s ), 
    lambdaTimes( st ), 
    mu( e ), 
    muTimes( et ),
    psi( p ), 
    psiTimes( pt ),
    rho( r ),
    rhoTimes( rt )
{
    addParameter( lambda );
    addParameter( lambdaTimes );
    addParameter( mu );
    addParameter( muTimes );
    addParameter( psi );
    addParameter( psiTimes );
    addParameter( rho );
    addParameter( rhoTimes );
    
    simulateTree();
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself 
 */
PiecewiseConstantFossilizedBirthDeathProcess* PiecewiseConstantFossilizedBirthDeathProcess::clone( void ) const
{
    return new PiecewiseConstantFossilizedBirthDeathProcess( *this );
}


/**
 * Compute the log probability of the current value under the current parameter values.
 * Tip-dating (Theorem 1, Stadler et al. 2013 PNAS)
 * Ancestral fossils will be considered in the future.
 */
double PiecewiseConstantFossilizedBirthDeathProcess::computeLnProbabilityTimes( void ) const
{
    // variable declarations and initialization
    double lnProbTimes = 0;
    
    // present time 
    double org = origin->getValue();
    
    // retrieved the speciation times
    std::vector<double>* agesInts  = getAgesOfInternalNodesFromMostRecentSample();
    std::vector<double>* agesTips  = getAgesOfTipsFromMostRecentSample();
    
    // numTaxa == tips.size() + ancs.size()
    // for the tip ages
    for (size_t i = 0; i < (*agesTips).size(); ++i)
    {
        if (lnProbTimes == RbConstants::Double::nan ||
            lnProbTimes == RbConstants::Double::inf || 
            lnProbTimes == RbConstants::Double::neginf)
        {
            return RbConstants::Double::nan;
        }
        
        double t = (*agesTips)[i];
        size_t index = l(t);
        if (t == 0.0 || find(rateChangeTimes.begin(), rateChangeTimes.end(), t) != rateChangeTimes.end())
        {
            lnProbTimes += log( sampling[index] );
            if (t > 0.0)
                lnProbTimes += log( p(index, rateChangeTimes[index]));
        }
        else
        {
            lnProbTimes += log( fossil[index] ) + log( p(index, t) / q(index, t) );
        }
    }
    
    // for the internal node ages
    for (size_t i = 0; i < (*agesTips).size() -1; ++i)
    {
        if (lnProbTimes == RbConstants::Double::nan ||
            lnProbTimes == RbConstants::Double::inf || 
            lnProbTimes == RbConstants::Double::neginf) 
        {
            return RbConstants::Double::nan;
        }
        
        double t = (*agesInts)[i];
        size_t index = l(t);
        lnProbTimes += log( q(index, t) * birth[index] );
    }

    // for the fossil ancestor ages
    // TODO: currently use agesTips to compile, will change to agesAncs!!
    for (size_t i = 0; i < (*agesTips).size(); ++i)
    {
        if (lnProbTimes == RbConstants::Double::nan ||
            lnProbTimes == RbConstants::Double::inf ||
            lnProbTimes == RbConstants::Double::neginf)
        {
            return RbConstants::Double::nan;
        }
        
        double t = (*agesTips)[i];
        size_t index = l(t);
        if (find(rateChangeTimes.begin(), rateChangeTimes.end(), t) != rateChangeTimes.end())
        {
            lnProbTimes += log( sampling[index] ) - log( 1 - sampling[index] );
        }
        else
        {
            lnProbTimes += log( fossil[index] );
        }
    }

    // for the degree-two vertices
    for (size_t i = 0; i < rateChangeTimes.size(); ++i)
    {
        if (lnProbTimes == RbConstants::Double::nan ||
            lnProbTimes == RbConstants::Double::inf || 
            lnProbTimes == RbConstants::Double::neginf)
        {
            return RbConstants::Double::nan;
        }
        
        double t = rateChangeTimes[i];
        int div = survivors(t);
        lnProbTimes += div * log( (1.0 - sampling[i+1]) * q(i, t) );
    }
    
    lnProbTimes += log( q(rateChangeTimes.size(), org ) );
    
    delete agesInts;
    delete agesTips;
    
    return lnProbTimes;
}


/**
 * return the index i so that t_i < t <= t_{i+1}
 * where t_i is the instantaneous sampling time (i = 0,...)
 * t_0 is present
 */
size_t PiecewiseConstantFossilizedBirthDeathProcess::l(double t) const
{
    return l(t,0,rateChangeTimes.size());
}


/**
 * recursive
 */
size_t PiecewiseConstantFossilizedBirthDeathProcess::l(double t, size_t min, size_t max) const
{
    size_t middle = min + (max - min) / 2; 
    if ( min == middle ) 
    {
        return ( rateChangeTimes.size() > 0 && rateChangeTimes[max-1] < t ) ? max : min;
    }
    
    if ( rateChangeTimes[middle-1] > t ) 
    {
        return l(t,min,middle);
    }
    else
    {
        return l(t,middle,max);
    }
}


/**
 * Compute the probability of survival if the process starts with one species at time start and ends at time end.
 *
 * \param[in]    start      Start time of the process.
 * \param[in]    end        End/stopping time of the process.
 *
 * \return Probability of survival.
 */
double PiecewiseConstantFossilizedBirthDeathProcess::pSurvival(double start, double end) const
{
    double t = end;
    
    double p0 = p(l(t), t);
    
    return 1.0 - p0;
}


/**
 * p_i(t)
 */
double PiecewiseConstantFossilizedBirthDeathProcess::p( size_t i, double t ) const
{
    if ( t == 0)
        return 1.0;
    
    // get the parameters
    double b = birth[i];
    double d = death[i];
    double f = fossil[i];
    double r = sampling[i];
    double ti = 0.0;
    if (i > 0) {
        ti = rateChangeTimes[i-1];
    }
    
    double diff = b - d - f;
    double bp   = b*f;
    double dt   = ti - t;
    
    double A = sqrt( diff*diff + 4.0*bp);
    double B = ( (1.0 - 2.0*(1.0-r)*p(i-1,ti) )*b + d + f ) / A;
    
    double e = exp(A*dt);
    double tmp = b + d + f - A * ((1.0+B)-e*(1.0-B))/((1.0+B)+e*(1.0-B));
    
    return tmp / (2.0*b);
}


/**
 *
 *
 */
void PiecewiseConstantFossilizedBirthDeathProcess::prepareProbComputation( void )
{
    // clean all the sets
    rateChangeTimes.clear();
    birth.clear();
    death.clear();
    fossil.clear();
    sampling.clear();
    
    std::set<double> eventTimes;  // size(eventTimes) = size(rates) -1
    
    const std::vector<double>& birthTimes = lambdaTimes->getValue();
    for (std::vector<double>::const_iterator it = birthTimes.begin(); it != birthTimes.end(); ++it) 
    {
        eventTimes.insert( *it );
    }
    
    const std::vector<double>& deathTimes = muTimes->getValue();
    for (std::vector<double>::const_iterator it = deathTimes.begin(); it != deathTimes.end(); ++it) 
    {
        eventTimes.insert( *it );
    }
    
    const std::vector<double>& fossilTimes = psiTimes->getValue();
    for (std::vector<double>::const_iterator it = fossilTimes.begin(); it != fossilTimes.end(); ++it) 
    {
        eventTimes.insert( *it );
    }
    
    const std::vector<double>& samplingTimes = rhoTimes->getValue();
    for (std::vector<double>::const_iterator it = samplingTimes.begin(); it != samplingTimes.end(); ++it) 
    {
        eventTimes.insert( *it );
    }
    
    size_t indexBirth = 0;
    size_t indexDeath = 0;
    size_t indexFossil = 0;
    
    const std::vector<double> &b = lambda->getValue();
    const std::vector<double> &d = mu->getValue();
    const std::vector<double> &f = psi->getValue();
    const std::vector<double> &s = rho->getValue();
    
    birth.push_back(    b[0] );  // fill in the first rate at time 0
    death.push_back(    d[0] );
    fossil.push_back(   f[0] );
    sampling.push_back( s[0] );
    
    size_t pos = 0;
    for (std::set<double>::const_iterator it = eventTimes.begin(); it != eventTimes.end(); ++it) 
    {
        double t = *it;
        
        // add the time to our vector
        rateChangeTimes.push_back( t );
        
        // add the speciation rate at the rate-change event t
        pos = size_t( find(birthTimes.begin(), birthTimes.end(), t) - birthTimes.begin() );
        if ( pos != birthTimes.size() ) 
        {
            indexBirth = pos;
        }
        birth.push_back( b[indexBirth+1] );
        
        // add the extinction rate at the rate-change event t
        pos = size_t( find(deathTimes.begin(), deathTimes.end(), t) - deathTimes.begin() );
        if ( pos != deathTimes.size() ) 
        {
            indexDeath = pos;
        }
        death.push_back( d[indexDeath+1] );
        
        // add the fossilization rate at the rate-change event t
        pos = size_t( find(fossilTimes.begin(), fossilTimes.end(), t) - fossilTimes.begin() );
        if ( pos != fossilTimes.size() ) 
        {
            indexFossil = pos;
        }
        fossil.push_back( f[indexFossil+1] );
        
        // add the sampling probability at the rate-change event t
        pos = size_t( find(samplingTimes.begin(), samplingTimes.end(), t) - samplingTimes.begin() );
        if ( pos != samplingTimes.size() ) 
        {
            sampling.push_back( s[pos+1] );
        }
        else
        {
            sampling.push_back( 0.0 );
        }
    }
}


/**
 * q_i(t)
 */
double PiecewiseConstantFossilizedBirthDeathProcess::q( size_t i, double t ) const
{
    
    if ( t == 0.0 ) 
    {
        return 1.0;
    }
    
    // get the parameters
    double b = birth[i];
    double d = death[i];
    double f = fossil[i];
    double r = sampling[i];
    double ti = 0.0;
    if (i > 0) {
        ti = rateChangeTimes[i-1];
    }
    
    double diff = b - d - f;
    double bp   = b*f;
    double dt   = ti - t;
    
    double A = sqrt( diff*diff + 4.0*bp);
    double B = ( (1.0 - 2.0*(1.0-r)*p(i-1,ti) )*b + d + f ) / A;
    
    double e = exp(A*dt);
    double tmp = ((1.0+B)+e*(1.0-B));
    
    return 4.0*e / (tmp*tmp);    
}


/**
 * Simulate new speciation times.
 */
double PiecewiseConstantFossilizedBirthDeathProcess::simulateDivergenceTime(double origin, double present) const
{
    
    
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // get the parameters
    double age = present - origin;
    double b = lambda->getValue()[0];
    double d = mu->getValue()[0];
    double rho = 1.0;
    
    
    // get a random draw
    double u = rng->uniform01();
    
    // compute the time for this draw
    double t = ( log( ( (b-d) / (1 - (u)*(1-((b-d)*exp((d-b)*age))/(rho*b+(b*(1-rho)-d)*exp((d-b)*age) ) ) ) - (b*(1-rho)-d) ) / (rho * b) ) + (d-b)*age )  /  (d-b);
    
    
    return present - t;
}


/**
 * Compute the diversity of the tree at time t.
 *
 * \param[in]    t      time at which we want to know the diversity.
 *
 * \return The diversity (number of species in the reconstructed tree).
 */
int PiecewiseConstantFossilizedBirthDeathProcess::survivors(double t) const
{

    const std::vector<TopologyNode*>& nodes = value->getNodes();
    
    int survivors = 0;
    for (std::vector<TopologyNode*>::const_iterator it = nodes.begin(); it != nodes.end(); ++it) 
    {
        TopologyNode* n = *it;
        if ( n->getAge() < t ) 
        {
            if ( n->isRoot() || n->getParent().getAge() > t ) 
            {
                survivors++;
            } 
        } 
    }
    
    return survivors;
}


/**
 * Swap the parameters held by this distribution.
 * 
 * \param[in]    oldP      Pointer to the old parameter.
 * \param[in]    newP      Pointer to the new parameter.
 */
void PiecewiseConstantFossilizedBirthDeathProcess::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    bool found = false;
    
    if (oldP == lambdaTimes) 
    {
        lambdaTimes = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
        found = true;
    }
    if (oldP == muTimes) 
    {
        muTimes = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
        found = true;
    }
    if (oldP == psiTimes) 
    {
        psiTimes = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
        found = true;
    }
    if (oldP == rhoTimes) 
    {
        rhoTimes = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
        found = true;
    }
    
    if (oldP == lambda) 
    {
        lambda = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    else if (oldP == mu) 
    {
        mu = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    else if (oldP == psi) 
    {
        psi = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    else if (oldP == rho) 
    {
        rho = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    else if (!found)
    {
        // delegate the super-class
        AbstractBirthDeathProcess::swapParameterInternal(oldP, newP);
    }
}
