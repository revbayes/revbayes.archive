#include "DistributionExponential.h"
#include "PiecewiseConstantFossilizedBirthDeathProcess.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbMathCombinatorialFunctions.h"
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
PiecewiseConstantFossilizedBirthDeathProcess::PiecewiseConstantFossilizedBirthDeathProcess(const TypedDagNode<double> *ra,
                                                                                           const DagNode *inspeciation,
                                                                                           const DagNode *inextinction,
                                                                                           const DagNode *inpsi,
                                                                                           const DagNode *incounts,
                                                                                           const TypedDagNode<double> *inrho,
                                                                                           const TypedDagNode< RbVector<double> > *intimes,
                                                                                           const std::string &incondition,
                                                                                           const std::vector<Taxon> &intaxa,
                                                                                           bool uo,
                                                                                           bool pa ) : AbstractBirthDeathProcess(ra, incondition, intaxa, uo),
    ascending(false), homogeneous_rho(inrho), timeline( intimes ), condition(incondition), taxa(intaxa), presence_absence(pa)
{
    // initialize all the pointers to NULL
    homogeneous_lambda             = NULL;
    homogeneous_mu                 = NULL;
    homogeneous_psi                = NULL;
    fossil_counts                  = NULL;
    heterogeneous_lambda           = NULL;
    heterogeneous_mu               = NULL;
    heterogeneous_psi              = NULL;
    interval_fossil_counts         = NULL;
    species_interval_fossil_counts = NULL;

    RbException no_timeline_err = RbException("No time intervals provided for piecewise constant fossilized birth death process");

    heterogeneous_lambda = dynamic_cast<const TypedDagNode<RbVector<double> >*>(inspeciation);
    homogeneous_lambda = dynamic_cast<const TypedDagNode<double >*>(inspeciation);

    addParameter( homogeneous_lambda );
    addParameter( heterogeneous_lambda );

    if ( heterogeneous_lambda == NULL && homogeneous_lambda == NULL)
    {
        throw(RbException("Speciation rate must be of type RealPos or RealPos[]"));
    }
    else if( heterogeneous_lambda != NULL )
    {
        if( timeline == NULL ) throw(no_timeline_err);

        if (heterogeneous_lambda->getValue().size() != timeline->getValue().size() + 1)
        {
            std::stringstream ss;
            ss << "Number of speciation rates (" << heterogeneous_lambda->getValue().size() << ") does not match number of time intervals (" << timeline->getValue().size() + 1 << ")";
            throw(RbException(ss.str()));
        }
    }


    heterogeneous_mu = dynamic_cast<const TypedDagNode<RbVector<double> >*>(inextinction);
    homogeneous_mu = dynamic_cast<const TypedDagNode<double >*>(inextinction);

    addParameter( homogeneous_mu );
    addParameter( heterogeneous_mu );

    if ( heterogeneous_mu == NULL && homogeneous_mu == NULL)
    {
        throw(RbException("Extinction rate must be of type RealPos or RealPos[]"));
    }
    else if( heterogeneous_mu != NULL )
    {
        if( timeline == NULL ) throw(no_timeline_err);

        if (heterogeneous_mu->getValue().size() != timeline->getValue().size() + 1)
        {
            std::stringstream ss;
            ss << "Number of extinction rates (" << heterogeneous_mu->getValue().size() << ") does not match number of time intervals (" << timeline->getValue().size() + 1 << ")";
            throw(RbException(ss.str()));
        }
    }


    heterogeneous_psi = dynamic_cast<const TypedDagNode<RbVector<double> >*>(inpsi);
    homogeneous_psi = dynamic_cast<const TypedDagNode<double >*>(inpsi);

    addParameter( homogeneous_psi );
    addParameter( heterogeneous_psi );

    if ( heterogeneous_psi == NULL && homogeneous_psi == NULL)
    {
        throw(RbException("Fossilization rate must be of type RealPos or RealPos[]"));
    }
    else if( heterogeneous_psi != NULL )
    {
        if( timeline == NULL ) throw(no_timeline_err);

        if (heterogeneous_psi->getValue().size() != timeline->getValue().size() + 1)
        {
            std::stringstream ss;
            ss << "Number of fossilization rates (" << heterogeneous_psi->getValue().size() << ") does not match number of time intervals (" << timeline->getValue().size() + 1 << ")";
            throw(RbException(ss.str()));
        }
    }

    species_interval_fossil_counts = dynamic_cast<const TypedDagNode<RbVector<RbVector<long> > >*>(incounts);
    interval_fossil_counts         = dynamic_cast<const TypedDagNode<RbVector<long> >*>(incounts);
    fossil_counts                  = dynamic_cast<const TypedDagNode<long> *>(incounts);

    addParameter( species_interval_fossil_counts );
    addParameter( interval_fossil_counts );
    addParameter( fossil_counts );

    marginalize_k = ( species_interval_fossil_counts == NULL && interval_fossil_counts == NULL && fossil_counts == NULL);

    if( marginalize_k && presence_absence )
    {
        throw(RbException("Cannot marginalize fossil presence absence data"));
    }

    if( species_interval_fossil_counts == NULL && presence_absence )
    {
        throw(RbException("Presence absence data must be provided by species and interval"));
    }

    if ( fossil_counts != NULL && homogeneous_psi == NULL)
    {
        throw(RbException("Heterogeneous fossil sampling rates provided, but homogeneous fossil counts"));
    }
    else if ( interval_fossil_counts != NULL || species_interval_fossil_counts != NULL )
    {
        if( timeline == NULL ) throw(no_timeline_err);

        if ( interval_fossil_counts != NULL && interval_fossil_counts->getValue().size() != timeline->getValue().size() + 1)
        {
            std::stringstream ss;
            ss << "Number of fossil counts (" << interval_fossil_counts->getValue().size() << ") does not match number of time intervals (" << timeline->getValue().size() + 1 << ")";
            throw(RbException(ss.str()));
        }
        else if ( species_interval_fossil_counts != NULL && species_interval_fossil_counts->getValue().size() != taxa.size())
        {
            std::stringstream ss;
            ss << "Number of species fossil counts (" << species_interval_fossil_counts->getValue().size() << ") does not match number of taxa (" << taxa.size() << ")";
            throw(RbException(ss.str()));
        }
        else if ( species_interval_fossil_counts != NULL && species_interval_fossil_counts->getValue().front().size() != timeline->getValue().size() + 1)
        {
            std::stringstream ss;
            ss << "Number of fossil counts per species (" << species_interval_fossil_counts->getValue().front().size() << ") does not match number of time intervals (" << timeline->getValue().size() + 1 << ")";
            throw(RbException(ss.str()));
        }
    }

    addParameter( homogeneous_rho );
    addParameter( timeline );
    
    num_intervals = timeline == NULL ? 1 : timeline->getValue().size()+1;

    if ( num_intervals > 1 )
    {
        std::vector<double> times = timeline->getValue();
        std::vector<double> times_sorted_ascending = times;
        std::vector<double> times_sorted_descending = times;

        sort(times_sorted_ascending.begin(), times_sorted_ascending.end() );
        sort(times_sorted_descending.rbegin(), times_sorted_descending.rend() );

        if( times == times_sorted_ascending )
        {
            ascending = true;
        }
        else if ( times != times_sorted_descending )
        {
            throw(RbException("Interval times must be provided in order"));
        }
    }

    p_i         = std::vector<double>(num_intervals, 1.0);
    q_i         = std::vector<double>(num_intervals, 1.0);
    q_tilde_i   = std::vector<double>(num_intervals, 1.0);

    birth       = std::vector<double>(num_intervals, 0.0);
    death       = std::vector<double>(num_intervals, 0.0);
    fossil      = std::vector<double>(num_intervals, 0.0);
    times       = std::vector<double>(num_intervals, 0.0);

    oldest_intervals = std::vector<size_t>( taxa.size(), num_intervals - 1 );
    youngest_intervals = std::vector<size_t>( taxa.size(), num_intervals - 1 );

    updateIntervals();

    redrawValue();
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
 * Compute the log-transformed probability of the current value under the current parameter values.
 *
 */
double PiecewiseConstantFossilizedBirthDeathProcess::computeLnProbabilityTimes( void ) const
{
    // prepare the probability computation
    updateIntervals();

    // variable declarations and initialization
    double lnProbTimes = 0.0;
    
    size_t num_extant_sampled = 0;
    size_t num_extant_unsampled = 0;

    double maxb = 0;
    double maxl = 0;

    std::vector<long> kappa_prime (num_intervals, 0);
    std::vector<double> L (num_intervals, 0.0);

    // add the fossil tip age terms
    for (size_t i = 0; i < value->getNumberOfTips(); ++i)
    {
        if ( RbMath::isFinite(lnProbTimes) == false )
        {
            return RbConstants::Double::neginf;
        }

        
        const TopologyNode& node = value->getNode(i);

        double b = getBirthTime( node );
        double d = node.getAge();
        double o = node.getTaxon().getAgeRange().getMax();
        double y = node.getTaxon().getAgeRange().getMin();

        size_t bi = l(b);
        size_t di = l(d);
        size_t oi = presence_absence ? oldest_intervals[i] : l(o);
        size_t yi = presence_absence ? youngest_intervals[i] : l(y);


        // check constraints
        if( presence_absence )
        {
            if( !( b > d && (( y == 0.0 && d == 0.0 ) || ( y != 0.0 && d >= 0.0 && yi <= di )) ) )
            {
                return RbConstants::Double::neginf;
            }
        }
        else if ( !( b > o && o >= y && (y > d || (y == d && y == 0.0)) && d >= 0.0 ) )
        {
            return RbConstants::Double::neginf;
        }


        // count the number of rho-sampled tips
        num_extant_sampled  += (d == 0.0 && y == 0.0);  // l
        num_extant_unsampled += (d == 0.0 && y != 0.0); // n - m - l


        // find the origin time
        if (b > maxb)
        {
            maxb = b;
            maxl = birth[bi];
        }


        // include speciation density
        lnProbTimes += log( birth[bi] );

        // multiply by q at the birth time
        lnProbTimes += log( q(bi, b) );

        // include intermediate q terms
        for (size_t j = bi; j < oi; j++)
        {
            lnProbTimes += log( q_i[j] );
        }

        // include factor for the first appearance
        if( presence_absence == false )
        {
            lnProbTimes += log( q(oi, o, true) ) - log( q(oi, o) );
        }

        // include intermediate q_tilde terms
        for (size_t j = oi; j < di; j++)
        {
            lnProbTimes += log( q_tilde_i[j] );
        }

        // divide by q_tilde at the death time
        lnProbTimes -= log( q( di, d, true) );

        // include extinction density
        if (d > 0.0) lnProbTimes += log( death[di] );


        // update the marginalized fossil count data
        if( marginalize_k )
        {
            if( o > 0.0 )
            {
                kappa_prime[oi]++;
            }
            if( o != y && y > 0.0)
            {
                kappa_prime[yi]++;
            }

            double y_star = std::max(y, times[oi]);

            L[oi] += o - y_star;

            for(size_t j = oi + 1; j <= yi; j++)
            {
                L[j] += times[j-1] - std::max(y, times[j]);
            }
        }
        else if( presence_absence )
        {
            if( getFossilCount(oi, i) )
            {
                double o_star = oi > 0 ? std::min(b, times[oi-1]) : b;
                double y_star = std::max(d, times[oi]);

                double t_alpha = times[oi];

                double H = integrateQ(oi, o_star) - integrateQ(oi, y_star);

                L[oi] += log( H ) + log( fossil[oi] ) - fossil[oi]*( y_star - t_alpha );

                for(size_t j = oi + 1; j <= yi; j++)
                {
                    if( getFossilCount(j, i) )
                    {
                        double Ls = times[j-1] - std::max(d, times[j]);

                        L[j] += fossil[j]*Ls + log( 1.0 - exp( - Ls * fossil[j] ) );
                    }
                }
            }
        }
    }
    
    // the origin is not a speciation event
    lnProbTimes -= log(maxl);

    // check if we marginalize over fossil counts
    for (size_t i = 0; i < num_intervals; ++i)
    {
        if( presence_absence )
        {
            lnProbTimes += L[i];
        }
        else
        {
            size_t k;

            if( marginalize_k )
            {
                k = kappa_prime[i];

                lnProbTimes += fossil[i]*L[i];
            }
            else
            {
                k = getFossilCount(i);
            }

            lnProbTimes += k*log(fossil[i]);
        }
    }

    // add the sampled extant tip age term
    if ( homogeneous_rho->getValue() > 0.0)
        lnProbTimes += num_extant_sampled * log( homogeneous_rho->getValue() );
    // add the unsampled extant tip age term
    if ( homogeneous_rho->getValue() < 1.0)
        lnProbTimes += num_extant_unsampled * log( 1.0 - homogeneous_rho->getValue() );

    // condition on survival
    if ( condition == "survival" )
    {
        lnProbTimes -= log( pSurvival(maxb,0) );
    }

    if ( RbMath::isFinite(lnProbTimes) == false )
    {
        return RbConstants::Double::neginf;
    }


    return lnProbTimes;
}


double PiecewiseConstantFossilizedBirthDeathProcess::getBirthTime( const TopologyNode& node ) const
{
    if( node.isRoot() )
    {
        return getOriginAge();
    }
    else
    {
        const TopologyNode& parent = node.getParent();

        // define left child as ancestral species
        if( node.getIndex() == parent.getChild(0).getIndex() )
        {
            return getBirthTime( parent );
        }
        else
        {
            return parent.getAge();
        }
    }
}


double PiecewiseConstantFossilizedBirthDeathProcess::getMaxTaxonAge( const TopologyNode& node ) const
{
    if( node.isTip() )
    {
        return node.getTaxon().getAgeRange().getMax();
    }
    else
    {
        double max = 0;

        for( size_t i = 0; i < node.getNumberOfChildren(); i++)
        {
            max = std::max( getMaxTaxonAge( node.getChild(i) ), max );
        }

        return max;
    }
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
        size_t num = heterogeneous_mu->getValue().size();

        if (index >= num)
        {
            throw(RbException("Extinction rate index out of bounds"));
        }
        return ascending ? heterogeneous_mu->getValue()[num - 1 - index] : heterogeneous_mu->getValue()[index];
    }
}


long PiecewiseConstantFossilizedBirthDeathProcess::getFossilCount( size_t interval, size_t species ) const
{

    // remove the old parameter first
    if ( fossil_counts != NULL )
    {
        return fossil_counts->getValue();
    }
    else if( interval_fossil_counts != NULL)
    {
        size_t num = interval_fossil_counts->getValue().size();

        if (interval >= num)
        {
            throw(RbException("Fossil count index out of bounds"));
        }
        return ascending ? interval_fossil_counts->getValue()[num - 1 - interval] : interval_fossil_counts->getValue()[interval];
    }
    else if( species_interval_fossil_counts != NULL )
    {
        size_t num = species_interval_fossil_counts->getValue().size();

        if (species >= num)
        {
            throw(RbException("Fossil count index out of bounds"));
        }

        num = species_interval_fossil_counts->getValue()[species].size();

        if (interval >= num)
        {
            throw(RbException("Fossil count index out of bounds"));
        }
        return ascending ? species_interval_fossil_counts->getValue()[species][num - 1 - interval] : species_interval_fossil_counts->getValue()[species][interval];
    }

    throw(RbException("Fossil counts have been marginalized"));
}


long PiecewiseConstantFossilizedBirthDeathProcess::getFossilCount( size_t interval ) const
{

    // remove the old parameter first
    if ( fossil_counts != NULL )
    {
        return fossil_counts->getValue();
    }
    else if( interval_fossil_counts != NULL)
    {
        size_t num = interval_fossil_counts->getValue().size();

        if (interval >= num)
        {
            throw(RbException("Fossil count index out of bounds"));
        }
        return ascending ? interval_fossil_counts->getValue()[num - 1 - interval] : interval_fossil_counts->getValue()[interval];
    }
    else if( species_interval_fossil_counts != NULL )
    {
        size_t num = species_interval_fossil_counts->getValue().front().size();

        if (interval >= num)
        {
            throw(RbException("Fossil count index out of bounds"));
        }

        long total = 0;

        for(size_t i = 0; i < species_interval_fossil_counts->getValue().size(); i++)
        {
            total += ascending ? species_interval_fossil_counts->getValue()[i][num - 1 - interval] : species_interval_fossil_counts->getValue()[i][interval];
        }

        return total;
    }

    throw(RbException("Fossil counts have been marginalized"));
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
        size_t num = heterogeneous_psi->getValue().size();

        if (index >= num)
        {
            throw(RbException("Fossil sampling rate index out of bounds"));
        }
        return ascending ? heterogeneous_psi->getValue()[num - 1 - index] : heterogeneous_psi->getValue()[index];
    }
}


double PiecewiseConstantFossilizedBirthDeathProcess::getIntervalTime( size_t index ) const
{

    if ( index == num_intervals - 1 )
    {
        return 0.0;
    }
    // remove the old parameter first
    else if ( timeline != NULL )
    {
        size_t num = timeline->getValue().size();

        if (index >= num)
        {
            throw(RbException("Interval time index out of bounds"));
        }
        return ascending ? timeline->getValue()[num - 1 - index] : timeline->getValue()[index];
    }
    else
    {
        throw(RbException("Interval time index out of bounds"));
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
        size_t num = heterogeneous_lambda->getValue().size();

        if (index >= num)
        {
            throw(RbException("Speciation rate index out of bounds"));
        }
        return ascending ? heterogeneous_lambda->getValue()[num - 1 - index] : heterogeneous_lambda->getValue()[index];
    }
}


/**
 * \ln\int exp(psi t) q_tilde(t)/q(t) dt
 */
double PiecewiseConstantFossilizedBirthDeathProcess::integrateQ( size_t i, double t ) const
{
    // get the parameters
    double b = birth[i];
    double d = death[i];
    double f = fossil[i];
    double r = (i == num_intervals - 1 ? homogeneous_rho->getValue() : 0.0);
    double ti = times[i];

    double diff = b - d - f;
    double bp   = b*f;
    double dt   = t - ti;

    double A = sqrt( diff*diff + 4.0*bp);
    double B = ( (1.0 - 2.0*(1.0-r)*p_i[i] )*b + d + f ) / A;

    double e = exp(-A*dt);

    double diff2 = b + d - f;
    double tmp = (1+B)/(A-diff2) - e*(1-B)/(A+diff2);
    double intQ = exp(-(diff2-A)*dt/2) * tmp;

//    double tmp = (1.0+B) + e*(1.0-B);
//    double q = 4.0*e / (tmp*tmp);
//    double q_tilde = sqrt(q*exp(-(b+d+f)*dt));
//    double p = b + d + f - A * ((1.0+B)-e*(1.0-B))/((1.0+B)+e*(1.0-B));
//    p /= 2*b;
//    double intQ = q_tilde/q * exp(f*dt) * ( b*p - f)/(b*d-f*d-b*f);

    return intQ;
}


/**
 * return the index i so that t_{i-1} > t >= t_i
 * where t_i is the instantaneous sampling time (i = 0,...,l)
 * t_0 is origin
 * t_l = 0.0
 */
size_t PiecewiseConstantFossilizedBirthDeathProcess::l(double t) const
{
    return times.rend() - std::upper_bound( times.rbegin(), times.rend(), t);
}


double PiecewiseConstantFossilizedBirthDeathProcess::lnProbTreeShape(void) const
{
    // the fossilized birth death divergence times density is derived for an unlabeled oriented tree
    // so we convert to a labeled oriented tree probability by multiplying by 1 / n!
    // where n is the number of extant tips

    return - RbMath::lnFactorial( value->getNumberOfExtantTips() );
}


/**
 * p_i(t)
 */
double PiecewiseConstantFossilizedBirthDeathProcess::p( size_t i, double t ) const
{
    if ( t == 0) return 1.0;

    // get the parameters
    double b = birth[i];
    double d = death[i];
    double f = fossil[i];
    double r = (i == num_intervals - 1 ? homogeneous_rho->getValue() : 0.0);
    double ti = times[i];
    
    double diff = b - d - f;
    double bp   = b*f;
    double dt   = t - ti;
    
    double A = sqrt( diff*diff + 4.0*bp);
    double B = ( (1.0 - 2.0*(1.0-r)*p_i[i] )*b + d + f ) / A;
    
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
double PiecewiseConstantFossilizedBirthDeathProcess::pSurvival(double start, double end) const
{
    double t = start;

    //std::vector<double> fossil_bak = fossil;

    //std::fill(fossil.begin(), fossil.end(), 0.0);

    double p0 = p(l(t), t);

    //fossil = fossil_bak;

    return 1.0 - p0;
}


/**
 * q_i(t)
 */
double PiecewiseConstantFossilizedBirthDeathProcess::q( size_t i, double t, bool tilde ) const
{
    
    if ( t == 0.0 ) return 1.0;
    
    // get the parameters
    double b = birth[i];
    double d = death[i];
    double f = fossil[i];
    double r = (i == num_intervals - 1 ? homogeneous_rho->getValue() : 0.0);
    double ti = times[i];
    
    double diff = b - d - f;
    double bp   = b*f;
    double dt   = t - ti;

    double A = sqrt( diff*diff + 4.0*bp);
    double B = ( (1.0 - 2.0*(1.0-r)*p_i[i] )*b + d + f ) / A;

    double e = exp(-A*dt);
    double tmp = (1.0+B) + e*(1.0-B);

    double q = 4.0*e / (tmp*tmp);

    if (tilde) q = sqrt(q*exp(-(b+d+f)*dt));
    
    return q;
}


/**
 *
 */
void PiecewiseConstantFossilizedBirthDeathProcess::simulateClade(std::vector<TopologyNode *> &n, double age, double present)
{

    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;

    // get the minimum birth age
    std::vector<double> first_occurrences;

    double current_age = RbConstants::Double::inf;
    double minimum_age = 0.0;

    for (size_t i = 0; i < n.size(); ++i)
    {
        // make sure the tip age is younger than the last occurrence
        if( n[i]->isTip() )
        {
            double min = n[i]->getTaxon().getAgeRange().getMin();

            if( min == n[i]->getAge() )
            {
                n[i]->setAge( present + rng->uniform01() * ( min - present ) );
            }
        }

        double first_occurrence = getMaxTaxonAge( *n[i] );

        if( first_occurrence > minimum_age )
        {
            minimum_age = first_occurrence;
        }

        first_occurrences.push_back( first_occurrence );

        if ( current_age > n[i]->getAge() )
        {
            current_age = n[i]->getAge();
        }

    }

    // reset the age
    double max_age = getOriginAge();

    if ( age <= minimum_age )
    {
        age = rng->uniform01() * ( max_age - minimum_age ) + minimum_age;
    }


    std::vector<double> ages;
    while ( n.size() > 2 && current_age < age )
    {

        // get all the nodes with first occurrences younger than the current age
        std::vector<TopologyNode*> active_nodes;
        std::vector<TopologyNode*> active_right_nodes;
        for (size_t i = 0; i < n.size(); ++i)
        {

            if ( current_age >= n[i]->getAge() )
            {
                active_nodes.push_back( n[i] );
            }
            if( current_age >= first_occurrences[i] )
            {
                active_right_nodes.push_back( n[i] );
            }

        }

        // we need to get the next node age older than the current age
        double next_node_age = age;
        for (size_t i = 0; i < n.size(); ++i)
        {
            if ( current_age < n[i]->getAge() && n[i]->getAge() < next_node_age )
            {
                next_node_age = n[i]->getAge();
            }
            if ( current_age < first_occurrences[i] && first_occurrences[i] < next_node_age )
            {
                next_node_age = first_occurrences[i];
            }

        }

        // only simulate if there are at least two valid/active nodes and one active left node
        if ( active_nodes.size() <= 2 || active_right_nodes.empty() )
        {
            current_age = next_node_age;
        }
        else
        {
            // now we simulate new ages
            double next_sim_age = simulateNextAge(active_nodes.size()-2, age, present, current_age);

            if ( next_sim_age < next_node_age )
            {
                // randomly pick two nodes
                size_t index_left = static_cast<size_t>( floor(rng->uniform01()*active_nodes.size()) );
                TopologyNode* left_child = active_nodes[index_left];

                size_t index_right = static_cast<size_t>( floor(rng->uniform01()*active_right_nodes.size()) );
                TopologyNode* right_child = active_right_nodes[index_right];

                while( left_child == right_child )
                {
                    index_left = static_cast<size_t>( floor(rng->uniform01()*active_nodes.size()) );
                    left_child = active_nodes[index_left];

                    index_right = static_cast<size_t>( floor(rng->uniform01()*active_right_nodes.size()) );
                    right_child = active_right_nodes[index_right];
                }

                // erase the nodes also from the origin nodes vector
                std::vector<TopologyNode *>::iterator child_it_left = std::find( n.begin(), n.end(), left_child );
                std::vector<double>::iterator fa_it_left = first_occurrences.begin() + std::distance( n.begin(), child_it_left );
                double fa_left = *fa_it_left;

                first_occurrences.erase(fa_it_left);
                n.erase(child_it_left);

                std::vector<TopologyNode *>::iterator child_it_right = std::find( n.begin(), n.end(), right_child );
                std::vector<double>::iterator fa_it_right = first_occurrences.begin() + std::distance( n.begin(), child_it_right );
                double fa_right = *fa_it_right;

                first_occurrences.erase(fa_it_right);
                n.erase(child_it_right);


                // create a parent for the two
                TopologyNode *parent = new TopologyNode();
                parent->addChild( left_child );
                parent->addChild( right_child );
                left_child->setParent( parent );
                right_child->setParent( parent );
                parent->setAge( next_sim_age );

                // insert the parent to our list
                n.push_back( parent );
                first_occurrences.push_back( std::max(fa_left, fa_right) );

                current_age = next_sim_age;
                ages.push_back( next_sim_age );
            }
            else
            {
                current_age = next_node_age;
            }

        }

        if ( n.size() > 2 && current_age >= age  ) throw RbException("Unexpected number of taxa (remaining #taxa was " + StringUtilities::toString(n.size()) + " and age was " + current_age + " with maximum age of " + age + ") in tree simulation");

    }


    if ( n.size() == 2 )
    {

        // pick two nodes
        TopologyNode* left_child = n[0];
        TopologyNode* right_child = n[1];

        // make sure the speciation event is older than the new species first occurrence
        if( first_occurrences[1] > age )
        {
            if( first_occurrences[0] > age )
            {
                throw(RbException("Cannot simulate clade of age " + StringUtilities::toString(age) + ", minimum age is " + StringUtilities::toString(minimum_age) ));
            }
            else
            {
                std::swap( left_child, right_child );
            }
        }
        else if( age > first_occurrences[0] )
        {
            if( rng->uniform01() < 0.5 )
            {
                std::swap( left_child, right_child );
            }
        }

        // erase the nodes also from the origin nodes vector
        n.clear();

        // create a parent for the two
        TopologyNode *parent = new TopologyNode();
        parent->addChild( left_child );
        parent->addChild( right_child );
        left_child->setParent( parent );
        right_child->setParent( parent );
        parent->setAge( age );

        // insert the parent to our list
        n.push_back( parent );
    }
    else
    {
        throw RbException("Unexpected number of taxa (" + StringUtilities::toString(n.size()) + ") in tree simulation");
    }


}


/**
 * Simulate new speciation times.
 */
double PiecewiseConstantFossilizedBirthDeathProcess::simulateDivergenceTime(double origin, double present) const
{
    // incorrect placeholder for constant FBDP

    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;

    size_t i = l(present);

    // get the parameters
    double age = origin - present;
    double b = birth[i];
    double d = death[i];
    //double f = fossil[i];
    double r = homogeneous_rho->getValue();


    // get a random draw
    double u = rng->uniform01();

    // compute the time for this draw
    // see Hartmann et al. 2010 and Stadler 2011
    double t = 0.0;
    if ( b > d )
    {
        t = ( log( ( (b-d) / (1 - (u)*(1-((b-d)*exp((d-b)*age))/(r*b+(b*(1-r)-d)*exp((d-b)*age) ) ) ) - (b*(1-r)-d) ) / (r * b) ) )  /  (b-d);
    }
    else
    {
        t = ( log( ( (b-d) / (1 - (u)*(1-(b-d)/(r*b*exp((b-d)*age)+(b*(1-r)-d) ) ) ) - (b*(1-r)-d) ) / (r * b) ) )  /  (b-d);
    }

    return present + t;
}


void PiecewiseConstantFossilizedBirthDeathProcess::keepSpecialization(DagNode *toucher)
{
}


void PiecewiseConstantFossilizedBirthDeathProcess::restoreSpecialization(DagNode *toucher)
{

}


void PiecewiseConstantFossilizedBirthDeathProcess::touchSpecialization(DagNode *toucher, bool touchAll)
{

}


/**
 *
 *
 */
void PiecewiseConstantFossilizedBirthDeathProcess::updateIntervals( ) const
{
    std::vector<bool> youngest(taxa.size(), true);

    for (int i = num_intervals - 1; i >= 0; i--)
    {
        double b = getSpeciationRate(i);
        double d = getExtinctionRate(i);
        double f = getFossilizationRate(i);
        double ti = getIntervalTime(i);

        birth[i] = b;
        death[i] = d;
        fossil[i] = f;
        times[i] = ti;

        if (i > 0)
        {

            double r = (i == num_intervals - 1 ? homogeneous_rho->getValue() : 0.0);
            double t = getIntervalTime(i-1);

            double diff = b - d - f;
            double dt   = t - ti;

            double A = sqrt( diff*diff + 4.0*b*f);
            double B = ( (1.0 - 2.0*(1.0-r)*p_i[i] )*b + d + f ) / A;

            double e = exp(-A*dt);

            double tmp = (1.0 + B) + e*(1.0 - B);

            q_i[i-1]       = 4.0*e / (tmp*tmp);
            q_tilde_i[i-1] = sqrt(q_i[i-1]*exp(-(b+d+f)*dt));
            p_i[i-1]       = (b + d + f - A * ((1.0+B)-e*(1.0-B))/tmp)/(2.0*b);
        }

        if( presence_absence )
        {
            for(size_t j = 0; j < taxa.size(); j++)
            {
                if( getFossilCount(i,j) > 0 )
                {
                    oldest_intervals[j] = i;
                    if( youngest[j] )
                    {
                        youngest_intervals[j] = i;
                        youngest[j] = false;
                    }
                }
            }
        }
    }
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
    else if (oldP == fossil_counts)
    {
        fossil_counts = static_cast<const TypedDagNode< long >* >( newP );
    }
    else if (oldP == interval_fossil_counts)
    {
        interval_fossil_counts = static_cast<const TypedDagNode< RbVector<long> >* >( newP );
    }
    else if (oldP == species_interval_fossil_counts)
    {
        species_interval_fossil_counts = static_cast<const TypedDagNode< RbVector<RbVector<long> > >* >( newP );
    }
}
