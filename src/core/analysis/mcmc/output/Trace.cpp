#include "GewekeTest.h"
#include "RbConstants.h"
#include "StationarityTest.h"
#include "Trace.h"
#include "TraceAnalysisContinuous.h"

#include "RbUtil.h"

#include <sstream>
#include <string>
#include <vector>

using namespace RevBayesCore;

Trace::Trace()
{
    invalidate();
}


void Trace::addObject(double d) {
    values.push_back(d);
    
    // invalidate for recalculation of meta data
    invalidate();
}


void Trace::computeStatistics( void ) { 
    
    // check if we need to set the burnin
    if ( burnin == RbConstants::Size_t::nan ) {
        burnin = size();
        burnin *= stepSize;
        burnin = (size_t)(burnin * 0.1);
    }
    
    TraceAnalysisContinuous* analysis = new TraceAnalysisContinuous();
    analysis->analyseCorrelation(values, burnin);
    ess = analysis->getEss();
    mean = analysis->getMean();
    sem = analysis->getStdErrorOfMean();
    
    
    
    // test stationarity within chain
    size_t nBlocks = 10;
    StationarityTest testS = StationarityTest(nBlocks, 0.01);
    passedStationarityTest = testS.assessConvergenceSingleChain(values, burnin);
    
    // Geweke's test for convergence within a chain
    GewekeTest testG = GewekeTest(0.01);
    passedGewekeTest = testG.assessConvergenceSingleChain(values, burnin);
        
}


/** Clone function */
Trace* Trace::clone() const {
    
    return new Trace(*this);
}


void Trace::invalidate() {
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


void Trace::removeObjectAtIndex (int index) {
    // create a iterator for the vector
    std::vector<double>::iterator it = values.begin();
    
    //jump to the position to remove
    it += index;
    
    // remove the element
    values.erase(it);
    
    // invalidate for recalculation of meta data
    invalidate();
}

void Trace::removeLastObject() {
    // remove object from list
    values.pop_back();
    
    // invalidate for recalculation of meta data
    invalidate();
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const Trace& x) {
    o << x.getParameterName();
    o << " (";
    const std::vector<double>& values = x.getValues();
    for (std::vector<double>::const_iterator it = values.begin(); it != values.end(); ++it) {
        if ( it != values.begin() ) {
            o << ", ";
        }
        o << *it;
    }
    o << ")";
    
    return o;
}





