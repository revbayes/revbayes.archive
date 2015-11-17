#include "DagNode.h"
#include "MaxIterationStoppingRule.h"
#include "MonteCarloAnalysis.h"
#include "MonteCarloSampler.h"
#include "PosteriorPredictiveAnalysis.h"
#include "RbException.h"
#include "RlUserInterface.h"

#include <cmath>
#include <typeinfo>


#ifdef RB_MPI
#include <mpi.h>
#endif

using namespace RevBayesCore;

PosteriorPredictiveAnalysis::PosteriorPredictiveAnalysis( const MonteCarloAnalysis &m, const std::string &fn ) : Cloneable( ),
    active_PID( 0 ),
    directory( fn ),
    num_processes( 1 ),
    num_runs( 0 ),
    pid( 0 ),
    processActive( true )
{
    
#ifdef RB_MPI
    num_processes = MPI::COMM_WORLD.Get_size();
    pid = MPI::COMM_WORLD.Get_rank();
#endif
    
    processActive = (pid == active_PID);
    
    
    // create the directory if necessary
    RbFileManager fm = RbFileManager( directory );
    if ( !fm.testFile() && !fm.testDirectory() )
    {
        std::string errorStr = "";
        fm.formatError(errorStr);
        throw RbException("Could not find file or path with name \"" + fn + "\"");
    }
    
    // set up a vector of strings containing the name or names of the files to be read
    std::vector<std::string> dir_names;
    if ( fm.isDirectory() == true )
    {
        fm.setStringWithNamesOfFilesInDirectory( dir_names, false );
    }
    else
    {
        throw RbException( "\"" + fn + "\" is not a directory.");
    }
    
    for ( size_t i = 0; i < dir_names.size(); ++i)
    {
        // increase the counter of the number of runs
        ++num_runs;
        
        // create an independent copy of the analysis
        MonteCarloAnalysis *current_analysis = m.clone();
        
        // get the model of the analysis
        Model* current_model = current_analysis->getModel().clone();
        
        // get the DAG nodes of the model
        std::vector<DagNode*> &current_nodes = current_model->getDagNodes();
        
        for (size_t j = 0; j < current_nodes.size(); ++j)
        {
            DagNode *the_node = current_nodes[j];
            if ( the_node->isClamped() == true )
            {
                the_node->setValueFromFile( dir_names[i] );
            }
            
        }
        
        RbFileManager tmp = RbFileManager( dir_names[i] );
        
        // now set the model of the current analysis
        current_analysis->setModel( current_model );
        
        // set the monitor index
        current_analysis->addFileMonitorExtension(tmp.getLastPathComponent(), true);
        
        // add the current analysis to our vector of analyses
        runs.push_back( current_analysis );
    }
    
    
#ifdef RB_MPI
    size_t numProcessesPerReplicate = num_processes / num_runs;
    for (size_t i = 0; i < num_runs; ++i)
    {
        if ( num_runs > 1 )
        {
            runs[i]->setReplicateIndex( i+1 );
        }
        runs[i]->setActive( true );
        runs[i]->setNumberOfProcesses( numProcessesPerReplicate );
    }
#endif
    
}


PosteriorPredictiveAnalysis::PosteriorPredictiveAnalysis(const PosteriorPredictiveAnalysis &a) : Cloneable( a ),
    active_PID( a.active_PID ),
    directory( a.directory ),
    num_processes( a.num_processes ),
    num_runs( a.num_runs ),
    pid( a.pid ),
    processActive( a.processActive )
{
    
    // create replicate Monte Carlo samplers
    for (size_t i=0; i < num_runs; ++i)
    {
        runs.push_back( a.runs[i]->clone() );
    }
    
}


PosteriorPredictiveAnalysis::~PosteriorPredictiveAnalysis(void)
{
    // free the runs
    for (size_t i = 0; i < num_runs; ++i)
    {
        MonteCarloAnalysis *sampler = runs[i];
        delete sampler;
    }
}


/**
 * Overloaded assignment operator.
 * We need to keep track of the MonteCarloSamplers
 */
PosteriorPredictiveAnalysis& PosteriorPredictiveAnalysis::operator=(const PosteriorPredictiveAnalysis &a)
{
    
    if ( this != &a )
    {
        
        // free the runs
        for (size_t i = 0; i < num_runs; ++i)
        {
            MonteCarloAnalysis *sampler = runs[i];
            delete sampler;
        }
        runs.clear();
        
        active_PID      = a.active_PID;
        directory       = a.directory;
        num_processes   = a.num_processes;
        num_runs        =
        pid             = a.pid;
        processActive   = a.processActive;
        
        
        // create replicate Monte Carlo samplers
        for (size_t i=0; i < num_runs; ++i)
        {
            runs.push_back( a.runs[i]->clone() );
        }
        
    }
    
    return *this;
}


/** Run burnin and autotune */
void PosteriorPredictiveAnalysis::burnin(size_t generations, size_t tuningInterval)
{
    
    if ( processActive == true )
    {
        // Let user know what we are doing
        std::stringstream ss;
        ss << "\n";
        ss << "Running burn-in phase of Monte Carlo sampler " << num_runs <<  " each for " << generations << " iterations.\n";
        RBOUT( ss.str() );
    
        // Print progress bar (68 characters wide)
        std::cout << std::endl;
        std::cout << "Progress:" << std::endl;
        std::cout << "0---------------25---------------50---------------75--------------100" << std::endl;
        std::cout.flush();
    }
    
    // compute which block of the data this process needs to compute
    size_t run_block_start = size_t(floor( (double(pid)   / num_processes ) * num_runs) );
    size_t run_block_end   = size_t(floor( (double(pid+1) / num_processes ) * num_runs) );
    //    size_t stone_block_size  = stone_block_end - stone_block_start;
    
    // Run the chain
    size_t numStars = 0;
    for (size_t i = run_block_start; i < run_block_end; ++i)
    {
        if ( processActive == true )
        {
            size_t progress = 68 * (double) i / (double) (run_block_end - run_block_start);
            if ( progress > numStars )
            {
                for ( ;  numStars < progress; ++numStars )
                    std::cout << "*";
                std::cout.flush();
            }
        }
        
        // run the i-th analyses
        runs[i]->burnin(generations, tuningInterval, false);
        
    }
    
    if ( processActive == true )
    {
        std::cout << std::endl;
    }
    
}



PosteriorPredictiveAnalysis* PosteriorPredictiveAnalysis::clone( void ) const
{
    
    return new PosteriorPredictiveAnalysis( *this );
}


void PosteriorPredictiveAnalysis::runAll(size_t gen)
{
    
    // print some information to the screen but only if we are the active process
    if ( processActive )
    {
        std::cout << std::endl;
        std::cout << "Running posterior predictive analysis ..." << std::endl;
    }
    
    // compute which block of the runs this process needs to compute
    size_t run_block_start = size_t(floor( (double(pid)   / num_processes ) * num_runs) );
    size_t run_block_end   = size_t(floor( (double(pid+1) / num_processes ) * num_runs) );
    //    size_t stone_block_size  = stone_block_end - stone_block_start;
    
    // Run the chain
    for (size_t i = run_block_start; i < run_block_end; ++i)
    {
        
        // run the i-th stone
        runSim(i, gen);
        
    }
    
    
}



void PosteriorPredictiveAnalysis::runSim(size_t idx, size_t gen)
{
    // print some info
    if ( processActive )
    {
        size_t digits = size_t( ceil( log10( num_runs ) ) );
        std::cout << "Sim ";
        for (size_t d = size_t( ceil( log10( idx+1.1 ) ) ); d < digits; d++ )
        {
            std::cout << " ";
        }
        std::cout << (idx+1) << " / " << num_runs;
        std::cout << "\t\t";
    }
    
    // get the current sample
    MonteCarloAnalysis *analysis = runs[idx];
    
    // run the analysis
    RbVector<StoppingRule> rules;
    
    size_t currentGen = analysis->getCurrentGeneration();
    rules.push_back( MaxIterationStoppingRule(gen + currentGen) );
    
    analysis->run(gen, rules, false);
    
    std::cout << std::endl;
    
}

