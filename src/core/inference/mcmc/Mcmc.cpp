/**
 * @file
 * This file contains the implementation of Mcmc, which is used to hold
 * information about and run an mcmc analysis.
 *
 * @brief Implementation of Mcmc
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-29 23:23:09 +0100 (Tis, 29 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 * @interface Mcmc
 * @package distributions
 *
 * $Id: Mcmc.h 211 2009-12-29 22:23:09Z ronquist $
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
#include "RangeRule.h"
#include "RbUtil.h"
#include "RbString.h"
#include "StochasticNode.h"
#include "ValueRule.h"
#include "Vector.h"
#include "VectorString.h"
#include "VariableNode.h"
#include "Workspace.h"

#include <cmath>
#include <fstream>
#include <sstream>
#include <string>


// Definition of the static type spec member
const TypeSpec Mcmc::typeSpec(Mcmc_name);

/** Constructor passes member rules and method inits to base class */
Mcmc::Mcmc(void) : ConstantMemberObject(getMemberRules()) {
}

/** Copy constructor */
Mcmc::Mcmc(const Mcmc &x) : ConstantMemberObject(x) {
    
}


/** Clone object */
Mcmc* Mcmc::clone(void) const {

    return new Mcmc(*this);
}


/** Map calls to member methods */
RbLanguageObject* Mcmc::executeOperationSimple(const std::string& name, Environment* args) {

    if (name == "run") {
        const RbLanguageObject* argument = (*args)[0]->getValue();
        int n = static_cast<const Natural*>( argument )->getValue();
        run(n);
        return NULL;
    }

    return MemberObject::executeOperationSimple( name, args );
}


/** Get class vector describing type of object */
const VectorString& Mcmc::getClass(void) const {

    static VectorString rbClass = VectorString(Mcmc_name) + ConstantMemberObject::getClass();
    return rbClass;
}


/** Get member rules */
const MemberRules* Mcmc::getMemberRules(void) const {

    static MemberRules* memberRules = new ArgumentRules();
    static bool        rulesSet = false;

    if (!rulesSet) {

        memberRules->push_back( new ValueRule ( "model"    , Model_name    ) );
        memberRules->push_back( new ValueRule ( "moves"    , TypeSpec(Vector_name,new TypeSpec(Move_name) ) ) );
        memberRules->push_back( new ValueRule ( "monitors" , TypeSpec(Vector_name,new TypeSpec(FileMonitor_name) ) ) );

        rulesSet = true;
    }

    return memberRules;
}


/** Get methods */
const MethodTable* Mcmc::getMethods(void) const {

    static MethodTable* methods = new MethodTable();
    static ArgumentRules* updateArgRules = new ArgumentRules();
    static bool          methodsSet = false;

    if (!methodsSet) {

        updateArgRules->push_back( new ValueRule( "generations", Natural_name     ) );
        methods->addFunction("run", new MemberFunction( RbVoid_name, updateArgRules) );

        methods->setParentTable( MemberObject::getMethods() );
        methodsSet = true;
    }

    return methods;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Mcmc::getTypeSpec(void) const {
    return typeSpec;
}


/** Allow only constant member variables */
void Mcmc::setMemberVariable(const std::string& name, Variable* var) {

    // we need to change the DAG nodes to which the moves are pointing to
    // when the moves where created they pointed to DAG nodes in the workspace
    // but the model created clones of theses nodes.
    // Hence we need to set the DAG nodes of the moves to these clones.
    if ( name == "moves" ) {
        // get the DAG nodes
        const Model* theModel = dynamic_cast<Model*>( getMemberValue("model") );
        
        Vector* moves = static_cast<Vector*>(var->getValue()->convertTo(TypeSpec(Vector_name, new TypeSpec(Move_name) ) ) );
        for (size_t i=0; i<moves->size(); i++) {
            // get the move #i
            Move* theMove( static_cast<Move*>( moves->getElement(i) ) );
            
            // get the DAG node for this move
            std::vector<StochasticNode*> &theOldNodes = theMove->getDagNodes();
            
            // convert the old nodes from Stochastic nodes to DAGNode
            std::vector<DAGNode*> oldNodes;
            for (std::vector<StochasticNode*>::iterator it = theOldNodes.begin(); it != theOldNodes.end(); it++) {
                oldNodes.push_back( *it );
            }
            
            // get the DAG node which corresponds in the model to the cloned original node
            std::vector<DAGNode*> theNewNodes = theModel->getClonedDagNodes(oldNodes);
            
            // clone the move and replace the node
            Move* newMove = theMove->clone();
            // convert the new nodes from DAGNode to Stochastic Nodes
            std::vector<StochasticNode*> newNodes;
            for (std::vector<DAGNode*>::iterator it = theNewNodes.begin(); it != theNewNodes.end(); it++) {
                newNodes.push_back( static_cast<StochasticNode*>( *it ) );
            }
            newMove->replaceDagNodes(newNodes);
            moves->setElement(i, newMove);
            
        }
        
        setMemberDagNode(name, new ConstantNode( moves ) );
    }
    else if ( name == "monitors" ) {
        // get the DAG nodes
        const Model* theModel = static_cast<Model*>( getMemberValue("model") );
        
        Vector* monitors = static_cast<Vector*>(var->getValue()->convertTo(TypeSpec(Vector_name, new TypeSpec(FileMonitor_name) ) ) );
        for (size_t i=0; i<monitors->size(); i++) {
            // get the monitor #i
            FileMonitor* theMonitor = static_cast<FileMonitor*>( monitors->getElement(i) );
            
            // get the DAG node for this monitor
            std::vector<VariableNode*> &theOldNodes = theMonitor->getDagNodes();
            
            // convert the old nodes from Stochastic nodes to DAGNode
            std::vector<DAGNode*> oldNodes;
            for (std::vector<VariableNode*>::iterator it = theOldNodes.begin(); it != theOldNodes.end(); it++) {
                oldNodes.push_back( *it );
            }
            // get the DAG node which corresponds in the model to the cloned original node
            std::vector<DAGNode*> theNewNodes = theModel->getClonedDagNodes(oldNodes);
            
            // clone the move and replace the node
            FileMonitor* newMonitor = theMonitor->clone();
            // convert the new nodes from DAGNode to Stochastic Nodes
            std::vector<VariableNode*> newNodes;
            for (std::vector<DAGNode*>::iterator it = theNewNodes.begin(); it != theNewNodes.end(); it++) {
                newNodes.push_back( static_cast<VariableNode*>( *it ) );
            }
            newMonitor->replaceDagNodes(newNodes);
            monitors->setElement(i, newMonitor );
            
        }
        
        setMemberDagNode(name, new ConstantNode( monitors ) );
    }
    else {
        ConstantMemberObject::setMemberVariable(name, var);
    }
}


/** Run the mcmc chain */
void Mcmc::run(size_t ngen) {

    std::cerr << "Initializing mcmc chain ..." << std::endl;

    /* Get the dag nodes from the model */
    std::vector<RbPtr<DAGNode> > dagNodes = (static_cast<Model*>( getMemberValue("model") ) )->getDAGNodes();

    /* Get the moves and monitors */
    Vector* monitors = static_cast<Vector*>( getMemberDagNode( "monitors" )->getValue() );
    Vector*    moves = static_cast<Vector*>( getMemberDagNode( "moves"    )->getValue() );

    /* Get the chain settings */
    std::cerr << "Getting the chain settings ..." << std::endl;

    RandomNumberGenerator* rng        = GLOBAL_RNG;

    /* Open the output file and print headers */
    std::cerr << "Opening file and printing headers ..." << std::endl;
    for (size_t i=0; i<monitors->size(); i++) {
        // get the monitor
        FileMonitor* theMonitor = static_cast<FileMonitor*>( monitors->getElement(i) );
        
        // open the file stream for the monitor
        theMonitor->openStream();
        
        // print the header information
        theMonitor->printHeader();
    }


    /* Get initial lnProbability of model */
    double lnProbability = 0.0;
    std::vector<double> initProb;
    for (std::vector<RbPtr<DAGNode> >::iterator i=dagNodes.begin(); i!=dagNodes.end(); i++) {
        DAGNode* node = (*i);
        if (node->isType(StochasticNode_name)) {
            StochasticNode* stochNode = dynamic_cast<StochasticNode*>( node );
            double lnProb = stochNode->calculateLnProbability();
            lnProbability += lnProb;
//            initProb.push_back(lnProb);
        }
        node->keep();
    }
    std::cerr << "Initial lnProbability = " << lnProbability << std::endl;

    /* Run the chain */
    std::cerr << "Running the chain ..." << std::endl;

    std::cout << std::endl;
    std::cout << "Gen\tlnProbability" << std::endl;
    
    /* Monitor */
    for (size_t i=0; i<monitors->size(); i++) {
        static_cast<FileMonitor*>( monitors->getElement(i) )->monitor(0);
    }

    for (unsigned int gen=1; gen<=ngen; gen++) {

        for (size_t i=0; i<moves->size(); i++) {
            /* Get the move */
            Move* theMove = static_cast<Move*>( moves->getElement(i) );

            /* Propose a new value */
            double lnProbabilityRatio;
            double lnHastingsRatio = theMove->performMove(lnProbabilityRatio);
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
                theMove->acceptMove();
                lnProbability += lnProbabilityRatio;
            }
            else {
                theMove->rejectMove();
            }

#ifdef DEBUG_MCMC
            /* Assert that the probability calculation shortcuts work */
            double curLnProb = 0.0;
            std::vector<double> lnRatio;
            for (std::vector<DAGNode*>::iterator i=dagNodes.begin(); i!=dagNodes.end(); i++) {
                if ((*i)->isType(StochasticNode_name)) {
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
        for (size_t i=0; i<monitors->size(); i++) {
            static_cast<FileMonitor*>( (RbObject*)monitors->getElement(i) )->monitor(gen);
        }

        /* Print to screen */
        std::cout << gen << "\t" << lnProbability << std::endl;

    }

    std::cerr << "Finished chain" << std::endl;

    std::cout << std::endl;
}

