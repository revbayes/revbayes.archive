#include "DagNode.h"
#include "FileMonitor.h"
#include "MonteCarloSampler.h"
#include "MoveSchedule.h"
#include "PowerPosteriorAnalysis.h"
#include "RandomMoveSchedule.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbFileManager.h"
#include "RbOptions.h"
#include "SequenctialMoveSchedule.h"

#include <cmath>
#include <typeinfo>


#ifdef RB_MPI
#include <mpi.h>
#endif

using namespace RevBayesCore;

PowerPosteriorAnalysis::PowerPosteriorAnalysis(MonteCarloSampler *m, const std::string &fn) : Cloneable( ), Parallelizable(),
    filename( fn ),
    powers(),
    sampler( m ),
    sampleFreq( 100 )
{
    
    sampler->setActivePID( pid );
    sampler->setNumberOfProcesses( 1 );
    
}


PowerPosteriorAnalysis::PowerPosteriorAnalysis(const PowerPosteriorAnalysis &a) : Cloneable( a ), Parallelizable( a ),
    filename( a.filename ),
    powers( a.powers ),
    sampler( a.sampler->clone() ),
    sampleFreq( a.sampleFreq )
{
    
}


PowerPosteriorAnalysis::~PowerPosteriorAnalysis(void)
{
    delete sampler;
}


/**
 * Overloaded assignment operator.
 * We need to keep track of the MonteCarloSamplers
 */
PowerPosteriorAnalysis& PowerPosteriorAnalysis::operator=(const PowerPosteriorAnalysis &a)
{
    Parallelizable::operator=( a );
    
    if ( this != &a )
    {
        
        // free the sampler
        delete sampler;
        
        filename        = a.filename;
        powers          = a.powers;
        sampler         = a.sampler->clone();
        sampleFreq      = a.sampleFreq;
        
    }
    
    return *this;
}


/** Run burnin and autotune */
void PowerPosteriorAnalysis::burnin(size_t generations, size_t tuningInterval)
{
    
    // Initialize objects needed by chain
    sampler->initializeSampler();
    
    
    // reset the counters for the move schedules
    sampler->reset();
    
    if ( process_active == true )
    {
        // Let user know what we are doing
        std::stringstream ss;
        ss << "\n";
        ss << "Running burn-in phase of Power Posterior sampler for " << generations << " iterations.\n";
        ss << sampler->getStrategyDescription();
        std::cout << ss.str() << std::endl;
    
        // Print progress bar (68 characters wide)
        std::cout << std::endl;
        std::cout << "Progress:" << std::endl;
        std::cout << "0---------------25---------------50---------------75--------------100" << std::endl;
        std::cout.flush();
    }
    
    
    // Run the chain
    size_t numStars = 0;
    for (size_t k=1; k<=generations; k++)
    {
        if ( process_active == true )
        {
            size_t progress = 68 * (double) k / (double) generations;
            if ( progress > numStars )
            {
                for ( ;  numStars < progress; ++numStars )
                    std::cout << "*";
                
                std::cout.flush();
            }
        }
        
        sampler->nextCycle(false);
            
        // check for autotuning
        if ( k % tuningInterval == 0 && k != generations )
        {
                
            sampler->tune();
            
        }
        
    }
    
    
    if ( process_active == true )
    {
        std::cout << std::endl;
    }
    
}



PowerPosteriorAnalysis* PowerPosteriorAnalysis::clone( void ) const
{
    
    return new PowerPosteriorAnalysis( *this );
}


void PowerPosteriorAnalysis::runAll(size_t gen)
{
    
    // print some information to the screen but only if we are the active process
    if ( process_active == true )
    {
        std::cout << std::endl;
        std::cout << "Running power posterior analysis ..." << std::endl;
    }
    
    // compute which block of the data this process needs to compute
    size_t stone_block_start = size_t(floor( (double(pid)   / num_processes ) * powers.size()) );
    size_t stone_block_end   = size_t(floor( (double(pid+1) / num_processes ) * powers.size()) );
    
    /* Run the chain */
    for (size_t i = stone_block_start; i < stone_block_end; ++i)
    {
    
        // run the i-th stone
        runStone(i, gen);
        
    }
    
#ifdef RB_MPI
    // wait until all chains complete
    MPI::COMM_WORLD.Barrier();
#endif
    if ( process_active == true )
    {
        summarizeStones();
    }
    
}



void PowerPosteriorAnalysis::runStone(size_t idx, size_t gen)
{
    
    // create the directory if necessary
    RbFileManager fm = RbFileManager(filename);
    std::string stoneFileName = fm.getFileNameWithoutExtension() + "_stone_" + idx + "." + fm.getFileExtension();

    RbFileManager f = RbFileManager(fm.getFilePath(), stoneFileName);
    f.createDirectoryForFile();
    
    std::fstream outStream;
    outStream.open( f.getFullFileName().c_str(), std::fstream::out);
    outStream << "state\t" << "power\t" << "likelihood" << std::endl;
    
    // reset the counters for the move schedules
    sampler->reset();
    
//    if ( sampler->getCurrentGeneration() == 0 )
//    {
//    }
    
    /* Reset the monitors */
    //    for (size_t i=0; i<replicates; ++i)
    //    {
    //        for (size_t j=0; i<runs[i].getMonitors().size(); i++)
    //        {
    //            runs[i].getMonitors()[j].reset( kIterations);
    //        }
    //    }
    
    // reset the stopping rules
//    for (size_t i=0; i<rules.size(); ++i)
//    {
//        rules[i].runStarted();
//    }

    
    size_t burnin = size_t( ceil( 0.25*gen ) );
    
    size_t printInterval = size_t( round( fmax(1,gen/20.0) ) );
    size_t digits = size_t( ceil( log10( powers.size() ) ) );
    
    // Run the chain
    if ( process_active ==true )
    {
        std::cout << "Step ";
        for (size_t d = size_t( ceil( log10( idx+1.1 ) ) ); d < digits; d++ )
        {
            std::cout << " ";
        }
        std::cout << (idx+1) << " / " << powers.size();
        std::cout << "\t\t";
    }
    
    // set the power of this sampler
    sampler->setLikelihoodHeat( powers[idx] );
    
    std::stringstream ss;
    ss << "_stone_" << idx;
    sampler->addFileMonitorExtension( ss.str(), false);
    
    
    // Monitor
    sampler->startMonitors(gen);
    sampler->monitor(0);
    
    double p = powers[idx];
    for (size_t k=1; k<=gen; k++)
    {
        
        if ( process_active == true )
        {
            if ( k % printInterval == 0 )
            {
                std::cout << "**";
                std::cout.flush();
            }
        }
        
        sampler->nextCycle( true );

        // Monitor
        sampler->monitor(k);
        
        // sample the likelihood
        if ( k > burnin && k % sampleFreq == 0 )
        {
            // compute the joint likelihood
            double likelihood = sampler->getModelLnProbability();
            outStream << k << "\t" << p << "\t" << likelihood << std::endl;
        }
            
    }
    
    if ( process_active == true )
    {
        std::cout << std::endl;
    }
    
    outStream.close();
    
    
}


void PowerPosteriorAnalysis::summarizeStones( void )
{
    // create the directory if necessary
    RbFileManager f = RbFileManager(filename);
    f.createDirectoryForFile();
    
    std::ofstream outStream;
    outStream.open( filename.c_str(), std::fstream::out);
    outStream << "state\t" << "power\t" << "likelihood" << std::endl;

    /* Append each stone */
    for (size_t idx = 0; idx < powers.size(); ++idx)
    {
        RbFileManager fm = RbFileManager(filename);
        std::string stoneFileName = fm.getFileNameWithoutExtension() + "_stone_" + idx + "." + fm.getFileExtension();
        
        RbFileManager f = RbFileManager(fm.getFilePath(), stoneFileName);

        // read the i-th stone
        std::ifstream inStream;
        inStream.open( f.getFullFileName().c_str(), std::fstream::in);
        if (inStream.is_open())
        {
            bool header = true;
            std::string line = "";
            while ( std::getline (inStream,line) )
            {
                // we need to skip the header line
                if ( header == true )
                {
                    header  = false;
                }
                else
                {
                    outStream << line << std::endl;
                }
                
            }
            inStream.close();
        }
        else
        {
            std::cerr << "Problem reading stone " << idx+1 << " from file " << stoneFileName << "." << std::endl;
        }

    }

}


void PowerPosteriorAnalysis::setPowers(const std::vector<double> &p)
{
    powers = p;
}


void PowerPosteriorAnalysis::setSampleFreq(size_t sf)
{
    sampleFreq = sf;
}

