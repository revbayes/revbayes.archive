#include "GewekeTest.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "StationarityTest.h"
#include "TraceSimplex.h"
#include "TraceAnalysisContinuous.h"

#include "RbUtil.h"

#include <cmath>
#include <sstream>
#include <string>
#include <vector>

using namespace RevBayesCore;

TraceSimplex::TraceSimplex()
{
    invalidate();
}


void TraceSimplex::addObject(Simplex d)
{
    values.push_back(d);
    
    // invalidate for recalculation of meta data
    invalidate();
}


void TraceSimplex::addValueFromString(const std::string &s)
{
    
    Simplex *d = new Simplex();
    Serializer<Simplex, IsDerivedFrom<Simplex, Serializable>::Is >::ressurectFromString( d, s );
    
    addObject( *d );
    
    // free memory
    delete d;
    
}

void TraceSimplex::computeStatistics( void )
{
    
    
}


/** Clone function */
TraceSimplex* TraceSimplex::clone() const
{
    
    return new TraceSimplex(*this);
}


void TraceSimplex::invalidate()
{
    // set values to defaults and mark for recalculation
    burnin                          = RbConstants::Size_t::nan;
    ess                             = -1;
    mean                            = 0.0;
    median                          = 0.0;
    sem                             = -1;
    stepSize                        = 1;
    
    converged                       = NOT_CHECKED;
    passedStationarityTest          = NOT_CHECKED;
    passedGewekeTest                = NOT_CHECKED;
    //    passedHeidelbergerWelchStatistic = NOT_CHECKED;
    //    passedRafteryLewisStatistic = NOT_CHECKED;
    passedEssThreshold              = NOT_CHECKED;
    passedSemThreshold              = NOT_CHECKED;
    passedIidBetweenChainsStatistic = NOT_CHECKED;
    passedGelmanRubinTest           = NOT_CHECKED;
    
    
}


bool TraceSimplex::isCoveredInInterval(const std::string &v, double i, bool verbose) const
{
    
    Simplex sample = Simplex();
    sample.initFromString( v );
    
    //    double alpha = 1.0 - std::pow(1.0-i,double(sample.size()));
    double alpha = i;
    
    Simplex smaller_values_count = Simplex(sample.size(), 0.0);
    for (size_t i=0; i<values.size(); ++i)
    {
        
        for (size_t j=0; j<sample.size(); ++j)
        {
            
            if (values[i][j] < sample[j] )
            {
                ++smaller_values_count[j];
            }
            
        }
        
    }
    
    
    bool covered = true;
    double num_covered = 0.0;
    for (size_t j=0; j<sample.size(); ++j)
    {
        double quantile = smaller_values_count[j] / double(values.size());
        double lower = (1.0 - alpha) / 2.0;
        double upper = 1.0 - lower;
        if ( quantile >= lower && quantile <= upper )
        {
            ++num_covered;
        }
        covered &= ( quantile >= lower && quantile <= upper );
    }
    
    RandomNumberGenerator *rng = GLOBAL_RNG;
    double include_prob = num_covered / sample.size();
    covered = ( include_prob > rng->uniform01() );
    
    return covered;
}


void TraceSimplex::removeObjectAtIndex (int index)
{
    
}

void TraceSimplex::removeLastObject() {
    // remove object from list
    values.pop_back();
    
    // invalidate for recalculation of meta data
    invalidate();
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const TraceSimplex& x) {
    o << x.getParameterName();
    o << " (";
    //    const std::vector<double>& values = x.getValues();
    //    for (std::vector<double>::const_iterator it = values.begin(); it != values.end(); ++it) {
    //        if ( it != values.begin() ) {
    //            o << ", ";
    //        }
    //        o << *it;
    //    }
    o << ")";
    
    return o;
}





