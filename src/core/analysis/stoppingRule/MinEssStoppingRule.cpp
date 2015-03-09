#include "EssTest.h"
#include "MinEssStoppingRule.h"
#include "RbFileManager.h"
#include "StringUtilities.h"
#include "TraceContinuousReader.h"


using namespace RevBayesCore;


MinEssStoppingRule::MinEssStoppingRule(double m, const std::string &fn, size_t f, BurninEstimatorContinuous *be) : StoppingRule(),
    burninEst( be ),
    checkFrequency( f ),
    filename( fn ),
    minEss( m ),
    numReplicates( 1 )
{
    
}


MinEssStoppingRule::MinEssStoppingRule(const MinEssStoppingRule &sr) : StoppingRule(),
    burninEst( sr.burninEst->clone() ),
    checkFrequency( sr.checkFrequency ),
    filename( sr.filename ),
    minEss( sr.minEss ),
    numReplicates( sr.numReplicates )
{
    
}



MinEssStoppingRule::~MinEssStoppingRule()
{
    
    delete burninEst;
}


/**
 * Should we check at the given iteration for convergence?
 * Only for given iteration because this is a time consuming test.
 */
bool MinEssStoppingRule::checkAtIteration(size_t g) const
{
    // test if the iteration matches the pre-specified frequency
    return (g % checkFrequency) == 0;
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself
 */
MinEssStoppingRule* MinEssStoppingRule::clone( void ) const
{
    
    return new MinEssStoppingRule( *this );
}


/**
 * Is this a stopping rule? Yes!
 */
bool MinEssStoppingRule::isConvergenceRule( void ) const
{
    return true;
}


/**
 * The run just started. For this rule we do not need to do anything.
 */
void MinEssStoppingRule::runStarted( void )
{
    // nothing to do
}


/**
 * Set the number of runs/replicates.
 * Here we need to adjust the files from which we read in.
 */
void MinEssStoppingRule::setNumberOfRuns(size_t n)
{
    numReplicates = n;
}


/**
 * Should we stop now?
 * Yes, if the minimum ESS is larger than the provided threshold.
 */
bool MinEssStoppingRule::stop( size_t g )
{
    
    bool passed = true;
    
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
    
        for ( size_t i = 0; i < data.size(); ++i)
        {
            Trace &t = data[i];
            const std::vector<double> &v = t.getValues();
            size_t b = burninEst->estimateBurnin( v );
            if ( maxBurnin < b )
            {
                maxBurnin = b;
            }
        }
    
        EssTest essTest = EssTest( minEss );
        
        for ( size_t i = 0; i < data.size(); ++i)
        {
            RevBayesCore::Trace &t = data[i];
            const std::vector<double> &v = t.getValues();
            t.setBurnin( maxBurnin );
            t.computeStatistics();
        
            passed &= essTest.assessConvergenceSingleChain( v, maxBurnin );
        }
        
    }
    
//    if ( passed )
//    {
//        std::cerr << "Reached minimum ESS!" << std::endl;
//    }
    
    
    return passed;
}
