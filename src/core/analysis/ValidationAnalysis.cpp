#include "DagNode.h"
#include "MaxIterationStoppingRule.h"
#include "MonteCarloAnalysis.h"
#include "MonteCarloSampler.h"
#include "RbException.h"
#include "RlUserInterface.h"
#include "StochasticVariableMonitor.h"
#include "Trace.h"
#include "TraceReader.h"
#include "ValidationAnalysis.h"

#include <cmath>
#include <typeinfo>


#ifdef RB_MPI
#include <mpi.h>
#endif

using namespace RevBayesCore;

ValidationAnalysis::ValidationAnalysis( const MonteCarloAnalysis &m, size_t n ) : Cloneable( ),
    active_PID( 0 ),
    num_processes( 1 ),
    num_runs( n ),
    pid( 0 ),
    processActive( true )
{
    
#ifdef RB_MPI
    num_processes = MPI::COMM_WORLD.Get_size();
    pid = MPI::COMM_WORLD.Get_rank();
#endif
    
    processActive = (pid == active_PID);
    
    // remove all monitors if there are any
    MonteCarloAnalysis *sampler = m.clone();
    sampler->removeMonitors();
    
    StochasticVariableMonitor mntr = StochasticVariableMonitor(10, "output/posterior_samples.var", "\t");
    sampler->addMonitor( mntr );
    
    
    for ( size_t i = 0; i < num_runs; ++i)
    {
        
        // create an independent copy of the analysis
        MonteCarloAnalysis *current_analysis = sampler->clone();
        
        // get the model of the analysis
        Model* current_model = current_analysis->getModel().clone();
        
        // get the DAG nodes of the model
        std::vector<DagNode *> current_ordered_nodes = current_model->getOrderedStochasticNodes();
        
        for (size_t j = 0; j < current_ordered_nodes.size(); ++j)
        {
            DagNode *the_node = current_ordered_nodes[j];
            
            if ( the_node->isStochastic() == true )
            {
                the_node->redraw();
//                std::cerr << the_node->getName() << ":\t" << the_node->getValueAsString() << std::endl;
            }
            
        }
        
        // now set the model of the current analysis
        current_analysis->setModel( current_model );
        
        std::stringstream ss;
        ss << "Validation_Sim_" << i;
        
        // set the monitor index
        current_analysis->addFileMonitorExtension(ss.str(), true);
        
        // add the current analysis to our vector of analyses
        runs.push_back( current_analysis );
        simulation_values.push_back( runs[i]->getModel().clone() );
        
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
    
    delete sampler;
    
}


ValidationAnalysis::ValidationAnalysis(const ValidationAnalysis &a) : Cloneable( a ),
    active_PID( a.active_PID ),
    num_processes( a.num_processes ),
    num_runs( a.num_runs ),
    pid( a.pid ),
    processActive( a.processActive )
{
    
    // create replicate Monte Carlo samplers
    for (size_t i=0; i < num_runs; ++i)
    {
        // only copy the runs which this process needs to execute
        if ( a.runs[i] == NULL )
        {
            runs.push_back( NULL );
            simulation_values.push_back( NULL );
        }
        else
        {
            runs.push_back( a.runs[i]->clone() );
            simulation_values.push_back( runs[i]->getModel().clone() );
        }
        
    }
    
}


ValidationAnalysis::~ValidationAnalysis(void)
{
    // free the runs
    for (size_t i = 0; i < num_runs; ++i)
    {
        MonteCarloAnalysis *sampler = runs[i];
        delete sampler;
        
        Model *m = simulation_values[i];
        delete m;
    }
    
}


/**
 * Overloaded assignment operator.
 * We need to keep track of the MonteCarloSamplers
 */
ValidationAnalysis& ValidationAnalysis::operator=(const ValidationAnalysis &a)
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
        num_processes   = a.num_processes;
        num_runs        =
        pid             = a.pid;
        processActive   = a.processActive;
        
        
        // create replicate Monte Carlo samplers
        for (size_t i=0; i < num_runs; ++i)
        {
            // only copy the runs which this process needs to execute
            if ( a.runs[i] == NULL )
            {
                runs.push_back( NULL );
            }
            else
            {
                runs.push_back( a.runs[i]->clone() );
            }
            
        }
        
    }
    
    return *this;
}


/** Run burnin and autotune */
void ValidationAnalysis::burnin(size_t generations, size_t tuningInterval)
{
    
    if ( processActive == true )
    {
        // Let user know what we are doing
        std::stringstream ss;
        ss << "\n";
        ss << "Running burn-in phase of " << num_runs <<  " Monte Carlo samplers each for " << generations << " iterations.\n";
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
        
        // run the i-th analyses
        runs[i]->burnin(generations, tuningInterval, false);
        
        if ( processActive == true )
        {
            size_t progress = 68 * (double) (i+1.0) / (double) (run_block_end - run_block_start);
            if ( progress > numStars )
            {
                for ( ;  numStars < progress; ++numStars )
                    std::cout << "*";
                std::cout.flush();
            }
            
        }
        
    }
    
    if ( processActive == true )
    {
        std::cout << std::endl;
    }
    
}



ValidationAnalysis* ValidationAnalysis::clone( void ) const
{
    
    return new ValidationAnalysis( *this );
}


void ValidationAnalysis::runAll(size_t gen)
{
    
    // print some information to the screen but only if we are the active process
    if ( processActive )
    {
        std::cout << std::endl;
        std::cout << "Running validation analysis ..." << std::endl;
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



void ValidationAnalysis::runSim(size_t idx, size_t gen)
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
        
        std::cout << std::endl;
    }
    
    // get the current sample
    MonteCarloAnalysis *analysis = runs[idx];
    
    // run the analysis
    RbVector<StoppingRule> rules;
    
    size_t currentGen = analysis->getCurrentGeneration();
    rules.push_back( MaxIterationStoppingRule(gen + currentGen) );
    
    analysis->run(gen, rules, false);
    
}


void ValidationAnalysis::readModelTraces( void )
{
    
    
//    std::stringstream ss;
//    ss << "Validation_Sim_" << i;
//    
//    StochasticVariableMonitor mntr = StochasticVariableMonitor(10, "output/posterior_samples.var", "\t");
//
//    
//    TraceReader reader;
//    std::vector<ModelTrace> traces = reader.readStochasticVariableTrace( fn, "\t");
    
}



void ValidationAnalysis::summarizeAll( void )
{
    
    // print some information to the screen but only if we are the active process
    if ( processActive )
    {
        std::cout << std::endl;
        std::cout << "Summarizing analysis ..." << std::endl;
    }
    
    // reset the counter
    coverage_count = std::map<std::string, int>();
    
    // compute which block of the runs this process needs to compute
    size_t run_block_start = size_t(floor( (double(pid)   / num_processes ) * num_runs) );
    size_t run_block_end   = size_t(floor( (double(pid+1) / num_processes ) * num_runs) );
    //    size_t stone_block_size  = stone_block_end - stone_block_start;
    
    // Run the chain
    for (size_t i = run_block_start; i < run_block_end; ++i)
    {
        
        // summarize the i-th simulation
        summarizeSim(i);
        
    }
    
    std::cerr << std::endl;
    std::cerr << "Coverage frequencies of parameters in validation analysis:" << std::endl;
    std::cerr << "==========================================================" << std::endl;
    for (std::map<std::string, int>::iterator it = coverage_count.begin(); it != coverage_count.end(); ++it)
    {
        std::string n = it->first;
        StringUtilities::formatFixedWidth(n, 20, true);
        std::cerr << n << "\t\t" << double(it->second) / num_runs << std::endl;
    }
    std::cerr << std::endl;
    
    
}



void ValidationAnalysis::summarizeSim(size_t idx)
{
    
//    readModelTraces();
    
    std::stringstream ss;
    ss << "output/Validation_Sim_" << idx << "/" << "posterior_samples.var";
    std::string fn = ss.str();
    
    
//    std::cout << "Summarizing results for:\t" << fn << std::endl;
    
    TraceReader reader;
    std::vector<ModelTrace> traces = reader.readStochasticVariableTrace( fn, "\t");
    
    size_t n_samples = traces[0].size();
    size_t n_traces = traces.size();
    
    std::vector<DagNode*> nodes = simulation_values[idx]->getDagNodes();
    
    std::map<std::string,Trace*> trace_map;
    // now for the numerical parameters
    for ( size_t j=0; j<n_traces; ++j )
    {
        std::string parameter_name = traces[j].getParameterName();
        
        // iterate over all DAG nodes (variables)
        for ( std::vector<DagNode*>::iterator it = nodes.begin(); it!=nodes.end(); ++it )
        {
            DagNode *the_node = *it;
            
            if ( the_node->getName() == parameter_name )
            {
                // create a trace
                Trace *t = the_node->createTraceObject();
                trace_map[parameter_name] = t;
            }
            
        }
        
    }
    
    // add each sample
    for (size_t i=0; i<n_samples; ++i)
    {
        // to each of the traces
        for ( size_t j=0; j<n_traces; ++j )
        {
            
            const std::string &parameter_name = traces[j].getParameterName();
            if ( trace_map.find( parameter_name ) != trace_map.end() )
            {
                std::string parameter_name = traces[j].getParameterName();
                trace_map[parameter_name]->addValueFromString( traces[j].objectAt( i ) );
            }
            
        }
        
    }
    
    std::stringstream ss_out;
    ss_out << "validation_summary_sim_" << idx  << ".txt";
    std::string out_file = ss_out.str();

    std::ofstream outStream;
    // open the stream to the file
    outStream.open(out_file.c_str(), std::fstream::out);
    
    // iterate over all DAG nodes (variables)
    for ( std::vector<DagNode*>::iterator it = nodes.begin(); it!=nodes.end(); ++it )
    {
        DagNode *the_node = *it;
        
        if ( the_node->isStochastic() == true )
        {
            const std::string &parameter_name = the_node->getName();
            
            if ( trace_map.find( parameter_name ) != trace_map.end() )
            {
                // create a trace
                bool cov = trace_map[parameter_name]->isCoveredInInterval(the_node->getValueAsString(), 0.95);
                outStream << parameter_name << ":\t\t" << (cov ? "TRUE" : "FALSE") << std::endl;
                
                if ( coverage_count.find(parameter_name) == coverage_count.end() )
                {
                    coverage_count.insert( std::pair<std::string,int>(parameter_name,0) );
                }
                if ( cov == true )
                {
                    coverage_count[ parameter_name ]++;
                }
                
            }
        
        }
        
    }

    outStream.close();
                
    
}

