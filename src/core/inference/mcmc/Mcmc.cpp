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

#include "ConstantInferenceNode.h"
#include "ConstArgumentRule.h"
#include "DagNodeContainer.h"
#include "Integer.h"
#include "InferenceDagNode.h"
#include "InferenceMove.h"
#include "InferenceMonitor.h"
#include "Mcmc.h"
#include "Model.h"
#include "FileMonitor.h"
#include "Move.h"
#include "Natural.h"
#include "ParserMove.h"
#include "ParserMonitor.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbNullObject.h"
#include "RangeRule.h"
#include "RbUtil.h"
#include "RbString.h"
#include "RbVector.h"
#include "SimpleMemberFunction.h"
#include "StochasticInferenceNode.h"
#include "VariableNode.h"
#include "Workspace.h"

#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <typeinfo>



/** Constructor passes member rules and method inits to base class */
Mcmc::Mcmc(void) : MemberObject( getMemberRules() ) {
}

/** Copy constructor */
Mcmc::Mcmc(const Mcmc &x) : MemberObject(x), model( x.model ) { 
    
    extractDagNodesFromModel( model );
    
    // add the moves
    for (std::vector<InferenceMove *>::const_iterator i = x.moves.begin(); i != x.moves.end(); ++i) {
        addMove( *i );
    }
    
    // add the monitors
    for (std::vector<InferenceMonitor *>::const_iterator i = x.monitors.begin(); i != x.monitors.end(); ++i) {
        addMonitor( *i );
    }
}


Mcmc& Mcmc::operator=(const Mcmc &m) {
    // check for self-assignment
    if ( &m != this ) {
        
        // clear all elements
        dagNodes.clear();
        moves.clear();
        monitors.clear();
        
        // set the model first
        model = m.model;
        
        // add the moves
        for (std::vector<InferenceMove *>::const_iterator i = m.moves.begin(); i != m.moves.end(); ++i) {
            addMove( *i );
        }
        
        // add the monitors
        for (std::vector<InferenceMonitor *>::const_iterator i = m.monitors.begin(); i != m.monitors.end(); ++i) {
            addMonitor( *i );
        }
    }
    
    return *this;
}


/*
 * Add a move and exchange the DAG nodes.
 */
void Mcmc::addMove( const DAGNode *m ) {
    
    // test whether var is a DagNodeContainer
    if ( m != NULL && m->getValue().isTypeSpec( DagNodeContainer::getClassTypeSpec() ) ) {
        const RbPtr<const RbLanguageObject>& objPtr = m->getValue().getSingleValue();
        const DagNodeContainer *container = static_cast<const DagNodeContainer *>( (const RbLanguageObject *) objPtr );
        for (size_t i = 0; i < container->size(); ++i) {
            const RbObject& elemPtr = container->getElement(i);
            addMove(static_cast<const VariableSlot&>( elemPtr ).getVariable().getDagNode() );
        }
    }
    else {
        // first, we cast the value to a parser move
        const ParserMove *move = static_cast<const ParserMove *>( (const RbLanguageObject *) m->getValue().getSingleValue() );
        
        // we need to extract the lean move
        InferenceMove* leanMove = move->getLeanMove()->clone();
        
        // create a named DAG node map
        std::map<std::string, InferenceDagNode *> nodesMap;
        for (std::vector<InferenceDagNode *>::iterator i = dagNodes.begin(); i != dagNodes.end(); ++i) {
            const std::string n = (*i)->getName();
            if ( n != "" ) {
                nodesMap.insert( std::pair<std::string, InferenceDagNode *>(n, *i) );
            }
        }
        
        // replace the DAG nodes of the move to point to our cloned DAG nodes
        const std::vector<RbPtr<const DAGNode> >& orgNodes = move->getMoveArgumgents();
        std::vector<StochasticInferenceNode*> clonedNodes;
        for (std::vector<RbPtr<const DAGNode> >::const_iterator i = orgNodes.begin(); i != orgNodes.end(); ++i) {
            const DAGNode *orgNode = *i;
            const std::map<std::string, InferenceDagNode*>::const_iterator& it = nodesMap.find( orgNode->getName() );
            if ( it == nodesMap.end() ) {
                std::cerr << "Could not find the DAG node with name \"" << orgNode->getName() << "\" in the cloned model." << std::endl;
            }
            else {
                InferenceDagNode* clonedNode = it->second;
                if ( typeid( *clonedNode ) != typeid( StochasticInferenceNode ) ) {
                    throw RbException("We do not support moves on non-stochastic nodes.");
                }
                clonedNodes.push_back( static_cast<StochasticInferenceNode *>( clonedNode ) );
            }
        }
        leanMove->setArguments( clonedNodes );
        
        // finally, add the move to our moves list
        moves.push_back( leanMove );
    }
}


void Mcmc::addMove(const InferenceMove *m) {
    // we need to extract the lean move
    InferenceMove* leanMove = m->clone();
    
    // create a named DAG node map
    std::map<std::string, InferenceDagNode *> nodesMap;
    for (std::vector<InferenceDagNode *>::iterator i = dagNodes.begin(); i != dagNodes.end(); ++i) {
        const std::string n = (*i)->getName();
        if ( n != "" ) {
            nodesMap.insert( std::pair<std::string, InferenceDagNode *>(n, *i) );
        }
    }
    
    // replace the DAG nodes of the move to point to our cloned DAG nodes
    const std::vector<StochasticInferenceNode *>& orgNodes = m->getDagNodes();
    std::vector<StochasticInferenceNode*> clonedNodes;
    for (std::vector<StochasticInferenceNode *>::const_iterator i = orgNodes.begin(); i != orgNodes.end(); ++i) {
        const StochasticInferenceNode *orgNode = *i;
        const std::map<std::string, InferenceDagNode*>::const_iterator& it = nodesMap.find( orgNode->getName() );
        if ( it == nodesMap.end() ) {
            std::cerr << "Could not find the DAG node with name \"" << orgNode->getName() << "\" in the cloned model." << std::endl;
        }
        else {
            InferenceDagNode* clonedNode = it->second;
            if ( typeid( *clonedNode ) != typeid( StochasticInferenceNode ) ) {
                throw RbException("We do not support moves on non-stochastic nodes.");
            }
            clonedNodes.push_back( static_cast<StochasticInferenceNode *>( clonedNode ) );
        }
    }
    leanMove->setArguments( clonedNodes );
    
    // finally, add the move to our moves list
    moves.push_back( leanMove );
}


/*
 * Add a monitor and exchange the DAG nodes.
 */
void Mcmc::addMonitor( const DAGNode *m ) {
    
    // test whether var is a DagNodeContainer
    if ( m != NULL && m->getValue().isTypeSpec( DagNodeContainer::getClassTypeSpec() ) ) {
        const RbPtr<const RbLanguageObject>& objPtr = m->getValue().getSingleValue();
        const DagNodeContainer *container = static_cast<const DagNodeContainer *>( (const RbLanguageObject *) objPtr );
        for (size_t i = 0; i < container->size(); ++i) {
            const RbObject& elemPtr = container->getElement(i);
            addMonitor(static_cast<const VariableSlot&>( elemPtr ).getVariable().getDagNode() );
        }
    }
    else {
        // first, we cast the value to a parser monitor
        const ParserMonitor *monitor = static_cast<const ParserMonitor *>( (const RbLanguageObject *) m->getValue().getSingleValue() );
        
        // we need to extract the lean move
        InferenceMonitor* leanMonitor = monitor->getLeanMonitor()->clone();
        
        // create a named DAG node map
        std::map<std::string, InferenceDagNode *> nodesMap;
        for (std::vector<InferenceDagNode *>::iterator i = dagNodes.begin(); i != dagNodes.end(); ++i) {
            const std::string n = (*i)->getName();
            if ( n != "" ) {
                nodesMap.insert( std::pair<std::string, InferenceDagNode *>(n, *i) );
            }
        }
        
        // replace the DAG nodes of the monitor to point to our cloned DAG nodes
        const std::vector<const DAGNode *> orgNodes = monitor->getMonitorArgumgents();
        std::vector<InferenceDagNode *> clonedNodes;
        std::vector<RbLanguageObject *> templateObjects;
        for (std::vector<const DAGNode *>::const_iterator i = orgNodes.begin(); i != orgNodes.end(); ++i) {
            const DAGNode *orgNode = *i;
            const std::map<std::string, InferenceDagNode*>::const_iterator& it = nodesMap.find( orgNode->getName() );
            if ( it == nodesMap.end() ) {
                std::cerr << "Could not find the DAG node with name \"" << orgNode->getName() << "\" in the cloned model." << std::endl;
            }
            else {
                InferenceDagNode* clonedNode = it->second;
                clonedNodes.push_back( clonedNode );
                templateObjects.push_back( orgNode->getValue().getSingleValue()->clone() );
            }
        }
        leanMonitor->setArguments( clonedNodes );
        leanMonitor->setTemplateObjects( templateObjects );
        
        // finally, add the move to our moves list
        monitors.push_back( leanMonitor );
    }
}


void Mcmc::addMonitor(const InferenceMonitor *m) {
    // we need to extract the lean move
    InferenceMonitor* leanMonitor = m->clone();
    
    // create a named DAG node map
    std::map<std::string, InferenceDagNode *> nodesMap;
    for (std::vector<InferenceDagNode *>::iterator i = dagNodes.begin(); i != dagNodes.end(); ++i) {
        const std::string n = (*i)->getName();
        if ( n != "" ) {
            nodesMap.insert( std::pair<std::string, InferenceDagNode *>(n, *i) );
        }
    }
    
    // replace the DAG nodes of the move to point to our cloned DAG nodes
    const std::vector<InferenceDagNode *>& orgNodes = m->getDagNodes();
    std::vector<InferenceDagNode *> clonedNodes;
    for (std::vector<InferenceDagNode *>::const_iterator i = orgNodes.begin(); i != orgNodes.end(); ++i) {
        const InferenceDagNode *orgNode = *i;
        const std::map<std::string, InferenceDagNode*>::const_iterator& it = nodesMap.find( orgNode->getName() );
        if ( it == nodesMap.end() ) {
            std::cerr << "Could not find the DAG node with name \"" << orgNode->getName() << "\" in the cloned model." << std::endl;
        }
        else {
            InferenceDagNode* clonedNode = it->second;
            clonedNodes.push_back( clonedNode );
        }
    }
    leanMonitor->setArguments( clonedNodes );
    
    // finally, add the move to our moves list
    monitors.push_back( leanMonitor );
}

/** Clone object */
Mcmc* Mcmc::clone(void) const {

    return new Mcmc(*this);
}


/** Map calls to member methods */
RbPtr<RbLanguageObject> Mcmc::executeSimpleMethod(const std::string& name, const std::vector<const RbObject *>& args) {

    if (name == "run") {
        int n = static_cast<const Natural *>( args[0] )->getValue();
        run(n);
        return NULL;
    }
//    else if ( name == "getMonitors" ) {
//        return monitors;
//    }

    return MemberObject::executeSimpleMethod( name, args );
}


/**
 * Clone the model and extract the DAG nodes, moves and monitors.
 *
 * First we clone the entire DAG, then we clone the moves and the monitors and set the new nodes appropriately.
 */
void Mcmc::extractDagNodesFromModel(const Model& source) {
    dagNodes = source.getLeanDagNodes();
    
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

        memberRules.push_back( new ConstArgumentRule( "model"    , Model::getClassTypeSpec()    ) );
        memberRules.push_back( new ConstArgumentRule( "moves"    , RbObject::getClassTypeSpec() ) );
        memberRules.push_back( new ConstArgumentRule( "monitors" , RbObject::getClassTypeSpec() ) );

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
        updateArgRules->push_back( new ConstArgumentRule( "generations", Natural::getClassTypeSpec()     ) );
        methods.addFunction("run", new SimpleMemberFunction( RbVoid_name, updateArgRules) );
        
//        // get Monitors
//        ArgumentRules* getMonitorsRules = new ArgumentRules();
//        methods.addFunction("getMonitors", new MemberFunction( RbVector<Monitor>::getClassTypeSpec() , getMonitorsRules) );

        methods.setParentTable( &MemberObject::getMethods() );
        methodsSet = true;
    }

    return methods;
}


void Mcmc::printValue( std::ostream &o ) const  {
    model.printValue( o );
    
    o << std::endl;
    o << "Moves" <<  std::endl;
    o << "-------------------------------------" << std::endl;
    size_t move_index = 1;
    for (std::vector<InferenceMove *>::const_iterator i = moves.begin(); i != moves.end(); ++i) {
        o << move_index << ". Move" << std::endl;
        o << typeid( *(*i) ).name() << " (";
        (*i)->printValue(o);
        o << ")" << std::endl;
        move_index++;
    }
    
    o << std::endl;
    o << "Monitors" <<  std::endl;
    o << "-------------------------------------" << std::endl;
    size_t monitor_index = 1;
    for (std::vector<InferenceMonitor *>::const_iterator i = monitors.begin(); i != monitors.end(); ++i) {
        o << monitor_index << ". Monitor" << std::endl;
        o << typeid( *(*i) ).name() << " (";
        (*i)->printValue(o);
        o << ")" << std::endl;
        monitor_index++;
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
        if ( typeid(*monitors[i]) == typeid(FileMonitor) ) {
            
            FileMonitor* theMonitor = static_cast<FileMonitor*>( monitors[i] );
            
            // open the file stream for the monitor
            theMonitor->openStream();
            
            // print the header information
            theMonitor->printHeader();
        }
    }


    /* Get initial lnProbability of model */
    
    // first we touch all nodes so that the likelihood is dirty
    for (std::vector<InferenceDagNode *>::iterator i=dagNodes.begin(); i!=dagNodes.end(); i++) {
        (*i)->touch();
    }
    
    double lnProbability = 0.0;
    // just a debug counter
    size_t numSummedOver = 0;
    size_t numEliminated = 0;
    std::vector<double> initProb;
    for (std::vector<InferenceDagNode *>::iterator i=dagNodes.begin(); i!=dagNodes.end(); i++) {
        InferenceDagNode* node = (*i);
        if ( typeid(*node) == typeid(StochasticInferenceNode) ) {
            StochasticInferenceNode* stochNode = dynamic_cast<StochasticInferenceNode*>( node );
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
                for (std::set<InferenceDagNode *>::iterator j = stochNode->getParents().begin(); j != stochNode->getParents().end(); j++) {
    
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
    for (std::vector<InferenceDagNode *>::iterator i=dagNodes.begin(); i!=dagNodes.end(); i++) {
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
        monitors[i]->monitor(0);
    }

    for (unsigned int gen=1; gen<=ngen; gen++) {

        for (size_t i=0; i<moves.size(); i++) {
            /* Get the move */
            InferenceMove* theMove = moves[i];

            /* Propose a new value */
            double lnProbabilityRatio;
            double lnHastingsRatio = theMove->perform(lnProbabilityRatio);
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
                theMove->accept();
                lnProbability += lnProbabilityRatio;
            }
            else {
                theMove->reject();
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
            monitors[i]->monitor(gen);
        }

        /* Print to screen */
        std::cout << gen << "\t" << lnProbability << std::endl;

    }

    std::cerr << "Finished chain" << std::endl;
    
    
    /* Open the output file and print headers */
    std::cerr << "Closing files ..." << std::endl;
    for (size_t i=0; i<monitors.size(); i++) {
        // get the monitor
        if ( typeid(*monitors[i]) == typeid(FileMonitor) ) {
            
            FileMonitor* theMonitor = static_cast<FileMonitor*>( monitors[i] );
            
            // open the file stream for the monitor
            theMonitor->closeStream();
        }
    }

    std::cout << std::endl;
}

