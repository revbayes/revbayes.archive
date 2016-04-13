#include "DagNode.h"
#include "FileMonitor.h"
#include "Mcmc.h"
#include "MoveSchedule.h"
#include "RandomMoveSchedule.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbMathLogic.h"
#include "RbOptions.h"
#include "RlUserInterface.h"
#include "SequenctialMoveSchedule.h"
#include "SingleRandomMoveSchedule.h"
#include "RandomMoveSchedule.h"
#include "ExtendedNewickTreeMonitor.h"

#include <unistd.h>

#include <cmath>
#include <iomanip>
#include <sstream>
#include <typeinfo>

#ifdef RB_MPI
#include <mpi.h>
#endif


using namespace RevBayesCore;


/**
 * Constructor. We create an independent copy of the model and thus of all DAG nodes.
 * Someone might have wanted to run another MCMC with different settings on the same model.
 * Thus we also create our own copies of the monitors and moves.
 *
 * \param[in]    m    The model containing all DAG nodes.
 * \param[in]    mvs  The vector of moves.
 * \param[in]    mons The vector of monitors.
 */
Mcmc::Mcmc(const Model& m, const RbVector<Move> &mvs, const RbVector<Monitor> &mons) : MonteCarloSampler(),
    chain_active( true ),
    chain_likelihood_heat( 1.0 ),
    chain_posterior_heat( 1.0 ),
    chain_idx( 0 ),
    model( m.clone() ),
    monitors( mons ),
    moves( mvs ),
    schedule(NULL),
    schedule_type("random")
{
    // create an independent copy of the model, monitors and moves
    replaceDag(mvs,mons);
    
    initializeSampler();
    initializeMonitors();

}


/**
  * Copy constructor. For more details see the constructor.
  *
  * \param[in]    m    The MonteCarloSampler object to copy.
  */
Mcmc::Mcmc(const Mcmc &m) : MonteCarloSampler(m),
    chain_active( m.chain_active ),
    chain_likelihood_heat( m.chain_likelihood_heat ),
    chain_posterior_heat( m.chain_posterior_heat ),
    chain_idx( m.chain_idx ),
    model( m.model->clone() ),
    monitors( m.monitors ),
    moves( m.moves ),
    schedule( NULL ),
    schedule_type( m.schedule_type )
{
    
    // temporary references
    const RbVector<Monitor>& mons = m.monitors;
    const RbVector<Move>& mvs = m.moves;
    
    
    // create an independent copy of the model, monitors and moves
    replaceDag(mvs,mons);
    
    initializeSampler();
    initializeMonitors();
}


/**
 * Destructor. Frees the DAG nodes (the model), moves, monitor and the move schedule.
 */
Mcmc::~Mcmc(void)
{
    
    
    // delete the move schedule
    delete schedule;
    
    // delete the model
    delete model;
    
}


/**
 * Copy constructor. For more details see the constructor.
 *
 * \param[in]    m    The MonteCarloSampler object to copy.
 */
Mcmc& Mcmc::operator=(const Mcmc &m)
{
    
    if ( this != &m )
    {
        delete model;
        model = m.model->clone();
        
        // temporary references
        const RbVector<Monitor>& mons = m.monitors;
        const RbVector<Move>& mvs = m.moves;
    
    
        // create an independent copy of the model, monitors and moves
        replaceDag(mvs,mons);
    
        initializeSampler();
        initializeMonitors();
    }
    
    return *this;
}


/**
 * Add an extension to the name of the monitor.
 * We tell this to all our monitors.
 */
void Mcmc::addFileMonitorExtension(const std::string &s, bool dir)
{
    
    // tell each monitor
    for (RbIterator<Monitor> it=monitors.begin(); it!=monitors.end(); ++it)
    {
        it->addFileExtension( s, dir );
    }
    
}


void Mcmc::addMonitor(const Monitor &m)
{
    
    monitors.push_back( m );
    
}


/**
 * Disable all screen monitors. This means we simply delete it.
 */
void Mcmc::disableScreenMonitor( bool all, size_t rep )
{
    
    // tell each monitor
    for (size_t i=0; i < monitors.size(); ++i)
    {
     
        if ( all == true || rep > 0 || process_active == false )
        {
            
            bool is = monitors[i].isScreenMonitor();
            if ( is == true )
            {
                monitors[i].disable();
            }
            
        }
        
    }
    
}


Mcmc* Mcmc::clone( void ) const
{
    
    return new Mcmc( *this );
}


/**
 * Finish the monitors which will close the output streams.
 */
void Mcmc::finishMonitors( size_t n_reps )
{
    
    // iterate over all monitors
    for (size_t i=0; i<monitors.size(); i++)
    {
        
        // if this chain is active, then close the stream
        if ( chain_active == true && process_active == true )
        {
            monitors[i].closeStream();
            
            // combine results if we used more than one replicate
            if ( n_reps > 1 )
            {
                monitors[i].combineReplicates( n_reps );
            }
            
        }
        
    }
    
}


/**
 * Get the heat of the likelihood of this chain.
 */
double Mcmc::getChainLikelihoodHeat(void) const
{
    return chain_likelihood_heat;
}


/**
 * Get the heat of the posterior of this chain.
 */
double Mcmc::getChainPosteriorHeat(void) const
{
    return chain_posterior_heat;
}


/**
 * Get the index of this chain.
 */
size_t Mcmc::getChainIndex(void) const
{
    return chain_idx;
}


/**
 * Is the current chain active?
 */
bool Mcmc::isChainActive(void)
{
    return chain_active;
}


/**
 * Get the model instance.
 */
const Model& Mcmc::getModel( void ) const
{
    
    return *model;
}


/**
 * Get the joint posterior probability of the current state for this model.
 * Note that the joint posterior is the true, unscaled and unheated value.
 */
double Mcmc::getModelLnProbability(void)
{
    const std::vector<DagNode*> &n = model->getDagNodes();
    double pp = 0.0;
    for (std::vector<DagNode*>::const_iterator it = n.begin(); it != n.end(); ++it)
    {
        pp += (*it)->getLnProbability();
    }
    return pp;
}


/**
 * Get the vector of monitors for this sampler.
 */
RbVector<Monitor>& Mcmc::getMonitors(void)
{
    return monitors;
}


/**
 * Get the vector of moves for this sampler.
 */
RbVector<Move>& Mcmc::getMoves(void)
{
    return moves;
}


/**
 * Get a const-reference move-schedule for this sampler.
 */
const MoveSchedule& Mcmc::getSchedule(void) const
{
    return *schedule;
}


/**
 * Get a non-const reference to the move-schedule of this sampler.
 */
MoveSchedule& Mcmc::getSchedule(void)
{
    return *schedule;
}

/**
 * Get the schedule type of this sampler.
 */
const std::string& Mcmc::getScheduleType( void ) const
{
    return schedule_type;
}


std::string Mcmc::getStrategyDescription( void ) const
{
    
    std::string description = "";
    std::stringstream stream;
    if ( schedule_type == "single" )
    {
        stream << "The simulator uses " << moves.size() << " different moves, with a single move picked randomly per iteration" << std::endl;
    }
    else if ( schedule_type == "random" )
    {
        stream << "The simulator uses " << moves.size() << " different moves in a random move schedule with " << schedule->getNumberMovesPerIteration() << " moves per iteration" << std::endl;
    }
    else if ( schedule_type == "sequential" )
    {
        stream << "The simulator uses " << moves.size() << " different moves in a sequential move schedule with " << schedule->getNumberMovesPerIteration() << " moves per iteration" << std::endl;
    }
    description = stream.str();

    return description;
}


void Mcmc::initializeSampler( bool prior_only )
{
    
    std::vector<DagNode *> &dag_nodes = model->getDagNodes();
    std::vector<DagNode *> ordered_stoch_nodes = model->getOrderedStochasticNodes(  );
    
    // Get rid of previous move schedule, if any
    if ( schedule != NULL )
    {
        delete schedule;
    }
    schedule = NULL;
    
    // Get initial ln_probability of model
    
    // first we touch all nodes so that the likelihood is dirty
    for (std::vector<DagNode *>::iterator i=dag_nodes.begin(); i!=dag_nodes.end(); ++i)
    {
        
        DagNode *the_node = *i;
        the_node->setMcmcMode( true );
        the_node->setPriorOnly( prior_only );
        the_node->touch();
        
    }
    
    
    if ( chain_active == false )
    {

        for (std::vector<DagNode *>::iterator i=ordered_stoch_nodes.begin(); i!=ordered_stoch_nodes.end(); i++)
        {
            
            if ( !(*i)->isClamped() && (*i)->isStochastic() )
            {
                (*i)->redraw();
                (*i)->reInitialized();
    
            }
            else if ( (*i)->isClamped() )
            {
                // make sure that the clamped node also recompute their probabilities
                (*i)->touch();
            }
    
        }
        
    }
    
    int num_tries    = 0;
    int max_num_tries = 100;
    double ln_probability = 0.0;
    for ( ; num_tries < max_num_tries; ++num_tries )
    {
        // a flag if we failed to find a valid starting value
        bool failed = false;
        
        ln_probability = 0.0;
        for (std::vector<DagNode *>::iterator i=dag_nodes.begin(); i!=dag_nodes.end(); ++i)
        {
            DagNode* the_node = (*i);
            the_node->touch();
            
            double ln_prob = the_node->getLnProbability();
            
            if ( RbMath::isAComputableNumber(ln_prob) == false )
            {
                std::stringstream ss;
                ss << "Could not compute lnProb for node " << the_node->getName() << "." << std::endl;
                std::ostringstream o1;
                the_node->printValue( o1 );
                ss << StringUtilities::oneLiner( o1.str(), 54 ) << std::endl;

                ss << std::endl;
                RBOUT( ss.str() );
                
                // set the flag
                failed = true;
                
                break;
            }
            ln_probability += ln_prob;
            
        }
        
        // now we keep all nodes so that the likelihood is stored
        for (std::vector<DagNode *>::iterator i=dag_nodes.begin(); i!=dag_nodes.end(); ++i)
        {
            (*i)->keep();
        }
        
        if ( failed == true )
        {
            std::cout << "Drawing new initial states ... " << std::endl;
            for (std::vector<DagNode *>::iterator i=ordered_stoch_nodes.begin(); i!=ordered_stoch_nodes.end(); ++i)
            {
                DagNode *the_node = *i;
                if ( the_node->isClamped() == false && (*i)->isStochastic() == true )
                {
                    the_node->redraw();
                    the_node->reInitialized();
                    
                }
                else if ( the_node->isClamped() == true )
                {
                    // make sure that the clamped node also recompute their probabilities
                    the_node->reInitialized();
                    the_node->touch();
                }
                
            }
        }
        else
        {
            break;
        }
        
    }
    
    if ( num_tries == max_num_tries )
    {
        std::stringstream msg;
        msg << "Unable to find a starting state with computable probability";
        if ( num_tries > 1 )
        {
            msg << " after " << num_tries << " tries";
        }
        throw RbException( msg.str() );
        
    }
    
    // Create the move scheduler
    if ( schedule_type == "sequential" )
    {
        schedule = new SequentialMoveSchedule( &moves );
    }
    else if ( schedule_type == "single" )
    {
        schedule = new SingleRandomMoveSchedule( &moves );
    }
    else
    {
        schedule = new RandomMoveSchedule( &moves );
    }
    
    generation = 0;
}



void Mcmc::initializeMonitors(void)
{
    
    for (size_t i=0; i<monitors.size(); ++i)
    {
        monitors[i].setModel( model );
    }
    
}


void Mcmc::monitor(unsigned long g)
{
    
    if ( chain_active == true && process_active == true )
    {
        // Monitor
        for (size_t i = 0; i < monitors.size(); ++i)
        {
            
            monitors[i].monitor( g );
        
        }
        
    }
    
}


void Mcmc::nextCycle(bool advanceCycle)
{
    
    size_t proposals = size_t( round( schedule->getNumberMovesPerIteration() ) );
    for (size_t i=0; i<proposals; ++i)
    {
        
        // Get the move
        Move& the_move = schedule->nextMove( generation );
                
        // Perform the move
        the_move.performMcmcStep( chain_likelihood_heat, chain_posterior_heat );
        
    }
    
    
    // advance gen cycle if needed (i.e. run()==true, burnin()==false)
    if ( advanceCycle == true )
    {
        ++generation;
    }

}



void Mcmc::printOperatorSummary(void) const
{
    
    
    // printing the moves summary
    std::cout << std::endl;
    std::cout << "                  Name                  | Param              |  Weight  |  Tried   | Accepted | Acc. Ratio| Parameters" << std::endl;
    std::cout << "===============================================================================================================================" << std::endl;
    for (RbConstIterator<Move> it = moves.begin(); it != moves.end(); ++it)
    {
        it->printSummary(std::cout);
    }
    
    std::cout << std::endl;
}


void Mcmc::replaceDag(const RbVector<Move> &mvs, const RbVector<Monitor> &mons)
{
    
    moves.clear();
    monitors.clear();
    
    // we need to replace the DAG nodes of the monitors and moves
    const std::vector<DagNode*>& model_nodes = model->getDagNodes();
    for (RbConstIterator<Move> it = mvs.begin(); it != mvs.end(); ++it)
    {
        
        Move *the_move = it->clone();
        std::vector<DagNode*> nodes = the_move->getDagNodes();
        for (std::vector<DagNode*>::const_iterator j = nodes.begin(); j != nodes.end(); ++j)
        {
            
            RevBayesCore::DagNode *the_node = *j;
            
            // error checking
            if ( the_node->getName() == "" )
            {
                throw RbException( "Unable to connect move '" + the_move->getMoveName() + "' to DAG copy because variable name was lost");
            }
            
            DagNode* the_new_node = NULL;
            for (std::vector<DagNode*>::const_iterator k = model_nodes.begin(); k != model_nodes.end(); ++k)
            {
                if ( (*k)->getName() == the_node->getName() )
                {
                    the_new_node = *k;
                    break;
                }
            }
            // error checking
            if ( the_new_node == NULL )
            {
                throw RbException("Cannot find node with name '" + the_node->getName() + "' in the model but received a move working on it.");
            }
            
            // now swap the node
            the_move->swapNode( *j, the_new_node );
        }
        moves.push_back( *the_move );
        delete the_move;
    }
    
    for (RbConstIterator<Monitor> it = mons.begin(); it != mons.end(); ++it)
    {
        Monitor *the_monitor = it->clone();
        std::vector<DagNode*> nodes = the_monitor->getDagNodes();
        for (std::vector<DagNode*>::const_iterator j = nodes.begin(); j != nodes.end(); ++j)
        {
            
            RevBayesCore::DagNode *the_node = (*j);
            
            // error checking
            if ( the_node->getName() == "" )
            {
                throw RbException( "Unable to connect monitor to DAG copy because variable name was lost");
            }
            
            DagNode* theNewNode = NULL;
            for (std::vector<DagNode*>::const_iterator k = model_nodes.begin(); k != model_nodes.end(); ++k)
            {
                if ( (*k)->getName() == the_node->getName() )
                {
                    theNewNode = *k;
                    break;
                }
            }
            // error checking
            if ( theNewNode == NULL )
            {
                throw RbException("Cannot find node with name '" + the_node->getName() + "' in the model but received a monitor working on it.");
            }
            
            // now swap the node
            the_monitor->swapNode( *j, theNewNode );
        }
        monitors.push_back( *the_monitor );
        delete the_monitor;
        
    }
    
}


void Mcmc::removeMonitors( void )
{
    
    // just clear the vector
    monitors.clear();
    
}


/**
 * Reset the sampler.
 * We reset the counters of all moves.
 */
void Mcmc::reset( void )
{
    
    double movesPerIteration = 0.0;
    for (RbIterator<Move> it = moves.begin(); it != moves.end(); ++it)
    {

        it->resetCounters();
        movesPerIteration += it->getUpdateWeight();
        
    }

}


/**
 * Set the active PID of this specific MCMC simulation.
 */
void Mcmc::setActivePIDSpecialized(size_t n)
{
    
    // delegate the call to the model
    model->setActivePID(n);
    
    
    // tell each monitor
    for (size_t i=0; i < monitors.size(); ++i)
    {
        
        if ( process_active == true )
        {
            monitors[i].enable();
        }
        else
        {
            monitors[i].disable();
        }
        
    }
    
}


/**
 * Set if the current chain is the active chain.
 * Only active chains print to the monitors.
 */
void Mcmc::setChainActive(bool tf)
{
    chain_active = tf;
}


/**
 * Set the heat of the likelihood of the current chain.
 * This heat is used in posterior posterior MCMC algorithms to
 * heat the likelihood
 * The heat is passed to the moves for the accept-reject mechanism.
 */
void Mcmc::setChainLikelihoodHeat(double h)
{
    chain_likelihood_heat = h;
}


/**
 * Set the heat of the likelihood of the current chain.
 * This heat is used in posterior posterior MCMC algorithms to
 * heat the likelihood
 * The heat is passed to the moves for the accept-reject mechanism.
 */
void Mcmc::setLikelihoodHeat(double h)
{
    chain_likelihood_heat = h;
}


/**
 * Set the number of processes available to this specific MCMC simulation.
 * If there is more than one process available, then we can use these
 * to compute the likelihood in parallel. Yeah!
 */
void Mcmc::setNumberOfProcessesSpecialized(size_t n)
{

    // delegate the call to the model
    model->setNumberOfProcesses(n);
    
    
    // tell each monitor
    for (size_t i=0; i < monitors.size(); ++i)
    {
        
        if ( process_active == true )
        {
            monitors[i].enable();
        }
        else
        {
            monitors[i].disable();
        }
        
    }
    
}


/**
 * Set the heat of the posterior of the current chain.
 * The heat of the posterior is used in the MC^3 algorithm.
 * The heat is passed to the moves for the accept-reject mechanism.
 */
void Mcmc::setChainPosteriorHeat(double h)
{
    chain_posterior_heat = h;
}


/**
 * Get the index of the current chain.
 */
void Mcmc::setChainIndex(size_t x)
{
    chain_idx = x;
}


/**
 * Set the model by delegating the model to the chains.
 */
void Mcmc::setModel( Model *m )
{
    
    model = m;
    
}


void Mcmc::setScheduleType(const std::string &s)
{
    
    schedule_type = s;
}


/**
 * Start the monitors which will open the output streams.
 */
void Mcmc::startMonitors( size_t num_cycles )
{
    
    // Open the output file and print headers
    for (size_t i=0; i<monitors.size(); ++i)
    {
        
        // open filestream for each monitor
        monitors[i].openStream();
        
        // reset the monitor
        monitors[i].reset( num_cycles );
        
    }
    
}

/**
 * Write the header for each of the monitors.
 */
void Mcmc::writeMonitorHeaders( void )
{
    
    // Open the output file and print headers
    for (size_t i=0; i<monitors.size(); ++i)
    {
        
        // if this chain is active, print the header
        if ( chain_active == true && process_active == true )
        {
            monitors[i].printHeader();
        }
        
    }
    
}


/**
 * Tune the sampler.
 * Here we just tune all the moves.
 */
void Mcmc::tune( void )
{
    
    // iterate over the moves
    for (RbIterator<Move> it=moves.begin(); it!=moves.end(); ++it)
    {
        // tune the move
        it->autoTune();
    }
    
}

