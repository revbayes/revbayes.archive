#include "DistributionExponential.h"
#include "PiecewiseConstantFossilizedBirthDeathProcess.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbMathLogic.h"

#include <algorithm>
#include <cmath>

using namespace RevBayesCore;

/**
 * Constructor. 
 * We delegate most parameters to the base class and initialize the members.
 *
 * \param[in]    s              Speciation rates.
 * \param[in]    e              Extinction rates.
 * \param[in]    p              Fossil sampling rates.
 * \param[in]    r              Instantaneous sampling probabilities.
 * \param[in]    t              Rate change times.
 * \param[in]    cdt            Condition of the process (none/survival/#Taxa).
 * \param[in]    tn             Taxa.
 * \param[in]    c              Clades conditioned to be present.
 */
PiecewiseConstantFossilizedBirthDeathProcess::PiecewiseConstantFossilizedBirthDeathProcess(const TypedDagNode<double> *ra,
                                                                                           const DagNode *s,
                                                                                           const DagNode *e,
                                                                                           const DagNode *p,
                                                                                           const DagNode *r,
                                                                                           const TypedDagNode< RbVector<double> > *t,
                                                                                           bool uo,
                                                                                           const std::string &cdt,
                                                                                           const std::vector<Taxon> &tn ) : AbstractBirthDeathProcess( ra, cdt, tn ),
    times( t ), useOrigin(uo)
{
    const TypedDagNode<RbVector<double> > *tmp_v = dynamic_cast<const TypedDagNode<RbVector<double> >*>(s);
    const TypedDagNode<double> *tmp_c = dynamic_cast<const TypedDagNode<double >*>(s);

    if(tmp_v == NULL && tmp_c == NULL)
    {
        throw(RbException("Speciation rate must be of type RealPos or RealPos[]"));
    }
    else if(tmp_v == NULL)
    {
        homogeneous_lambda = tmp_c;
        addParameter( homogeneous_lambda );
    }
    else
    {
        heterogeneous_lambda = tmp_v;
        if(heterogeneous_lambda->getValue().size() != times->getValue().size() + 1)
        {
            throw(RbException("Number of speciation rates does not match number of epochs (# rate change times + 1)"));
        }

        addParameter( heterogeneous_lambda );
    }

    tmp_v = dynamic_cast<const TypedDagNode<RbVector<double> >*>(e);
    tmp_c = dynamic_cast<const TypedDagNode<double >*>(e);

    if(tmp_v == NULL && tmp_c == NULL)
    {
        throw(RbException("Extinction rate must be of type RealPos or RealPos[]"));
    }
    else if(tmp_v == NULL)
    {
        homogeneous_mu = tmp_c;
        addParameter( homogeneous_mu );
    }
    else
    {
        heterogeneous_mu = tmp_v;
        if(heterogeneous_mu->getValue().size() != times->getValue().size() + 1)
        {
            throw(RbException("Number of extinction rates does not match number of epochs (# rate change times + 1)"));
        }

        addParameter( heterogeneous_mu );
    }

    tmp_v = dynamic_cast<const TypedDagNode<RbVector<double> >*>(p);
    tmp_c = dynamic_cast<const TypedDagNode<double >*>(p);

    if(tmp_v == NULL && tmp_c == NULL)
    {
        throw(RbException("Fossil recovery rate must be of type RealPos or RealPos[]"));
    }
    else if(tmp_v == NULL)
    {
        homogeneous_psi = tmp_c;
        addParameter( homogeneous_psi );
    }
    else
    {
        heterogeneous_psi = tmp_v;
        if(heterogeneous_psi->getValue().size() != times->getValue().size() + 1)
        {
            throw(RbException("Number of fossilization rates does not match number of epochs (# rate change times + 1)"));
        }

        addParameter( heterogeneous_psi );
    }

    tmp_v = dynamic_cast<const TypedDagNode<RbVector<double> >*>(p);
    tmp_c = dynamic_cast<const TypedDagNode<double >*>(p);

    if(tmp_v == NULL && tmp_c == NULL)
    {
        throw(RbException("Sampling probability must be of type RealPos or RealPos[]"));
    }
    else if(tmp_v == NULL)
    {
        homogeneous_rho = tmp_c;
        addParameter( homogeneous_rho );
    }
    else
    {
        heterogeneous_rho = tmp_v;
        if(heterogeneous_rho->getValue().size() != times->getValue().size() + 1)
        {
            throw(RbException("Number of sampling probabilities does not match number of epochs (# rate change times + 1)"));
        }

        addParameter( heterogeneous_rho );
    }

    addParameter( times );
    
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
 * If conditioning on the origin, then return the age of the root node
 * or zero if the tree is empty
 */
double PiecewiseConstantFossilizedBirthDeathProcess::getRootAge( void ) const
{
    if(useOrigin)
    {
        if(value->getNumberOfNodes() > 0)
        {
            return value->getRoot().getAge();
        }
        else
        {
            return 0;
        }
    }
    else
        return getOriginTime();
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
    double org = root_age->getValue();
    
    // retrieved the speciation times
    std::vector<double>* agesInts  = getAgesOfInternalNodesFromMostRecentSample();
    std::vector<double>* agesTips  = getAgesOfTipsFromMostRecentSample();
    
    // num_taxa == tips.size() + ancs.size()
    // for the tip ages
    for (size_t i = 0; i < (*agesTips).size(); ++i)
    {
        if ( RbMath::isFinite(lnProbTimes) == false )
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
        if ( RbMath::isFinite(lnProbTimes) == false )
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
        if ( RbMath::isFinite(lnProbTimes) == false )
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
        if ( RbMath::isFinite(lnProbTimes) == false )
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
void PiecewiseConstantFossilizedBirthDeathProcess::prepareProbComputation( void ) const
{
    // clean all the sets
    birth.clear();
    death.clear();
    fossil.clear();
    sampling.clear();
    
    rateChangeTimes = times->getValue();

    for (size_t i = 0; i < rateChangeTimes.size() + 1; i++)
    {
        birth.push_back( getSpeciationRate(i) );

        death.push_back( getExtinctionRate(i) );

        fossil.push_back( getFossilizationRate(i) );

        sampling.push_back( getSamplingProbability(i) );
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


double PiecewiseConstantFossilizedBirthDeathProcess::getExtinctionRate( size_t index ) const
{

    // remove the old parameter first
    if ( homogeneous_mu != NULL )
    {
        return homogeneous_mu->getValue();
    }
    else
    {
        if(index > heterogeneous_mu->getValue().size())
        {
            throw(RbException("Extinction rate index out of bounds"));
        }
        return heterogeneous_mu->getValue()[index];
    }
}


double PiecewiseConstantFossilizedBirthDeathProcess::getFossilizationRate( size_t index ) const
{

    // remove the old parameter first
    if ( homogeneous_psi != NULL )
    {
        return homogeneous_psi->getValue();
    }
    else
    {
        if(index > heterogeneous_psi->getValue().size())
        {
            throw(RbException("Fossil recovery rate index out of bounds"));
        }
        return heterogeneous_psi->getValue()[index];
    }
}


double PiecewiseConstantFossilizedBirthDeathProcess::getSamplingProbability( size_t index ) const
{

    // remove the old parameter first
    if ( homogeneous_rho != NULL )
    {
        return homogeneous_rho->getValue();
    }
    else
    {
        if(index > heterogeneous_rho->getValue().size())
        {
            throw(RbException("Sampling probability index out of bounds"));
        }
        return heterogeneous_rho->getValue()[index];
    }
}


double PiecewiseConstantFossilizedBirthDeathProcess::getSpeciationRate( size_t index ) const
{

    // remove the old parameter first
    if ( homogeneous_lambda != NULL )
    {
        return homogeneous_lambda->getValue();
    }
    else
    {
        if(index > heterogeneous_lambda->getValue().size())
        {
            throw(RbException("Speciation rate index out of bounds"));
        }
        return heterogeneous_lambda->getValue()[index];
    }
}


/**
 * Simulate new speciation times.
 */
double PiecewiseConstantFossilizedBirthDeathProcess::simulateDivergenceTime(double origin, double present) const
{
    // incorrect placeholder for constant FBDP
    // previous simSpeciations did not generate trees with defined likelihoods
    
    
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // get the parameters
    double age = present - origin;
    double b = getSpeciationRate(0);
    double d = getExtinctionRate(0);
    double r = getSamplingProbability(0);
    
    
    // get a random draw
    double u = rng->uniform01();
    
    // compute the time for this draw
    double t = 0.0;
    if ( b > d )
    {
        t = ( log( ( (b-d) / (1 - (u)*(1-((b-d)*exp((d-b)*age))/(r*b+(b*(1-r)-d)*exp((d-b)*age) ) ) ) - (b*(1-r)-d) ) / (r * b) ) + (d-b)*age )  /  (d-b);
    }
    else
    {
        t = ( log( ( (b-d) / (1 - (u)*(1-(b-d)/(r*b*exp((b-d)*age)+(b*(1-r)-d) ) ) ) - (b*(1-r)-d) ) / (r * b) ) + (d-b)*age )  /  (d-b);
    }
    
    
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
    if (oldP == heterogeneous_lambda)
    {
        heterogeneous_lambda = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    else if (oldP == heterogeneous_mu)
    {
        heterogeneous_mu = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    else if (oldP == heterogeneous_psi)
    {
        heterogeneous_psi = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    else if (oldP == heterogeneous_rho)
    {
        heterogeneous_rho = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    else if (oldP == homogeneous_lambda)
    {
        homogeneous_lambda = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == homogeneous_mu)
    {
        homogeneous_mu = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == homogeneous_psi)
    {
        homogeneous_psi = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == homogeneous_rho)
    {
        homogeneous_rho = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == times)
    {
        times = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    else
    {
        // delegate the super-class
        AbstractBirthDeathProcess::swapParameterInternal(oldP, newP);
    }
}
