#include "DagNode.h"
#include "DistributionNormal.h"
#include "NormalDistribution.h"
#include "EllipticalSliceSamplingSimpleMove.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbMathLogic.h"
#include "TypedDagNode.h"

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
EllipticalSliceSamplingSimpleMove::EllipticalSliceSamplingSimpleMove( std::vector< StochasticNode<double> *> n, double window_, double weight_, bool t ) : AbstractMove( std::vector<DagNode*>(), weight_ ,t),
    variables( n ),
    window( window_ ),
    total_movement( 0.0 )
{
    // Check window is valid (in [0,2*pi])
    if (window > RbConstants::TwoPI)
    {
        throw(RbException("mvEllipticalSliceSamplingSimple requires the window to be in the interval(0,2*pi]"));
    }
    
    // tell the base class to add the node
    for (size_t i=0; i<variables.size(); ++i)
    {
        assert( not variables[i]->isClamped() );
        
        NormalDistribution* dist = dynamic_cast<NormalDistribution *>( &variables[i]->getDistribution() );
        
        if (dist == NULL)
        {
            throw(RbException("mvEllipticalSliceSamplingSimple only works for vector of variables with Normal(0,sigma) Distribution"));
        }

        if (dist->getMean()->getValue() != 0.0)
        {
            throw(RbException("mvEllipticalSliceSamplingSimple only works for vector of variables with Normal(0,sigma) Distribution"));
        }

        addNode( variables[i] );
    }

}


/**
 * Basic destructor doing nothing.
 */
EllipticalSliceSamplingSimpleMove::~EllipticalSliceSamplingSimpleMove( void )
{
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the EllipticalSliceSamplingSimpleMove. 
 */
EllipticalSliceSamplingSimpleMove* EllipticalSliceSamplingSimpleMove::clone( void ) const 
{
    return new EllipticalSliceSamplingSimpleMove( *this );
}


/**
 * Get moves' name of object 
 *
 * \return The moves' name.
 */
const std::string& EllipticalSliceSamplingSimpleMove::getMoveName( void ) const 
{
    static std::string name = "EllipticalSliceSamplingSimpleMove";

    return name;
}

double unif()
{
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    return rng->uniform01();
}


/// This object allow computing the probability of the current point, and also store the variables's range
class slice_function
{
  std::vector<StochasticNode<double> *> variables;
  double lHeat;
  double pHeat;
  RbOrderedSet<DagNode*> affectedNodes;
  int num_evals;

public:

  int get_num_evals() const {return num_evals;}

  double operator()() 
  {
      
//      std::cout << "Hello from slice_sample::operator()()" << std::endl;
      double lnPrior = 0.0;
      double lnLikelihood = 0.0;
    
      
    // Under Murray's sampler, we're drawing new values from this prior, and we don't need to compute their probability
    // 2. then we recompute the probability for all the affected nodes
    for (RbOrderedSet<DagNode*>::iterator it = affectedNodes.begin(); it != affectedNodes.end(); ++it)
    {
//        std::cout << "slice_sample::operator()() (*it)->getName() " << (*it)->getName() << std::endl;
//        std::cout << "slice_sample::operator()() (*it)->isClamped() " << (*it)->isClamped() << std::endl;
        if ( (*it)->isClamped() )
            lnLikelihood += (*it)->getLnProbability();
        else
            lnPrior += (*it)->getLnProbability();
    }
//      std::cout << "slice_sample::operator()() computed likelihood" << std::endl;
    // 3. exponentiate with the chain heat
    double lnPosterior = pHeat * (lHeat * lnLikelihood + lnPrior);
//      std::cout << "slice_sample::operator()() computed posterior" << std::endl;

    return lnPosterior;
  }

    
  double operator()(std::vector<double> x)
  {
//      std::cout << "Hello from slice_sample::operator()(std::vector<double> x)" << std::endl;

      ++num_evals;
    
      for (size_t i=0; i<variables.size(); ++i)
      {
//          std::cout << "slice_sample::operator()(std::vector<double> x) in first loop" << std::endl;
//          std::cout << "slice_sample::operator()(std::vector<double> x) variables[i] contains " << variables[i] << std::endl;
//          std::cout << "slice_sample::operator()(std::vector<double> x) variables[i]->getValue contains " << variables[i]->getValue() << std::endl;
//          variables[i]->setValue(&x[i]);
//          std::cout << "slice_sample::operator()(std::vector<double> x) variables[i]->setValue(&x[i]);" << std::endl;
          variables[i]->getValue() = x[i];
          
          // first we touch all the nodes
          // that will set the flags for recomputation
          variables[i]->touch();
//          std::cout << "slice_sample::operator()(std::vector<double> x) variables[i]->getValue() = x[i];" << std::endl;
          
//          assert( not variables[i]->isClamped() );
//          std::cout << "slice_sample::operator()(std::vector<double> x) assert( not variables[i]->isClamped() );" << std::endl;
      }
//      std::cout << "slice_sample::operator()(std::vector<double> x) checked clamps and touched nodes" << std::endl;

      
      double Pr_ = (*this)();
    
//      std::cout << "And we're back! slice_sample::operator()(std::vector<double> x) computed \"likelihood\" " << std::endl;
      
    // call accept for each node  --  automatically includes affected nodes
      for (size_t i=0; i<variables.size(); ++i)
      {
        variables[i]->keep();
      }
//      std::cout << "slice_sample::operator()(std::vector<double> x) kept nodes" << std::endl;

    return Pr_;
  }
    
  std::vector<double> current_value() const
  {
      std::vector<double> vals;
      
      for (std::vector< StochasticNode<double> *>::const_iterator it = variables.begin(); it != variables.end(); it++)
      {
          vals.push_back( (*it)->getValue() );
      }
      
      return vals;
  }

    slice_function(std::vector<StochasticNode<double> *> n, double l, double p)
    :variables(n),
     lHeat(l),
     pHeat(p),
     num_evals(0)
  {
      for (std::vector< StochasticNode<double> *>::const_iterator it = variables.begin(); it != variables.end(); it++)
      {
          (*it)->getAffectedNodes( affectedNodes );
      }
  }
};

//std::pair<double,double> 
//find_initial_slice_boundaries(slice_function& g,double logy, double w,int m)
//{
////    assert(g.in_range(x0));
//
//    double u = unif()*RbConstants::TwoPI;
//    double L = u - RbConstants::TwoPI;
//    double R = u;
//
//  return std::pair<double,double>(L,R);
//}
//
//double draw_nu()
//{
//    
//    return(nu);
//}

//double search_interval(std::vector<double> x0, std::vector<double> nu, double& L, double& R, slice_function& g,double logy)
//{
//    //  assert(g(x0) > g(L) and g(x0) > g(R));
//    assert(g(x0) >= logy);
//    assert(L < R);
//
//    for (int i=0;i<200;i++)
//    {
//      double theta = L + unif()*(R-L);
//        std::vector<double> x1;
//        for (i = 0; i < x0.size(); i++)
//        {
//            x1[i] = x0 * std::cos(theta) + nu * std::sin(theta);
//        }
//        
//
//      if (gx1 >= logy) return x1;
//
//      if (x > 0.0)
//          R = theta;
//      else
//          L = theta;
//    }
//
//    std::abort();
//
//    return x0;
//}

//double slice_sample(double x0, slice_function& g,double w, int m)
//{
//  assert(g.in_range(x0));
//
//  double gx0 = g();
////#ifndef NDEBUG
////  volatile double diff = gx0 - g(x0);
////  assert(std::abs(diff) < 1.0e-9);
////#endif
//
//  // Determine the slice level, in log terms.
//
//  double logy = gx0 + log(unif());
//
//  // Find the initial interval to sample from.
//
//  std::pair<double,double> interval = find_initial_slice_boundaries(x0,g,logy,w,m);
//  double L = interval.first;
//  double R = interval.second;
//
//  // Sample from the interval, shrinking it on each rejection
//
//  return search_interval(x0,L,R,g,logy);
//}


//void EllipticalSliceSamplingSimpleMove::performMcmcMove( double lHeat, double pHeat )
//{
//    slice_function g(variables, lHeat, pHeat);
//
//    double x2 = slice_sample(g, window, 100);
//
//    total_movement += std::abs(x2 - x1);
//
//    numPr += g.get_num_evals();
//
////  if (auto_tuning and num_tried > 3)
////  {
////    double predicted_window = 4.0*total_movement/num_tried;
////    window = 0.95*window + 0.05*predicted_window;
////  }
//    
//}

void EllipticalSliceSamplingSimpleMove::performMcmcMove( double lHeat, double pHeat )
{
//    std::cout << "Calling EllipticalSliceSamplingSimpleMove::performMcmcMove" << std::endl;

    slice_function lnL(variables, lHeat, pHeat);

    // Current values
    std::vector<double> f = lnL.current_value();
    
    // Choose ellipse (Murray step 1)
    std::vector<double> nu = std::vector<double>(variables.size(),0.0);
    size_t i = 0;
    for (size_t i=0; i<variables.size(); ++i)
    {
        NormalDistribution* normal = dynamic_cast<NormalDistribution *>( &variables[i]->getDistribution() );
        
        const TypedDagNode<double>* sd = normal->getStDev();
        
        nu[i] = sd->getValue() * RbStatistics::Normal::rv(*GLOBAL_RNG);
        
    }
//    std::cout << "Chose ellipse" << std::endl;
//    std::cout << "nu[0] " << nu[0] << std::endl;
//    std::cout << "nu[98] " << nu[98] << std::endl;

    // Log-likelihood threshold (Murray step 2)
    double L_f = lnL();
    
    double logy = L_f + log(unif());
    
//    std::cout << "Starting \"likelihood\" = " << L_f << "; threshold = " << logy << std::endl;

    // Find initial boundaries and draw first value (Murray step 3)
    double theta = unif()*RbConstants::TwoPI;
    double L = theta - RbConstants::TwoPI;
    double R = theta;
    
    double sin_theta = std::sin(theta);
    double cos_theta = std::cos(theta);

//    std::cout << "Computed windows" << std::endl;
    
    // Run slice loop (Murray steps 4-10, with automatic termination after 200 steps)
    std::vector<double> f_prime = std::vector<double>(variables.size(),0.0);
    for (size_t i=0;i<200;++i)
    {
//        std::cout << "Slicing, theta = " << theta << std::endl;

        // step 4
        for (size_t j = 0; j < f.size(); j++)
        {
            f_prime[j] = f[j] * cos_theta + nu[j] * sin_theta;
        }
//        std::cout << "f[0] " << f[0] << std::endl;
//        std::cout << "f[98] " << f[98] << std::endl;
        
//        std::cout << "f_prime[0] " << f_prime[0] << std::endl;
//        std::cout << "f_prime[98] " << f_prime[98] << std::endl;
//        std::cout << "f\' computed" << std::endl;

        // step 5 (step 6 is return value)
        double L_f_prime = lnL(f_prime);
        
//        std::cout << "\"likelihood\" of f\' = " << L_f_prime << std::endl;

        if (L_f_prime > logy)
        {
//            std::cout << "Found acceptable slice" << std::endl;
            break;
        }
        
        // steps 7-9 (10 being loop again)
        if (theta > 0.0) {
            R = theta;
        }
        else {
            L = theta;
        }
        // Draw next theta, compute sine/cosine
        theta = L + unif()*(R-L);
        sin_theta = std::sin(theta);
        cos_theta = std::cos(theta);
//        std::cout << "Hi from end of loop" << std::endl;
    }
    
    if ( i == 200 ) {
        std::abort();
    }
    
    double new_movement = 0.0;
    
    for (size_t i = 0; i < f.size(); ++i)
    {
        new_movement += std::pow((f_prime[i] - f[i]),2.0);
    }
    total_movement += std::sqrt(new_movement);
    
    numPr += lnL.get_num_evals();
    
//    if (auto_tuning and (num_tried > 3) and (numPr/num_tried > 9) )
//    {
//        window *= 0.95;
//    }
    
}


/**
 * Print the summary of the move.
 *
 * The summary just contains the current value of the tuning parameter.
 * It is printed to the stream that it passed in.
 *
 * \param[in]     o     The stream to which we print the summary.
 */
void EllipticalSliceSamplingSimpleMove::printSummary(std::ostream &o) const 
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
    int w_length = 4;
    if (weight > 0) w_length -= (int)log10(weight);
    for (int i = 0; i < w_length; ++i) {
        o << " ";
    }
    o << weight;
    o << " ";
    
    // print the number of tries
    int t_length = 9;
    if (num_tried > 0) t_length -= (int)log10(num_tried);
    for (int i = 0; i < t_length; ++i) {
        o << " ";
    }
    o << num_tried;
    o << " ";
    
    // print the average distance moved
    o<<"\n";
    if (num_tried > 0)
    {
      o<<"  Ave. ||x2-x1|| = "<<total_movement/num_tried<<std::endl;
    }

    // print the average distance moved
    if (num_tried > 0)
    {
      o<<"  Ave. # of Pr evals = "<<double(numPr)/num_tried<<std::endl;
    }

    //    proposal->printParameterSummary( o );
    o<<"  window = "<<window<<std::endl;
    
    o << std::endl;
    
    o.setf(previousFlags);
    o.precision(previousPrecision);
    
    
}

/**
 * Reset the move counters. Here we only reset the counter for the number of accepted moves.
 *
 */
void EllipticalSliceSamplingSimpleMove::resetMoveCounters( void )
{
    total_movement = 0.0;
    num_tried = 0;
    numPr = 0;
}


/**
 * Swap the current variables for a new one.
 *
 * \param[in]     oldN     The old variables that needs to be replaced.
 * \param[in]     newN     The new Revvariables.
 */
void EllipticalSliceSamplingSimpleMove::swapNodeInternal(DagNode *oldN, DagNode *newN)
{
    
    for (size_t i = 0; i < variables.size(); ++i)
    {
        if ( variables[i] == oldN )
        {
            variables[i] = static_cast<StochasticNode<double> *>(newN);
            assert( not variables[i]->isClamped() );
        }
    }
    
}


/**
 * Tune the move to accept the desired acceptance ratio.
 * We only compute the acceptance ratio here and delegate the call to the proposal.
 */
void EllipticalSliceSamplingSimpleMove::tune( void ) 
{

    if (numPr/num_tried > 9)
    {
        window *= 0.9;
    }

}

