#include "DagNode.h"
#include "SliceSamplingMove.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbMathLogic.h"

#include <cmath>
#include <cassert>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <utility>

using namespace RevBayesCore;


/** 
 * Constructor
 *
 * Here we simply allocate and initialize the move object.
 *
 * \param[in]    w   The weight how often the proposal will be used (per iteration).
 * \param[in]    t   If auto tuning should be used.
 */
SliceSamplingMove::SliceSamplingMove( StochasticNode<double> *n, double window_, double weight_, bool t ) 
  : AbstractMove(weight_ ,t),
    affectedNodes(),
    nodes(),
    numAccepted( 0 ),
    variable( n ),
    window( window_ )
{
    assert( not variable->isClamped() );

    nodes.insert( n );
    
    for (std::set<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
    {
        (*it)->getAffectedNodes( affectedNodes );
    }
}


/**
 * Copy constructor.
 *
 * \param[in]   m   The object to copy.
 *
 */
SliceSamplingMove::SliceSamplingMove(const SliceSamplingMove &m) 
  : AbstractMove(m),
    affectedNodes( m.affectedNodes ),
    nodes( m.nodes ),
    numAccepted( m.numAccepted ),
    variable( m.variable ),
    window ( m.window )
{
    
}


/**
 * Basic destructor doing nothing.
 */
SliceSamplingMove::~SliceSamplingMove( void )
{
}


/** 
 * Overloaded assignment operator.
 * We need a deep copy of the operator.
 */
SliceSamplingMove& SliceSamplingMove::operator=(const RevBayesCore::SliceSamplingMove &m)
{
    if ( this != &m )
    {
        affectedNodes = m.affectedNodes;
        nodes = m.nodes;
        numAccepted = m.numAccepted;
        variable = m.variable;
	window = m.window;
    }
    
    return *this;
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the SliceSamplingMove. 
 */
SliceSamplingMove* SliceSamplingMove::clone( void ) const 
{
    return new SliceSamplingMove( *this );
}


/**
 * Get the set of nodes on which this move is working on.
 *
 * \return The set of nodes.
 */
const std::set<DagNode*>& SliceSamplingMove::getDagNodes( void ) const
{
    return nodes;
}


/**
 * Get moves' name of object 
 *
 * \return The moves' name.
 */
const std::string& SliceSamplingMove::getMoveName( void ) const 
{
    static std::string name = "SliceSampling";

    return name;
}

struct interval
{
  bool has_lower_bound;
  double lower_bound;

  bool has_upper_bound;
  double upper_bound;

  /// check if a value is below the lower bound on the range
  bool below_lower_bound(double x) const { return (has_lower_bound and x<lower_bound); }
  /// check if a value is above the upper bound on the range
  bool above_upper_bound(double x) const { return (has_upper_bound and x>upper_bound); }
  /// check if a value is in the range or not
  bool in_range(double x) const  { return (not below_lower_bound(x) and not above_upper_bound(x));}

  interval():has_lower_bound(false),lower_bound(0),has_upper_bound(false),upper_bound(0.0) {}
  interval(double l, double u):has_lower_bound(true),lower_bound(l),has_upper_bound(true),upper_bound(u) {}
  interval(bool hl,double l, bool hu, double u):has_lower_bound(hl),lower_bound(l),has_upper_bound(hu),upper_bound(u) {}
};

class slice_function: public interval
{
  StochasticNode<double>* variable;
  double heat;
  bool raiseLikelihoodOnly;
  std::set<DagNode*> affectedNodes;

public:

  double operator()() 
  {
    double lnPrior = 0.0;
    double lnLikelihood = 0.0;

    // 1. compute the probability of the current value for each node
    lnPrior += variable->getLnProbability();
    
    // 2. then we recompute the probability for all the affected nodes
    for (std::set<DagNode*>::iterator it = affectedNodes.begin(); it != affectedNodes.end(); ++it) 
    {
        if ( (*it)->isClamped() )
            lnLikelihood += (*it)->getLnProbability();
        else
            lnPrior += (*it)->getLnProbability();
    }

    // 3. exponentiate with the chain heat
    double lnPosterior;
    if ( raiseLikelihoodOnly )
        lnPosterior = heat * lnLikelihood + lnPrior;
    else
        lnPosterior = heat * (lnLikelihood + lnPrior);

    return lnPosterior;
  }

  double operator()(double x)
  {
    variable->getValue() = x;

    // first we touch all the nodes
    // that will set the flags for recomputation
    variable->touch();
    
    assert( not variable->isClamped() );

    double Pr_ = (*this)();
    
    // call accept for each node  --  automatically includes affected nodes
    variable->keep();

    return Pr_;
  }

  double current_value() const
  {
    return variable->getValue();
  }

  slice_function(StochasticNode<double> *n, double h, bool r)
    :variable(n),
     heat(h),
     raiseLikelihoodOnly(r)
  {
    variable->getAffectedNodes( affectedNodes );
  }
};

std::pair<double,double> 
find_slice_boundaries_stepping_out(double x0,slice_function& g,double logy, double w,int m)
{
}

double search_interval(double x0,double& L, double& R, slice_function& g,double logy)
{
}

double uniform()
{
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    return rng->uniform01();
}

double slice_sample(double x0, slice_function& g,double w, int m)
{
  assert(g.in_range(x0));

  double gx0 = g();
  volatile double diff = gx0 - g(x0);
  assert(std::abs(diff) < 1.0e-9);

  // Determine the slice level, in log terms.

  double logy = gx0 + log(uniform()); // - exponential(1.0);

  // Find the initial interval to sample from.

  std::pair<double,double> interval = find_slice_boundaries_stepping_out(x0,g,logy,w,m);
  double L = interval.first;
  double R = interval.second;

  // Sample from the interval, shrinking it on each rejection

  return search_interval(x0,L,R,g,logy);
}


void SliceSamplingMove::performMove( double heat, bool raiseLikelihoodOnly )
{
    double storedValue = variable->getValue();

    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;

    //-------------------- Propose a new value--------------------------
    // Generate new value (no reflection, so we simply abort later if we propose value here outside of support)
    double u = rng->uniform01();
    const double lambda = 1.0;
    double scalingFactor = std::exp( lambda * ( u - 0.5 ) );
    double newValue = storedValue * scalingFactor;
      
      // compute the Hastings ratio
    double lnHastingsRatio = log( scalingFactor );

    //--------------------------------------------------------------------
    slice_function f(variable, heat, raiseLikelihoodOnly);
    double p1 = f();
    assert( std::abs(p1 - f(storedValue) ) < 1.0e-8);
    double p3 = f(newValue);

    double ratio = (p3 - p1) + lnHastingsRatio;

    if (ratio > 0.0 or rng->uniform01() < exp(ratio))
    { /* accept */ }
    else
    {
      /* reject */
      double p4 = f(storedValue);
      assert( std::abs(p1 - p4) < 1.0e-8);
    }

    //    std::cerr<<"p1 = "<<p1<<"     p3 = "<<p3<<std::endl;
    //    std::cerr<<"x1 = "<<storedValue<<"    x-proposed = "<<newValue<<"       x-new = "<<variable->getValue()<<std::endl;
}


/**
 * Print the summary of the move.
 *
 * The summary just contains the current value of the tuning parameter.
 * It is printed to the stream that it passed in.
 *
 * \param[in]     o     The stream to which we print the summary.
 */
void SliceSamplingMove::printSummary(std::ostream &o) const 
{
    std::streamsize previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();
    
    o << std::fixed;
    o << std::setprecision(4);
    
    // print the name
    const std::string &n = getMoveName();
    size_t spaces = 40 - (n.length() > 40 ? 40 : n.length());
    o << n;
    for (size_t i = 0; i < spaces; ++i) {
        o << " ";
    }
    o << " ";
    
    // print the DagNode name
    const std::string &dn_name = (*nodes.begin())->getName();
    spaces = 20 - (dn_name.length() > 20 ? 20 : dn_name.length());
    o << dn_name;
    for (size_t i = 0; i < spaces; ++i) {
        o << " ";
    }
    o << " ";
    
    // print the weight
    int w_length = 4 - (int)log10(weight);
    for (int i = 0; i < w_length; ++i) {
        o << " ";
    }
    o << weight;
    o << " ";
    
    // print the number of tries
    int t_length = 9 - (int)log10(numTried);
    for (int i = 0; i < t_length; ++i) {
        o << " ";
    }
    o << numTried;
    o << " ";
    
    // print the number of accepted
    int a_length = 9;
    if (numAccepted > 0) a_length -= (int)log10(numAccepted);
    
    for (int i = 0; i < a_length; ++i) {
        o << " ";
    }
    o << numAccepted;
    o << " ";
    
    // print the acceptance ratio
    double ratio = numAccepted / (double)numTried;
    if (numTried == 0) ratio = 0;
    int r_length = 5;
    
    for (int i = 0; i < r_length; ++i) {
        o << " ";
    }
    o << ratio;
    o << " ";
    
    //    proposal->printParameterSummary( o );
    o<<"window = "<<window<<std::endl;
    o<<"weight = "<<weight<<std::endl;
    
    o << std::endl;
    
    o.setf(previousFlags);
    o.precision(previousPrecision);
    
    
}

/**
 * Reset the move counters. Here we only reset the counter for the number of accepted moves.
 *
 */
void SliceSamplingMove::resetMoveCounters( void )
{
    numAccepted = 0;
}


/**
 * Swap the current variable for a new one.
 *
 * \param[in]     oldN     The old variable that needs to be replaced.
 * \param[in]     newN     The new variable.
 */
void SliceSamplingMove::swapNode(DagNode *oldN, DagNode *newN) 
{
    
    // find the old node
    std::set<DagNode*>::iterator pos = nodes.find( oldN );
    // remove it from the set if it was contained
    if ( pos != nodes.end() )
    {
        nodes.erase( pos );
    }
    // insert the new node
    nodes.insert( newN );
        
    affectedNodes.clear();
    for (std::set<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
    {
        (*it)->getAffectedNodes( affectedNodes );
    }
    
    variable = static_cast<StochasticNode<double>* >(newN) ;
}


/**
 * Tune the move to accept the desired acceptance ratio.
 * We only compute the acceptance ratio here and delegate the call to the proposal.
 */
void SliceSamplingMove::tune( void ) {
    
  //    double rate = numAccepted / double(numTried);
    
    //    proposal->tune( rate );
    
}

