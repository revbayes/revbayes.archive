#include "DagNode.h"
#include "FileMonitor.h"
#include "Mcmc.h"
#include "MoveSchedule.h"
#include "PathSampleMonitor.h"
#include "RandomMoveSchedule.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbOptions.h"
#include "SequenctialMoveSchedule.h"
#include "RandomMoveSchedule.h"
#include "ExtendedNewickTreeMonitor.h"
#include "ExtendedNewickAdmixtureTreeMonitor.h"
#include "AdmixtureBipartitionMonitor.h"

#include <cmath>
#include <typeinfo>

using namespace RevBayesCore;

Mcmc::Mcmc(const Model& m, const std::vector<Move*> &mvs, const std::vector<Monitor*> &mons, bool ca, double ch, int ci) : model( m ), moves(), monitors(), schedule(std::vector<Move*>()), chainActive(ca), chainHeat(ch), chainIdx(ci) {
    
    replaceDag(mvs,mons);
    initialize();
}


Mcmc::Mcmc(const Mcmc &m) : model( m.model ), schedule(std::vector<Move*>()), chainActive(m.chainActive), chainHeat(m.chainHeat), chainIdx(m.chainIdx) {
   
    const std::vector<Monitor*>& mons = m.monitors;
    const std::vector<Move*>& mvs = m.moves;
    
    replaceDag(mvs,mons);
    initialize();
}


Mcmc::~Mcmc(void) {
    // free the moves and monitors
    for (std::vector<Move*>::iterator it = moves.begin(); it != moves.end(); ++it) {
        Move *theMove = (*it);
        delete theMove;
    }
    
    for (std::vector<Monitor*>::iterator it = monitors.begin(); it != monitors.end(); ++it) {
        Monitor *theMonitor = (*it);
        delete theMonitor;
    }
}


void Mcmc::burnin(int generations, int tuningInterval) {
    
    if (chainActive)
    {
        std::cout << "burning in the chain ..." << std::endl;
        std::cout << "0--------25--------50--------75--------100" << std::endl;
        std::cout << "*";
        std::cout.flush();
    }
    
#ifdef DEBUG_MCMC
    std::vector<DagNode *>& dagNodes = model.getDagNodes();
#endif
    
    // reset the counters for the move schedules
    for (std::vector<Move*>::iterator it = moves.begin(); it != moves.end(); ++it) 
    {
        (*it)->resetCounters();
    }
    
    int printInterval = int(fmax(1,generations/20.0));
    
    // Run the chain
    for (int k=1; k<=generations; k++) 
    {
        
        if ( k % printInterval == 0 ) 
        {
            std::cout << "**";
            std::cout.flush();
        }
        
        nextCycle(false);
        
        // check for autotuning
        if ( k % tuningInterval == 0 ) 
        {
            
            // tune the moves
            for (size_t i=0; i<moves.size(); i++) {
                moves[i]->autoTune();
            }
        }
        
    }
    
    if (chainActive)
        std::cout << std::endl;
}


Mcmc* Mcmc::clone( void ) const {
    return new Mcmc( *this );
}


/** Creates a vector of stochastic nodes, starting from the source nodes to the sink nodes */
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
    else { //if the node is stochastic or deterministic
        //First I have to visit my parents
        const std::set<const DagNode *>& parents = dagNode->getParents() ;
        std::set<const DagNode *>::const_iterator it;
        for ( it=parents.begin() ; it != parents.end(); it++ ) 
            getOrderedStochasticNodes(*it, orderedStochasticNodes, visitedNodes);
        
        //Then I can add myself to the nodes visited, and to the ordered vector of stochastic nodes
//        visitedNodes.insert(dagNode);
        if ( dagNode->isStochastic() ) //if the node is stochastic
            orderedStochasticNodes.push_back( const_cast<DagNode*>( dagNode ) );
        
        //Finally I will visit my children
        std::set<DagNode*> children = dagNode->getChildren() ;
        std::set<DagNode*>::iterator it2;
        for ( it2 = children.begin() ; it2 != children.end(); it2++ ) 
            getOrderedStochasticNodes(*it2, orderedStochasticNodes, visitedNodes);
    }
    
    return; 
}

void Mcmc::initialize( void ) {
    
    std::vector<DagNode *>& dagNodes = model.getDagNodes();
    std::vector<DagNode *> orderedStochNodes;
    std::set< const DagNode *> visited;
    getOrderedStochasticNodes(dagNodes[0],orderedStochNodes, visited );
    
    //std::cout << "#ordered-nodes: " << orderedStochNodes.size() << std::endl;
    //std::cout << "#dag-nodes: " << dagNodes.size() << std::endl;
    
    /* Open the output file and print headers */
    for (size_t i=0; i<monitors.size(); i++) 
    {
        
        // open filestream for each monitor
        monitors[i]->openStream();

        // if this chain is active, print the header
        if (chainActive) // surprised this works properly...
            monitors[i]->printHeader();
        
        // set the model
        monitors[i]->setModel( &model );
        
        // set the mcmc
        monitors[i]->setMcmc( this );
        
    }
    
    
    /* Get initial lnProbability of model */
    
    // first we touch all nodes so that the likelihood is dirty
    for (std::vector<DagNode *>::iterator i=dagNodes.begin(); i!=dagNodes.end(); i++) 
    {
        (*i)->touch();
    }
    
    
    // redraw parameters for inactive chains in pMC^3 team
    if (chainActive == false)
    {
        for (std::vector<DagNode *>::iterator i=orderedStochNodes.begin(); i!=orderedStochNodes.end(); i++)
        {
            //std::cout << (*i)->getName() << std::endl;
            if ( !(*i)->isClamped() && (*i)->isStochastic() )
            {
                std::cout << "Redrawing values for node " << (*i)->getName() << std::endl;
                (*i)->redraw();
                //                    (*i)->touch(); Not necessary. The distribution will automaticall call touch().
            }
            else if ( (*i)->isClamped() )
            {
                // make sure that the clamped node also recompute their probabilities
                (*i)->touch();
            }
            
        }
    }
    
    lnProbability = 0.0;
    do
    {
        lnProbability = 0.0;
        for (std::vector<DagNode *>::iterator i=dagNodes.begin(); i!=dagNodes.end(); i++) 
        {
            DagNode* node = (*i);
            (*i)->touch();
            
            double lnProb = node->getLnProbability();
            lnProbability += lnProb;
//            std::cout << node->getName() << " " << node->getLnProbability() << " " << ( node->isClamped() ? "*" : "_") << "\n";
            
        }
        
        // now we keep all nodes so that the likelihood is stored
        for (std::vector<DagNode *>::iterator i=dagNodes.begin(); i!=dagNodes.end(); i++) 
        {
            (*i)->keep();
        }
        
        if (lnProbability != lnProbability || lnProbability == RbConstants::Double::neginf || lnProbability == RbConstants::Double::nan)
        {
            std::cerr << "Drawing new initial states ... " << std::endl;
            for (std::vector<DagNode *>::iterator i=orderedStochNodes.begin(); i!=orderedStochNodes.end(); i++) 
            {
                //std::cout << (*i)->getName() << std::endl;
                if ( !(*i)->isClamped() && (*i)->isStochastic() )
                {
                    std::cout << "Redrawing values for node " << (*i)->getName() << std::endl;
                    (*i)->redraw();
//                    (*i)->touch(); Not necessary. The distribution will automaticall call touch().
                }
                else if ( (*i)->isClamped() ) 
                {
                    // make sure that the clamped node also recompute their probabilities
                    (*i)->reInitialized();
                    (*i)->touch();
                }
                
            }
        }
        
    } while (lnProbability != lnProbability || lnProbability == RbConstants::Double::neginf || lnProbability == RbConstants::Double::nan);
    

    schedule = RandomMoveSchedule(moves);
    //if (moveSchedule != NULL)
        
    gen = 0;
}

void Mcmc::replaceDag(const std::vector<Move *> &mvs, const std::vector<Monitor *> &mons)
{
    // we need to replace the DAG nodes of the monitors and moves
    const std::vector<DagNode*>& modelNodes = model.getDagNodes();
    for (std::vector<Move*>::const_iterator it = mvs.begin(); it != mvs.end(); ++it) {
        Move *theMove = (*it)->clone();
        std::set<DagNode*> nodes = theMove->getDagNodes();
        for (std::set<DagNode*>::const_iterator j = nodes.begin(); j != nodes.end(); ++j) {
            DagNode* theNewNode = NULL;
            for (std::vector<DagNode*>::const_iterator k = modelNodes.begin(); k != modelNodes.end(); ++k) {
                if ( (*k)->getName() == (*j)->getName() ) {
                    theNewNode = *k;
                    break;
                }
            }
            // error checking
            if ( theNewNode == NULL ) {
                throw RbException("Cannot find node with name '" + (*j)->getName() + "' in the model but received a move working on it.");
            }
            
            // now swap the node
            theMove->swapNode( *j, theNewNode );
        }
        moves.push_back( theMove );
    }
    
    for (std::vector<Monitor*>::const_iterator it = mons.begin(); it != mons.end(); ++it) {
        Monitor *theMonitor = (*it)->clone();
        std::vector<DagNode*> nodes = theMonitor->getDagNodes();
        for (std::vector<DagNode*>::const_iterator j = nodes.begin(); j != nodes.end(); ++j) {
            DagNode* theNewNode = NULL;
            for (std::vector<DagNode*>::const_iterator k = modelNodes.begin(); k != modelNodes.end(); ++k) {
                if ( (*k)->getName() == (*j)->getName() ) {
                    theNewNode = *k;
                    break;
                }
            }
            // error checking
            if ( theNewNode == NULL ) {
                throw RbException("Cannot find node with name '" + (*j)->getName() + "' in the model but received a monitor working on it.");
            }
            
            // now swap the node
            theMonitor->swapNode( *j, theNewNode );
        }
        monitors.push_back( theMonitor );
    }
}


void Mcmc::run(int generations) {
    
    if ( gen == 0 ) 
    {
        // Monitor
        monitor(0);
    }
    
#ifdef DEBUG_MCMC
    std::vector<DagNode *>& dagNodes = model.getDagNodes();
#endif
    
    // reset the counters for the move schedules
    for (std::vector<Move*>::iterator it = moves.begin(); it != moves.end(); ++it) 
    {
        (*it)->resetCounters();
    }
    
    // Run the chain
    for (int k=1; k<=generations; k++) 
    {
        nextCycle(true);
        
        // Monitor
        monitor(gen);
                        
    }
    
    
}

int Mcmc::nextCycle(bool advanceCycle) {

    size_t proposals = round( schedule.getNumberMovesPerIteration() );
    for (size_t i=0; i<proposals; i++) 
    {
        // Get the move
        Move* theMove = schedule.nextMove( gen );
        
        if ( theMove->isGibbs() ) 
        {
            // do Gibbs proposal
            theMove->performGibbs();
            // theMove->accept(); // Not necessary, because Gibbs samplers are automatically accepted.
        } 
        else 
        {
            // do a Metropolois-Hastings proposal
            
            // Propose a new value
            double lnProbabilityRatio;
            double lnHastingsRatio = theMove->perform(lnProbabilityRatio);

            // Calculate acceptance ratio
            double lnR = chainHeat * (lnProbabilityRatio) + lnHastingsRatio;
            
            //std::cout << "\n**********\n";
            //std::cout << theMove->getMoveName() << "\n";
            //std::cout << lnHastingsRatio << "  " << lnProbabilityRatio << ";  " << lnProbability << " -> " << lnProbability + lnProbabilityRatio << "\n";
            //std::cout << "pre-mcmc     " << lnProbability << "\n";
            
            if (lnR >= 0.0) 
            {
                theMove->accept();
                lnProbability += lnProbabilityRatio;
              //  if (lnProbability > 0.0)
                {
                   
                }
            }
            else if (lnR < -300.0)
            {
                theMove->reject();
            }
            else 
            {
                double r = exp(lnR);
                // Accept or reject the move
                double u = GLOBAL_RNG->uniform01();
                if (u < r) 
                {
                    theMove->accept();
                    lnProbability += lnProbabilityRatio;
                }
                else 
                {
                    theMove->reject();
                }
            }
            //std::cout << "post-mcmc    " << lnProbability << "\n";
            //std::cout << "model        " << getModelLnProbability() << "\n";
            //std::cout << "**********\n\n";
        }
        
#ifdef DEBUG_MCMC
        // Assert that the probability calculation shortcuts work
        double curLnProb = 0.0;
        std::vector<double> lnRatio;
        for (std::vector<DagNode*>::iterator i=dagNodes.begin(); i!=dagNodes.end(); i++) 
        {
            (*i)->touch();
            double lnProb = (*i)->getLnProbability();
            curLnProb += lnProb;
        }
        if (fabs(lnProbability - curLnProb) > 1E-8)
            throw RbException("Error in ln probability calculation shortcuts");
        else
            lnProbability = curLnProb;              // otherwise rounding errors accumulate
#endif
    }
    
    
    // advance gen cycle if needed (i.e. run()==true, burnin()==false)
    if (advanceCycle)
        gen++;
    
    // gen number used for p(MC)^3
    return gen;
}

double Mcmc::getLnPosterior(void)
{
    return lnProbability;
}

double Mcmc::getModelLnProbability(void)
{
    const std::vector<DagNode*> &n = model.getDagNodes();
    double pp = 0.0;
    for (std::vector<DagNode*>::const_iterator it = n.begin(); it != n.end(); ++it) {
        //std::cout << (*it)->getName() << "  " << (*it)->getLnProbability() << "\n";
        //(*it)->touch();
        pp += (*it)->getLnProbability();
    }
    return pp;
}

bool Mcmc::isChainActive(void)
{
    return chainActive;
}

double Mcmc::getChainHeat(void)
{
    return chainHeat;
}

void Mcmc::setChainActive(bool tf)
{
    chainActive = tf;
}

void Mcmc::setChainHeat(double v)
{
    chainHeat = v;
}

int Mcmc::getChainIdx(void)
{
    return chainIdx;
}

void Mcmc::setChainIdx(int x)
{
    chainIdx = x;
}

std::vector<Monitor*>& Mcmc::getMonitors(void)
{
    return monitors; 
}

void Mcmc::monitor(int g)
{
    // Monitor
    for (size_t i = 0; i < monitors.size(); i++) 
    {
        monitors[i]->monitor(g);
    }
}

void Mcmc::printOperatorSummary(void) const {
    
    
    // printing the moves summary
    
    std::cerr << "                  Name                  | Param    |  Weight  |  Tried   | Accepted | Acc. Ratio| Parameters" << std::endl;
    std::cerr << "=====================================================================================================================" << std::endl;
    for (std::vector<Move*>::const_iterator it = moves.begin(); it != moves.end(); ++it) 
    {
        (*it)->printSummary(std::cerr);
    }
    
    std::cout << std::endl;
}
