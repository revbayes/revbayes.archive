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
#include "ContainerIterator.h"
#include "Integer.h"
#include "Mcmc.h"
#include "MemberFunction.h"
#include "Model.h"
#include "Move.h"
#include "MoveSchedule.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RangeRule.h"
#include "ReferenceRule.h"
#include "RbNames.h"
#include "RbString.h"
#include "StochasticNode.h"
#include "ValueRule.h"
#include "VectorString.h"
#include "VariableNode.h"
#include "Workspace.h"

#include <cmath>
#include <fstream>
#include <sstream>
#include <string>


/** Constructor passes member rules and method inits to base class */
Mcmc::Mcmc(void)
    : MemberObject(getMemberRules()) {
}


/** Constructor for internal use */
Mcmc::Mcmc(Model* model, int ngen, int printfreq, int samplefreq, std::string filename)
    : MemberObject(getMemberRules()) {

    setValue("model",      model);
    setValue("ngen",       new Integer(ngen));
    setValue("printfreq",  new Integer(printfreq));
    setValue("samplefreq", new Integer(samplefreq));
    setValue("filename",   new RbString(filename));
}


/** Clone object */
Mcmc* Mcmc::clone(void) const {

    return new Mcmc(*this);
}


/** Map calls to member methods */
DAGNode* Mcmc::executeOperation(const std::string& name, ArgumentFrame& args) {

    if (name == "update") {
       update();
       return NULL;
    }

    return MemberObject::executeOperation( name, args );
}


/** Get class vector describing type of object */
const VectorString& Mcmc::getClass(void) const {

    static VectorString rbClass = VectorString(Mcmc_name) + MemberObject::getClass();
    return rbClass;
}


/** Get member rules */
const MemberRules& Mcmc::getMemberRules(void) const {

    static MemberRules memberRules;
    static bool        rulesSet = false;

    if (!rulesSet) {

        memberRules.push_back( new ValueRule         ( "model"     , Model_name                         ) );
        memberRules.push_back( new RangeRule<Integer>( "ngen"      , new Integer(1), Integer(1), Integer(INT_MAX) ) );
        memberRules.push_back( new RangeRule<Integer>( "printfreq" , new Integer(1), Integer(1), Integer(INT_MAX) ) );
        memberRules.push_back( new RangeRule<Integer>( "samplefreq", new Integer(1), Integer(1), Integer(INT_MAX) ) );
        memberRules.push_back( new ValueRule         ( "filename"  , new RbString( "out" )              ) );

        rulesSet = true;
    }

    return memberRules;
}


/** Get methods */
const MethodTable& Mcmc::getMethods(void) const {

    static MethodTable   methods;
    static ArgumentRules updateArgRules;
    static bool          methodsSet = false;

    if (!methodsSet) {

        updateArgRules.push_back( new ReferenceRule( "", MemberObject_name ) );
        methods.addFunction("update", new MemberFunction(RbNULL_name, updateArgRules));

        methods.setParentTable( const_cast<MethodTable*>( &MemberObject::getMethods() ) );
        methodsSet = true;
    }

    return methods;
}


/** Allow only constant member variables */
void Mcmc::setVariable(const std::string& name, DAGNode* var) {

    if ( name != "model" && !var->isDAGType( ConstantNode_name ) )
        throw RbException( "Only constant member values allowed" );

    if ( name == "model" && members["model"].getVariable()->getValue() != NULL )
        throw RbException( "Cannot reset model" );

    MemberObject::setVariable(name, var);
}


/** Run the mcmc chain */
void Mcmc::update(void) {

    std::cerr << "Initializing mcmc chain ..." << std::endl;

    /* Get the dag nodes from the model */
    std::vector<DAGNode*>& dagNodes = ((Model*)(getValue("model")))->getDAGNodes();

    /* Get the moves, monitors, variable nodes and stochastic nodes */
    std::vector<Monitor*>        monitors;
    std::map<double, Move*>      tempMoves;
    double                       sumWeights = 0.0;
    std::vector<StochasticNode*> stochasticNodes;
    std::vector<VariableNode*>   variableNodes;

    for (std::vector<DAGNode*>::iterator i=dagNodes.begin(); i!=dagNodes.end(); i++) {
        if ((*i)->isDAGType(VariableNode_name)) {

            VariableNode*          theNode         = dynamic_cast<VariableNode*>(*i);
            std::vector<Monitor*>  theMonitors     = theNode->getDefaultMonitors();
            MoveSchedule*          theMoveSchedule;
            if (theNode->isDAGType(StochasticNode_name) && ((StochasticNode*)(theNode))->isClamped())
                theMoveSchedule = NULL;
            else
                theMoveSchedule = theNode->getDefaultMoves();

            /* Update value of all variable nodes in case the parser left them in 'dirty' state */
            theNode->keep();

            /* Get monitor */
            for (std::vector<Monitor*>::iterator j=theMonitors.begin(); j!=theMonitors.end(); j++)
                monitors.push_back(*j);

            /* Get moves */
            if (theMoveSchedule != NULL) {
                double              nodeWeight = theMoveSchedule->getUpdateWeight();
                std::vector<Move*>& theMoves   = theMoveSchedule->getMoves();
                for (std::vector<Move*>::iterator j=theMoves.begin(); j!=theMoves.end(); j++) {
                    double weight = (*j)->getUpdateWeight() * nodeWeight;
                    sumWeights += weight;
                    tempMoves.insert(std::pair<double, Move*>(sumWeights, *j));
                }
            }

            variableNodes.push_back(theNode);
            if (theNode->isDAGType(StochasticNode_name)) {
                stochasticNodes.push_back((StochasticNode*)(theNode));
            }
        }
    }

    /* Correct the temporary move map so the key is the lower cumulative probability of the move */
    std::map<double, Move*> moves;
    double cumulativeProb = 0.0;
    for (std::map<double, Move*>::iterator i=tempMoves.begin(); i!=tempMoves.end(); i++) {
        cumulativeProb = (*i).first / sumWeights;
        moves.insert(std::pair<double, Move*>(cumulativeProb, (*i).second));
    }
    tempMoves.clear();

    /* Get the chain settings */
    std::cerr << "Getting the chain settings ..." << std::endl;

    int                    ngen       = ((Integer*)(getValue("ngen")))->getValue();
    int                    printfreq  = ((Integer*)(getValue("printfreq")))->getValue();
    int                    samplefreq = ((Integer*)(getValue("samplefreq")))->getValue();
    std::string            fileName   = ((RbString*)(getValue("filename")))->getValue();
    RandomNumberGenerator* rng        = GLOBAL_RNG;

    /* Open the output file and print headers */
    std::cerr << "Opening file and printing headers ..." << std::endl;

    std::ofstream outFile(fileName.c_str());
    outFile << "gen";
    for (std::vector<VariableNode*>::iterator i=variableNodes.begin(); i!=variableNodes.end(); i++) {
        outFile << "\t";
        if ((*i)->getName() == "")
            outFile << "Unnamed " << (*i)->getDAGType();
        else
            outFile << (*i)->getName();
    }
    outFile << std::endl;

    /* Print starting values to outfile */
    for (std::vector<VariableNode*>::iterator i=variableNodes.begin(); i!=variableNodes.end(); i++) {
        (*i)->printValue(outFile);
        outFile << std::endl;
    }

    /* Get initial lnProbability of model */
    double lnProbability = 0.0;
    std::vector<double> initProb;
    for (std::vector<StochasticNode*>::iterator i=stochasticNodes.begin(); i!=stochasticNodes.end(); i++) {
        double lnProb = (*i)->calculateLnProbability();
        lnProbability += lnProb;
        initProb.push_back(lnProb);
    }
    std::cerr << "Initial lnProbability = " << lnProbability << std::endl;

    /* Run the chain */
    std::cerr << "Running the chain ..." << std::endl;

    std::cout << std::endl;
    std::cout << "Gen\tlnProbability" << std::endl;

    for (int gen=1; gen<ngen; gen++) {

        /* Get the move */
        double u = rng->uniform01();
        std::map<double, Move*>::iterator it = moves.upper_bound(u);
        Move* theMove = (*it).second;

        /* Propose a new value */
        double lnProbabilityRatio, lnHastingsRatio;
        theMove->performMove(lnProbabilityRatio, lnHastingsRatio);

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
        u = rng->uniform01();
        if (u < r) {
            theMove->acceptMove();
            lnProbability += lnProbabilityRatio;
        }
        else {
            theMove->rejectMove();
        }

        /* Monitor */
        //! @todo : Do appropriate monitoring; just printing all variable nodes for now */
        if (gen % samplefreq == 0) {
            for (std::vector<VariableNode*>::iterator i=variableNodes.begin(); i!=variableNodes.end(); i++) {
                (*i)->printValue(outFile);
            }
            outFile << std::endl;
        }

        /* Print to screen */
        if (gen % printfreq == 0)
            std::cout << gen << "\t" << lnProbability << std::endl;

#ifndef NDEBUG
        /* Assert that the probability calculation shortcuts work */
        if (gen % samplefreq == 0) {
            double curLnProb = 0.0;
            std::vector<double> lnRatio;
            for (std::vector<StochasticNode*>::iterator i=stochasticNodes.begin(); i!=stochasticNodes.end(); i++) {
                 double lnProb = (*i)->calculateLnProbability();
                 curLnProb += lnProb;
                 lnRatio.push_back(initProb[i-stochasticNodes.begin()] - lnProb);
            }
            if (fabs(lnProbability - curLnProb) > 1E-8)
                throw RbException("Error in ln probability calculation shortcuts");
        }
#endif

    }

    std::cerr << "Finished chain" << std::endl;

    std::cout << std::endl;
}

