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

#include "ConstArgumentRule.h"
#include "ConstantNode.h"
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
#include <cmath>
#include <iomanip>
#include <sstream>
#include <vector>


/** Clone object */
Func_nj* Func_nj::clone(void) const {

    return new Func_nj(*this);
}


/** Execute function */
RbPtr<RbLanguageObject> Func_nj::executeFunction( const std::vector<const RbObject*> &args ) {

    // get the information from the arguments for reading the file
    const DistanceMatrix& dm          = static_cast<const DistanceMatrix&>( *args[0] );
    const RbString&       useBioNj    = static_cast<const RbString&>      ( *args[1] );
    const RbString&       tieBreaking = static_cast<const RbString&>      ( *args[2] );
        
    Topology* top = neighborJoining(dm);
    
    TreePlate *tree = new TreePlate();
    tree->setMember("topology", new Variable( new ConstantNode(top) ) );
    
    return RbPtr<RbLanguageObject>( tree );
}


/** Get argument rules */
const ArgumentRules& Func_nj::getArgumentRules(void) const {

    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet      = false;

    if (!rulesSet)
        {
        argumentRules.push_back( new ConstArgumentRule( "d",     DistanceMatrix::getClassTypeSpec() ) );
        argumentRules.push_back( new ConstArgumentRule( "bionj", RbString::getClassTypeSpec()       ) );
        argumentRules.push_back( new ConstArgumentRule( "ties",  RbString::getClassTypeSpec()       ) );
        rulesSet = true;
        }

    return argumentRules;
}


/** Get class name of object */
const std::string& Func_nj::getClassName(void) { 
    
    static std::string rbClassName = "Neighbor joining function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_nj::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_nj::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_nj::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = TreePlate::getClassTypeSpec();
    return returnTypeSpec;
}

void Func_nj::buildNj(std::vector<std::vector<double> > distances, std::vector<TopologyNode*> nodes, int nTips) {

    if (nTips > 2)
        {
        for (size_t i=0; i<distances.size(); i++)
            {
            for (size_t j=0; j<distances[i].size(); j++)
                std::cout << std::fixed << std::setprecision(4) << distances[i][j] << " ";
            std::cout << std::endl;
            }
        for (size_t i=0; i<nodes.size(); i++)
            std::cout << nodes[i] << " ";
        std::cout << std::endl;

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
        for (size_t i=0; i<ui.size(); i++)
            std::cout << "u[" << i << "] = " << ui[i] << std::endl;
            
        // find i-j tip pair for which d_ij - ui - uj is smallest
        int smallestI = -1, smallestJ = -1;
        double v = 1.0e100;
        for (size_t i=0; i<nTips; i++)
            {
            for (size_t j=i+1; j<nTips; j++)
                {
                double x = distances[i][j] - ui[i] - ui[j];
                std::cout << i << " " << j << " " << x << std::endl;
                if (x < v)
                    {
                    smallestI = i;
                    smallestJ = j;
                    v = x;
                    }
                }
            }
        std::cout << "smallestI = " << smallestI << std::endl;
        std::cout << "smallestJ = " << smallestJ << std::endl;
        std::cout << "v = " << v << std::endl;
            
        // join tips i and j
        TopologyNode* ndeI = nodes[smallestI];
        TopologyNode* ndeJ = nodes[smallestJ];
        TopologyNode* newNode = new TopologyNode;
        newNode->addChild(ndeI);
        newNode->addChild(ndeJ);
        ndeI->setParent(newNode);
        ndeJ->setParent(newNode);
        
        // construct a new distance matrix that is one row and one column smaller
        std::vector<std::vector<double> > newDistances(nTips-1);
        
        std::vector<TopologyNode*> newNodes;
        for (size_t i=0, k=0; i<nTips; i++)  
            {
            if (i != smallestI && i != smallestJ)
                {
                newNodes.push_back(nodes[i]);
                for (size_t j=0; j<nTips; j++)
                    {
                    if (j != smallestI && j != smallestJ)
                        newDistances[k].push_back(distances[i][j]);
                    }
                double x = distances[i][smallestI] + distances[i][smallestJ] - distances[smallestI][smallestJ];
                newDistances[k].push_back(x);
                newDistances[nTips-2].push_back(x);
                k++;
                }
            }
        newNodes.push_back(newNode);
        newDistances[nTips-2].push_back( 0.0 );
        
        // recursively call function again
        buildNj(newDistances, newNodes, nTips-1);
        }
    else if (nTips == 2)
        {
        /*TopologyNode* ndeI = nodes[0];
        TopologyNode* ndeJ = nodes[1];
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
        ndeJ->setParent(newNode);*/
        }
}

Topology* Func_nj::neighborJoining(const DistanceMatrix& d) {

    // allocate a topology
    Topology* topo = new Topology;
    
    // set the initial tree to be a start tree
    size_t nTaxa = d.getNumberOfTaxa();
    std::vector<TopologyNode*> activeNodes;
    for (int i=0; i<nTaxa; i++)
        {
        TopologyNode* nde = new TopologyNode();
        nde->setName(d.getTaxonNameWithIndex(i));
        activeNodes.push_back(nde);
        }
        
    // initialize a distance matrix
    std::vector<std::vector<double> > activeDistances;
    activeDistances.resize(nTaxa);
    for (int i=0; i<nTaxa; i++)
        activeDistances[i].resize(nTaxa);
    for (int i=0; i<nTaxa; i++)
        for (int j=0; j<nTaxa; j++)
            activeDistances[i][j] = d.getDistance(i, j);
            
    // recursively build the neighbor-joining tree
    buildNj(activeDistances, activeNodes, (int)nTaxa);
    
    // after the neighbor joining the matrix should only contain 1 node, which is our new root
    topo->setRoot(activeNodes[0]);
    TreePlate* tp = new TreePlate();
    tp->setMember("topology", new Variable( new ConstantNode(topo) ) );
    
    tp->printValue(std::cout);
//    std::string newickStr = tp->buildNewickString(topo->getRoot());
//    std::cout << newickStr << std::endl;
    
    return topo;
}

