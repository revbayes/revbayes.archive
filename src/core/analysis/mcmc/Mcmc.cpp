#include "DagNode.h"
#include "FileMonitor.h"
#include "Mcmc.h"
#include "MoveSchedule.h"
#include "PathSampleMonitor.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbOptions.h"
#include "SequenctialMoveSchedule.h"
#include "RandomMoveSchedule.h"
#include "ExtendedNewickTreeMonitor.h"
#include "ExtendedNewickAdmixtureTreeMonitor.h"

#include <cmath>
#include <typeinfo>

using namespace RevBayesCore;

Mcmc::Mcmc(const Model& m, const std::vector<Move*> &mvs, const std::vector<Monitor*> &mons) : model( m ), moves(), monitors() {
    
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
        std::set<DagNode*> nodes = theMonitor->getDagNodes();
//        orgNodesMonitors.insert( std::pair<Monitor*, std::set<DagNode*> >(theMonitor, nodes) );
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
                throw RbException("Cannot find node with name '" + (*j)->getName() + "' in the model but received a monitor working on it.");
            }
            
            // now swap the node
            theMonitor->swapNode( *j, theNewNode );
        }
        monitors.push_back( theMonitor );
    }
    
    initialize();
}


Mcmc::Mcmc(const Mcmc &m) : model( m.model ) {
    const std::vector<Monitor*>& mons = m.monitors;
    const std::vector<Move*>& mvs = m.moves;
    
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
        std::set<DagNode*> nodes = theMonitor->getDagNodes();
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
                throw RbException("Cannot find node with name '" + (*j)->getName() + "' in the model but received a monitor working on it.");
            }
            
            // now swap the node
            theMonitor->swapNode( *j, theNewNode );
        }
        monitors.push_back( theMonitor );
    }
    
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
    
    
    std::cout << "burning in the chain ..." << std::endl;
    std::cout << "0--------25--------50--------75--------100" << std::endl;
    std::cout << "*";
    std::cout.flush();
    
    RandomNumberGenerator* rng        = GLOBAL_RNG;
#ifdef DEBUG_MCMC
    std::vector<DagNode *>& dagNodes = model.getDagNodes();
#endif
    
    // reset the counters for the move schedules
    for (std::vector<Move*>::iterator it = moves.begin(); it != moves.end(); ++it) {
        (*it)->resetCounters();
    }
    
    /* Run the chain */    
    SequentialMoveSchedule schedule = SequentialMoveSchedule(moves);
    //RandomMoveSchedule schedule = RandomMoveSchedule(moves);
    
    for (int k=1; k<=generations; k++) {
        
        if ( k % (generations/20) == 0 ) {
            std::cout << "**";
            std::cout.flush();
        }
        
        size_t proposals = round( schedule.getNumberMovesPerIteration() );
        for (size_t i=0; i<proposals; i++) {
            /* Get the move */
            Move* theMove = schedule.nextMove();
            
            if ( theMove->isGibbs() ) {
                // do Gibbs proposal
                theMove->performGibbs();
                //                theMove->accept(); Not necessary, because Gibbs samplers are automatically accepted.
            } else {
                // do a Metropolois-Hastings proposal
                
                /* Propose a new value */
                double lnProbabilityRatio;
                double lnHastingsRatio = theMove->perform(lnProbabilityRatio);
                // QUESTION: How to Gibbs samplers by-pass the accept-reject?
                /* Calculate acceptance ratio */
                double lnR = lnProbabilityRatio + lnHastingsRatio;
                
                if (lnR >= 0.0) {
                    theMove->accept();
                    lnProbability += lnProbabilityRatio;
                }
                else if (lnR < -300.0){ 
                    theMove->reject();
                }
                else { 
                    double r = exp(lnR);
                    /* Accept or reject the move */
                    double u = rng->uniform01(); 
                    if (u < r) {
                        theMove->accept();
                        lnProbability += lnProbabilityRatio;
                    }
                    else {
                        theMove->reject();
                    }
                }
            }
            
            
            
#ifdef DEBUG_MCMC
            /* Assert that the probability calculation shortcuts work */
            double curLnProb = 0.0;
            std::vector<double> lnRatio;
            for (std::vector<DagNode*>::iterator i=dagNodes.begin(); i!=dagNodes.end(); i++) {
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
        
        // check for autotuning and only tune in first half
        if ( k % tuningInterval == 0 ) {
            
            /* tune the moves */
            for (size_t i=0; i<moves.size(); i++) {
                moves[i]->autoTune();
            }
        }
        
    }
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
    
        
    /* Open the output file and print headers */
    for (size_t i=0; i<monitors.size(); i++) {
                
        // get the monitor
        if ( typeid(*monitors[i]) == typeid(FileMonitor) ) {
            
            FileMonitor* theMonitor = static_cast<FileMonitor*>( monitors[i] );
            
            // open the file stream for the monitor
            theMonitor->openStream();
            
            // print the header information
            theMonitor->printHeader();
        }
        else if ( typeid(*monitors[i]) == typeid(ExtendedNewickTreeMonitor) ) {
            
            ExtendedNewickTreeMonitor* theMonitor = static_cast<ExtendedNewickTreeMonitor*>( monitors[i] );
            
            // open the file stream for the monitor
            theMonitor->openStream();
            
            // print the header information
            theMonitor->printHeader();
        }
        else if ( typeid(*monitors[i]) == typeid(ExtendedNewickAdmixtureTreeMonitor) ) {
            
            ExtendedNewickAdmixtureTreeMonitor* theMonitor = static_cast<ExtendedNewickAdmixtureTreeMonitor*>( monitors[i] );
            
            // open the file stream for the monitor
            theMonitor->openStream();
            
            // print the header information
            theMonitor->printHeader();
        }
        
        else if ( typeid(*monitors[i]) == typeid(PathSampleMonitor) ) {
            
            PathSampleMonitor* theMonitor = static_cast<PathSampleMonitor*>( monitors[i] );
            
            // open the file stream for the monitor
            theMonitor->openStream();
            
            // print the header information
            theMonitor->printHeader();
        }
        
        // set the model
        monitors[i]->setModel( &model );
        
    }
    
    
    /* Get initial lnProbability of model */
    
    // first we touch all nodes so that the likelihood is dirty
    for (std::vector<DagNode *>::iterator i=dagNodes.begin(); i!=dagNodes.end(); i++) {
        (*i)->touch();
    }
    
    lnProbability = 0.0;
    do {
        lnProbability = 0.0;
        for (std::vector<DagNode *>::iterator i=dagNodes.begin(); i!=dagNodes.end(); i++) {
            DagNode* node = (*i);
            
            double lnProb = node->getLnProbability();
            lnProbability += lnProb;
            
        }
        
        // now we keep all nodes so that the likelihood is stored
        for (std::vector<DagNode *>::iterator i=dagNodes.begin(); i!=dagNodes.end(); i++) {
            (*i)->keep();
        }
        
        if (lnProbability != lnProbability || lnProbability == RbConstants::Double::neginf || lnProbability == RbConstants::Double::nan) {
            std::cerr << "Drawing new initial states ... " << std::endl;
            for (std::vector<DagNode *>::iterator i=orderedStochNodes.begin(); i!=orderedStochNodes.end(); i++) {
                if ( !(*i)->isClamped() && (*i)->isStochastic() ) {
                    std::cout << "Redrawing values for node " << (*i)->getName() << std::endl;
                    (*i)->redraw();
//                    (*i)->touch(); Not necessary. The distribution will automaticall call touch().
                }
            }
        }
        
    } while (lnProbability != lnProbability || lnProbability == RbConstants::Double::neginf || lnProbability == RbConstants::Double::nan);
    
    gen = 0;
}


void Mcmc::run(int generations) {
    
    if ( gen == 0 ) {
        /* Monitor */
        for (size_t i=0; i<monitors.size(); i++) {
            monitors[i]->monitor(0);
        }
    }

    RandomNumberGenerator* rng        = GLOBAL_RNG;
#ifdef DEBUG_MCMC
    std::vector<DagNode *>& dagNodes = model.getDagNodes();
#endif
    
    // reset the counters for the move schedules
    for (std::vector<Move*>::iterator it = moves.begin(); it != moves.end(); ++it) {
        (*it)->resetCounters();
    }
    
    /* Run the chain */    
    SequentialMoveSchedule schedule = SequentialMoveSchedule(moves);
    //RandomMoveSchedule schedule = RandomMoveSchedule(moves);
    
    for (int k=1; k<=generations; k++) {
        gen++;
        
        size_t proposals = round( schedule.getNumberMovesPerIteration() );
        for (size_t i=0; i<proposals; i++) {
            /* Get the move */
            Move* theMove = schedule.nextMove();
            
            if ( theMove->isGibbs() ) {
                // do Gibbs proposal
                theMove->performGibbs();
//                theMove->accept(); Not necessary, because Gibbs samplers are automatically accepted.
            } else {
                // do a Metropolois-Hastings proposal
                
                /* Propose a new value */
                double lnProbabilityRatio;
                double lnHastingsRatio = theMove->perform(lnProbabilityRatio);
                // QUESTION: How to Gibbs samplers by-pass the accept-reject?
                /* Calculate acceptance ratio */
                double lnR = lnProbabilityRatio + lnHastingsRatio;
                
                if (lnR >= 0.0) {
                    theMove->accept();
                    lnProbability += lnProbabilityRatio;
                }
                else if (lnR < -300.0){ 
                    theMove->reject();
                }
                else { 
                    double r = exp(lnR);
                    /* Accept or reject the move */
                    double u = rng->uniform01(); 
                    if (u < r) {
                        theMove->accept();
                        lnProbability += lnProbabilityRatio;
                    }
                    else {
                        theMove->reject();
                    }
                }
            }
            
            
            
#ifdef DEBUG_MCMC
            /* Assert that the probability calculation shortcuts work */
            double curLnProb = 0.0;
            std::vector<double> lnRatio;
            for (std::vector<DagNode*>::iterator i=dagNodes.begin(); i!=dagNodes.end(); i++) {
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
        
        /* Monitor */
        for (size_t i=0; i<monitors.size(); i++) {
            monitors[i]->monitor(gen);
        }
        
//        // check for autotuning and only tune in first half
//        if ( gen <= generations/2 && gen % tuningInterval == 0 ) {
//            
//            /* tune the moves */
//            for (size_t i=0; i<moves.size(); i++) {
//                moves[i]->autoTune();
//            }
//        }
                
    }
    
    /* Close the output file and print headers */
//    for (size_t i=0; i<monitors.size(); i++) {
//        // get the monitor
//        if ( typeid(*monitors[i]) == typeid(FileMonitor) ) {
//            
//            FileMonitor* theMonitor = static_cast<FileMonitor*>( monitors[i] );
//            
//            // open the file stream for the monitor
//            theMonitor->closeStream();
//        }
//    }

    
}


void Mcmc::printOperatorSummary(void) const {
    
    
    // printing the moves summary
    
    std::cerr << "                  Name                  | Param    |  Weight  |  Tried   | Accepted | Acc. Ratio| Parameters" << std::endl;
    std::cerr << "=====================================================================================================================" << std::endl;
    for (std::vector<Move*>::const_iterator it = moves.begin(); it != moves.end(); ++it) {
        (*it)->printSummary(std::cerr);
    }
    
    std::cout << std::endl;
}
