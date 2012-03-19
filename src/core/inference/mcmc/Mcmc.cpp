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
Mcmc::Mcmc(void) : MemberObject(getMemberRules()),
    model( NULL ),
    moves( NULL ), 
    monitors( NULL ) {
}

/** Copy constructor */
Mcmc::Mcmc(const Mcmc &x) : MemberObject(x),
model( x.model ),
moves( x.moves ), 
monitors( x.monitors ) {
    
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
        const RbVector<Monitor>& mons = static_cast<const RbVector<Monitor>& >( monitors->getValue() );
        return mons;
    }

    return MemberObject::executeOperationSimple( name, args );
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
        memberRules.push_back( new ValueRule ( "moves"    , RbVector<Move>::getClassTypeSpec() ) );
        memberRules.push_back( new ValueRule ( "monitors" , RbVector<Monitor>::getClassTypeSpec() ) );

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

    // we need to change the DAG nodes to which the moves are pointing to
    // when the moves where created they pointed to DAG nodes in the workspace
    // but the model created clones of theses nodes.
    // Hence we need to set the DAG nodes of the moves to these clones.
    if ( name == "moves" ) {
        
//        Vector* moves = static_cast<Vector*>(var->getValue().convertTo( TypeSpec(Vector::getClassName(), NULL, new TypeSpec(Move::getClassTypeSpec()) ) ) );
        // The parser should have made sure that we get a vector of moves, even if it is only a single move.
//        RbVector<Move>& moves = static_cast<RbVector<Move>& >( var->getValue() );
//        // Our new vector of moves
//        Vector* myMoves = new Vector( Move::getClassTypeSpec() ); 
//        for (size_t i=0; i<moves.size(); i++) {
//            // get the move #i
//            Move& theMove = static_cast<Move&>( moves.getElement(i) );
//            
//            // clone the move and replace the node
//            Move* newMove = theMove.clone();
//            
//            // add this new move to my vector of moves
//            myMoves->push_back(newMove);
//        }
//        
//        this->moves = RbVariablePtr( new Variable ( new ConstantNode( myMoves ) ) );
////        setMemberDagNode(name, new ConstantNode( moves ) );
//        
//        
//        if (model != NULL) {
//            // get the DAG nodes
//            const Model& theModel = dynamic_cast<Model&>( model->getValue() );
//            for (size_t i=0; i<myMoves->size(); i++) {
//                // get the move #i
//                Move& theMove = static_cast<Move&>( myMoves->getElement(i) );
//                
//                // get the DAG node for this move
//                std::vector<StochasticNode*> &theOldNodes = theMove.getDagNodes();
//                
//                // convert the old nodes from Stochastic nodes to DAGNode
//                std::vector<DAGNode*> oldNodes;
//                for (std::vector<StochasticNode*>::iterator it = theOldNodes.begin(); it != theOldNodes.end(); it++) {
//                    oldNodes.push_back( *it );
//                }
//                
//                // test whether this node is already a cloned one
//                if (!theModel.areDagNodesCloned(oldNodes) ) {
//                    // get the DAG node which corresponds in the model to the cloned original node
//                    std::vector<DAGNode*> theNewNodes = theModel.getClonedDagNodes(oldNodes);
//                    
//                    // convert the new nodes from DAGNode to Stochastic Nodes
//                    std::vector<StochasticNode*> newNodes;
//                    for (std::vector<DAGNode*>::iterator it = theNewNodes.begin(); it != theNewNodes.end(); it++) {
//                        newNodes.push_back( static_cast<StochasticNode*>( *it ) );
//                    }
//                    theMove.replaceDagNodes(newNodes);
//                }
//                
//            }
//        }
    }
    else if ( name == "monitors" ) {
//        
////        Vector* monitors = static_cast<Vector*>(var->getValue().convertTo(TypeSpec(Vector::getClassName(), NULL, new TypeSpec(FileMonitor::getClassTypeSpec()) ) ) );
//        // The parser should have made sure that we get a vector of moves, even if it is only a single move.
//        Vector& monitors = static_cast<Vector&>( var->getValue() );
//        // Our new vector of moves
//        Vector* myMonitors = new Vector( Monitor::getClassTypeSpec() ); 
//        for (size_t i=0; i<monitors.size(); i++) {
//            // get the monitor #i
//            Monitor& theMonitor = static_cast<Monitor&>( monitors.getElement(i) );
//                        
//            // clone the move and replace the node
//            Monitor* newMonitor = theMonitor.clone();
//            
//            // add this monitor to the list of my monitors
//            myMonitors->push_back(newMonitor );
//            
//        }
//        
//        this->monitors = RbVariablePtr( new Variable ( new ConstantNode( myMonitors ) ) );
//        
//        if (model != NULL) {
//            // get the DAG nodes
//            const Model& theModel = dynamic_cast<Model&>( model->getValue() );
//            for (size_t i=0; i<myMonitors->size(); i++) {
//                // get the move #i
//                Monitor& theMonitor = static_cast<Monitor&>( myMonitors->getElement(i) );
//                
//                // get the DAG node for this move
//                std::vector<RbVariablePtr> &theOldNodes = theMonitor.getDagNodes();
//                
//                // convert the old nodes from Stochastic nodes to DAGNode
//                std::vector<DAGNode*> oldNodes;
//                for (std::vector<RbVariablePtr>::iterator it = theOldNodes.begin(); it != theOldNodes.end(); it++) {
//                    oldNodes.push_back( (*it)->getDagNode() );
//                }
//                
//                // test whether this node is already a cloned one
//                if (!theModel.areDagNodesCloned(oldNodes) ) {
//                    // get the DAG node which corresponds in the model to the cloned original node
//                    std::vector<DAGNode*> theNewNodes = theModel.getClonedDagNodes(oldNodes);
//                
//                    // convert the new nodes from DAGNode to Stochastic Nodes
//                    std::vector<VariableNode*> newNodes;
//                    for (std::vector<DAGNode*>::iterator it = theNewNodes.begin(); it != theNewNodes.end(); it++) {
//                        newNodes.push_back( static_cast<VariableNode*>( *it ) );
//                    }
//                    theMonitor.replaceDagNodes(newNodes);
//                }
//
//            }
//        }
//            
    } 
    else if ( name == "model" ) {
//        model = var;
//        
//        // update the moves
//        if (moves != NULL) {
//            // get the DAG nodes
//            Vector& myMoves = static_cast<Vector&>( moves->getValue() );
//            const Model& theModel = dynamic_cast<Model&>( model->getValue() );
//            for (size_t i=0; i<myMoves.size(); i++) {
//                // get the move #i
//                Move& theMove = static_cast<Move&>( myMoves.getElement(i) );
//                
//                // get the DAG node for this move
//                std::vector<StochasticNode*> &theOldNodes = theMove.getDagNodes();
//                
//                // convert the old nodes from Stochastic nodes to DAGNode
//                std::vector<DAGNode*> oldNodes;
//                for (std::vector<StochasticNode*>::iterator it = theOldNodes.begin(); it != theOldNodes.end(); it++) {
//                    oldNodes.push_back( *it );
//                }
//                
//                // test whether this node is already a cloned one
//                if (!theModel.areDagNodesCloned(oldNodes) ) {
//                    // get the DAG node which corresponds in the model to the cloned original node
//                    std::vector<DAGNode*> theNewNodes = theModel.getClonedDagNodes(oldNodes);
//                
//                    // convert the new nodes from DAGNode to Stochastic Nodes
//                    std::vector<StochasticNode*> newNodes;
//                    for (std::vector<DAGNode*>::iterator it = theNewNodes.begin(); it != theNewNodes.end(); it++) {
//                        newNodes.push_back( static_cast<StochasticNode*>( *it ) );
//                    }
//                    theMove.replaceDagNodes(newNodes);
//                }
//            }
//        }
//
//        // update the monitors
//        if (monitors != NULL) {
//            // get the DAG nodes
//            Vector& myMonitors = static_cast<Vector&>( monitors->getValue() );
//            const Model& theModel = dynamic_cast<Model&>( model->getValue() );
//            for (size_t i=0; i<myMonitors.size(); i++) {
//                // get the move #i
//                Monitor& theMonitor = static_cast<Monitor&>( myMonitors.getElement(i) );
//               
//                // get the DAG node for this move
//                std::vector<RbVariablePtr> &theOldNodes = theMonitor.getDagNodes();
//               
//                // convert the old nodes from Stochastic nodes to DAGNode
//                std::vector<DAGNode*> oldNodes;
//                for (std::vector<RbVariablePtr>::iterator it = theOldNodes.begin(); it != theOldNodes.end(); it++) {
//                    oldNodes.push_back( (*it)->getDagNode() );
//                }
//                
//                // test whether this node is already a cloned one
//                if (!theModel.areDagNodesCloned(oldNodes) ) {
//                    // get the DAG node which corresponds in the model to the cloned original node
//                    std::vector<DAGNode*> theNewNodes = theModel.getClonedDagNodes(oldNodes);
//               
//                    // convert the new nodes from DAGNode to Stochastic Nodes
//                    std::vector<VariableNode*> newNodes;
//                    for (std::vector<DAGNode*>::iterator it = theNewNodes.begin(); it != theNewNodes.end(); it++) {
//                        newNodes.push_back( static_cast<VariableNode*>( *it ) );
//                    }
//                    theMonitor.replaceDagNodes(newNodes);
//                }
//            }
//        }
    }
    else {
        MemberObject::setMemberVariable(name, var);
    }
}


/** Run the mcmc chain */
void Mcmc::run(size_t ngen) {

//    std::cerr << "Initializing mcmc chain ..." << std::endl;
//
//    /* Get the dag nodes from the model */
//    std::vector<RbDagNodePtr> dagNodes = (static_cast<Model&>( model->getValue() ) ).getDAGNodes();
//
//    /* Get the moves and monitors */
//    Vector& theMonitors = static_cast<Vector&>( monitors->getValue() );
//    Vector& theMoves = static_cast<Vector&>( moves->getValue() );
//
//    /* Get the chain settings */
//    std::cerr << "Getting the chain settings ..." << std::endl;
//
//    RandomNumberGenerator* rng        = GLOBAL_RNG;
//
//    /* Open the output file and print headers */
//    std::cerr << "Opening file and printing headers ..." << std::endl;
//    for (size_t i=0; i<theMonitors.size(); i++) {
//        // get the monitor
//        if (theMonitors.getElement(i).isTypeSpec( FileMonitor::getClassTypeSpec() ) ) {
//            
//            FileMonitor& theMonitor = static_cast<FileMonitor&>( theMonitors.getElement(i) );
//            
//            // open the file stream for the monitor
//            theMonitor.openStream();
//            
//            // print the header information
//            theMonitor.printHeader();
//        }
//    }
//
//
//    /* Get initial lnProbability of model */
//    
//    // first we touch all nodes so that the likelihood is dirty
////    for (std::vector<RbDagNodePtr>::iterator i=dagNodes.begin(); i!=dagNodes.end(); i++) {
////        (*i)->touch();
////    }
//    double lnProbability = 0.0;
//    // just a debug counter
//    size_t numSummedOver = 0;
//    size_t numEliminated = 0;
//    std::vector<double> initProb;
//    for (std::vector<RbDagNodePtr>::iterator i=dagNodes.begin(); i!=dagNodes.end(); i++) {
//        DAGNode* node = (*i);
//        if (node->isTypeSpec(StochasticNode::getClassTypeSpec())) {
//            StochasticNode* stochNode = dynamic_cast<StochasticNode*>( node );
//            // if this node is eliminated, then we do not need to add its log-likelihood
//            if ( stochNode->isNotInstantiated() ) {
//                
//                // test whether the factor root is NULL
//                // if so, we need to construct the sum-product sequence for this node
//                if ( stochNode->getFactorRoot() == NULL ) {
//                    stochNode->constructFactor();
//                }
//                
//                // only if this node is actually the factor root we add the log-likelihood
//                if ( stochNode->getFactorRoot() == stochNode) {
//                    
//                    double lnProb = stochNode->calculateLnProbability();
//                    lnProbability += lnProb;
//                }
//                
//                if ( stochNode->isSummedOver() ) {
//                    numSummedOver++;
//                }
//                else {
//                    numEliminated++;
//                }
//                
//            }
//            else {
//                
//                // if one of my parents is eliminated, then my likelihood should be added either
//                bool eliminated = false;
//                for (std::set<DAGNode*>::iterator j = stochNode->getParents().begin(); j != stochNode->getParents().end(); j++) {
//    
//                    if ( (*j)->isNotInstantiated() ) {
//                        eliminated = true;
//                        numEliminated++;
//                        
//                        // we don't need to check the other parents
//                        break;
//                    }
//                }
//                
//                // since this node is neither eliminated nor one of its parents, we add the log-likelihood
//                if ( !eliminated ) {
//                    
//                    double lnProb = stochNode->calculateLnProbability();
//                    lnProbability += lnProb;
//                }
//            }
////            initProb.push_back(lnProb);
//        }
//        node->keep();
//    }
//    std::cerr << "Number eliminated nodes = " << numEliminated << std::endl;
//    std::cerr << "Number summedOver nodes = " << numSummedOver << std::endl;
//    std::cerr << "Initial lnProbability = " << lnProbability << std::endl;
//
//    /* Run the chain */
//    std::cerr << "Running the chain ..." << std::endl;
//
//    std::cout << std::endl;
//    std::cout << "Gen\tlnProbability" << std::endl;
//    
//    /* Monitor */
//    for (size_t i=0; i<theMonitors.size(); i++) {
//        static_cast<Monitor&>( theMonitors.getElement(i) ).monitor(0);
//    }
//
//    for (unsigned int gen=1; gen<=ngen; gen++) {
//
//        for (size_t i=0; i<theMoves.size(); i++) {
//            /* Get the move */
//            Move& theMove = static_cast<Move&>( theMoves.getElement(i) );
//
//            /* Propose a new value */
//            double lnProbabilityRatio;
//            double lnHastingsRatio = theMove.performMove(lnProbabilityRatio);
//            // QUESTION: How to Gibbs samplers by-pass the accept-reject?
//            /* Calculate acceptance ratio */
//            double lnR = lnProbabilityRatio + lnHastingsRatio;
//            double r;
//            if (lnR > 0.0)
//                r = 1.0;
//            else if (lnR < -300.0)
//                r = 0.0;
//            else
//                r = exp(lnR);
//    
//            /* Accept or reject the move */
//            double u = rng->uniform01(); // TODO No need to draw rng if lnR > 0.0x
//            if (u < r) {
//                theMove.acceptMove();
//                lnProbability += lnProbabilityRatio;
//            }
//            else {
//                theMove.rejectMove();
//            }
//
//#ifdef DEBUG_MCMC
//            /* Assert that the probability calculation shortcuts work */
//            double curLnProb = 0.0;
//            std::vector<double> lnRatio;
//            for (std::vector<DAGNode*>::iterator i=dagNodes.begin(); i!=dagNodes.end(); i++) {
//                if ((*i)->isTypeSpec(StochasticNode::getClassTypeSpec())) {
//                    StochasticNode* stochNode = dynamic_cast<StochasticNode*>( *i );
//                    double lnProb = stochNode->calculateLnProbability();
//                    curLnProb += lnProb;
//                    //                lnRatio.push_back(initProb[stochNode-dagNodes.begin()] - lnProb);
//                }
//            }
//            if (fabs(lnProbability - curLnProb) > 1E-8)
//                throw RbException("Error in ln probability calculation shortcuts");
//#endif
//        }
//
//        /* Monitor */
//        for (size_t i=0; i<theMonitors.size(); i++) {
//            static_cast<Monitor&>( theMonitors.getElement(i) ).monitor(gen);
//        }
//
//        /* Print to screen */
//        std::cout << gen << "\t" << lnProbability << std::endl;
//
//    }
//
//    std::cerr << "Finished chain" << std::endl;
//
//    std::cout << std::endl;
}

