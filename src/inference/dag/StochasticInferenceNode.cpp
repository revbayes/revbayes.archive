/**
 * @file
 * This file contains the implementation of StochasticInferenceNode, which is derived
 * from VariableNode. StochasticInferenceNode is used for DAG nodes holding stochastic
 * variables with an associated distribution object.
 *
 * @brief Implementation of StochasticInferenceNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-24 20:57:15 -0700 (Tue, 24 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id: StochasticInferenceNode.cpp 1455 2012-04-25 03:57:15Z hoehna $
 */

#include "InferenceDagNode.h"
#include "InferenceDistribution.h"
#include "RbException.h"
#include "StochasticInferenceNode.h"
#include "RbValue.h"

#include <algorithm>
#include <cassert>
#include <cmath>



/** Constructor from distribution */
StochasticInferenceNode::StochasticInferenceNode( const RbValue<void*> &v, InferenceDistribution* dist ) : VariableInferenceNode( v ), clamped( false ), distribution( dist ), type( INSTANTIATED ), needsProbabilityRecalculation( true ), needsLikelihoodRecalculation( true ) {
    
}


/** Copy constructor */
StochasticInferenceNode::StochasticInferenceNode( const StochasticInferenceNode& x ) : VariableInferenceNode( x ) {
    
    /* Set distribution */
    distribution = x.distribution->clone();
    
}


/** Destructor */
StochasticInferenceNode::~StochasticInferenceNode( void ) {
    
}



/** Get the conditional ln probability of the node; do not rely on stored values */
double StochasticInferenceNode::calculateLnProbability( void ) {
    
    if (factorRoot != NULL) {
        lnProb =  factorRoot->calculateSummedLnProbability(0);
    }
    else if (needsProbabilityRecalculation) {
        // \TODO: Hack! We should make sure that the likelihood is properly calculated. (Sebastian)
        // I switched this test of, because we now instantiate the model from the RevLanguage
        // but do not actually sum over all possible values. This is only done in the MCMC.
        // Trying to safe some time ...
        //        if (type == INSTANTIATED) { // this should always be true
        lnProb = distribution->jointLnPdf();
        //        }
        //        else {
        //            throw RbException("We are asked to calculate the summed ln probability but do not have the root of the factor set. Oh oh ...");
        //        }
    }
    
    return lnProb;
}


/** 
 * Calculate the ln-probability summed over all possible states.
 * This is the variable elimination algorithm. It only works if we can condition on only one parent being eliminated.
 */
double StochasticInferenceNode::calculateEliminatedLnProbability( bool enforceProbabilityCalculation ) {
    
    //    // if this node is not eliminated, then we have reached the bottom of the elimination algorithm
    //    if ( type == INSTANTIATED ) {
    //        if (needsProbabilityRecalculation) {
    //            if (type == INSTANTIATED) { // this should always be true
    //                lnProb = distribution->lnPdf( *value );
    //            }
    //            else {
    //                throw RbException("We are asked to calculate the summed ln probability but do not have the root of the factor set. Oh oh ...");
    //            }
    //        }
    //        
    //        // \TODO: here we maybe could return our stored probability for this parent?
    //        return lnProb;
    //    }
    //    
    //    if (needsProbabilityRecalculation || needsLikelihoodRecalculation || enforceProbabilityCalculation) {
    //        // initialize the probability
    //        double prob = 0.0;
    //        
    //        // we need to iterate over my states
    //        DistributionDiscrete* d = static_cast<DistributionDiscrete*>( distribution );
    //        
    //        // we ask for the state vector
    //        const std::vector<RbLanguageObject*>& states = d->getStateVector();
    //        
    //        // now we calculate the probabilities and likelihoods
    //        size_t i = 0;
    //        for (std::vector<RbLanguageObject*>::const_iterator state = states.begin(); state != states.end(); state++) {
    //            // I set my value so that my children can access if for recalculation
    //            RbLanguageObject* v = (*state);
    //            // we set the value here and do not call set value because we do not want that the memory of the old value gets freed
    //            value = v;
    //            
    //            // since we just set the new value we need to flag all children for recalculation
    ////            markChildrenForRecalculation();
    //            
    //            // recalculate the likelihoods if necessary    
    //            if ( needsLikelihoodRecalculation ) {
    //                
    //                
    //                likelihoods[i] = 0.0;
    //                size_t j = 0;
    //                
    //                // I need to ask for the likelihood of my children
    //                for (std::set<VariableNode*>::iterator child = children.begin(); child != children.end(); child++) {
    //                    // only if the child is flagged as changed I need to ask for the likelihood
    //                    if ( (*child)->isTouched() ) {
    //                        partialLikelihoods[i][j] = (*child)->calculateEliminatedLnProbability(true);
    //                    }
    //                    
    //                    // add the log-likelihood for this child
    //                    likelihoods[i] += partialLikelihoods[i][j];
    //                    
    //                    // increment the child index
    //                    j++;
    //                }
    //                
    //            }
    //            
    //            // only if the likelihood of this value has changed we need to update it
    ////            if ( needsProbabilityRecalculation ) {
    //                // set the likelihood for this state to the probability array
    //                probabilities[i] = distribution->lnPdf( *value );
    ////            }
    //            
    //            // add this partial probability to the total probability
    //            prob += exp(probabilities[i] + likelihoods[i]);
    //            
    //            // increment the state index
    //            i++;
    //        }
    //        
    //        lnProb = log(prob);
    //        
    //        needsLikelihoodRecalculation    = false;
    //        needsProbabilityRecalculation   = false;
    //    }
    
    
    return lnProb;
}



/** 
 * Calculate the ln-probability summed over all possible states.
 * This is the sum-product algorithm. We try to use variable elimination for parts of the graph internally.
 */
double StochasticInferenceNode::calculateSummedLnProbability(size_t nodeIndex) {
    // get the node we are talking about from the sum-product sequence
    StochasticInferenceNode* theNode = sumProductSequence[nodeIndex];
    
    //    // test if this node is eliminated or summed-over
    //    if ( theNode->type == ELIMINATED ) {
    //        return theNode->calculateEliminatedLnProbability( false ); // + ( nodeIndex == sumProductSequence.size() - 1 ? 0.0 : calculateSummedLnProbability( nodeIndex + 1) );
    //    }
    //    else if ( theNode->type == INSTANTIATED ) {
    //        return theNode->getDistribution().lnPdf( theNode->getValue() ) + ( nodeIndex == sumProductSequence.size() - 1 ? 0.0 : calculateSummedLnProbability( nodeIndex + 1) );
    //    }
    //    
    //    if (theNode->needsProbabilityRecalculation || theNode->needsLikelihoodRecalculation || true) {
    //        // initialize the probability
    //        double sumProb = 0.0;
    //        
    //        // we need to iterate over my states
    //        DistributionDiscrete& d = static_cast<DistributionDiscrete&>( theNode->getDistribution() );
    //        
    //        // store the current value
    //        RbLanguageObject* tmp_value = theNode->value;
    //        
    //        // we ask for the state vector
    //        const std::vector<RbLanguageObject*>& states = d.getStateVector();
    //        
    //        // now we calculate the probabilities
    //        for (std::vector<RbLanguageObject*>::const_iterator state = states.begin(); state != states.end(); state++) {
    //            // I set my value so that my children can access if for recalculation
    //            RbLanguageObject* v = (*state);
    //            // we set the value here and do not call set value because we do not want that the memory of the old value gets freed
    //            theNode->value = v;
    //            theNode->touch();
    //            
    //            // \TODO: we need to stop somehow the recursion
    //            if ( nodeIndex < sumProductSequence.size() - 1 ) {
    //                double prob = calculateSummedLnProbability( nodeIndex + 1 );
    //                
    //                // add the probability for being in this state
    //                sumProb += d.pdf( *v ) * exp(prob);
    //            }
    //            else {
    //                
    //                // add the probability for being in this state
    //                sumProb += d.pdf( *v );
    //            }
    //            
    //        }
    //        
    //        // restore the current value
    //        theNode->value = tmp_value;
    //        
    //            
    //        // the ln prob is just the log of the sum of the single probs
    //        theNode->lnProb = log(sumProb);
    //    }
    //    
    ////    if ( lnProb == NAN || lnProb < -1000000) {
    ////        std::cerr << "Oh oh, didn't get a valid likelihood ..." << std::endl;
    ////    }
    //    
    //    theNode->needsProbabilityRecalculation = theNode->needsLikelihoodRecalculation = false;
    
    return theNode->lnProb;
}



/**
 * Construct the whole factor.
 *
 * We first construct the sum-product sequence, then set the factor root for all nodes in the sequences
 * and set the sequence to the factor root. This should enable calculation using the sum-product algorithm.
 */
void StochasticInferenceNode::constructFactor( void ) {
    
    // get all nodes of the factor
    std::set<VariableInferenceNode*> factor;
    std::vector<StochasticInferenceNode*> sequence;
    
    // construct the sum product sequence
    constructSumProductSequence(factor, sequence);
    
    // now we have to set the sequence to the factor root
    sequence[0]->setSumProductSequence( sequence );
    
    // next, we set the factor root for all nodes in the sequence
    for (std::vector<StochasticInferenceNode*>::iterator i = sequence.begin(); i != sequence.end(); ++i) {
        (*i)->setFactorRoot( sequence[0] );
    }
    
    // \TODO: We need a better algorithm to find the elimination sequence
    // now we try to find nodes which can eliminated
    for (std::vector<StochasticInferenceNode*>::iterator i = sequence.begin(); i != sequence.end(); ++i) {
        StochasticInferenceNode* currNode = *i;
        
        // first we check if this node is clamped (or instantiated), because then we cannot eliminate it
        if ( currNode->isNotInstantiated() ) {
            // test whether this node has at most one eliminated parent
            size_t eliminatedParentCount = 0;
            for (std::set<InferenceDagNode*>::const_iterator j = currNode->getParents().begin(); j != currNode->getParents().end(); ++j) {
                if ( (*j)->isNotInstantiated() ) {
                    eliminatedParentCount++;
                }
            }
            
            if ( eliminatedParentCount <= 1 ) {
                currNode->setSummationType( ELIMINATED );
            }
            else {
                currNode->setSummationType( SUMMED_OVER );
            }
        }
    }
}


/**
 * Construct the set of nodes which are not instantiated.
 *
 * I need to add all nodes which are either:
 * a) not instantiated, or
 * b) have not instantiated parents
 */
void StochasticInferenceNode::constructSumProductSequence( std::set<VariableInferenceNode *> &nodes, std::vector<StochasticInferenceNode*>& sequence ) {
    
    
    // if I was added already, then I'm done
    if ( nodes.find( this ) == nodes.end() ) {
        nodes.insert( this );
        
        // first the parents
        for (std::set<InferenceDagNode*>::iterator i = parents.begin(); i != parents.end(); i++) {
            if ( (*i)->isNotInstantiated() ) {
                static_cast<VariableInferenceNode*>( *i )->constructSumProductSequence(nodes,sequence);
            }
        }
        
        // insert the node now after all parents have been inserted
        sequence.push_back( this );
        
        // test whether I'm actually eliminated
        if ( isNotInstantiated() ) {
            // if so, add my children
            
            // then the children
            for (std::set<VariableInferenceNode*>::iterator i = children.begin(); i != children.end(); i++) {
                static_cast<VariableInferenceNode*>( *i )->constructSumProductSequence(nodes,sequence);
            }
        }
    }
    
}



/** Get affected nodes: insert this node and only stop recursion here if instantiated, otherwise (if integrated over) we pass on the recursion to our children */
void StochasticInferenceNode::getAffected( std::set<StochasticInferenceNode* >& affected ) {
    
    // if this node is integrated out, then we need to add the factor root, otherwise myself
    if (factorRoot == NULL) {
        affected.insert( this );
    }
    else {
        if ( affected.find( factorRoot )  == affected.end() )
            affected.insert( factorRoot );
    }
}


/** Get the ln probability ratio of this node */
double StochasticInferenceNode::getLnProbabilityRatio( void ) {
    
    if ( !probabilityRecalculated ) {
        
        return 0.0;
    }
    else {
        //        assert( !areDistributionParamsTouched() );
        double lnR = calculateLnProbability() - storedLnProb;
        
        return lnR;
    }
}



/**
 * Is this node eliminated.
 * We can check that here by looking if the node is instantiated.
 */
bool StochasticInferenceNode::isEliminated( void ) const {
    return type == ELIMINATED;
}


/**
 * Is this node eliminated.
 * We can check that here by looking if the node is instantiated.
 */
bool StochasticInferenceNode::isNotInstantiated( void ) const {
    return type != INSTANTIATED;
}


/**
 * Keep the current value of the node. 
 * At this point, we also need to make sure we update the stored ln probability.
 */
void StochasticInferenceNode::keepMe() {
    
    if ( touched ) {
        
        // @TODO: Call keep of moves and distribution
//        distribution->keep();
        
        storedLnProb = 1.0E6;       // An almost impossible value for the density
        if (needsProbabilityRecalculation || needsLikelihoodRecalculation) {
            lnProb = calculateLnProbability();
        }
        
        // Tell all my not instantiated parents that they need keeping.
        for (std::set<InferenceDagNode*>::iterator i = parents.begin(); i != parents.end(); ++i) {
            if ( (*i)->isNotInstantiated() ) {
                // since only variable nodes can be eliminated
                static_cast<VariableInferenceNode*>( *i )->keepMe();
            }
        }
        
    }
    
    touched = false;
    probabilityRecalculated         = false;
    needsLikelihoodRecalculation    = false;
    needsProbabilityRecalculation   = false;
    
}


/**
 * The likelihoods for this node need recalculations. This should only ever be called if this node is eliminated.
 * We set the flag for likelihood recalculation and also tell that our eliminated parents.
 */
void StochasticInferenceNode::likelihoodsNeedUpdates() {
    
    // We need to mark this node as dirty so that the probability and likelihood are recomputed
    if (!touched) {
        // Store the current lnProb 
        touched      = true;
        
        storedLnProb = lnProb;
        
        storedProbabilities         = probabilities;
        storedLikelihoods           = likelihoods;
        storedPartialLikelihoods    = partialLikelihoods;
        
        probabilityRecalculated     = true;
    }
    
    // if we are already flagged for likelihood recalculation, then we have already told our parents about this
    if (!needsLikelihoodRecalculation) {
        
        //  I need to tell all my eliminated parents that they need to update their likelihoods
        for (std::set<InferenceDagNode*>::iterator i = parents.begin(); i != parents.end(); i++) {
            if ( (*i)->isNotInstantiated() ) {
                // since only variable nodes can be eliminated
                static_cast<VariableInferenceNode*>( *i )->likelihoodsNeedUpdates();
            }
        }
    }
    
    needsLikelihoodRecalculation = true;
}


/** 
 * Mark this stochastic node for recalculation.
 * We set the flag for probability recalculation here.
 */
void StochasticInferenceNode::markForRecalculation(void) {
    needsProbabilityRecalculation = true;
}


/**
 * Does the probability or likelihood of this node needs updateing?
 * We return true if the likelihood or the probability are marked as dirty.
 */
bool StochasticInferenceNode::needsRecalculation() const {
    
    return needsLikelihoodRecalculation || needsProbabilityRecalculation;
}


///** Print struct for user */
//void StochasticInferenceNode::printStruct( std::ostream& o ) const {
//    
//    o << "_Class        = " << getClassTypeSpec() << std::endl;
//    o << "_Adress       = " << this << std::endl;
//    o << "_valueType    = " << value->getTypeSpec() << std::endl;
//    o << "_distribution = ";
//    distribution->printValue(o);
//    o << std::endl;
//    o << "_touched      = " << ( touched ? RbBoolean( true ) : RbBoolean( false ) ) << std::endl;
//    o << "_clamped      = " << ( clamped ? RbBoolean( true ) : RbBoolean( false ) ) << std::endl;
//    o << "_type         = ";
//    if ( type == INSTANTIATED ) {
//        o << "instantiated" << std::endl;
//        o << "_value        = ";
//        value->printValue(o);
//        o << std::endl;
//    }
//    else if ( type == ELIMINATED ) {
//        o << "eliminated" << std::endl;
//    }
//    else {
//        o << "summed over" << std::endl;
//    }
//    
//    // print the factor root if available
//    if ( factorRoot != NULL ) {
//        o << "_factor root  = " << factorRoot->getName() << std::endl;
//    }
//    else {
//        // only nodes which are not part of a eliminated subgraph have a probability
//        o << "_lnProb       = " << lnProb << std::endl;
//    }
//    // if we are the factor root than we can print the likelihood of the eliminated subgraph.
//    if ( factorRoot == this ) {
//        o << "_lnLikelihood = " << lnProb << std::endl;
//    }
//    if ( touched && storedValue != NULL) {
//        o << "_storedValue  = "; 
//        storedValue->printValue(o);
//        o << std::endl;
//    }
//    if ( touched )
//        o << "_storedLnProb = " << storedLnProb << std::endl;    
//    
//    o << "_parents      = ";
//    printParents(o);
//    o << std::endl;
//    
//    o << "_children     = ";
//    printChildren(o);
//    o << std::endl;
//    
//    o << std::endl;
//}


/** Restore the old value of the node and tell affected */
void StochasticInferenceNode::restoreMe() {
    
    if ( touched ) {
        
        lnProb          = storedLnProb;
        storedLnProb    = 1.0E6;    // An almost impossible value for the density
        
        if ( type != INSTANTIATED) {
            probabilities       = storedProbabilities;
            likelihoods         = storedLikelihoods;
            partialLikelihoods  = storedPartialLikelihoods;
        }
        
        //  I need to tell all my eliminated parents that they need to restore their likelihoods
        for (std::set<InferenceDagNode*>::iterator i = parents.begin(); i != parents.end(); ++i) {
            if ( (*i)->isNotInstantiated() ) {
                // since only variable nodes can be eliminated
                static_cast<VariableInferenceNode*>( *i )->restoreMe();
            }
        }
        
        // reset flags that recalculation is not needed
        needsProbabilityRecalculation = needsLikelihoodRecalculation = false;
    }
    
    touched = false;
    probabilityRecalculated     = false;
}


/*
 * Set whether the node needs to be instantiated or should be summed over.
 * During the MCMC run, an instantiated node has one specific value and hence needs moves attached to it.
 * If the node is summed over, no value will be associated with the node.
 * The node is in a sort of fuzzy state.
 */
void StochasticInferenceNode::setInstantiated(bool inst) {
    
    if (type != INSTANTIATED && inst) {
        // clear the probability and likelihood values
        probabilities.clear();
        likelihoods.clear();
        partialLikelihoods.clear();
        
        //        value = distribution->rv().clone();
        
        type = INSTANTIATED;
        
        // if we still have parent which are eliminated we keep the factor root
        if ( factorRoot == this ) {
            factorRoot = NULL;
        }
        
        // \TODO: I need to tell my children that I'm not eliminated anymore
        
        
        // flag for recalculation
        needsProbabilityRecalculation = true;
        
    }
    else if ( type == INSTANTIATED && !inst) {
        // set our new type
        type = SUMMED_OVER;
        
        // for now, we don't automatically recompute the sum-product sequence.
        // Instead, this needs to be called actively, as done by the MCMC class. (Sebastian)
        // (Therefore, we also don't need to store the likelihood vectors)
        
        //        // get the number of states
        //        DistributionDiscrete* discreteDist = static_cast<DistributionDiscrete*>( distribution );
        //        size_t nStates = discreteDist->getNumberOfStates();
        //        
        //        // resize the probability vector
        //        probabilities.resize( nStates );
        //        likelihoods.resize( nStates );
        //        for (size_t i = 0; i < nStates; i++) {
        //            partialLikelihoods.push_back( std::vector<double>(nStates) );
        //        }
        //        
        ////        delete value;
        ////        value = NULL;
        //        
        //        // recalculate the factor
        //        std::vector<StochasticInferenceNode*> sequence = constructSumProductSequence();
        //        
        //        // for all nodes in the sequence, set the new factor root
        //        for (std::vector<StochasticInferenceNode*>::iterator i = sequence.begin(); i != sequence.end(); i++) {
        //            (*i)->setFactorRoot( sequence[0] );
        //            (*i)->touch();
        //        }
        //        
        //        // set the sum-product sequence for the factor root
        //        sequence[0]->setSumProductSequence( sequence );
        //        
        //        // I need to tell my children that I'm eliminated
        //        for (std::set<VariableNode*>::iterator i = children.begin(); i != children.end(); i++) {
        //            (*i)->setFactorRoot( factorRoot );
        //        }
        
        // flag for recalculation
        needsProbabilityRecalculation = true;
        needsLikelihoodRecalculation = true;
    }
}


/** 
 * Set the summation type for this node.
 * We might need to initialize the likelihood and probability vectors.
 */
void StochasticInferenceNode::setSummationType(StochasticInferenceNode::VariableType t) {
    
    //    if ( type != ELIMINATED && t == ELIMINATED ) {
    //        // get the number of states
    //        DistributionDiscrete* discreteDist = static_cast<DistributionDiscrete*>( distribution );
    //        size_t nStates = discreteDist->getNumberOfStates();
    //        
    //        // resize the probability vector
    //        probabilities.resize( nStates );
    //        likelihoods.resize( nStates );
    //        for (size_t i = 0; i < nStates; i++) {
    //            partialLikelihoods.push_back( std::vector<double>(nStates) );
    //        }
    //    }
    //    
    //    type = t;
    
}


/** Setting the sum-product sequence */
void StochasticInferenceNode::setSumProductSequence(const std::vector<StochasticInferenceNode *> seq) {
    sumProductSequence = seq;
}


///**
// * Set value: same as clamp, but do not clamp. This function will
// * also be used by moves to propose a new value.
// */
//void StochasticInferenceNode::setValue( RbLanguageObject* val ) {
//    
//    if (val == NULL) {
//        std::cerr << "Ooops ..." << std::endl;
//    }
//    
//    if ( clamped )
//        throw RbException( "Cannot change value of clamped node" );
//    
//    // only if the node is instantiated (i.e. not summed out) setting the value should have an effect.
//    // summed out nodes might set the value for internal computations.
//    if ( type == INSTANTIATED ) {
//        // touch the node (which will store the lnProb)
//        touch();
//    }
//    
//    // delete the stored value
//    if (storedValue == NULL) {
//        storedValue = value;
//    }
//    else {
//        delete value;
//    }
//    
//    
//    // set the value
//    value = val;
//    if (value == NULL) {
//        std::cerr << "Ooops, setting value to NULL!\n";
//    }
//}


/** touch this node for recalculation */
void StochasticInferenceNode::touchMe( void ) {
    
    if (!touched) {
        // Store the current lnProb 
        touched      = true;
        
        storedLnProb = lnProb;
        
        probabilityRecalculated     = true;
        
        
        if ( isNotInstantiated() ) {
            
            storedProbabilities         = probabilities;
            storedLikelihoods           = likelihoods;
            storedPartialLikelihoods    = partialLikelihoods;
        }
    }
    
    //  I need to tell all my eliminated parents that they need to update their likelihoods
    for (std::set<InferenceDagNode*>::iterator i = parents.begin(); i != parents.end(); i++) {
        if ( (*i)->isNotInstantiated() ) {
            // since only variable nodes can be eliminated
            static_cast<VariableInferenceNode*>( *i )->likelihoodsNeedUpdates();
        }
    }
    
    needsProbabilityRecalculation = true;
}

/** Unclamp the value; use the clamped value as initial value */
void StochasticInferenceNode::unclamp( void ) {
    
    clamped = false;
}

