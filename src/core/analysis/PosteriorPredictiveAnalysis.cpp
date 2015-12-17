#include "DagNode.h"
#include "MaxIterationStoppingRule.h"
#include "MonteCarloAnalysis.h"
#include "MonteCarloSampler.h"
#include "PosteriorPredictiveAnalysis.h"
#include "RbException.h"
#include "RlUserInterface.h"

#include <cmath>
#include <typeinfo>


using namespace RevBayesCore;

PosteriorPredictiveAnalysis::PosteriorPredictiveAnalysis( const MonteCarloAnalysis &m, const std::string &fn ) : Cloneable( ), Parallelizable(),
    directory( fn ),
    processors_per_likelihood( 1 ),
    template_sampler( m )
{
    
//    // MPI settings
//    size_t active_proc = size_t( floor( pid / double(processors_per_likelihood)) );
//    m.setActivePID( active_proc );
//    m.setNumberOfProcesses( processors_per_likelihood );
    
    
}


PosteriorPredictiveAnalysis::PosteriorPredictiveAnalysis(const PosteriorPredictiveAnalysis &a) : Cloneable( a ), Parallelizable(a),
    directory( a.directory ),
    processors_per_likelihood( a.processors_per_likelihood ),
    template_sampler( a.template_sampler )
{
    
}


PosteriorPredictiveAnalysis::~PosteriorPredictiveAnalysis(void)
{
    
}


///** Run burnin and autotune */
//void PosteriorPredictiveAnalysis::burnin(size_t generations, size_t tuningInterval)
//{
//    
//    if ( process_active == true )
//    {
//        // Let user know what we are doing
//        std::stringstream ss;
//        ss << "\n";
//        ss << "Running burn-in phase of Monte Carlo sampler " << num_runs <<  " each for " << generations << " iterations.\n";
//        RBOUT( ss.str() );
//    
//        // Print progress bar (68 characters wide)
//        std::cout << std::endl;
//        std::cout << "Progress:" << std::endl;
//        std::cout << "0---------------25---------------50---------------75--------------100" << std::endl;
//        std::cout.flush();
//    }
//    
//    // compute which block of the data this process needs to compute
//    size_t run_block_start = size_t(floor( (double(pid)   / num_processes ) * num_runs) );
//    size_t run_block_end   = size_t(floor( (double(pid+1) / num_processes ) * num_runs) );
//    //    size_t stone_block_size  = stone_block_end - stone_block_start;
//    
//    // Run the chain
//    size_t numStars = 0;
//    for (size_t i = run_block_start; i < run_block_end; ++i)
//    {
//        if ( process_active == true )
//        {
//            size_t progress = 68 * (double) i / (double) (run_block_end - run_block_start);
//            if ( progress > numStars )
//            {
//                for ( ;  numStars < progress; ++numStars )
//                    std::cout << "*";
//                std::cout.flush();
//            }
//        }
//        
//        // run the i-th analyses
//        runs[i]->burnin(generations, tuningInterval, false);
//        
//    }
//    
//    if ( process_active == true )
//    {
//        std::cout << std::endl;
//    }
//    
//}



PosteriorPredictiveAnalysis* PosteriorPredictiveAnalysis::clone( void ) const
{
    
    return new PosteriorPredictiveAnalysis( *this );
}


void PosteriorPredictiveAnalysis::runAll(size_t gen)
{
    // print some information to the screen but only if we are the active process
    if ( process_active == true )
    {
        std::cout << std::endl;
        std::cout << "Running posterior predictive analysis ..." << std::endl;
    }

    
    // create the directory if necessary
    RbFileManager fm = RbFileManager( directory );
    if ( !fm.testFile() && !fm.testDirectory() )
    {
        std::string errorStr = "";
        fm.formatError(errorStr);
        throw RbException("Could not find file or path with name \"" + directory + "\"");
    }
    
    // set up a vector of strings containing the name or names of the files to be read
    std::vector<std::string> dir_names;
    if ( fm.isDirectory() == true )
    {
        fm.setStringWithNamesOfFilesInDirectory( dir_names, false );
    }
    else
    {
        throw RbException( "\"" + directory + "\" is not a directory.");
    }
    
    
    size_t num_runs = dir_names.size();
    //    size_t run_pid_start =  floor( ( floor( pid   /double(processors_per_likelihood)) / (double(num_processes) / processors_per_likelihood) ) * powers.size() );
    //    size_t run_pid_end   =  floor( ( ceil( (pid+1)/double(processors_per_likelihood)) / (double(num_processes) / processors_per_likelihood) ) * powers.size() );
    for ( size_t i = 0; i < num_runs; ++i)
    {
        
        size_t run_pid_start = size_t(floor( (double(i) / num_runs ) * num_processes ) );
        size_t run_pid_end   = std::max( int(run_pid_start), int(floor( (double(i+1) / num_runs ) * num_processes ) ) - 1);
        
        int number_processes_per_run = int(run_pid_end) - int(run_pid_start) + 1;
        
        if ( pid >= run_pid_start && pid <= run_pid_end)
        {
            // create an independent copy of the analysis
            MonteCarloAnalysis *current_analysis = template_sampler.clone();
            
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
            
            // set the processors for this analysis
            current_analysis->setActivePID( run_pid_start );
            current_analysis->setNumberOfProcesses( number_processes_per_run );
            
            // print some info
            if ( process_active == true )
            {
                size_t digits = size_t( ceil( log10( num_runs ) ) );
                std::cout << "Sim ";
                for (size_t d = size_t( ceil( log10( i+1.1 ) ) ); d < digits; d++ )
                {
                    std::cout << " ";
                }
                std::cout << (i+1) << " / " << num_runs;
                std::cout << "\t\t";
            }
            
            // run the i-th stone
            runSim(current_analysis, gen);
            
            
            std::cout << std::endl;
            
            // free memory
            delete current_analysis;
            
        }
        
    }
    
    
}



void PosteriorPredictiveAnalysis::runSim(MonteCarloAnalysis *sampler, size_t gen)
{
    
    // run the analysis
    RbVector<StoppingRule> rules;
    
    size_t currentGen = sampler->getCurrentGeneration();
    rules.push_back( MaxIterationStoppingRule(gen + currentGen) );
    
    sampler->run(gen, rules, false);
    
}

