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

#include <cmath>
#include <iomanip>
#include <sstream>
#include <typeinfo>

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
    chainActive( true ),
    chainLikelihoodHeat( 1.0 ),
    chainPosteriorHeat( 1.0 ),
    chainIdx( 0 ),
    model( m ),
    monitors( mons ),
    moves( mvs ),
    schedule(NULL),
    scheduleType("random")
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
    chainActive( m.chainActive ),
    chainLikelihoodHeat( m.chainLikelihoodHeat ),
    chainPosteriorHeat( m.chainPosteriorHeat ),
    chainIdx( m.chainIdx ),
    model( m.model ),
    monitors( m.monitors ),
    moves( m.moves ),
    schedule( NULL ),
    scheduleType( m.scheduleType )
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



Mcmc* Mcmc::clone( void ) const
{
    
    return new Mcmc( *this );
}


/**
 * Get the heat of the likelihood of this chain.
 */
double Mcmc::getChainLikelihoodHeat(void) const
{
    return chainLikelihoodHeat;
}


/**
 * Get the heat of the posterior of this chain.
 */
double Mcmc::getChainPosteriorHeat(void) const
{
    return chainPosteriorHeat;
}


/**
 * Get the index of this chain.
 */
size_t Mcmc::getChainIndex(void) const
{
    return chainIdx;
}


/**
 * Is the current chain active?
 */
bool Mcmc::isChainActive(void)
{
    return chainActive;
}


/**
 * Get the joint posterior probability of the current state for this model.
 * Note that the joint posterior is the true, unscaled and unheated value.
 */
double Mcmc::getModelLnProbability(void)
{
    const std::vector<DagNode*> &n = model.getDagNodes();
    double pp = 0.0;
    for (std::vector<DagNode*>::const_iterator it = n.begin(); it != n.end(); ++it)
    {
        //std::cout << (*it)->getName() << "  " << (*it)->getLnProbability() << "\n";
        //(*it)->touch();
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
    return scheduleType;
}


std::string Mcmc::getStrategyDescription( void ) const
{
    
    std::string description = "";
    std::stringstream stream;
    if ( scheduleType == "single" )
    {
        stream << "The simulator uses " << moves.size() << " different moves, with a single move picked randomly per iteration" << std::endl;
    }
    else if ( scheduleType == "random" )
    {
        stream << "The simulator uses " << moves.size() << " different moves in a random move schedule with " << schedule->getNumberMovesPerIteration() << " moves per iteration" << std::endl;
    }
    else if ( scheduleType == "sequential" )
    {
        stream << "The simulator uses " << moves.size() << " different moves in a sequential move schedule with " << schedule->getNumberMovesPerIteration() << " moves per iteration" << std::endl;
    }
    description = stream.str();

    return description;
}

/**
 * Creates a vector of stochastic nodes, starting from the source nodes to the sink nodes
 */
void Mcmc::getOrderedStochasticNodes(const DagNode* dagNode,  std::vector<DagNode*>& orderedStochasticNodes, std::set<const DagNode*>& visitedNodes) {
    
    if (visitedNodes.find(dagNode) != visitedNodes.end()) { //The node has been visited before
        //we do nothing
        return;
    }
    
    // add myself here for safety reasons
    visitedNodes.insert( dagNode );
    
    if ( dagNode->isConstant() ) { //if the node is constant: no parents to visit
        std::set<DagNode*> children = dagNode->getChildren() ;
        visitedNodes.insert(dagNode);
        std::set<DagNode*>::iterator it;
        for ( it = children.begin() ; it != children.end(); it++ )
            getOrderedStochasticNodes(*it, orderedStochasticNodes, visitedNodes);
    }
    else //if the node is stochastic or deterministic
    {
        // First I have to visit my parents
        std::set<const DagNode *> parents = dagNode->getParents() ;
        std::set<const DagNode *>::const_iterator it;
        for ( it=parents.begin() ; it != parents.end(); it++ )
        {
            getOrderedStochasticNodes(*it, orderedStochasticNodes, visitedNodes);
        }
        
        // Then I can add myself to the nodes visited, and to the ordered vector of stochastic nodes
        //        visitedNodes.insert(dagNode);
        if ( dagNode->isStochastic() ) //if the node is stochastic
        {
            orderedStochasticNodes.push_back( const_cast<DagNode*>( dagNode ) );
        }
        
        // Finally I will visit my children
        std::set<DagNode*> children = dagNode->getChildren() ;
        std::set<DagNode*>::iterator it2;
        for ( it2 = children.begin() ; it2 != children.end(); it2++ )
        {
            getOrderedStochasticNodes(*it2, orderedStochasticNodes, visitedNodes);
        }
    }
    
}

void Mcmc::initializeSampler( bool priorOnly )
{
    
    std::vector<DagNode *>& dagNodes = model.getDagNodes();
    std::vector<DagNode *> orderedStochNodes;
    std::set< const DagNode *> visited;
    getOrderedStochasticNodes(dagNodes[0],orderedStochNodes, visited );
    
    // Get rid of previous move schedule, if any
    if ( schedule )
    {
        delete schedule;
    }
    schedule = NULL;
    
    // Get initial lnProbability of model
    
    // first we touch all nodes so that the likelihood is dirty
    for (std::vector<DagNode *>::iterator i=dagNodes.begin(); i!=dagNodes.end(); i++)
    {
        
        DagNode *the_node = *i;
        the_node->setMcmcMode( true );
        the_node->setPriorOnly( priorOnly );
        the_node->touch();
        
    }
    
    
    // redraw parameters for inactive chains in pMC^3 team
    if (chainActive == false)
    {
        for (std::vector<DagNode *>::iterator i=orderedStochNodes.begin(); i!=orderedStochNodes.end(); i++)
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
    
    int numTries    = 0;
    int maxNumTries = 100;
    double lnProbability = 0.0;
    for ( ; numTries < maxNumTries; numTries ++ )
    {
        // a flag if we failed to find a valid starting value
        bool failed = false;
        
        lnProbability = 0.0;
        for (std::vector<DagNode *>::iterator i=dagNodes.begin(); i!=dagNodes.end(); i++)
        {
            DagNode* node = (*i);
            (*i)->touch();
            
            double lnProb = node->getLnProbability();
            
            if ( !RbMath::isAComputableNumber(lnProb) )
            {
                std::stringstream ss;
                ss << "Could not compute lnProb for node " << node->getName() << "." << std::endl;
                node->printValue( ss );
                ss << std::endl;
                RBOUT( ss.str() );
                
                // set the flag
                failed = true;
                
                break;
            }
            lnProbability += lnProb;
            
        }
        
        // now we keep all nodes so that the likelihood is stored
        for (std::vector<DagNode *>::iterator i=dagNodes.begin(); i!=dagNodes.end(); i++)
        {
            (*i)->keep();
        }
        
        if ( failed == true )
        {
            std::cerr << "Drawing new initial states ... " << std::endl;
            for (std::vector<DagNode *>::iterator i=orderedStochNodes.begin(); i!=orderedStochNodes.end(); i++)
            {
                
                if ( !(*i)->isClamped() && (*i)->isStochastic() )
                {
                    (*i)->redraw();
                    (*i)->reInitialized();
                    
                }
                else if ( (*i)->isClamped() )
                {
                    // make sure that the clamped node also recompute their probabilities
                    (*i)->reInitialized();
                    (*i)->touch();
                }
                
            }
        }
        else
        {
            break;
        }
        
    }
    
    if ( numTries == maxNumTries )
    {
        std::stringstream msg;
        msg << "Unable to find a starting state with computable probability";
        if ( numTries > 1 )
        {
            msg << " after " << numTries << " tries";
        }
        throw RbException( msg.str() );
        
    }
    
    /* Create the move scheduler */
    if ( scheduleType == "sequential" )
    {
        schedule = new SequentialMoveSchedule( &moves );
    }
    else if ( scheduleType == "single" )
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
    for (size_t i=0; i<monitors.size(); i++)
    {
        monitors[i].setModel( &model );
    }
}


void Mcmc::monitor(unsigned long g)
{
    
    // Monitor
    for (size_t i = 0; i < monitors.size(); i++)
    {
        monitors[i].monitor( g );
    }
    
}


void Mcmc::nextCycle(bool advanceCycle)
{
    
#ifdef DEBUG_MCMC
    std::vector<DagNode *>& dagNodes = model.getDagNodes();
#endif
    
    size_t proposals = size_t( round( schedule->getNumberMovesPerIteration() ) );
    for (size_t i=0; i<proposals; i++)
    {
        
#ifdef DEBUG_MCMC
        double oldLnProb = 0.0;
        for (std::vector<DagNode*>::iterator it = dagNodes.begin(); it != dagNodes.end(); ++it)
        {
            oldLnProb += (*it)->getLnProbability();
        }
#endif
        
        // Get the move
        Move& theMove = schedule->nextMove( generation );
        
#ifdef DEBUG_MCMC
#ifdef DEBUG_MCMC_DETAILS
        std::cerr << "\nPerforming move " << theMove.getMoveName() << " on " << ( *theMove.getDagNodes().begin() )->getName() << std::endl;
        
        std::cerr << "\nValues before move " << std::endl << std::endl;
        
        for (std::vector<DagNode*>::iterator it = dagNodes.begin(); it != dagNodes.end(); ++it)
        {
            if ( !(*it)->isConstant() )
            {
                std::cerr << (*it)->getName() << " <" << (*it) << "> :" << std::endl;
                (*it)->printValue( std::cerr );
                std::cerr << std::endl << std::endl;
            }
        }
#endif
#endif
        
        // Perform the move
        theMove.perform( chainLikelihoodHeat, chainPosteriorHeat);
        
#ifdef DEBUG_MCMC
#ifdef DEBUG_MCMC_DETAILS
        std::cerr << "\nValues after move " << std::endl << std::endl;
        
        for (std::vector<DagNode*>::iterator it = dagNodes.begin(); it != dagNodes.end(); ++it)
        {
            if ( !(*it)->isConstant() )
            {
                std::cerr << (*it)->getName() << " <" << (*it) << "> :" << std::endl;
                (*it)->printValue( std::cerr );
                std::cerr << std::endl << std::endl;
            }
        }
        
//        getchar();
        
        std::cerr << std::endl << "With shortcuts" << std::endl;
#endif
        double lnProb = 0.0;
        double lnLikelihoodProb = 0.0;
        double lnPriorProb = 0.0;
        for (std::vector<DagNode*>::iterator it = dagNodes.begin(); it != dagNodes.end(); ++it)
        {
            double pr = (*it)->getLnProbability();
            lnProb += pr;
            
#ifdef DEBUG_MCMC_DETAILS
            if ( (*it)->isStochastic() )
                std::cerr << (*it)->getName() << "<" << (*it) << "> returned lnprob = " << pr << std::endl;
#endif
            if ( (*it)->isClamped() )
            {
                lnLikelihoodProb += (*it)->getLnProbability();
            }
            else
            {
                lnPriorProb += (*it)->getLnProbability();
            }
        }
        
#ifdef DEBUG_MCMC_DETAILS
        std::cerr << std::endl << "After touching everything" << std::endl;
#endif
        for (std::vector<DagNode*>::iterator it = dagNodes.begin(); it != dagNodes.end(); ++it)
        {
            (*it)->touch();
        }
        double touchedLnProb = 0.0;
        double touchedLnLikelihoodProb = 0.0;
        double touchedLnPriorProb = 0.0;
        for (std::vector<DagNode*>::iterator it = dagNodes.begin(); it != dagNodes.end(); ++it)
        {
            double pr = (*it)->getLnProbability();
            touchedLnProb += pr;
            
#ifdef DEBUG_MCMC_DETAILS
            if ( (*it)->isStochastic() )
                std::cerr << (*it)->getName() << "<" << (*it) << "> returned lnprob = " << pr << std::endl;
#endif
            if ( (*it)->isClamped() )
            {
                touchedLnLikelihoodProb += (*it)->getLnProbability();
            }
            else
            {
                touchedLnPriorProb += (*it)->getLnProbability();
            }
        }
        
        for (std::vector<DagNode*>::iterator it = dagNodes.begin(); it != dagNodes.end(); ++it)
        {
            (*it)->keep();
            (*it)->touch();
        }
        double touchedAgainLnProb = 0.0;
        double touchedAgainLnLikelihoodProb = 0.0;
        double touchedAgainLnPriorProb = 0.0;
        for (std::vector<DagNode*>::iterator it = dagNodes.begin(); it != dagNodes.end(); ++it)
        {
            touchedAgainLnProb += (*it)->getLnProbability();
            if ( (*it)->isClamped() )
            {
                touchedAgainLnLikelihoodProb += (*it)->getLnProbability();
            }
            else
            {
                touchedAgainLnPriorProb += (*it)->getLnProbability();
            }
        }
        // Keep everything to be ready for next cycle
        for (std::vector<DagNode*>::iterator it = dagNodes.begin(); it != dagNodes.end(); ++it)
        {
            (*it)->keep();
        }
        
        if ( fabs(lnProb - touchedLnProb) > 1E-8 )
        {
            std::cout << "Probability        shortcut\t\t-\t\ttouched all\t\t-\t\ttouched all again" << std::endl;
            std::cout << "lnPriorProb      = " << lnPriorProb << "\t\t-\t\t" << touchedLnPriorProb << "\t\t-\t\t" << touchedAgainLnPriorProb << std::endl;
            std::cout << "lnLikelihoodProb = " << lnLikelihoodProb << "\t\t-\t\t" << touchedLnLikelihoodProb << "\t\t-\t\t" << touchedAgainLnLikelihoodProb << std::endl;
            std::cout << "lnProb           = " << lnProb << "\t\t-\t\t" << touchedLnProb << "\t\t-\t\t" << touchedAgainLnProb << std::endl;
            std::cout << "Failure occurred after move:\t" << theMove.getMoveName() << std::endl;
            
            //            throw RbException("Error in MCMC probability computation.");
        }
#endif
        
    }
    
    
    // advance gen cycle if needed (i.e. run()==true, burnin()==false)
    if (advanceCycle)
    {
        generation++;
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
    const std::vector<DagNode*>& modelNodes = model.getDagNodes();
    for (RbConstIterator<Move> it = mvs.begin(); it != mvs.end(); ++it)
    {
        if ( it->getMoveName() == "SpeciesSubtreeScale" )
        {
            std::cerr << "Cloning SpeciesSubtreeScale.\n";
        }
        
        Move *theMove = it->clone();
        std::vector<DagNode*> nodes = theMove->getDagNodes();
        for (std::vector<DagNode*>::const_iterator j = nodes.begin(); j != nodes.end(); ++j)
        {
            
            RevBayesCore::DagNode *theNode = *j;
            
            // error checking
            if ( theNode->getName() == "" )
            {
                std::cerr << "The move has the following nodes:\n";
                for (std::vector<DagNode*>::const_iterator k = nodes.begin(); k != nodes.end(); ++k)
                {
                    std::cerr << (*k)->getName() << std::endl;
                }
                std::cerr << "The model has the following nodes:\n";
                for (std::vector<DagNode*>::const_iterator k = modelNodes.begin(); k != modelNodes.end(); ++k)
                {
                    std::cerr << (*k)->getName() << std::endl;
                }
                throw RbException( "Unable to connect move '" + theMove->getMoveName() + "' to DAG copy because variable name was lost");
            }
            
            DagNode* theNewNode = NULL;
            for (std::vector<DagNode*>::const_iterator k = modelNodes.begin(); k != modelNodes.end(); ++k)
            {
                if ( (*k)->getName() == theNode->getName() )
                {
                    theNewNode = *k;
                    break;
                }
            }
            // error checking
            if ( theNewNode == NULL )
            {
                throw RbException("Cannot find node with name '" + theNode->getName() + "' in the model but received a move working on it.");
            }
            
            // now swap the node
            theMove->swapNode( *j, theNewNode );
        }
        moves.push_back( *theMove );
        delete theMove;
    }
    
    for (RbConstIterator<Monitor> it = mons.begin(); it != mons.end(); ++it)
    {
        Monitor *theMonitor = it->clone();
        std::vector<DagNode*> nodes = theMonitor->getDagNodes();
        for (std::vector<DagNode*>::const_iterator j = nodes.begin(); j != nodes.end(); ++j)
        {
            
            RevBayesCore::DagNode *theNode = (*j);
            
            // error checking
            if ( theNode->getName() == "" )
            {
                throw RbException( "Unable to connect monitor to DAG copy because variable name was lost");
            }
            
            DagNode* theNewNode = NULL;
            for (std::vector<DagNode*>::const_iterator k = modelNodes.begin(); k != modelNodes.end(); ++k)
            {
                if ( (*k)->getName() == theNode->getName() )
                {
                    theNewNode = *k;
                    break;
                }
            }
            // error checking
            if ( theNewNode == NULL )
            {
                throw RbException("Cannot find node with name '" + theNode->getName() + "' in the model but received a monitor working on it.");
            }
            
            // now swap the node
            theMonitor->swapNode( *j, theNewNode );
        }
        monitors.push_back( *theMonitor );
        delete theMonitor;
        
    }
    
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
 * Set if the current chain is the active chain.
 * Only active chains print to the monitors.
 */
void Mcmc::setChainActive(bool tf)
{
    chainActive = tf;
}


/**
 * Set the heat of the likelihood of the current chain.
 * This heat is used in posterior posterior MCMC algorithms to
 * heat the likelihood
 * The heat is passed to the moves for the accept-reject mechanism.
 */
void Mcmc::setChainLikelihoodHeat(double h)
{
    chainLikelihoodHeat = h;
}


/**
 * Set the heat of the likelihood of the current chain.
 * This heat is used in posterior posterior MCMC algorithms to
 * heat the likelihood
 * The heat is passed to the moves for the accept-reject mechanism.
 */
void Mcmc::setLikelihoodHeat(double h)
{
    chainLikelihoodHeat = h;
}


/**
 * Set the number of processes available to this specific MCMC simulation.
 * If there is more than one process available, then we can use these
 * to compute the likelihood in parallel. Yeah!
 */
void Mcmc::setNumberOfProcesses(size_t n, size_t offset)
{
    MonteCarloSampler::setNumberOfProcesses(n, offset);
    
    // delegate the call to the model
    model.setNumberOfProcesses(n,offset);
}


/**
 * Set the heat of the posterior of the current chain.
 * The heat of the posterior is used in the MC^3 algorithm.
 * The heat is passed to the moves for the accept-reject mechanism.
 */
void Mcmc::setChainPosteriorHeat(double h)
{
    chainPosteriorHeat = h;
}


/**
 * Get the index of the current chain.
 */
void Mcmc::setChainIndex(size_t x)
{
    chainIdx = x;
}


/**
 * Set the index of the replicate.
 * We tell this to all our monitors.
 */
void Mcmc::setReplicateIndex(size_t idx)
{
    this->replicateIndex = idx;
    
    // tell each monitor
    for (RbIterator<Monitor> it=monitors.begin(); it!=monitors.end(); ++it)
    {
        it->setReplicateIndex( idx );
    }
    
}


/**
 * Set the index of the replicate.
 * We tell this to all our monitors.
 */
void Mcmc::setStoneIndex(size_t idx)
{
    
    // tell each monitor
    for (RbIterator<Monitor> it=monitors.begin(); it!=monitors.end(); ++it)
    {
        it->setStoneIndex( idx );
    }
    
}


void Mcmc::setScheduleType(const std::string &s)
{
    
    scheduleType = s;
}


void Mcmc::startMonitors( size_t numCycles )
{
    
    /* Open the output file and print headers */
    for (size_t i=0; i<monitors.size(); i++)
    {
        
        // open filestream for each monitor
        //        monitors[i].openStream();
        
        // reset the monitor
        monitors[i].reset( numCycles );
        
        
        // if this chain is active, print the header
        if (chainActive)
        {
            monitors[i].openStream();
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

