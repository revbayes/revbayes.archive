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
#include "Mcmc.h"
#include "MemberFunction.h"
#include "MinmaxRule.h"
#include "Model.h"
#include "Move.h"
#include "MoveSchedule.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "Integer.h"
#include "RbNames.h"
#include "RbString.h"
#include "StochasticNode.h"
#include "VectorString.h"
#include "VariableNode.h"
#include "Workspace.h"
#include "WrapperRule.h"

#include <cmath>
#include <fstream>
#include <sstream>
#include <string>


/** Constructor passes member rules and method inits to base class */
Mcmc::Mcmc(void)
    : MemberObject(getMemberRules(), getMethodInits()) {
}


/** Constructor for internal use */
Mcmc::Mcmc(Model* model, int ngen, int printfreq, int samplefreq, std::string filename)
    : MemberObject(getMemberRules(), getMethodInits()) {

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
DAGNode* Mcmc::executeOperation(const std::string& name, std::vector<DAGNode*>& args) {

    if (name == "update") {
       update();
    }
    else
        throw RbException("No member method called '" + name + "'");

    return NULL;
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

        memberRules.push_back(new ArgumentRule("model",      Model_name));
        memberRules.push_back(new MinmaxRule(  "ngen",       Integer_name, new Integer(1), NULL));
        memberRules.push_back(new MinmaxRule(  "printfreq",  Integer_name, new Integer(1), NULL));
        memberRules.push_back(new MinmaxRule(  "samplefreq", Integer_name, new Integer(1), NULL));
        memberRules.push_back(new ArgumentRule("filename",   new RbString("out")));

        rulesSet = true;
    }

    return memberRules;
}


/** Get method specifications */
const MethodTable& Mcmc::getMethodInits(void) const {

    static MethodTable   methodInits;
    static ArgumentRules updateArgRules;
    static bool          initsSet = false;

    if (!initsSet) {

        methodInits.addFunction("update", new MemberFunction(RbNULL_name, updateArgRules));

        initsSet = true;
    }

    return methodInits;
}


/** Allow only constant member variables */
void Mcmc::setVariable(const std::string& name, DAGNode* var) {

    if (!var->isDAGType(ConstantNode_name))
        throw RbException("Only constant member values allowed");

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
            theNode->getValue();

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

    /* Get initial lnProbability of model */
    double lnProbability = 0.0;
    std::vector<double> initProb;
    for (std::vector<StochasticNode*>::iterator i=stochasticNodes.begin(); i!=stochasticNodes.end(); i++) {
        lnProbability += (*i)->calculateLnProbability();
        initProb.push_back((*i)->calculateLnProbability());
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
        double lnPriorRatio, lnLikelihoodRatio, lnHastingsRatio;
        theMove->performMove(lnPriorRatio, lnLikelihoodRatio, lnHastingsRatio);

        /* Calculate acceptance ratio */
        double lnR = lnPriorRatio + lnLikelihoodRatio + lnHastingsRatio;
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
            lnProbability += lnPriorRatio + lnLikelihoodRatio;
        }
        else {
            theMove->rejectMove();
        }

        /* Monitor; TODO: Just printing all variable nodes for now */
        if (gen % samplefreq == 0)
        for (std::vector<VariableNode*>::iterator i=variableNodes.begin(); i!=variableNodes.end(); i++) {
            (*i)->printValue(outFile);
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
                 curLnProb += (*i)->calculateLnProbability();
                 lnRatio.push_back(initProb[i-stochasticNodes.begin()] - (*i)->calculateLnProbability());
            }
            if (fabs(lnProbability - curLnProb) > 1E-8)
                throw RbException("Error in ln probability calculation shortcuts");
        }
#endif

    }

    std::cerr << "Finished chain" << std::endl;

    std::cout << std::endl;
}

