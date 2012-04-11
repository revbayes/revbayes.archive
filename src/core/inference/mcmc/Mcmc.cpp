/**
 * @file
 * This file contains the implementation of Mcmc, which is used to hold
 * information about and run an mcmc analysis.
 *
 * @brief Implementation of Mcmc
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 * @interface Mcmc
 * @package distributions
 *
 * $Id$
 */

#include "ConstantNode.h"
#include "Integer.h"
#include "Mcmc.h"
#include "MemberFunction.h"
#include "Model.h"
#include "FileMonitor.h"
#include "Move.h"
#include "Natural.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbNullObject.h"
#include "RangeRule.h"
#include "RbUtil.h"
#include "RbString.h"
#include "RbVector.h"
#include "StochasticNode.h"
#include "ValueRule.h"
#include "VariableNode.h"
#include "Workspace.h"

#include <cmath>
#include <fstream>
#include <sstream>
#include <string>



/** Constructor passes member rules and method inits to base class */
Mcmc::Mcmc(void) : MemberObject(getMemberRules()), model( NULL ) {
}

/** Copy constructor */
Mcmc::Mcmc(const Mcmc &x) : MemberObject(x), model( x.model ) { //, dagNodes( x.dagNodes ), moves( x.moves ),  monitors( x.monitors ) {
    
    if ( model != NULL ) {
        extractDagNodesFromModel( static_cast<const Model&>( model->getValue() ) );
    }
}


/** Clone object */
Mcmc* Mcmc::clone(void) const {

    return new Mcmc(*this);
}


/** Map calls to member methods */
const RbLanguageObject& Mcmc::executeOperationSimple(const std::string& name, const std::vector<Argument>& args) {

    if (name == "run") {
        const RbLanguageObject& argument = args[0].getVariable().getValue();
        int n = static_cast<const Natural&>( argument ).getValue();
        run(n);
        return RbNullObject::getInstance();
    }
    else if ( name == "getMonitors" ) {
        return monitors;
    }

    return MemberObject::executeOperationSimple( name, args );
}


/**
 * Clone the model and extract the DAG nodes, moves and monitors.
 *
 * First we clone the entire DAG, then we clone the moves and the monitors and set the new nodes appropriately.
 */
void Mcmc::extractDagNodesFromModel(const Model& source) {
    
    const std::set<const DAGNode*> sourceNodes = source.getSourceNodes();
    
    // if the var is not NULL we pull the DAG from it
    std::map<const DAGNode*, RbDagNodePtr> nodesMap;
    
    for (std::set<const DAGNode*>::const_iterator it = sourceNodes.begin(); it != sourceNodes.end(); it++) {
        const DAGNode* theSourceNode = *it;
        
        if (theSourceNode == NULL)
            throw RbException("Cannot instantiate a model with a NULL DAG node.");
        
        theSourceNode->cloneDAG(nodesMap);
    }

    
    /* insert new nodes in dagNodes member frame and direct access vector */
    std::map<const DAGNode*, RbDagNodePtr>::iterator i = nodesMap.begin();
    
    while ( i != nodesMap.end() ) {
        
        DAGNode* theNewNode = (*i).second;
        
        // increment the iterator;
        ++i;
            
        // insert in direct access vector
        dagNodes.push_back( theNewNode );
        
    }
        
    /////////////////////////
    // now we clone the moves
    
    // we first empty our current vector of moves
    moves.clear();
    
    // first we create a map from the old moves to the new ones
    std::map<Move*, Move*> moveMap;
    
    // next, we iterate over all dag nodes to collect the move
    for (std::map<const DAGNode*, RbDagNodePtr>::iterator i = nodesMap.begin(); i != nodesMap.end(); ++i) {
        // get all moves for this node
        const DAGNode* orgNode = i->first;
        if ( orgNode->isTypeSpec( StochasticNode::getClassTypeSpec() ) ) {
            const StochasticNode* orgStochNode = static_cast<const StochasticNode*>( orgNode );
            const std::vector<Move*>& orgMoves = orgStochNode->getMoves();
            
            for (std::vector<Move*>::const_iterator j = orgMoves.begin(); j != orgMoves.end(); ++j) {
                // check if we already have this move in our list
                std::map<Move*,Move*>::iterator k = moveMap.find( *j );
                
                Move* clonedMove = NULL;
                // create a new move if necessary
                if ( k == moveMap.end() ) {
                    clonedMove = (*j)->clone();
                    
                    // add the new move to the moves map
                    moveMap.insert( std::pair<Move*, Move*>(*j, clonedMove) );
                    
                    // add the new move to our moves vector
                    moves.push_back( clonedMove );
                    
                }
                else {
                    clonedMove = k->second;
                }
                
                // add the clone DAG node to the cloned move
                clonedMove->addDagNode( static_cast<StochasticNode*>( (DAGNode*)i->second ) );
                
                // add the cloned move to the cloned DAG node
                static_cast<StochasticNode*>( (DAGNode*)i->second )->addMove( clonedMove );
            }
        }
    }
    
    ////////////////////////////
    // now we clone the monitors
    
    // we first empty our current vector of monitors
    monitors.clear();
    
    // first we create a map from the old moves to the new ones
    std::map<Monitor*, Monitor*> monitorMap;
    
    // next, we iterate over all dag nodes to collect the move
    for (std::map<const DAGNode*, RbDagNodePtr>::iterator i = nodesMap.begin(); i != nodesMap.end(); ++i) {
        // get all moves for this node
        const DAGNode* orgNode = i->first;
        
        const std::vector<Monitor*>& orgMonitors = orgNode->getMonitors();
        
        for (std::vector<Monitor*>::const_iterator j = orgMonitors.begin(); j != orgMonitors.end(); ++j) {
            // check if we already have this move in our list
            std::map<Monitor*,Monitor*>::iterator k = monitorMap.find( *j );
            
            Monitor* clonedMonitor = NULL;
            // create a new move if necessary
            if ( k == monitorMap.end() ) {
                clonedMonitor = (*j)->clone();
                
                // add the new monitor to the monitors map
                monitorMap.insert( std::pair<Monitor*, Monitor*>(*j, clonedMonitor) );
                
                // add the new monitor to our monitors vector
                monitors.push_back( clonedMonitor );
            }
            else {
                clonedMonitor = k->second;
            }
            
            // add the clone DAG node to the cloned monitor
            clonedMonitor->addDagNode( i->second );
            
            // add the cloned monitor to the cloned DAG node
            i->second->addMonitor( clonedMonitor );
            
        }
    }
}


/** Get class name of object */
const std::string& Mcmc::getClassName(void) { 
    
    static std::string rbClassName = "MCMC";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Mcmc::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( MemberObject::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Mcmc::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get member rules */
const MemberRules& Mcmc::getMemberRules(void) const {

    static MemberRules memberRules = ArgumentRules();
    static bool        rulesSet = false;

    if (!rulesSet) {

        memberRules.push_back( new ValueRule ( "model"    , Model::getClassTypeSpec()    ) );

        rulesSet = true;
    }

    return memberRules;
}


/** Get methods */
const MethodTable& Mcmc::getMethods(void) const {

    static MethodTable methods = MethodTable();
    static bool          methodsSet = false;

    if (!methodsSet) {
        
        // method "run" for "n" generations
        ArgumentRules* updateArgRules = new ArgumentRules();
        updateArgRules->push_back( new ValueRule( "generations", Natural::getClassTypeSpec()     ) );
        methods.addFunction("run", new MemberFunction( RbVoid_name, updateArgRules) );
        
        // get Monitors
        ArgumentRules* getMonitorsRules = new ArgumentRules();
        methods.addFunction("getMonitors", new MemberFunction( RbVector<Monitor>::getClassTypeSpec() , getMonitorsRules) );

        methods.setParentTable( &MemberObject::getMethods() );
        methodsSet = true;
    }

    return methods;
}


/** Allow only constant member variables */
void Mcmc::setMemberVariable(const std::string& name, const Variable* var) {

    if ( name == "model" ) {
        model = var;
        extractDagNodesFromModel( static_cast<const Model&>( model->getValue() ) );
    }
    else {
        MemberObject::setMemberVariable(name, var);
    }
}


/** Run the mcmc chain */
void Mcmc::run(size_t ngen) {

    std::cerr << "Initializing mcmc chain ..." << std::endl;

    /* Get the chain settings */
    std::cerr << "Getting the chain settings ..." << std::endl;

    RandomNumberGenerator* rng        = GLOBAL_RNG;

    /* Open the output file and print headers */
    std::cerr << "Opening file and printing headers ..." << std::endl;
    for (size_t i=0; i<monitors.size(); i++) {
        // get the monitor
        if (monitors[i].isTypeSpec( FileMonitor::getClassTypeSpec() ) ) {
            
            FileMonitor& theMonitor = static_cast<FileMonitor&>( monitors[i] );
            
            // open the file stream for the monitor
            theMonitor.openStream();
            
            // print the header information
            theMonitor.printHeader();
        }
    }


    /* Get initial lnProbability of model */
    
    // first we touch all nodes so that the likelihood is dirty
    for (std::vector<RbDagNodePtr>::iterator i=dagNodes.begin(); i!=dagNodes.end(); i++) {
        (*i)->touch();
    }
    
    double lnProbability = 0.0;
    // just a debug counter
    size_t numSummedOver = 0;
    size_t numEliminated = 0;
    std::vector<double> initProb;
    for (std::vector<RbDagNodePtr>::iterator i=dagNodes.begin(); i!=dagNodes.end(); i++) {
        DAGNode* node = (*i);
        if (node->isTypeSpec(StochasticNode::getClassTypeSpec())) {
            StochasticNode* stochNode = dynamic_cast<StochasticNode*>( node );
            // if this node is eliminated, then we do not need to add its log-likelihood
            if ( stochNode->isNotInstantiated() ) {
                
                // test whether the factor root is NULL
                // if so, we need to construct the sum-product sequence for this node
                if ( stochNode->getFactorRoot() == NULL ) {
                    stochNode->constructFactor();
                }
                
                // only if this node is actually the factor root we add the log-likelihood
                if ( stochNode->getFactorRoot() == stochNode) {
                    
                    double lnProb = stochNode->calculateLnProbability();
                    lnProbability += lnProb;
                }
                
                if ( stochNode->isSummedOver() ) {
                    numSummedOver++;
                }
                else {
                    numEliminated++;
                }
                
            }
            else {
                
                // if one of my parents is eliminated, then my likelihood shouldn't be added either
                bool eliminated = false;
                for (std::set<DAGNode*>::iterator j = stochNode->getParents().begin(); j != stochNode->getParents().end(); j++) {
    
                    if ( (*j)->isNotInstantiated() ) {
                        eliminated = true;
                        numEliminated++;
                        
                        // we don't need to check the other parents
                        break;
                    }
                }
                
                // since this node is neither eliminated nor one of its parents, we add the log-likelihood
                if ( !eliminated ) {
                    
                    double lnProb = stochNode->calculateLnProbability();
                    lnProbability += lnProb;
                }
            }
//            initProb.push_back(lnProb);
        }
    }
    
    // now we keep all nodes so that the likelihood is stored
    for (std::vector<RbDagNodePtr>::iterator i=dagNodes.begin(); i!=dagNodes.end(); i++) {
        (*i)->keep();
    }
    
    std::cerr << "Number eliminated nodes = " << numEliminated << std::endl;
    std::cerr << "Number summedOver nodes = " << numSummedOver << std::endl;
    std::cerr << "Initial lnProbability = " << lnProbability << std::endl;

    /* Run the chain */
    std::cerr << "Running the chain ..." << std::endl;

    std::cout << std::endl;
    std::cout << "Gen\tlnProbability" << std::endl;
    
    /* Monitor */
    for (size_t i=0; i<monitors.size(); i++) {
        monitors[i].monitor(0);
    }

    for (unsigned int gen=1; gen<=ngen; gen++) {

        for (size_t i=0; i<moves.size(); i++) {
            /* Get the move */
            Move& theMove = moves[i];

            /* Propose a new value */
            double lnProbabilityRatio;
            double lnHastingsRatio = theMove.performMove(lnProbabilityRatio);
            // QUESTION: How to Gibbs samplers by-pass the accept-reject?
            /* Calculate acceptance ratio */
            double lnR = lnProbabilityRatio + lnHastingsRatio;
            double r;
            if (lnR > 0.0)
                r = 1.0;
            else if (lnR < -300.0)
                r = 0.0;
            else
                r = exp(lnR);
    
            /* Accept or reject the move */
            double u = rng->uniform01(); // TODO No need to draw rng if lnR > 0.0x
            if (u < r) {
                theMove.acceptMove();
                lnProbability += lnProbabilityRatio;
            }
            else {
                theMove.rejectMove();
            }

#ifdef DEBUG_MCMC
            /* Assert that the probability calculation shortcuts work */
            double curLnProb = 0.0;
            std::vector<double> lnRatio;
            for (std::vector<DAGNode*>::iterator i=dagNodes.begin(); i!=dagNodes.end(); i++) {
                if ((*i)->isTypeSpec(StochasticNode::getClassTypeSpec())) {
                    StochasticNode* stochNode = dynamic_cast<StochasticNode*>( *i );
                    double lnProb = stochNode->calculateLnProbability();
                    curLnProb += lnProb;
                    //                lnRatio.push_back(initProb[stochNode-dagNodes.begin()] - lnProb);
                }
            }
            if (fabs(lnProbability - curLnProb) > 1E-8)
                throw RbException("Error in ln probability calculation shortcuts");
#endif
        }

        /* Monitor */
        for (size_t i=0; i<monitors.size(); i++) {
            monitors[i].monitor(gen);
        }

        /* Print to screen */
        std::cout << gen << "\t" << lnProbability << std::endl;

    }

    std::cerr << "Finished chain" << std::endl;

    std::cout << std::endl;
}

