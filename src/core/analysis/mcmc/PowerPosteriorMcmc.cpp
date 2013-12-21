#include "DagNode.h"
#include "FileMonitor.h"
#include "PowerPosteriorMcmc.h"
#include "MoveSchedule.h"
#include "PathSampleMonitor.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbOptions.h"
#include "SequenctialMoveSchedule.h"

#include <cmath>
#include <typeinfo>

using namespace RevBayesCore;

PowerPosteriorMcmc::PowerPosteriorMcmc(const Model& m, const std::vector<Move*> &mvs) : model( m ) {
    
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
    
    initialize();
}


PowerPosteriorMcmc::PowerPosteriorMcmc(const PowerPosteriorMcmc &m) : model( m.model ), beta( m.beta ), sampleFreq( m.sampleFreq ) {
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
    
    initialize();
}


PowerPosteriorMcmc::~PowerPosteriorMcmc(void) {
    // free the moves and monitors
    for (std::vector<Move*>::iterator it = moves.begin(); it != moves.end(); ++it) {
        Move *theMove = (*it);
        delete theMove;
    }
}

void PowerPosteriorMcmc::burnin(int generations, int tuningInterval) {
    
    
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
            Move* theMove = schedule.nextMove(k);
            
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



PowerPosteriorMcmc* PowerPosteriorMcmc::clone( void ) const {
    return new PowerPosteriorMcmc( *this );
}



const std::vector<double>& PowerPosteriorMcmc::getSamples( void ) const {
    return samples;
}



void PowerPosteriorMcmc::initialize( void ) {
    
    std::cerr << "Initializing power posterior ..." << std::endl;
    
    std::vector<DagNode *>& dagNodes = model.getDagNodes();
    
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
            for (std::vector<DagNode *>::iterator i=dagNodes.begin(); i!=dagNodes.end(); i++) {
                if ( !(*i)->isClamped() && (*i)->isStochastic() ) {
                    (*i)->redraw();
                    (*i)->touch();
                }
            }
        }
        
    } while (lnProbability != lnProbability || lnProbability == RbConstants::Double::neginf || lnProbability == RbConstants::Double::nan);
    
}


double PowerPosteriorMcmc::pathSampling( void ) {
    
    std::vector<double> pathValues;
    for (size_t i = 0; i < beta.size(); ++i) {
        
        double mean = 0.0;
        for (size_t j = 0; j < samplesPerPath; ++j) {
            mean += samples[i*samplesPerPath + j];
        }
        mean /= samplesPerPath;
        
        pathValues.push_back( mean );
        
    }
    
    double marginal = 0.0;
    for (size_t i = 0; i < pathValues.size(); ++i) {
        marginal += (pathValues[i] + pathValues[i+1])*(beta[i]-beta[i+1])/2.0;
    }
    
    return marginal;
}


void PowerPosteriorMcmc::run(int gen) {
    
    std::cerr << "Running power posterior ..." << std::endl;
    
    RandomNumberGenerator* rng        = GLOBAL_RNG;
    std::vector<DagNode *>& dagNodes = model.getDagNodes();
    
    // reset the counters for the move schedules
    for (std::vector<Move*>::iterator it = moves.begin(); it != moves.end(); ++it) {
        (*it)->resetCounters();
    }
    
    samples.clear();
    samplesPerPath = gen / sampleFreq;
    
    /* Run the chain */    
    SequentialMoveSchedule schedule = SequentialMoveSchedule(moves);
    for (size_t i = 0; i < beta.size(); ++i) {
        double b = beta[i];
        std::cerr << "Beta = " << b << std::endl;

        for (int k=1; k<=gen; k++) {
        
            size_t proposals = round( schedule.getNumberMovesPerIteration() );
            for (size_t j=0; j<proposals; j++) {
                /* Get the move */
                Move* theMove = schedule.nextMove(k);
            
                /* Propose a new value */
                double lnProbabilityRatio;
                double lnHastingsRatio = theMove->perform(lnProbabilityRatio);
                // QUESTION: How to Gibbs samplers by-pass the accept-reject?
                /* Calculate acceptance ratio */
                const std::set<DagNode*> &changedNodes = theMove->getDagNodes();
                double priorRatio = 0.0;
                double likelihoodRatio = 0.0;
                for (std::set<DagNode*>::const_iterator it = changedNodes.begin(); it != changedNodes.end(); ++it) {
                    priorRatio += (*it)->getLnProbabilityRatio();
                    
                    // get the affected nodes
                    std::set<DagNode*> affected;
                    (*it)->getAffectedNodes(affected);
                    
                    for (std::set<DagNode*>::const_iterator a = affected.begin(); a != affected.end(); ++a) {
                        if ( (*a)->isClamped() ) {
                            likelihoodRatio += (*a)->getLnProbabilityRatio();
                        } else {
                            priorRatio += (*a)->getLnProbabilityRatio();
                        }
                    }
                    
                }
                
                double acceptance_ratio;
                if ( b == 0.0 ) {
                    acceptance_ratio = priorRatio+lnHastingsRatio;
                } else { 
                    acceptance_ratio = priorRatio+b*likelihoodRatio+lnHastingsRatio;
                }
                
                double r;
                if (acceptance_ratio > 0.0)
                    r = 1.0;
                else if (acceptance_ratio < -300.0)
                    r = 0.0;
                else
                    r = exp(acceptance_ratio);
            
                /* Accept or reject the move */
                double u = rng->uniform01(); // TODO No need to draw rng if lnR > 0.0x
                if (u < r) {
                    theMove->accept();
//                    lnProbability += lnProbabilityRatio;
                }
                else {
                    theMove->reject();
                }
            }
        
            // sample the likelihood
            if ( k % sampleFreq == 0 ) {
                // compute the joint likelihood
                double likelihood = 0.0;
                for (std::vector<DagNode*>::const_iterator n = dagNodes.begin(); n != dagNodes.end(); ++n) {
                    if ( (*n)->isClamped() ) {
                        likelihood += (*n)->getLnProbability();
                    }
                }
                
                samples.push_back( likelihood );
            }
        
        }
        
    }
    
    
}


void PowerPosteriorMcmc::setBeta(const std::vector<double> &b) {
    beta = b;
}


void PowerPosteriorMcmc::setSampleFreq(size_t sf) {
    sampleFreq = sf;
}

double PowerPosteriorMcmc::steppingStoneSampling( void ) {
//    max <- samples[which.max(samples)]
//#    samples <- samples[-which.max(samples)]
//    if ( k > 1 ) {
//        BF <- BF + log(mean( exp(samples-max)^(beta[k-1]-beta[k]) ))+(beta[k-1]-beta[k])*max
//#      BF <- BF + log(sum( exp(samples-max)^(beta[k-1]-beta[k]) ))-log(iterations)+(beta[k-1]-beta[k])*max
//    }
    
    std::vector<double> pathValues;
    double marginal = 0.0;
    for (size_t i = 1; i < beta.size(); ++i) {
        
        double max = samples[i*samplesPerPath];
        for (size_t j = 1; j < samplesPerPath; ++j) {
            if (max < samples[i*samplesPerPath + j]) {
                max = samples[i*samplesPerPath + j];
            }
        }
        
        // mean( exp(samples-max)^(beta[k-1]-beta[k]) )     or
        // mean( exp( (samples-max)*(beta[k-1]-beta[k]) ) )
        double mean = 0.0;
        for (size_t j = 0; j < samplesPerPath; ++j) {
            mean += exp( (samples[i*samplesPerPath + j]-max)*(beta[i-1]-beta[i]) );
        }
        mean /= samplesPerPath;
        
        marginal += log(mean) + (beta[i-1]-beta[i])*max;
        
    }
        
    return marginal;
}
