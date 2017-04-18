#include "DistributionExponential.h"
#include "PiecewiseConstantFossilizedBirthDeathRangeProcess.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbMathLogic.h"
#include "StochasticNode.h"
#include "TypedDistribution.h"

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
 * \param[in]    c              Fossil observation counts.
 * \param[in]    r              Instantaneous sampling probabilities.
 * \param[in]    t              Rate change times.
 * \param[in]    cdt            Condition of the process (none/survival/#Taxa).
 * \param[in]    tn             Taxa.
 */
PiecewiseConstantFossilizedBirthDeathRangeProcess::PiecewiseConstantFossilizedBirthDeathRangeProcess(const DagNode *inspeciation,
                                                                                                     const DagNode *inextinction,
                                                                                                     const DagNode *inpsi,
                                                                                                     const DagNode *incounts,
                                                                                                     const TypedDagNode<double> *inrho,
                                                                                                     const TypedDagNode< RbVector<double> > *intimes,
                                                                                                     const std::string &incondition,
                                                                                                     const std::vector<Taxon> &intaxa ) : TypedDistribution<RbVector<RbVector<double> > >(new RbVector<RbVector<double> > (intaxa.size(), RbVector<double>(2))),
    homogeneous_rho(inrho), timeline( intimes ), condition(incondition), taxa(intaxa)
{
    // initialize all the pointers to NULL
    homogeneous_lambda   = NULL;
    homogeneous_mu       = NULL;
    homogeneous_psi      = NULL;
    homogeneous_fossil_counts   = NULL;
    heterogeneous_lambda = NULL;
    heterogeneous_mu     = NULL;
    heterogeneous_psi    = NULL;
    heterogeneous_fossil_counts = NULL;

    const TypedDagNode<RbVector<double> > *tmp_v = dynamic_cast<const TypedDagNode<RbVector<double> >*>(inspeciation);
    const TypedDagNode<double> *tmp_c = dynamic_cast<const TypedDagNode<double >*>(inspeciation);

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
        if(heterogeneous_lambda->getValue().size() != timeline->getValue().size() + 1)
        {
            std::stringstream ss;
            ss << "Number of speciation rates (" << heterogeneous_lambda->getValue().size() << ") does not match number of time intervals (" << timeline->getValue().size() + 1 << ")";
            throw(RbException(ss.str()));
        }

        addParameter( heterogeneous_lambda );
    }

    tmp_v = dynamic_cast<const TypedDagNode<RbVector<double> >*>(inextinction);
    tmp_c = dynamic_cast<const TypedDagNode<double >*>(inextinction);

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
        if(heterogeneous_mu->getValue().size() != timeline->getValue().size() + 1)
        {
            std::stringstream ss;
            ss << "Number of extinction rates (" << heterogeneous_mu->getValue().size() << ") does not match number of time intervals (" << timeline->getValue().size() + 1 << ")";
            throw(RbException(ss.str()));
        }

        addParameter( heterogeneous_mu );
    }

    const TypedDagNode<RbVector<int> > *tmp_cv = dynamic_cast<const TypedDagNode<RbVector<int> >*>(incounts);
    const TypedDagNode<int > *tmp_cc = dynamic_cast<const TypedDagNode<int >*>(incounts);

    if(tmp_cv == NULL && tmp_cc == NULL)
    {
        throw(RbException("Fossil counts rate must be of type Int or Int[]"));
    }
    else if(tmp_cv == NULL)
    {
        homogeneous_fossil_counts = tmp_cc;
        addParameter( homogeneous_fossil_counts );


        homogeneous_psi = dynamic_cast<const TypedDagNode<double >*>(inpsi);
        if( homogeneous_psi == NULL )
        {
            throw(RbException("Heterogeneous fossil sampling rates provided, but homogeneous fossil counts"));
        }
        addParameter( homogeneous_psi );
    }
    else
    {
        heterogeneous_fossil_counts = tmp_cv;
        if(heterogeneous_fossil_counts->getValue().size() != timeline->getValue().size() + 1)
        {
            std::stringstream ss;
            ss << "Number of fossil counts (" << heterogeneous_fossil_counts->getValue().size() << ") does not match number of time intervals (" << timeline->getValue().size() + 1 << ")";
            throw(RbException(ss.str()));
        }
        addParameter( heterogeneous_fossil_counts );


        heterogeneous_psi = dynamic_cast<const TypedDagNode<RbVector<double> >*>(inpsi);
        if( heterogeneous_psi == NULL )
        {
            homogeneous_psi = static_cast<const TypedDagNode<double >*>(inpsi);
            addParameter( homogeneous_psi );
        }
        else
        {
            if(heterogeneous_psi->getValue().size() != timeline->getValue().size() + 1)
            {
                std::stringstream ss;
                ss << "Number of fossil sampling rates (" << heterogeneous_psi->getValue().size() << ") does not match number of time intervals (" << timeline->getValue().size() + 1 << ")";
                throw(RbException(ss.str()));
            }
            addParameter( heterogeneous_psi );
        }
    }

    addParameter( homogeneous_rho );
    addParameter( timeline );
    
    redrawValue();
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself 
 */
PiecewiseConstantFossilizedBirthDeathRangeProcess* PiecewiseConstantFossilizedBirthDeathRangeProcess::clone( void ) const
{
    return new PiecewiseConstantFossilizedBirthDeathRangeProcess( *this );
}


/**
 * Compute the log-transformed probability of the current value under the current parameter values.
 *
 */
double PiecewiseConstantFossilizedBirthDeathRangeProcess::computeLnProbability( void )
{
    // prepare the probability computation
    prepareProbComputation();

    // variable declarations and initialization
    double lnProbTimes = 0.0;
    
    size_t num_extant_sampled = 0;
    size_t num_extant_unsampled = 0;

    double maxb = 0;
    double maxl = 0;
    // add the fossil tip age terms
    for (size_t i = 0; i < taxa.size(); ++i)
    {
        if ( RbMath::isFinite(lnProbTimes) == false )
        {
            return RbConstants::Double::nan;
        }
        
        double b = (*this->value)[i][0];
        double d = (*this->value)[i][1];
        double o = taxa[i].getAgeRange().getEnd();
        double y = taxa[i].getAgeRange().getStart();

        // check constraints
        if ( !( b > o && o >= y && (y > d || (y == d && y == 0.0)) ) )
        {
            return RbConstants::Double::nan;
        }

        num_extant_sampled  += (d == 0.0 && y == 0.0);  // l
        num_extant_unsampled += (d == 0.0 && y != 0.0); // n - m - l

        size_t bi = l(b);
        size_t di = l(d);
        size_t oi = l(o);

        if(b > maxb)
        {
            maxb = b;
            maxl = birth[ bi - 1 ];
        }

        if(d > 0.0) lnProbTimes += log( death[ di - 1] );

        lnProbTimes += log(birth[bi - 1]);
        lnProbTimes += log(gamma(i));
        lnProbTimes += log(q(oi, o, true)) + log(q(bi, b)) - log(q(di, d, true)) - log(q(oi, o));

        for(size_t j = bi; j < oi; j++)
        {
            lnProbTimes += log(q(j+1, times[j-1]));
        }
        for(size_t j = oi; j < di; j++)
        {
            lnProbTimes += log(q(j+1, times[j-1], true));
        }
    }
    
    // the origin is not a speciation event
    lnProbTimes -= log(maxl);

    for (size_t i = 0; i < fossil.size(); ++i)
    {
        lnProbTimes += counts[i]*log(fossil[i]);
    }

    // add the sampled extant tip age term
    if( homogeneous_rho->getValue() > 0.0)
        lnProbTimes += num_extant_sampled * log( homogeneous_rho->getValue() );
    // add the unsampled extant tip age term
    if( homogeneous_rho->getValue() < 1.0)
        lnProbTimes += num_extant_unsampled * log( 1.0 - homogeneous_rho->getValue() );

    

    // condition on survival
    if ( condition == "survival" )
    {
        lnProbTimes -= log( pSurvival(maxb,0) );
    }
    
    if ( RbMath::isFinite(lnProbTimes) == false )
    {
        return RbConstants::Double::nan;
    }


    return lnProbTimes;
}


/**
 * Compute the diversity of the tree at time t.
 *
 * \param[in]    t      time at which we want to know the diversity.
 *
 * \return The diversity (number of species in the reconstructed tree).
 */
int PiecewiseConstantFossilizedBirthDeathRangeProcess::gamma(size_t index) const
{
    int g = 0;
    for(size_t i = 0; i < taxa.size(); i++)
    {
        if(i == index) continue;

        double b = (*this->value)[index][0];

        if(b < (*this->value)[i][0] && b > (*this->value)[i][1] )
        {
            g++;
        }
    }
    if(g == 0) g = 1;

    return g;
}


double PiecewiseConstantFossilizedBirthDeathRangeProcess::getExtinctionRate( size_t index ) const
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


int PiecewiseConstantFossilizedBirthDeathRangeProcess::getFossilCount( size_t index ) const
{

    // remove the old parameter first
    if ( homogeneous_fossil_counts != NULL )
    {
        return homogeneous_fossil_counts->getValue();
    }
    else
    {
        if(index > heterogeneous_fossil_counts->getValue().size())
        {
            throw(RbException("Fossil count index out of bounds"));
        }
        return heterogeneous_fossil_counts->getValue()[index];
    }
}


double PiecewiseConstantFossilizedBirthDeathRangeProcess::getFossilizationRate( size_t index ) const
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
            throw(RbException("Fossil sampling rate index out of bounds"));
        }
        return heterogeneous_psi->getValue()[index];
    }
}


double PiecewiseConstantFossilizedBirthDeathRangeProcess::getSpeciationRate( size_t index ) const
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
 * return the index i so that t_{i-1} > t >= t_i
 * where t_i is the instantaneous sampling time (i = 0,...,l)
 * t_0 is origin
 * t_l = 0.0
 */
size_t PiecewiseConstantFossilizedBirthDeathRangeProcess::l(double t) const
{
    return times.rend() - std::upper_bound( times.rbegin(), times.rend(), t) + 1;
}


/**
 * p_i(t)
 */
double PiecewiseConstantFossilizedBirthDeathRangeProcess::p( size_t i, double t ) const
{
    if ( t == 0) return 1.0;

    // get the parameters
    double b = birth[i-1];
    double d = death[i-1];
    double f = fossil[i-1];
    double r = (i == times.size() ? homogeneous_rho->getValue() : 0.0);
    double ti = times[i-1];
    
    double diff = b - d - f;
    double bp   = b*f;
    double dt   = t - ti;
    
    double A = sqrt( diff*diff + 4.0*bp);
    double B = ( (1.0 - 2.0*(1.0-r)*p(i+1,ti) )*b + d + f ) / A;
    
    double e = exp(-A*dt);
    double tmp = b + d + f - A * ((1.0+B)-e*(1.0-B))/((1.0+B)+e*(1.0-B));
    
    return tmp / (2.0*b);
}


/**
 * Compute the probability of survival if the process starts with one species at time start and ends at time end.
 *
 * \param[in]    start      Start time of the process.
 * \param[in]    end        End/stopping time of the process.
 *
 * \return Probability of survival.
 */
double PiecewiseConstantFossilizedBirthDeathRangeProcess::pSurvival(double start, double end) const
{
    double t = start;

    std::vector<double> fossil_bak = fossil;

    //std::fill(fossil.begin(), fossil.end(), 0.0);

    double p0 = p(l(t), t);

    //fossil = fossil_bak;

    return 1.0 - p0;
}


/**
 *
 *
 */
void PiecewiseConstantFossilizedBirthDeathRangeProcess::prepareProbComputation( void ) const
{
    // clean all the sets
    birth.clear();
    death.clear();
    fossil.clear();
    counts.clear();
    times.clear();
    
    if(timeline != NULL)
        times = timeline->getValue();

    times.push_back(0.0);

    // put times in descending order
    std::sort(times.rbegin(), times.rend());

    for (size_t i = 0; i < times.size(); i++)
    {
        birth.push_back( getSpeciationRate(i) );

        death.push_back( getExtinctionRate(i) );

        fossil.push_back( getFossilizationRate(i) );

        counts.push_back( getFossilCount(i) );
    }
}


/**
 * q_i(t)
 */
double PiecewiseConstantFossilizedBirthDeathRangeProcess::q( size_t i, double t, bool tilde ) const
{
    
    if ( t == 0.0 ) return 1.0;
    
    // get the parameters
    double b = birth[i-1];
    double d = death[i-1];
    double f = fossil[i-1];
    double r = (i == times.size() ? homogeneous_rho->getValue() : 0.0);
    double ti = times[i-1];
    
    double diff = b - d - f;
    double bp   = b*f;
    double dt   = t - ti;

    double A = sqrt( diff*diff + 4.0*bp);
    double B = ( (1.0 - 2.0*(1.0-r)*p(i+1,ti) )*b + d + f ) / A;

    double e = exp(-A*dt);
    double tmp = (1.0+B) + e*(1.0-B);

    double q = 4.0*e / (tmp*tmp);

    if(tilde) q = sqrt(q*exp(-(b+d+f)*dt));
    
    return q;
}


/**
 * Simulate new speciation times.
 */
void PiecewiseConstantFossilizedBirthDeathRangeProcess::redrawValue(void)
{
    // incorrect placeholder
    
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    double max = 0;
    // get the max first occurence
    for(size_t i = 0; i < taxa.size(); i++)
    {
        double o = taxa[i].getAgeRange().getEnd();
        if( o > max ) max = o;
    }
    
    max *= 2;
    
    // get random uniform draws
    for(size_t i = 0; i < taxa.size(); i++)
    {
        double b = taxa[i].getAgeRange().getEnd() + rng->uniform01()*(max - taxa[i].getAgeRange().getEnd());
        double d = rng->uniform01()*taxa[i].getAgeRange().getStart();

        (*this->value)[i][0] = b;
        (*this->value)[i][1] = d;
    }
}


/**
 * Swap the parameters held by this distribution.
 * 
 * \param[in]    oldP      Pointer to the old parameter.
 * \param[in]    newP      Pointer to the new parameter.
 */
void PiecewiseConstantFossilizedBirthDeathRangeProcess::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
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
    else if (oldP == timeline)
    {
        timeline = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    else if (oldP == homogeneous_fossil_counts)
    {
        homogeneous_fossil_counts = static_cast<const TypedDagNode< int >* >( newP );
    }
    else if (oldP == heterogeneous_fossil_counts)
    {
        heterogeneous_fossil_counts = static_cast<const TypedDagNode< RbVector<int> >* >( newP );
    }
}
