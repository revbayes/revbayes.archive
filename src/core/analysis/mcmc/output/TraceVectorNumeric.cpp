#include "GewekeTest.h"
#include "RbConstants.h"
#include "StationarityTest.h"
#include "TraceVectorNumeric.h"
#include "TraceAnalysisContinuous.h"

#include "RbUtil.h"

#include <sstream>
#include <string>
#include <vector>

using namespace RevBayesCore;

TraceVectorNumeric::TraceVectorNumeric()
{
    invalidate();
}


void TraceVectorNumeric::addObject(RbVector<double> d)
{
    values.push_back(d);
    
    // invalidate for recalculation of meta data
    invalidate();
}


void TraceVectorNumeric::addValueFromString(const std::string &s)
{
    
    RbVector<double> *d = new RbVector<double>();
    Serializer<RbVector<double>, IsDerivedFrom<RbVector<double>, Serializable>::Is >::ressurectFromString( d, s );
    
    addObject( *d );
    
    // free memory
    delete d;
    
}

void TraceVectorNumeric::computeStatistics( void )
{
    
    
}


/** Clone function */
TraceVectorNumeric* TraceVectorNumeric::clone() const
{
    
    return new TraceVectorNumeric(*this);
}


void TraceVectorNumeric::invalidate()
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


bool TraceVectorNumeric::isCoveredInInterval(const std::string &v, double i) const
{
//    std::cerr << "Is '" << v << "' covered for parameter " << parmName << std::endl;
    
    RbVector<double> sample = RbVector<double>();
    sample.initFromString( v );
    
    RbVector<double> smaller_values_count = RbVector<double>(sample.size(), 0.0);
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
    for (size_t j=0; j<sample.size(); ++j)
    {
        double quantile = smaller_values_count[j] / double(values.size());
        double lower = (1.0 - i) / 2.0;
        double upper = 1.0 - lower;
        covered &= ( quantile >= lower && quantile <= upper );
    }
//    std::cerr << ( covered ? "YES" : "NO" ) << std::endl;
    
    return covered;
}


void TraceVectorNumeric::removeObjectAtIndex (int index)
{

}

void TraceVectorNumeric::removeLastObject() {
    // remove object from list
    values.pop_back();
    
    // invalidate for recalculation of meta data
    invalidate();
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const TraceVectorNumeric& x) {
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





