#include "ConstantRateFossilizedBirthDeathProcess.h"
#include "DistributionExponential.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbMathCombinatorialFunctions.h"
#include "StochasticNode.h"

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
ConstantRateFossilizedBirthDeathProcess::ConstantRateFossilizedBirthDeathProcess( const TypedDagNode<double> *o,
                                                                                  const TypedDagNode<double> *s,
                                                                                  const TypedDagNode<double> *e,
                                                                                  const TypedDagNode<double> *p,
                                                                                  const TypedDagNode<double> *r,
                                                                                  const bool& uo,
                                                                                  const std::string& cdt,
                                                                                  const std::vector<Taxon> &tn) : AbstractBirthDeathProcess( o, cdt, tn ),
    lambda( s ),
    mu( e ), 
    psi( p ), 
    rho( r ),
    useOrigin( uo )
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
 * If conditioning on the origin, then return the age of the root node
 * or zero if the tree is empty
 */
double ConstantRateFossilizedBirthDeathProcess::getRootAge( void ) const
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
 * Compute the log-transformed probability of the current value under the current parameter values.
 *
 */
double ConstantRateFossilizedBirthDeathProcess::computeLnProbabilityDivergenceTimes( void ) const
{
    // prepare the probability computation
    prepareProbComputation();

    // variable declarations and initialization
    double lnProbTimes = 0;

    // present time
    double present_time = value->getRoot().getAge();

    // what do we condition on?
    // did we condition on survival?
    if ( condition == "nTaxa" )
    {
        lnProbTimes = -lnProbNumTaxa( value->getNumberOfTips(), 0, present_time, true );
    }

    // multiply the probability of a descendant of the initial species
    lnProbTimes += computeLnProbabilityTimes();

    return lnProbTimes;
}


/**
 * Compute the log-transformed probability of the current value under the current parameter values.
 *
 * \return    The log-probability density.
 */
double ConstantRateFossilizedBirthDeathProcess::computeLnProbabilityTimes( void ) const
{
    
    double lnProbTimes = 0.0;
    double process_time = getOriginTime();
    size_t num_initial_lineages = 0;
    TopologyNode* root = &value->getRoot();
    
    if (useOrigin) {
        // If we are conditioning on survival from the origin,
        // then we must divide by 2 the log survival probability computed by AbstractBirthDeathProcess
        // TODO: Generalize AbstractBirthDeathProcess to allow conditioning on the origin
        num_initial_lineages = 1;
    }
    
    // if conditioning on root, root node must be a "true" bifurcation event
    else
    {
        if (root->getChild(0).isSampledAncestor() || root->getChild(1).isSampledAncestor())
            return RbConstants::Double::neginf;
        num_initial_lineages = 2;
    }

    // variable declarations and initialization
	double birth_rate = lambda->getValue();
    double death_rate = mu->getValue();
    double fossil_rate = psi->getValue();
    double sampling_prob = rho->getValue();
    
    // get helper variables
    double a = birth_rate - death_rate - fossil_rate;
    double c1 = std::fabs(sqrt(a * a + 4 * birth_rate * fossil_rate));
    double c2 = -(a - 2 * birth_rate * sampling_prob) / c1;
 
    // get node/time variables
    size_t num_nodes = value->getNumberOfNodes();

    // classify nodes
    int num_sampled_ancestors = 0;
    int num_fossil_taxa = 0;
    int num_extant_taxa = 0;
    int num_internal_nodes = 0;

    std::vector<double> fossil_tip_ages = std::vector<double>();
    std::vector<double> internal_node_ages = std::vector<double>();
	for (size_t i = 0; i < num_nodes; i++)
    {
		const TopologyNode& n = value->getNode( i );
        
        if ( n.isTip() && n.isFossil() && n.isSampledAncestor() )
        {
            // node is sampled ancestor
            num_sampled_ancestors++;
        }
        else if ( n.isTip() && n.isFossil() && !n.isSampledAncestor() )
        {
            // node is fossil leaf
            num_fossil_taxa++;
            fossil_tip_ages.push_back( n.getAge() );
        }
        else if ( n.isTip() && !n.isFossil() )
        {
            // node is extant leaf
            num_extant_taxa++;
        }
        else if ( n.isInternal() && !n.getChild(0).isSampledAncestor() && !n.getChild(1).isSampledAncestor() )
        {
            // node is bifurcation event (a "true" node)
            internal_node_ages.push_back( n.getAge() );
            num_internal_nodes++;
        }
	}
    
    // add the log probability for the fossilization events
    if (fossil_rate == 0.0 && num_fossil_taxa + num_sampled_ancestors > 0)
    {
        throw RbException("The sampling rate is zero, but the tree has sampled fossils.");
    }
    else if (fossil_rate > 0.0)
    {
        lnProbTimes += (num_fossil_taxa + num_sampled_ancestors) * log( fossil_rate );
    }
    
    // add the log probability for sampling the extant taxa
    lnProbTimes += num_extant_taxa * log( sampling_prob );
    
    // add the log probability of the initial sequences
    lnProbTimes += lnQ(process_time, c1, c2) - (num_initial_lineages - 1) * log(birth_rate);
	
    // add the log probability for the internal node ages
    lnProbTimes += internal_node_ages.size() * log(2.0 * birth_rate);
	for(size_t i=0; i<internal_node_ages.size(); i++)
    {
		double t = internal_node_ages[i];
		lnProbTimes += lnQ(t, c1, c2);
	}

	// add the log probability for the fossil tip ages
	for(size_t f=0; f < fossil_tip_ages.size(); f++)
    {
		double t = fossil_tip_ages[f];
		lnProbTimes += log(pZero(t, c1, c2)) - lnQ(t, c1, c2);
	}

	// condition on survival
    if( condition == "survival")
    {
        lnProbTimes -= num_initial_lineages * log(1.0 - pHatZero(process_time));
    }

    return lnProbTimes;
    
}


double ConstantRateFossilizedBirthDeathProcess::lnProbTreeShape(void) const
{
    // the birth death divergence times density is derived for a (ranked) unlabeled oriented tree
    // so we convert to a (ranked) labeled non-oriented tree probability by multiplying by 2^{n-1} / ((n-m)! m!)
    // where m is the number of extinct tips

    size_t num_taxa = value->getNumberOfTips();

    size_t num_extinct = 0;
    for( size_t i = 0; i < num_taxa; i++)
    {
        if( value->getNode(i).getAge() > 0 )
        {
            num_extinct++;
        }
    }

    return (num_taxa - 1) * RbConstants::LN2 - RbMath::lnFactorial(num_taxa - num_extinct) - RbMath::lnFactorial(num_extinct);
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
    return 1.0 - pHatZero(end);
}



/**
 * Simulate new speciation times.
 */
double ConstantRateFossilizedBirthDeathProcess::simulateDivergenceTime(double origin, double present) const
{

    // incorrect placeholder for constant FBDP
    // previous simSpeciations did not generate trees with defined likelihoods
    
    
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // get the parameters
    double age = present - origin;
    double b = lambda->getValue();
    double d = mu->getValue();
    double r = rho->getValue();
    
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
    
    //    return present - t;
    return present - t;
}


double ConstantRateFossilizedBirthDeathProcess::pZero(double t, double c1, double c2) const
{
	double b = lambda->getValue();
    double d = mu->getValue();
    double f = psi->getValue();
	double v1 = exp(-c1*t) * (1.0-c2) - (1.0+c2);
	double v2 = exp(-c1*t) * (1.0-c2) + (1.0+c2);
	double v = (b + d + f + c1 * (v1 / v2)) / (2.0 * b);
    return v;
}


double ConstantRateFossilizedBirthDeathProcess::lnQ(double t, double c1, double c2) const
{
    // numerically safe code
    double lnQt = log(4.0) - c1*t - 2 * log( exp(-c1*t) * (1-c2) + (1+c2));
	return lnQt;
}


double ConstantRateFossilizedBirthDeathProcess::pHatZero(double t) const
{
	double b = lambda->getValue();
    double d = mu->getValue();
    double r = rho->getValue();
	double val = r * (b-d) / (b*r + (b*(1-r)-d)*exp(-(b-d)*t));
	return 1.0 - val;
}

/**
 * Restore the current value and reset some internal flags.
 * If the root age variable has been restored, then we need to change the root age of the tree too.
 */
void ConstantRateFossilizedBirthDeathProcess::restoreSpecialization(DagNode *affecter)
{

    if ( affecter == root_age )
    {
        if( useOrigin )
        {
            if ( dag_node != NULL )
            {
                dag_node->restoreAffected();
            }
        }
        else
        {
            AbstractRootedTreeDistribution::restoreSpecialization(affecter);
        }
    }

}


/**
 * Set the current value.
 */
void ConstantRateFossilizedBirthDeathProcess::setValue(Tree *v, bool f )
{

    // delegate to super class
    TypedDistribution<Tree>::setValue(v, f);


    if ( root_age != NULL && !useOrigin )
    {
        const StochasticNode<double> *stoch_root_age = dynamic_cast<const StochasticNode<double>* >(root_age);
        if ( stoch_root_age != NULL )
        {
            const_cast<StochasticNode<double> *>(stoch_root_age)->setValue( new double( value->getRoot().getAge() ), f);
        }
        else
        {
            value->getRoot().setAge( root_age->getValue() );
        }

    }

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


/**
 * Touch the current value and reset some internal flags.
 * If the root age variable has been restored, then we need to change the root age of the tree too.
 */
void ConstantRateFossilizedBirthDeathProcess::touchSpecialization(DagNode *affecter, bool touchAll)
{

    if ( affecter == root_age )
    {
        if( useOrigin )
        {
            if ( dag_node != NULL )
            {
                dag_node->touchAffected();
            }
        }
        else
        {
            AbstractRootedTreeDistribution::touchSpecialization(affecter, touchAll);
        }
    }

}


