#include "StationarityTest.h"
#include "StationarityStoppingRule.h"
#include "RbException.h"
#include "RbFileManager.h"
#include "StringUtilities.h"
#include "TraceContinuousReader.h"


using namespace RevBayesCore;


StationarityStoppingRule::StationarityStoppingRule(double p, const std::string &fn, size_t f, BurninEstimatorContinuous *be) : AbstractConvergenceStoppingRule(fn, f, be),
    prob( p )
{
    
}



StationarityStoppingRule::~StationarityStoppingRule()
{
    
}



/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself
 */
StationarityStoppingRule* StationarityStoppingRule::clone( void ) const
{
    
    return new StationarityStoppingRule( *this );
}


/**
 * Set the number of runs/replicates.
 * Here we need to adjust the files from which we read in.
 */
void StationarityStoppingRule::setNumberOfRuns(size_t n)
{
    numReplicates = n;
    
    if ( n < 2 )
    {
        throw RbException("You need at least two replicates for the Stationarity between runs convergence statistic.");
    }
}


/**
 * Should we stop now?
 * Yes, if the ratio of the variance of samples between chains over the variance of samples withing chains
 * is smaller than the provided threshold.
 */
bool StationarityStoppingRule::stop( size_t g )
{
    
    bool passed = true;
    
    std::vector<std::vector<std::vector<double> > > values;
    std::vector<std::vector<size_t> > burnins;
    for ( size_t i = 1; i <= numReplicates; ++i)
    {
        std::string fn = filename;
        if ( numReplicates > 1 )
        {
            RbFileManager fm = RbFileManager(filename);
            fn = fm.getFilePath() + fm.getPathSeparator() + fm.getFileNameWithoutExtension() + "_run_" + StringUtilities::to_string(i) + "." + fm.getFileExtension();
        }
        
        TraceContinuousReader reader = TraceContinuousReader( fn );
        
        // get the vector of traces from the reader
        std::vector<Trace> &data = reader.getTraces();
        
        size_t maxBurnin = 0;
        
        for ( size_t j = 0; j < data.size(); ++j)
        {
            Trace &t = data[j];
            const std::vector<double> &v = t.getValues();
            size_t b = burninEst->estimateBurnin( v );
            if ( maxBurnin < b )
            {
                maxBurnin = b;
            }
            
        }
        
        // store the values
        for ( size_t j = 0; j < data.size(); ++j)
        {
            Trace &t = data[j];
            const std::vector<double> &v = t.getValues();
            
            // get the matrix of values for the j-th parameter
            if ( j >= values.size() )
            {
                values.push_back( std::vector<std::vector<double> >() );
            }
            
            std::vector<std::vector<double> > &valueMatrix = values[j];
            
            // get the vector of values for the i-th run/chain
            if ( i > valueMatrix.size() )
            {
                valueMatrix.push_back( std::vector<double>() );
            }
            
            std::vector<double> &valueVector = valueMatrix[i-1];
            
            //            valueVector.insert(valueVector.begin(), v.begin()+maxBurnin, v.end());
            valueVector.insert(valueVector.begin(), v.begin(), v.end());
            
            // get the matrix of values for the j-th parameter
            if ( j >= burnins.size() )
            {
                burnins.push_back( std::vector<size_t>() );
            }
            std::vector<size_t> &b = burnins[j];
            
            // setting the burnin for th i-th chain.
            b.push_back(maxBurnin);
            
        }
        
    }
    
    
    
    StationarityTest sTest = StationarityTest( numReplicates, prob );
    for ( size_t i = 0; i < values.size(); ++i)
    {
        const std::vector< std::vector<double> > &v = values[i];
        
        passed &= sTest.assessConvergenceMultipleChains(v, burnins[i]);
    }
    
    
    if ( passed )
    {
        std::cerr << "Passed Stationarity!" << std::endl;
    }
    else
    {
        std::cerr << "Failed Stationarity!" << std::endl;
    }
    
    
    return passed;
}
