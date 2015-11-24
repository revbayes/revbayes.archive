#include "ConstantRateFossilizedBirthDeathProcess.h"
#include "DistributionExponential.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"

#include <cmath>


using namespace RevBayesCore;


/**
 * Constructor. 
 * We delegate most parameters to the base class and initialize the members.
 *
 * \param[in]    o              Time of the origin/present/length of the process.
 * \param[in]    ra				Age or the root (=time of the process).
 * \param[in]    s              Speciation rate.
 * \param[in]    e              Extinction rate.
 * \param[in]    p              Extinction sampling rate.
 * \param[in]    r              Sampling probability at present time.
 * \param[in]    cdt            Condition of the process (none/survival/#Taxa).
 * \param[in]    tn             Taxa.
 * \param[in]    c              Clades conditioned to be present.
 */
ConstantRateFossilizedBirthDeathProcess::ConstantRateFossilizedBirthDeathProcess(const TypedDagNode<double> *o,
												const TypedDagNode<double> *ra,const TypedDagNode<double> *s,
												const TypedDagNode<double> *e,const TypedDagNode<double> *p,
												const TypedDagNode<double> *r,const std::string &cdt, const std::vector<Taxon> &tn) : AbstractBirthDeathProcess( o, ra, cdt, tn ),
    lambda( s ), 
    mu( e ), 
    psi( p ), 
    rho( r )
{
    addParameter( lambda );
    addParameter( mu );
    addParameter( psi );
    addParameter( rho );
    
    simulateTree();
    
}

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself 
 */
ConstantRateFossilizedBirthDeathProcess* ConstantRateFossilizedBirthDeathProcess::clone( void ) const
{
    
    return new ConstantRateFossilizedBirthDeathProcess( *this );
}

/**
 * Compute the log-transformed probability of the current value under the current parameter values.
 *
 * \return    The log-probability density.
 */
double ConstantRateFossilizedBirthDeathProcess::computeLnProbabilityTimes( void ) const
{
    
    // variable declarations and initialization
    double lnProbTimes = 0.0;
	double birth_rate = lambda->getValue();
    double fossil_rate = psi->getValue();
    double sampling_prob = rho->getValue();
    
    double process_time = 0.0;
    size_t num_nodes = value->getNumberOfNodes();
    size_t num_initial_lineages = 1;
    
    // test that the time of the process is larger or equal to the present time
    if ( startsAtRoot == false )
    {
        double org = origin->getValue();
        process_time = org;
        
    }
    else
    {
        process_time = value->getRoot().getAge();
        num_initial_lineages = 2;
    }

	int num_fossils = 0;
	int num_extant = 0;
	for (size_t i = 0; i < num_nodes; ++i)
    {
		const TopologyNode& n = value->getNode( i );
		if( n.isFossil() == true )
        {
			++num_fossils;
        }
        else if ( n.isTip() == true )
        {
			++num_extant;
        }
        
	}
	
	std::vector<double> fossil_tip_ages = std::vector<double>();
    for (size_t i = 0; i < num_nodes; ++i)
    {
        const TopologyNode& n = value->getNode( i );
		if( n.isFossil() == true && n.isSampledAncestor() == false )
        {
			double t = n.getAge();
			fossil_tip_ages.push_back(t);
		}
        
    }
	
	std::vector<double> internal_node_ages = std::vector<double>();
    for (size_t i = 0; i < num_nodes; ++i)
    {
        
        const TopologyNode& n = value->getNode( i );
		if( n.isInternal() == true && n.getChild(0).isSampledAncestor() == false && n.getChild(1).isSampledAncestor() == false)
        {
			double t = n.getAge();
			internal_node_ages.push_back(t);
		}
        
    }
	
    // add the log probability for the fossilization events
    lnProbTimes += num_fossils * log( fossil_rate );
    // add the log probability for sampling the extant taxa
    lnProbTimes += num_extant * log( sampling_prob );
    // compute the probability of the initial sequences
    lnProbTimes +=  lnQ(process_time) - num_initial_lineages * log(1.0 - pHatZero(process_time));
	
	for(size_t i=0; i<internal_node_ages.size(); i++)
    {
		double t = internal_node_ages[i];
        // probabilities of the observed branches and the speciation events leading to them
		lnProbTimes += log(2.0*birth_rate) + lnQ(t);
	}
	
    // What is this doing? (Sebastian)
	for(size_t f=0; f < fossil_tip_ages.size(); f++)
    {
		double t = fossil_tip_ages[f];
		lnProbTimes += log(pZero(t)) - lnQ(t);
	}
	
    return lnProbTimes;
    
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
double ConstantRateFossilizedBirthDeathProcess::pSurvival(double start, double end) const
{
    
	
    double p0 = pZero(end);
    
    return 1.0 - p0;
}



/**
 * Simulate new speciation times.
 */
std::vector<double>* ConstantRateFossilizedBirthDeathProcess::simSpeciations(size_t n, double origin) const
{

    // incorrect placeholder for constant BDP
    // previous simSpeciations did not generate trees with defined likelihoods
    
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // get the parameters
    double b = lambda->getValue();
    double d = mu->getValue();
    double r = rho->getValue();
    
    std::vector<double>* times = new std::vector<double>(n, 0.0);
    
    for (size_t i = 0; i < n; ++i)
    {
        // get a random draw
        double u = rng->uniform01();
        
        // compute the time for this draw
        double t = ( log( ( (b-d) / (1 - (u)*(1-((b-d)*exp((d-b)*origin))/(r*b+(b*(1-r)-d)*exp((d-b)*origin) ) ) ) - (b*(1-r)-d) ) / (r * b) ) + (d-b)*origin )  /  (d-b);
        
        // store the new time
        (*times)[i] = t;
    }
    
    // finally sort the times
    std::sort(times->begin(), times->end());
    
    return times;
}


/**
 * Swap the parameters held by this distribution.
 *
 * 
 * \param[in]    oldP      Pointer to the old parameter.
 * \param[in]    newP      Pointer to the new parameter.
 */
void ConstantRateFossilizedBirthDeathProcess::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == lambda) 
    {
        lambda = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == mu) 
    {
        mu = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == psi) 
    {
        psi = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == rho) 
    {
        rho = static_cast<const TypedDagNode<double>* >( newP );
    }
    else 
    {
        // delegate the super-class
        AbstractBirthDeathProcess::swapParameterInternal(oldP, newP);
    }
    
}

double ConstantRateFossilizedBirthDeathProcess::pZero(double t) const
{
	
	double b = lambda->getValue();
    double d = mu->getValue();
    double f = psi->getValue();
    double r = rho->getValue();
	double bdp = b-d-f;
	double cOne = std::abs(sqrt((bdp*bdp) + 4.0*b*f));
	double cTwo = -((b-d-(2.0*b*r)-f) / cOne);
	double v1 = b+d+f;
	double v2 = (exp(-cOne*t)*(1.0-cTwo)) - (1.0+cTwo);
	double v3 = (exp(-cOne*t)*(1.0-cTwo)) + (1.0+cTwo);
	double v = v1 + (cOne * (v2 / v3));
    
	return v / (2.0*b);
}


double ConstantRateFossilizedBirthDeathProcess::lnQ(double t) const
{
    
    // get the parameters
    double birth_rate       = lambda->getValue();
    double death_rate       = mu->getValue();
    double fossil_rate      = psi->getValue();
    double sampling_prob    = rho->getValue();
    
    double a = (birth_rate - death_rate - fossil_rate);
    double c1 = sqrt( a*a + 4*birth_rate*fossil_rate );
    double c2 = - (a-2.0*birth_rate*sampling_prob)/c1;
    
    double oneMinusC2 = 1.0-c2;
    double onePlusC2  = 1.0+c2;
    
    // original code
//    double b1 = 2.0*(1.0-c2*c2);
//    double b2 = exp(-c1*t)*oneMinusC2*oneMinusC2;
//    double b3 = exp(c1*t)*onePlusC2*onePlusC2;
//    
//    double lnQt = log( 4.0 ) - log( b1 + b2 + b3 );
    
    // numerically safe code
    double b1 = exp(-c1*t)*2.0*(1.0-c2*c2);
    double b2 = exp(-c1*t*2)*oneMinusC2*oneMinusC2;
    double b3 = onePlusC2*onePlusC2;
    
    double lnQt = log( 4.0 ) - log( b1 + b2 + b3 ) - c1*t;
    
	return lnQt;
}


double ConstantRateFossilizedBirthDeathProcess::pHatZero(double t) const
{
	
	double b = lambda->getValue();
    double d = mu->getValue();
    double r = rho->getValue();
	double val = (r * (b-d)) / ((b*r) + ((b*(1-r))-d)*exp(-(b-d)*t));
	return 1.0 - val;
}




