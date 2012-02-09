/**
 * @file
 * This file contains the implementation of Func_nj, a class
 * used to construct a distance matrix.
 *
 * @brief Implementation of Func_nj
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id$
 */

#include "DistanceMatrix.h"
#include "Func_nj.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbString.h"
#include "StringUtilities.h"
#include "Topology.h"
#include "TopologyNode.h"
#include "TreePlate.h"
#include "UserInterface.h"
#include "ValueRule.h"
#include "VectorString.h"
#include <cmath>
#include <sstream>
#include <vector>


// Definition of the static type spec member
const TypeSpec Func_nj::typeSpec(Func_nj_name);
const TypeSpec Func_nj::returnTypeSpec(TreePlate_name);

/** Clone object */
Func_nj* Func_nj::clone(void) const {

    return new Func_nj(*this);
}


/** Execute function */
RbLanguageObject* Func_nj::executeFunction(void) {

    // get the information from the arguments for reading the file
    DistanceMatrix* d           = static_cast<DistanceMatrix*>( (*args)[0]->getValue() );
    RbString*       useBioNj    = static_cast<RbString*>      ( (*args)[1]->getValue() );
    RbString*       tieBreaking = static_cast<RbString*>      ( (*args)[2]->getValue() );
        
    Topology* top = neighborJoining(d);
    
    TreePlate* t = new TreePlate(top);
    
    return t;
}


/** Get argument rules */
const ArgumentRules* Func_nj::getArgumentRules(void) const {

    static ArgumentRules* argumentRules = new ArgumentRules();
    static bool           rulesSet      = false;

    if (!rulesSet)
        {
        argumentRules->push_back( new ValueRule( "d",     DistanceMatrix_name ) );
        argumentRules->push_back( new ValueRule( "bionj", RbString_name       ) );
        argumentRules->push_back( new ValueRule( "ties",  RbString_name       ) );
        rulesSet = true;
        }

    return argumentRules;
}


/** Get class vector describing type of object */
const VectorString& Func_nj::getClass(void) const {

    static VectorString rbClass = VectorString( Func_nj_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec& Func_nj::getReturnType(void) const {

    return returnTypeSpec;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Func_nj::getTypeSpec(void) const {

    return typeSpec;
}

void Func_nj::buildNj(std::vector<std::vector<double> > distances, std::vector<TopologyNode*> nodes, int nTips) {

    if (nTips > 2)
        {
        // calculate the u_i for each tip
        std::vector<double> ui;
        for (size_t i=0; i<nTips; i++)
            {
            double sum = 0.0;
            for (size_t j=0; j<nTips; j++)
                {
                if (i != j)
                    sum += distances[i][j];
                }
            sum /= (nTips - 2);
            ui.push_back(sum);
            }
            
        // find i-j tip pair for which d_ij - ui - uj is smallest
        int smallestI = -1, smallestJ = -1;
        double v = -1.0;
        for (int i=0; i<nTips; i++)
            {
            for (int j=i+1; j<nTips; j++)
                {
                double x = distances[i][j] - ui[i] - ui[j];
                if (v < 0.0 || x < v)
                    {
                    smallestI = i;
                    smallestJ = j;
                    v = x;
                    }
                }
            }
            
        // join tips i and j
        TopologyNode* ndeI = nodes[smallestI];
        TopologyNode* ndeJ = nodes[smallestJ];
        TopologyNode* rootNode = ndeI->getParent();
        if (ndeJ->getParent() != rootNode)
            {
            // problem
            }
        rootNode->removeChild(ndeI);
        rootNode->removeChild(ndeJ);
        TopologyNode* newNode = new TopologyNode;
        newNode->setParent(rootNode);
        rootNode->addChild(newNode);
        newNode->addChild(ndeI);
        newNode->addChild(ndeJ);
        ndeI->setParent(newNode);
        ndeJ->setParent(newNode);
        
        // construct a new distance matrix that is one row and column smaller
        std::vector<std::vector<double> > newDistances;
        newDistances.resize(nTips-1);
        for (size_t i=0; i<nTips-1; i++)
            newDistances[i].resize(nTips-1);
        std::vector<TopologyNode*> newNodes;
        for (size_t i=0; i<nTips; i++)  
            {
            if (i != smallestI && i != smallestJ)
                {
                newNodes.push_back(nodes[i]);
                for (size_t j=0; j<nTips; j++)
                    {
                    
                    }
                }
            }
        }
    else if (nTips == 2)
        {
        
        }
}

Topology* Func_nj::neighborJoining(DistanceMatrix* d) {

    // allocate a topology
    Topology* topo = new Topology;
    
    // set the initial tree to be a start tree
    size_t nTaxa = d->getNumberOfTaxa();
    TopologyNode* rootNode = new TopologyNode;
    topo->setRoot(rootNode);
    std::vector<TopologyNode*> activeNodes;
    for (int i=0; i<nTaxa; i++)
        {
        TopologyNode* nde = new TopologyNode;
        nde->setName(d->getTaxonNameWithIndex(i));
        nde->setParent(rootNode);
        //nde->setIndex(i);
        rootNode->addChild(nde);
        activeNodes.push_back(nde);
        }
        
    // initialize a distance matrix
    std::vector<std::vector<double> > activeDistances = d->getValue();
        
    // recursively build the neighbor-joining tree
    buildNj(activeDistances, activeNodes, nTaxa);
    
    return topo;
}
