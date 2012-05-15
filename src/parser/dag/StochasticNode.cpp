/**
 * @file
 * This file contains the implementation of StochasticNode, which is derived
 * from VariableNode. StochasticNode is used for DAG nodes holding stochastic
 * variables with an associated distribution object.
 *
 * @brief Implementation of StochasticNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id$
 */

#include "ConstArgumentRule.h"
#include "ConstructorFunction.h"
#include "Container.h"
#include "DagNodeFunction.h"
#include "DeterministicNode.h"
#include "InferenceDagNode.h"
#include "Distribution.h"
#include "MethodTable.h"
#include "Model.h"
#include "ParserDistribution.h"
#include "Plate.h"
#include "RbBoolean.h"
#include "RbException.h"
#include "RbNullObject.h"
#include "RbUtil.h"
#include "RbValue.h"
#include "RbVector.h"
#include "StochasticNode.h"
#include "StochasticInferenceNode.h"
#include "UserInterface.h"
#include "Workspace.h"

#include <algorithm>
#include <cassert>
#include <cmath>


/** Constructor of empty StochasticNode */
StochasticNode::StochasticNode( void ) : VariableNode( NULL ), clamped( false ), distribution( NULL ), type( INSTANTIATED ), needsProbabilityRecalculation( true ), needsLikelihoodRecalculation( true ) {
}


/** Constructor of empty StochasticNode */
StochasticNode::StochasticNode( const RbPtr<const Plate> &p ) : VariableNode( p ), clamped( false ), distribution( NULL ), type( INSTANTIATED ), needsProbabilityRecalculation( true ), needsLikelihoodRecalculation( true ) {
}



/** Constructor from distribution */
StochasticNode::StochasticNode( const RbPtr<ParserDistribution> &dist, const RbPtr<const Plate> &p) : VariableNode( p ), clamped( false ), distribution( dist ), type( INSTANTIATED ), needsProbabilityRecalculation( true ), needsLikelihoodRecalculation( true ) {
    
    /* Get distribution parameters */
    const std::vector<RbPtr<Argument> >& params = dist->getParameters();
    
    /* Check for cycles */
    std::list<DAGNode*> done;
    for ( std::vector<RbPtr<Argument> >::const_iterator i = params.begin(); i != params.end(); i++ ) {
        done.clear();
        if ( (*i)->getVariable()->getDagNode()->isParentInDAG( this, done ) )
            throw RbException( "Invalid assignment: cycles in the DAG" );
    }
    
    /* Set parent(s) and add myself as a child to these */
    for ( std::vector<RbPtr<Argument> >::const_iterator i = params.begin(); i != params.end(); i++ ) {
        const RbPtr<DAGNode>& theParam = const_cast<DAGNode*>( (const DAGNode*)(*i)->getVariable()->getDagNode() );
        addParentNode( theParam );
        theParam->addChildNode(this);
    }
    
    // create a new random variable
    value = createRV();
    RbValue<void*> rvPrimitive = value.getLeanValue();
    dist->setValue( rvPrimitive );
    
    /* Get initial probability */
    lnProb = calculateLnProbability();
    
}


/** Copy constructor */
StochasticNode::StochasticNode( const StochasticNode& x ) : VariableNode( x ) {

    /* Set distribution */
    distribution = x.distribution->clone();

    /* Get distribution parameters */
    const std::vector<RbPtr<Argument> >& params = distribution->getParameters();

    /* Set parent(s) and add myself as a child to these */
    for ( std::vector<RbPtr<Argument> >::const_iterator i = params.begin(); i != params.end(); i++ ) {
        const RbPtr<DAGNode>& theParam = const_cast<DAGNode*>( (const DAGNode*)(*i)->getVariable()->getDagNode() );
        addParentNode( theParam );
        theParam->addChildNode(this);
    }

    clamped                         = x.clamped;
    type                            = x.type;
    needsProbabilityRecalculation   = x.needsProbabilityRecalculation;
    needsLikelihoodRecalculation    = x.needsLikelihoodRecalculation;
    touched                         = x.touched;
    
    RlValue<RbObject> tmp = x.value.clone();
    
    std::vector<RbPtr<RbLanguageObject> > vals;
    for (std::vector<RbPtr<RbObject> >::iterator i = tmp.value.begin(); i != tmp.value.end(); ++i) {
        vals.push_back( RbPtr<RbLanguageObject>( static_cast<RbLanguageObject *>( (RbObject *) *i ) ) );
    }
    
    value = RlValue<RbLanguageObject>(vals, tmp.lengths);
    
    lnProb                          = x.lnProb;
    storedLnProb                    = x.storedLnProb;
}



/** Assignment operator */
StochasticNode& StochasticNode::operator=( const StochasticNode& x ) {

    if ( this != &x ) {
        
        /* Remove parents first */
        for ( std::set<RbPtr<DAGNode> >::iterator i = parents.begin(); i != parents.end(); i++ ) {
            const RbPtr<DAGNode>& node = *i;
            node->removeChildNode( this );
        }
        parents.clear();

        /* Set distribution */
        distribution = x.distribution->clone();
        
        /* Get distribution parameters */
        const std::vector<RbPtr<Argument> >& params = distribution->getParameters();
        
        /* Set parent(s) and add myself as a child to these */
        for ( std::vector<RbPtr<Argument> >::const_iterator i = params.begin(); i != params.end(); i++ ) {
            const RbPtr<DAGNode>& theParam = const_cast<DAGNode*>( (const DAGNode*)(*i)->getVariable()->getDagNode() );
            addParentNode( theParam );
            theParam->addChildNode(this);
        }

        clamped                         = x.clamped;
        type                            = x.type;
        needsProbabilityRecalculation   = x.needsProbabilityRecalculation;
        needsLikelihoodRecalculation    = x.needsLikelihoodRecalculation;
        
        factorRoot                      = x.factorRoot;
        
        RlValue<RbObject> tmp = x.value.clone();
        
        std::vector<RbPtr<RbLanguageObject> > vals;
        for (std::vector<RbPtr<RbObject> >::iterator i = tmp.value.begin(); i != tmp.value.end(); ++i) {
            vals.push_back( RbPtr<RbLanguageObject>( static_cast<RbLanguageObject *>( (RbObject *) *i ) ) );
        }
        
        value = RlValue<RbLanguageObject>(vals, tmp.lengths);

        touched                         = x.touched;
        lnProb                          = x.lnProb;
        storedLnProb                    = x.storedLnProb;
        
        // set the name
        name = x.name;
    }

    return ( *this );
}


/** Get the conditional ln probability of the node; do not rely on stored values */
double StochasticNode::calculateLnProbability( void ) {
    
    if (factorRoot != NULL) {
        lnProb =  factorRoot->calculateSummedLnProbability(0);
    }
    else if (needsProbabilityRecalculation) {
        // \TODO: Hack! We should make sure that the likelihood is properly calculated. (Sebastian)
        // I switched this test of, because we now instantiate the model from the RevLanguage
        // but do not actually sum over all possible values. This is only done in the MCMC.
        // Trying to safe some time ...
//        if (type == INSTANTIATED) { // this should always be true
            lnProb = distribution->jointLnPdf( value );
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
double StochasticNode::calculateEliminatedLnProbability( bool enforceProbabilityCalculation ) {
    
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
double StochasticNode::calculateSummedLnProbability(size_t nodeIndex) {
    // get the node we are talking about from the sum-product sequence
    StochasticNode* theNode = sumProductSequence[nodeIndex];
    
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




/** Clamp the node to an observed value */
void StochasticNode::clamp( const RlValue<RbLanguageObject> &observedVal ) {

//    if ( touched )
//        throw RbException( "Cannot clamp stochastic node in volatile state" );

    // touch for recalculation
    touch(); 
    
    value = observedVal;

    clamped = true;
    lnProb  = calculateLnProbability();

}


/** Clone this object */
StochasticNode* StochasticNode::clone( void ) const {

    return new StochasticNode( *this );
}


/** Clone the entire graph: clone children, swap parents */
DAGNode* StochasticNode::cloneDAG( std::map<const DAGNode*, RbPtr<DAGNode> >& newNodes ) const {

    if ( newNodes.find( this ) != newNodes.end() )
        return ( newNodes[ this ] );

    /* Get pristine copy */
    StochasticNode* copy = new StochasticNode( );
    newNodes[ this ] = copy;
    
    /* Set the name so that the new node remains identifiable */
    copy->setName(name);

    /* Set the copy member variables */
    copy->distribution  = distribution->clone();
    copy->clamped       = clamped;
    copy->touched       = touched;
    
    RlValue<RbObject> tmp = value.clone();
    
    std::vector<RbPtr<RbLanguageObject> > vals;
    for (std::vector<RbPtr<RbObject> >::iterator i = tmp.value.begin(); i != tmp.value.end(); ++i) {
        vals.push_back( RbPtr<RbLanguageObject>( static_cast<RbLanguageObject *>( (RbObject *) *i ) ) );
    }
    
    copy->value = RlValue<RbLanguageObject>(vals, tmp.lengths);

    
    // set also the type of this node (if it is eliminated or not)
    copy->type                          = type;
    copy->lnProb                        = lnProb;
    copy->storedLnProb                  = storedLnProb;
    copy->needsProbabilityRecalculation = needsProbabilityRecalculation;
    copy->needsLikelihoodRecalculation  = needsLikelihoodRecalculation;

    /* Set the copy params to their matches in the new DAG */
    const std::vector<RbPtr<Argument> >& params     = distribution->getParameters();
    
    // first we need to remove the copied params
    copy->distribution->clear();
    
    for ( std::vector<RbPtr<Argument> >::const_iterator i = params.begin(); i != params.end(); i++ ) {
        
        // clone the i-th member and get the clone back
        const RbPtr<const DAGNode> &theParam = (*i)->getVariable()->getDagNode();
        // if we already have cloned this parent (parameter), then we will get the previously created clone
        DAGNode* theParamClone = theParam->cloneDAG( newNodes );
        
        // set the clone of the member as the member of the clone
        // \TODO: We should check that this does destroy the dependencies of the parameters.
        // Instead of creating a new variable we might need to get the pointer to the variable from somewhere.
        copy->distribution->setMember("", new Variable( theParamClone) );

        copy->addParentNode( theParamClone );
        theParamClone->addChildNode( copy );
    }

    /* Make sure the children clone themselves */
    for( std::set<VariableNode*>::const_iterator i = children.begin(); i != children.end(); i++ ) {
        VariableNode* theNewNode = *i;
        // do not add myself into the list of nodes
        if ( theNewNode->isTypeSpec( DeterministicNode::getClassTypeSpec() ) ) {
            DeterministicNode* theDetNode = dynamic_cast<DeterministicNode*>(theNewNode);
            const RbFunction& theFunction = theDetNode->getFunction();
            if (theFunction.isTypeSpec(ConstructorFunction::getClassTypeSpec())) {
                const ConstructorFunction& theConstructorFunction = dynamic_cast<const ConstructorFunction&>( theFunction );
                if ( theConstructorFunction.getReturnType() == Model::getClassTypeSpec() ) {
                    continue;
                }
            }
        }
        (*i)->cloneDAG( newNodes );
    }

    return copy;
}


/**
 * Construct the whole factor.
 *
 * We first construct the sum-product sequence, then set the factor root for all nodes in the sequences
 * and set the sequence to the factor root. This should enable calculation using the sum-product algorithm.
 */
void StochasticNode::constructFactor( void ) {
    
    // get all nodes of the factor
    std::set<VariableNode*> factor;
    std::vector<StochasticNode*> sequence;
    
    // construct the sum product sequence
    constructSumProductSequence(factor, sequence);
    
    // now we have to set the sequence to the factor root
    sequence[0]->setSumProductSequence( sequence );
    
    // next, we set the factor root for all nodes in the sequence
    for (std::vector<StochasticNode*>::iterator i = sequence.begin(); i != sequence.end(); ++i) {
        (*i)->setFactorRoot( sequence[0] );
    }
    
    // \TODO: We need a better algorithm to find the elimination sequence
    // now we try to find nodes which can eliminated
    for (std::vector<StochasticNode*>::iterator i = sequence.begin(); i != sequence.end(); ++i) {
        StochasticNode* currNode = *i;
        
        // first we check if this node is clamped (or instantiated), because then we cannot eliminate it
        if ( currNode->isNotInstantiated() ) {
            // test whether this node has at most one eliminated parent
            size_t eliminatedParentCount = 0;
            for (std::set<RbPtr<DAGNode> >::const_iterator j = currNode->getParents().begin(); j != currNode->getParents().end(); ++j) {
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
void StochasticNode::constructSumProductSequence( std::set<VariableNode *> &nodes, std::vector<StochasticNode*>& sequence ) {
    
    
    // if I was added already, then I'm done
    if ( nodes.find( this ) == nodes.end() ) {
        nodes.insert( this );
        
        // first the parents
        for (std::set<RbPtr<DAGNode> >::iterator i = parents.begin(); i != parents.end(); i++) {
            if ( (*i)->isNotInstantiated() ) {
                static_cast<VariableNode*>( (DAGNode*) *i )->constructSumProductSequence(nodes,sequence);
            }
        }
        
        // insert the node now after all parents have been inserted
        sequence.push_back( this );
        
        // test whether I'm actually eliminated
        if ( isNotInstantiated() ) {
            // if so, add my children
            
            // then the children
            for (std::set<VariableNode*>::iterator i = children.begin(); i != children.end(); i++) {
                static_cast<VariableNode*>( *i )->constructSumProductSequence(nodes,sequence);
            }
        }
    }
    
}

/**
 * A deterministic node create a lean constant DAG node and sets the value. 
 * Additionally we call the create lean DAG for all children and all parents.
 * If we already have a lean copy of this deterministic node, we will just return that copy
 * and not call our children and parents.
 */
InferenceDagNode* StochasticNode::createLeanDag(std::map<const DAGNode *, InferenceDagNode *> &newNodes) const {
    
    if ( newNodes.find( this ) != newNodes.end() )
        return ( newNodes[ this ] );
    
    // create a copy of the inference function
    Distribution* leanDistribution = distribution->getLeanDistribution()->clone();
    
    // make a copy of the current value
    RbValue<void*> leanValue = value.getLeanValue();
    
    /* Create a lean DAG node */
    StochasticInferenceNode* copy = new StochasticInferenceNode(leanValue, leanDistribution, name);
    newNodes[ this ] = copy;
    
    /* Set the copy params to their matches in the new DAG */
    const std::vector<RbPtr<Argument> >& params     = distribution->getParameters();
    
    std::vector<RbValue<void*> > leanArgs;
    for ( std::vector<RbPtr<Argument> >::const_iterator i = params.begin(); i != params.end(); i++ ) {
        
        // clone the i-th member and get the clone back
        const RbPtr<const DAGNode>& theParam = (*i)->getVariable()->getDagNode();
        
        // if we already have cloned this parent (parameter), then we will get the previously created clone
        InferenceDagNode* theParamClone = theParam->createLeanDag( newNodes );
        leanArgs.push_back( theParamClone->getValue() );
        
        copy->addParentNode( theParamClone );
        theParamClone->addChildNode( copy );
    }
    
    // set the value of the function to the args
    leanArgs.push_back( leanValue );
    
    // link lean args with inference function
    leanDistribution->setParameters( leanArgs );
    
    /* Make sure the children create a lean copy of themselves too */
    for( std::set<VariableNode* >::const_iterator i = children.begin(); i != children.end(); i++ ) {
        (*i)->createLeanDag( newNodes );
    }
    
    return copy;
}


/* 
 * Create a new rv for the node. 
 *
 */
RlValue<RbLanguageObject> StochasticNode::createRV( void ) {
    
    if ( plate == NULL ) {
        
        const std::vector<RbPtr<Argument> > &params = distribution->getParameters();
    
        std::vector<size_t> offsets;
        std::vector<RlValue<const RbLanguageObject> > newArgs;
        for ( std::vector<RbPtr<Argument> >::const_iterator i = params.begin(); i != params.end(); i++ ) {
            newArgs.push_back( (*i)->getVariable()->getValue() );
            offsets.push_back( 0 );
        }
        return createRV( std::vector<size_t>(), 0, offsets, newArgs );
    }
    else {
        // we need to get the arguments for checking if they live on the same plate or any parent plate of this
        const std::vector<RbPtr<Argument> > &params = distribution->getParameters();
        
        // get the plates of my arguments
        std::vector<const Plate*> argPlates;
        for ( std::vector<RbPtr<Argument> >::const_iterator i = params.begin(); i != params.end(); i++ ) {
        // test whether this argument lives on the same plate as myself
            argPlates.push_back( (*i)->getVariable()->getDagNode()->getPlate() );
        }
        
        // convert the argument into RbObjects
        std::vector<size_t> offsets;
        std::vector<RlValue<const RbLanguageObject> > newArgs;
        for ( std::vector<RbPtr<Argument> >::const_iterator i = params.begin(); i != params.end(); i++ ) {
            newArgs.push_back( (*i)->getVariable()->getValue() );
            offsets.push_back( 0 );
        }
        
        // we create a vector of lengths telling us the length of each plate
        // we only consider plate on which none of the arguments lives
        std::vector<size_t> plateLengths;
        const Plate *p = plate;
        while (p != NULL) {
            for ( std::vector<const Plate*>::const_iterator i = argPlates.begin(); i != argPlates.end(); i++ ) {
                // test whether this argument lives on the same plate as myself
                if ( p == *i ) {
                    p = NULL;
                    break;
                }
            }
            if ( p != NULL ) {
                plateLengths.insert( plateLengths.begin(), p->getLength() );
                p = p->getParentPlate();
            }
        }
        return createRV( plateLengths, 0, offsets, newArgs );
    }
}



/* 
 * Execute the Function. 
 *
 */
RlValue<RbLanguageObject> StochasticNode::createRV( const std::vector<size_t> &plateLengths, size_t level, const std::vector<size_t> &offsets, const std::vector<RlValue<const RbLanguageObject> > &args ) {
    
    // check each argument if it is a vector and hence the function needs repeated evaluation
    bool repeatedExecution = false;
    size_t size = 0;
    std::vector<size_t> new_offsets;
    for (size_t i = 0; i != args.size(); ++i) {
        if ( args[i].lengths.size() > level ) {
            // security check
            if ( repeatedExecution && size != args[i].lengths[level] ) {
                throw RbException("Received arguments with different lengths in the same dimension!");
            }
            
            repeatedExecution = true;
            size = args[i].lengths[level];
            new_offsets.push_back( offsets[i] * size);
        }
        else {
            new_offsets.push_back( offsets[i] );
        }
        
    }
    
    RlValue<RbLanguageObject> retVal;
    if ( repeatedExecution ) {
        for ( size_t j = 0; j < size; ++j) {
            
            // call the execute function now for the single elements
            const RlValue<RbLanguageObject>& singleRetVal = createRV(plateLengths, level+1, new_offsets, args);
            
            // push back all single elements to the retVal
            for (std::vector<RbPtr<RbLanguageObject> >::const_iterator i = singleRetVal.value.begin(); i != singleRetVal.value.end(); ++i) {
                retVal.value.push_back( *i );
            }
            
            // set the dimension correctly
            if ( j == 0) {
                for (std::vector<size_t>::const_iterator i = singleRetVal.lengths.begin(); i != singleRetVal.lengths.end(); ++i) {
                    retVal.lengths.push_back( *i );
                }
            }
            
            // update the offsets
            for (size_t i = 0; i != args.size(); ++i) {
                if ( args[i].lengths.size() > level ) {
                    new_offsets[i]++;
                }
            }
        }
    }
    else {
        std::vector<const RbObject *> newArgs;
        
        for (size_t i = 0; i != args.size(); ++i) {
            newArgs.push_back( args[i].value[offsets[i]] );
        }
        
        // get the value by executing the internal function
        retVal = createRVSingleValue(0, plateLengths, newArgs);
    }
    
    return retVal;
    
}


RlValue<RbLanguageObject> StochasticNode::createRVSingleValue(size_t plateIndex, const std::vector<size_t> &plateLengths, const std::vector<const RbObject *> &args) {
    
    if ( plateLengths.size() == plateIndex ) {
        // converting the arguments into atomic data types
        std::vector<RbValue<void*> > newArgs;
        for ( std::vector<const RbObject*>::const_iterator i = args.begin(); i != args.end(); i++ ) {
            RbValue<void*> arg;
            arg.value = (*i)->getLeanValue(arg.lengths);
            newArgs.push_back( arg );
        }
    
        // add te return value
        RbLanguageObject* retVal = distribution->getTemplateRandomVariable().clone();
        RbValue<void*> arg;
        arg.value = retVal->getLeanValue(arg.lengths);
        newArgs.push_back( arg );
    
        // Setting the parameter of the distribution
        distribution->setParameters( newArgs );
    
        distribution->rv();
    
        return RlValue<RbLanguageObject>( RbPtr<RbLanguageObject>( retVal ) );
    }
    else {
        // create a new vector for the rv's
        RlValue<RbLanguageObject> retVal;
        
        // iterate over all indices of the current plate
        for (size_t i = 0; i < plateLengths[plateIndex]; ++i) {
            RlValue<RbLanguageObject> tmp = createRVSingleValue( plateIndex+1, plateLengths, args);
            
            // push back all single elements to the retVal
            for (std::vector<RbPtr<RbLanguageObject> >::iterator j = tmp.value.begin(); j != tmp.value.end(); ++j) {
                retVal.value.push_back( *j );
            }
            
            // set the dimension correctly
            if ( i == 0) {
                for (std::vector<size_t>::iterator j = tmp.lengths.begin(); j != tmp.lengths.end(); ++j) {
                    retVal.lengths.push_back( *j );
                }
            }
        }
        
        return retVal;
    }
}
 

/** Complete info about object */
std::string StochasticNode::debugInfo(void) const {
    
    std::ostringstream o;
    o << "StochasticNode:" << std::endl;
    o << "Clamped      = " << ( clamped ? "true" : "false" ) << std::endl;
    o << "Touched      = " << ( touched ? "true" : "false" ) << std::endl;
    o << "Distribution = ";
    distribution->printValue( o );
    o << std::endl;
    o << "Value        = ";
    value.printValue( o );
    o << std::endl;
    
    return o.str();
}


/**
 * Map calls to member methods 
 */
RbPtr<RbLanguageObject> StochasticNode::executeMethod(std::string const &name, const std::vector<RlValue<const RbObject> > &args) {
    
    if (name == "clamp") {
        
        // get the observed value
        const RlValue<const RbObject>& observedValue = args[0];

        RlValue<RbObject> tmp = observedValue.clone();
        
        std::vector<RbPtr<RbLanguageObject> > vals;
        for (std::vector<RbPtr<RbObject> >::iterator i = tmp.value.begin(); i != tmp.value.end(); ++i) {
            vals.push_back( RbPtr<RbLanguageObject>( static_cast<RbLanguageObject *>( (RbObject *) *i ) ) );
        }
        
        RlValue<RbLanguageObject> clampValue = RlValue<RbLanguageObject>(vals, tmp.lengths);
        
        // clamp the observed value to myself
        clamp( clampValue );
        
        // we keep the new value
        keep();
        
        return NULL;
    } 
    
    return DAGNode::executeMethod( name, args );
}


void StochasticNode::expand( void ) {
    
    // draw a new value
    value = createRV();
}


/** Get affected nodes: insert this node and only stop recursion here if instantiated, otherwise (if integrated over) we pass on the recursion to our children */
void StochasticNode::getAffected( std::set<RbPtr<StochasticNode> >& affected ) {
    
    // if this node is integrated out, then we need to add the factor root, otherwise myself
    if (factorRoot == NULL) {
        affected.insert( this );
    }
    else {
        if ( affected.find( factorRoot )  == affected.end() )
            affected.insert( factorRoot );
    }
}


/** Get class name of object */
const std::string& StochasticNode::getClassName(void) { 
    
    static std::string rbClassName = "Stochastic DAG node";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& StochasticNode::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( VariableNode::getClassTypeSpec() ) );
    
	return rbClass; 
}


const ParserDistribution& StochasticNode::getDistribution(void) const {
    return *distribution;
}


ParserDistribution& StochasticNode::getDistribution(void) {
    return *distribution;
}


/** Get the ln probability ratio of this node */
double StochasticNode::getLnProbabilityRatio( void ) {
    
    if ( !probabilityRecalculated ) {

        return 0.0;
    }
    else {
        double lnR = calculateLnProbability() - storedLnProb;
        
        return lnR;
    }
}



/* Get method specifications */
const MethodTable& StochasticNode::getMethods(void) const {
    
    static MethodTable methods = MethodTable();
    static bool          methodsSet = false;
    
    if ( methodsSet == false ) 
    {
        
        // method "clamp"
        ArgumentRules* clampArgRules = new ArgumentRules();
        clampArgRules->push_back( new ConstArgumentRule("x", RbLanguageObject::getClassTypeSpec() ) );
        methods.addFunction("clamp", new DagNodeFunction( RbVoid_name, clampArgRules) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &DAGNode::getMethods() );
        methodsSet = true;
    }
    
    return methods;
}

/** Get type spec */
const TypeSpec& StochasticNode::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/* Get const value; we always know our value. */
const RlValue<const RbLanguageObject>& StochasticNode::getValue( void ) const {

    std::vector<RbPtr<const RbLanguageObject> > tmp;
    for (std::vector<RbPtr<RbLanguageObject> >::const_iterator i = value.value.begin(); i != value.value.end(); ++i) {
        tmp.push_back( RbPtr<const RbLanguageObject>() );
    }
    
    return RlValue<const RbLanguageObject>( tmp, value.lengths );
}

/* Get non-const value; we always know our value. */
const RlValue<RbLanguageObject>& StochasticNode::getValue( void ) {
    
    return value;
}


///** Get value pointer; we always know our value. */
//const RbLanguageObject* StochasticNode::getValuePtr( void ) const {
//    
//    return value;
//}


/**
* Is this node eliminated.
* We can check that here by looking if the node is instantiated.
*/
bool StochasticNode::isEliminated( void ) const {
    return type == ELIMINATED;
}


/**
 * Is this node eliminated.
 * We can check that here by looking if the node is instantiated.
 */
bool StochasticNode::isNotInstantiated( void ) const {
    return type != INSTANTIATED;
}



/**
 * Keep the current value of the node. 
 * At this point, we also need to make sure we update the stored ln probability.
 */
void StochasticNode::keepMe() {

    if ( touched ) {
        
        storedLnProb = 1.0E6;       // An almost impossible value for the density
        if (needsProbabilityRecalculation || needsLikelihoodRecalculation) {
            lnProb = calculateLnProbability();
        }
        
        // Tell all my not instantiated parents that they need keeping.
        for (std::set<RbPtr<DAGNode> >::iterator i = parents.begin(); i != parents.end(); ++i) {
            if ( (*i)->isNotInstantiated() ) {
                // since only variable nodes can be eliminated
                static_cast<VariableNode*>( (DAGNode*) *i )->keepMe();
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
void StochasticNode::likelihoodsNeedUpdates() {
    
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
        for (std::set<RbPtr<DAGNode> >::iterator i = parents.begin(); i != parents.end(); i++) {
            if ( (*i)->isNotInstantiated() ) {
                // since only variable nodes can be eliminated
                static_cast<VariableNode*>( (DAGNode*) *i )->likelihoodsNeedUpdates();
            }
        }
    }
    
    needsLikelihoodRecalculation = true;
}


/** 
 * Mark this stochastic node for recalculation.
 * We set the flag for probability recalculation here.
 */
void StochasticNode::markForRecalculation(void) {
    needsProbabilityRecalculation = true;
}


/**
 * Does the probability or likelihood of this node needs updateing?
 * We return true if the likelihood or the probability are marked as dirty.
 */
bool StochasticNode::needsRecalculation() const {
    
    return needsLikelihoodRecalculation || needsProbabilityRecalculation;
}


/** Print struct for user */
void StochasticNode::printStruct( std::ostream& o ) const {

    o << "_Class        = " << getClassTypeSpec() << std::endl;
    o << "_Adress       = " << this << std::endl;
    o << "_valueType    = " << value.getTypeSpec() << std::endl;
    o << "_distribution = ";
    distribution->printValue(o);
    o << std::endl;
    o << "_touched      = " << ( touched ? RbBoolean( true ) : RbBoolean( false ) ) << std::endl;
    o << "_clamped      = " << ( clamped ? RbBoolean( true ) : RbBoolean( false ) ) << std::endl;
    o << "_type         = ";
    if ( type == INSTANTIATED ) {
        o << "instantiated" << std::endl;
        o << "_value        = ";
        value.printValue(o);
        o << std::endl;
    }
    else if ( type == ELIMINATED ) {
        o << "eliminated" << std::endl;
    }
    else {
        o << "summed over" << std::endl;
    }
    
    // print the factor root if available
    if ( factorRoot != NULL ) {
        o << "_factor root  = " << factorRoot->getName() << std::endl;
    }
    else {
        // only nodes which are not part of a eliminated subgraph have a probability
        o << "_lnProb       = " << lnProb << std::endl;
    }
    // if we are the factor root than we can print the likelihood of the eliminated subgraph.
    if ( factorRoot == this ) {
        o << "_lnLikelihood = " << lnProb << std::endl;
    }
    if ( touched )
        o << "_storedLnProb = " << storedLnProb << std::endl;    

    o << "_parents      = ";
    printParents(o);
    o << std::endl;

    o << "_children     = ";
    printChildren(o);
    o << std::endl;

    o << std::endl;
}


/** Print value for user */
void StochasticNode::printValue( std::ostream& o ) const {

    if ( touched )
        RBOUT( "Warning: Variable in touched state" );

    value.printValue(o);
}


/** Restore the old value of the node and tell affected */
void StochasticNode::restoreMe() {

    if ( touched ) {
        
//        value = distribution->restore();
        
        lnProb          = storedLnProb;
        storedLnProb    = 1.0E6;    // An almost impossible value for the density
        
        if ( type != INSTANTIATED) {
            probabilities       = storedProbabilities;
            likelihoods         = storedLikelihoods;
            partialLikelihoods  = storedPartialLikelihoods;
        }
        
        //  I need to tell all my eliminated parents that they need to restore their likelihoods
        for (std::set<RbPtr<DAGNode> >::iterator i = parents.begin(); i != parents.end(); ++i) {
            if ( (*i)->isNotInstantiated() ) {
                // since only variable nodes can be eliminated
                static_cast<VariableNode*>( (DAGNode*) *i )->restoreMe();
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
void StochasticNode::setInstantiated(bool inst) {
    
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
//        std::vector<StochasticNode*> sequence = constructSumProductSequence();
//        
//        // for all nodes in the sequence, set the new factor root
//        for (std::vector<StochasticNode*>::iterator i = sequence.begin(); i != sequence.end(); i++) {
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
void StochasticNode::setSummationType(StochasticNode::VariableType t) {
    
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
void StochasticNode::setSumProductSequence(const std::vector<StochasticNode *> seq) {
    sumProductSequence = seq;
}


/**
 * Set value: same as clamp, but do not clamp. This function will
 * also be used by moves to propose a new value.
 */
void StochasticNode::setValue( const RlValue<RbLanguageObject> &val ) {
    
    if ( clamped )
        throw RbException( "Cannot change value of clamped node" );

    // only if the node is instantiated (i.e. not summed out) setting the value should have an effect.
    // summed out nodes might set the value for internal computations.
    if ( type == INSTANTIATED ) {
        // touch the node (which will store the lnProb)
        touch();
    }

    // set the value
    value = val;
}


/**
 * Swap parent node. Note that this function will be called by the tree plate
 * when the topology is changed. It is also used by the parser.
 * This function should be called from the mutateTo function, otherwise it
 * is dangerous because the distribution parameters will not be accommodated.
 */
void StochasticNode::swapParentNode(DAGNode* oldNode, DAGNode* newNode ) {

    if ( parents.find( oldNode ) == parents.end() )
        throw RbException( "Node is not a parent" );

    oldNode->removeChildNode( this );
    newNode->addChildNode   ( this );
    removeParentNode( oldNode );
    addParentNode( newNode );

    if ( clamped == false ) {

        touchMe();
        
        // We keep the current value; delegate moves can later change the value
        // if they are interested.
    }

    //for ( std::set<VariableNode*>::iterator i = children.begin(); i != children.end(); i++ )
    //    (*i)->touch();
}


/** touch this node for recalculation */
void StochasticNode::touchMe( void ) {
    
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
    for (std::set<RbPtr<DAGNode> >::iterator i = parents.begin(); i != parents.end(); i++) {
        if ( (*i)->isNotInstantiated() ) {
            // since only variable nodes can be eliminated
            static_cast<VariableNode*>( (DAGNode*) *i )->likelihoodsNeedUpdates();
        }
    }
    
    needsProbabilityRecalculation = true;
}

/** Unclamp the value; use the clamped value as initial value */
void StochasticNode::unclamp( void ) {

    clamped = false;
}

