#include "DistributionExponential.h"
#include "PiecewiseConstantSerialSampledBirthDeathProcess.h"
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
 * \param[in]    p              Extinction (fossil) sampling rates.
 * \param[in]    pt             Extinction (fossil) sampling rate-change times.
 * \param[in]    r              Instantaneous sampling probabilities.
 * \param[in]    rt             Instantaneous sampling times.
 * \param[in]    tLastSample    time between now and the last obtained sample.
 * \param[in]    cdt            Condition of the process (none/survival/#Taxa).
 * \param[in]    tn             Taxa.
 * \param[in]    c              Clades conditioned to be present.
 */
PiecewiseConstantSerialSampledBirthDeathProcess::PiecewiseConstantSerialSampledBirthDeathProcess(const TypedDagNode<double> *o, const TypedDagNode<double> *ra,
                                                                                                 const TypedDagNode<RbVector<double> > *s, const TypedDagNode<RbVector<double> > *st,
                                                                                                 const TypedDagNode<RbVector<double> > *e, const TypedDagNode<RbVector<double> > *et,
                                                                                                 const TypedDagNode<RbVector<double> > *p, const TypedDagNode<RbVector<double> > *pt,
                                                                                                 const TypedDagNode<RbVector<double> > *r, const TypedDagNode<RbVector<double> > *rt,
                                                                                                 double tLastSample, const std::string &cdt, 
                                                                                                 const std::vector<Taxon> &tn) : AbstractBirthDeathProcess( o, ra, cdt, tn ),
    lambda( s ), 
    lambdaTimes( st ), 
    mu( e ), 
    muTimes( et ),
    psi( p ), 
    psiTimes( pt ),
    rho( r ),
    rhoTimes( rt ),
    timeSinceLastSample( tLastSample )
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
PiecewiseConstantSerialSampledBirthDeathProcess* PiecewiseConstantSerialSampledBirthDeathProcess::clone( void ) const 
{
    
    return new PiecewiseConstantSerialSampledBirthDeathProcess( *this );
}


/**
 * Compute the log-transformed probability of the current value under the current parameter values.
 *
 */
double PiecewiseConstantSerialSampledBirthDeathProcess::computeLnProbabilityTimes( void ) const
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
    
//    double ra = value->getRoot().getAge();
    
    // retrieved the speciation times
    std::vector<double>* agesInternalNodes  = getAgesOfInternalNodesFromMostRecentSample();
    std::vector<double>* agesTips           = getAgesOfTipsFromMostRecentSample();
    
    // multiply the probabilities for the tip ages
    for (size_t i = 0; i < numTaxa; ++i) 
    {
        if ( lnProbTimes == RbConstants::Double::nan || 
            lnProbTimes == RbConstants::Double::inf || 
            lnProbTimes == RbConstants::Double::neginf ) 
        {
            return RbConstants::Double::nan;
        }
        
        double t = (*agesTips)[i];
        size_t index = l(t);
        if ( (t == 0.0 || find(rateChangeTimes.begin(), rateChangeTimes.end(), t) != rateChangeTimes.end()) && sampling[index] != 0.0 ) 
        {
            lnProbTimes += log( sampling[index] );
        }
        else
        {
            lnProbTimes += log( fossil[index] / q(index, t + timeSinceLastSample) );
        }
        
//        if ( lnProbTimes > 100 )
//        {
//            std::cerr << "holala" << std::endl;
//        }
        
    }
    
    for (size_t i = 0; i < numTaxa-1; ++i) 
    {
        if ( lnProbTimes == RbConstants::Double::nan || 
            lnProbTimes == RbConstants::Double::inf || 
            lnProbTimes == RbConstants::Double::neginf ) 
        {
            return RbConstants::Double::nan;
        }
        
        double t = (*agesInternalNodes)[i];
        size_t index = l(t);
        lnProbTimes += log( q(index,t+timeSinceLastSample) * birth[index] );
        
//        if ( lnProbTimes > 100 )
//        {
//            std::cerr << "holala" << std::endl;
//        }
    }
    
    for (size_t i = 0; i < rateChangeTimes.size(); ++i) 
    {
        if ( lnProbTimes == RbConstants::Double::nan || 
            lnProbTimes == RbConstants::Double::inf || 
            lnProbTimes == RbConstants::Double::neginf ) 
        {
            return RbConstants::Double::nan;
        }
        
        double t = rateChangeTimes[i];
        int div = survivors(t);
        lnProbTimes += div * log( (1.0 - sampling[i+1]) * q(i, t) );
        
//        if ( lnProbTimes > 100 )
//        {
//            std::cout << "t = " << t << "  -- div = " << div << "\n";
//            std::cout << "sampling = " << sampling[i+1] << "  -- i = " << i << "\n";
//            std::cout << "sampling size = " << sampling.size() << "   q(i,t) =" << q(i, t) << std::endl;
//            std::cerr << "holala" << std::endl;
//        }
		
        
    }

    
    
//    double test = log( q(rateChangeTimes.size(), org ) );
//    if ( test > 100 )
//    {
//        std::cerr << "holala" << std::endl;
//    }
    lnProbTimes += log( q(rateChangeTimes.size(), presentTime ) );
//    if ( lnProbTimes > 100 )
//    {
//        std::cerr << "holala" << std::endl;
//    }
    
    delete agesInternalNodes;
    delete agesTips;
    
    return lnProbTimes;
    
}


/**
 *
 *
 */
size_t PiecewiseConstantSerialSampledBirthDeathProcess::l(double t) const
{
    return l(t,0,rateChangeTimes.size());
}




/**
 *
 *
 */
size_t PiecewiseConstantSerialSampledBirthDeathProcess::l(double t, size_t min, size_t max) const
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
 *
 * \param[in]    start      Start time of the process.
 * \param[in]    end        End/stopping time of the process.
 *
 * \return Speciation rate at time t. 
 */
double PiecewiseConstantSerialSampledBirthDeathProcess::pSurvival(double start, double end) const 
{
    
    double t = end;
    
    double p0 = p(l(t), t);
    
    return 1.0 - p0;
}



/**
 *
 */
double PiecewiseConstantSerialSampledBirthDeathProcess::p( size_t i, double t ) const
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
    double p0 = p(i-1,ti);
    double B = ( (1.0 - 2.0*(1.0-r)*p0 )*b + d + f ) / A;
    
    double e = exp(A*dt);
    double tmp = b + d + f - A * ((1.0+B)-e*(1.0-B))/((1.0+B)+e*(1.0-B));
    
    return tmp / (2.0*b);    
}



/**
 *
 *
 */
void PiecewiseConstantSerialSampledBirthDeathProcess::prepareProbComputation( void )
{
    
    // clean all the sets
    rateChangeTimes.clear();
    birth.clear();
    death.clear();
    fossil.clear();
    sampling.clear();
    
    std::set<double> eventTimes;
    
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
    
    birth.push_back(    b[0] );
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
			birth.push_back( b[indexBirth+1] );
        }
		else{
			birth.push_back( b[indexBirth] );
		}
        
        
        // add the extinction rate at the rate-change event t
        pos = size_t( find(deathTimes.begin(), deathTimes.end(), t) - deathTimes.begin() );
        if ( pos != deathTimes.size() ) 
        {
            indexDeath = pos;
			death.push_back( d[indexDeath+1] );
        }
		else{
			death.push_back( d[indexDeath] );
		}
        
        // add the fossilization rate at the rate-change event t
        pos = size_t( find(fossilTimes.begin(), fossilTimes.end(), t) - fossilTimes.begin() );
        if ( pos != fossilTimes.size() ) 
        {
            indexFossil = pos;
			fossil.push_back( f[indexFossil+1] );
        }
        else{
			fossil.push_back( f[indexFossil] );
		}
        
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
 *
 */
double PiecewiseConstantSerialSampledBirthDeathProcess::q( size_t i, double t ) const
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
//    double tmp = ((1.0+B)+e*(1.0-B));
    
//    double tmp2 = 4.0*e / (tmp*tmp);
//    double tmp2 = (e/tmp)*(4.0/tmp);
    
    // (4 * Math.exp(Ai[index] * (t - ti))) / (Math.exp(Ai[index] * (t - ti)) * (1 - Bi[index]) + (1 + Bi[index])) / (Math.exp(Ai[index] * (t - ti)) * (1 - Bi[index]) + (1 + Bi[index]));
    
    // (4 * e) / (e * (1 - B) + (1 + B)) / (e * (1 - B) + (1 + B));
    double test2 = (4 * e) / (e * (1 - B) + (1 + B)) / (e * (1 - B) + (1 + B));
    double test3 = (4 ) / ((1 - B) + (1 + B) / e) / ((1 - B) + (1 + B) / e);
    
//    if ( fabs( tmp2 - test2 ) > 1E-8 || fabs( tmp2 - test3 ) > 1E-8 || fabs( test3 - test2 ) > 1E-8 )
//    {
//        std::cerr << "Numerical problems ..." << std::endl;
//    }
    
    if ( e < 1E-6 )
    {
        return test3;
    }
    else //if ( fabs(B + 1.0) < 1E-4)
    {
        return test2;
    }
	
//	return test2;
}


/**
 * Simulate new speciation times.
 */
std::vector<double>* PiecewiseConstantSerialSampledBirthDeathProcess::simSpeciations(size_t n, double origin) const
{
    
    // Get the rng
    // RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // get the parameters
//    double birth = lambda->getValue();
//    double death = mu->getValue();
//    double p     = psi->getValue();
//    double r     = rho->getValue();
    
    
    std::vector<double> *times = new std::vector<double>();
    for (size_t i = 0; i < n; i++ )
    {
        // draw the times
        times->push_back( (i+1) / (n+1) * origin );
    }
	
	
    return times;
}




/**
 * Compute the diversity of the tree at time t.
 *
 * \param[in]    t      time at which we want to know the diversity.
 *
 * \return The diversity (number of species in the reconstructed tree).
 */
int PiecewiseConstantSerialSampledBirthDeathProcess::survivors(double t) const
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
 * 
 * \param[in]    oldP      Pointer to the old parameter.
 * \param[in]    newP      Pointer to the new parameter.
 */
void PiecewiseConstantSerialSampledBirthDeathProcess::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
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
