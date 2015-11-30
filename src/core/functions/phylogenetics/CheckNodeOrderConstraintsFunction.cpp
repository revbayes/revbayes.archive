//
//  CheckNodeOrderConstraintsFunction.cpp
//  revbayes-proj
//
//  Created by Bastien Boussau on 16/11/15.
//  Copyright (c) 2015 Bastien Boussau. All rights reserved.
//

#include "CheckNodeOrderConstraintsFunction.h"
#include "RbException.h"

using namespace RevBayesCore;

CheckNodeOrderConstraintsFunction::CheckNodeOrderConstraintsFunction(const TypedDagNode<Tree> *t, const TypedDagNode<RelativeNodeAgeConstraints> *co) : TypedFunction<Boolean>( new Boolean(false) ),
tau( t ),
constraints( co ),
nodeAges(),
constrainedNodes()
{
    // add the constraints parameter as a parent
    addParameter( tau );
    addParameter( constraints );
    
    update();
}


CheckNodeOrderConstraintsFunction::~CheckNodeOrderConstraintsFunction( void )
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



CheckNodeOrderConstraintsFunction* CheckNodeOrderConstraintsFunction::clone( void ) const
{
    return new CheckNodeOrderConstraintsFunction( *this );
}


void CheckNodeOrderConstraintsFunction::keep(DagNode *affecter)
{
    //delegate to base class
    TypedFunction< Boolean >::keep( affecter );
    
}


void CheckNodeOrderConstraintsFunction::reInitialized( void )
{
    *value = Boolean(false);
}


void CheckNodeOrderConstraintsFunction::restore(DagNode *restorer)
{
    //delegate to base class
    TypedFunction< Boolean >::restore( restorer );
}


void CheckNodeOrderConstraintsFunction::touch(DagNode *toucher)
{
    
    //delegate to base class
    TypedFunction< Boolean >::touch( toucher );
    
}


bool CheckNodeOrderConstraintsFunction::checkNodeAgeConstraints ( void )
{
    std::vector <std::pair < std::pair<std::string, std::string>, std::pair<std::string, std::string> > > constra = constraints->getValue().getConstraints();
    for (size_t i = 0; i < constra.size() ; ++i) {
        if ( nodeAges[constra[i].first] <  nodeAges[constra[i].second] ) {
            return false;
        }
    }
    return true;
}


void CheckNodeOrderConstraintsFunction::update( void )
{
    (*value) = Boolean(false);
    
    updateMapOfNodeAges();

    (*value) = Boolean( checkNodeAgeConstraints() );

    return;
    
}


void CheckNodeOrderConstraintsFunction::updateSetOfConstrainedNodes()
{
    std::vector <std::pair < std::pair<std::string, std::string>, std::pair<std::string, std::string> > > constra = constraints->getValue().getConstraints();
    for (size_t i = 0; i < constra.size() ; ++i) {
        constrainedNodes.insert(constra[i].first);
        constrainedNodes.insert(constra[i].second);
    }
    return;
}


double CheckNodeOrderConstraintsFunction::getAgeOfMRCA(std::string first, std::string second) {
    TopologyNode node1 = tau->getValue().getTipNodeWithName(first);
    TopologyNode node2 = tau->getValue().getTipNodeWithName(second);
    if (! (node2.equals( node1 ) ) )
    {
        TopologyNode root = tau->getValue().getRoot();
        TopologyNode *n = &node1;
        std::unordered_set <TopologyNode* > pathFromNode1ToRoot;
        while (! (n->equals( root ) ) )
        {
            pathFromNode1ToRoot.insert( n );
            n = &(n->getParent() );
        }
        std::vector <TopologyNode* > pathFromNode2ToRoot;
        bool found = false;
        n = &node2;
        while (!found) {
            pathFromNode2ToRoot.push_back( n );
            n = &(n->getParent() );
            if ( pathFromNode1ToRoot.find(n) != pathFromNode1ToRoot.end() ) {
                found = true;
            }
        }
        return n->getAge();//tau->getValue().getNode( n ).getAge();
    }
    else {
        return node1.getAge();//tau->getValue().getNode( node1 ).getAge();
    }

    
}



void CheckNodeOrderConstraintsFunction::updateMapOfNodeAges()
{
    nodeAges.clear();
    for (const auto& elem: constrainedNodes) {
        nodeAges[elem] = -1.0;
        getAgeOfMRCA(elem.first, elem.second);
    }
    
    //Now we compute node ages from the current tree.
    //There must be a smart and efficient way of doing that.
    //For the moment we do it dumb and slow.

    /*Attempt at a smart algorithm
    std::unordered_set<std::string> tipsToCareAbout;
    for (const auto& elem: constrainedNodes) {
        nodeAges[elem] = -1.0;
        tipsToCareAbout.insert(elem.first);
        tipsToCareAbout.insert(elem.second);
    }
    //First find all tip nodes of interest
    std::map < TopologyNode*, bool > nodesOfInterest;
    for (size_t i = 0; i < tree->getValue().getNumberOfTips()-1; ++i)
    {
        TopologyNode *node = n[i];
        if (tipsToCareAbout.contains( node->getName()) ) {
            nodesOfInterest[node] = true;
        }
    }
    //Now for internal nodes
    for (size_t i = tree->getValue().getNumberOfTips(); i < n.size(); ++i)
    {
        TopologyNode *node = n[i];
        std::vector< TopologyNode* > kids = node->getChildren();
        bool doneOne = false;
        for (size_t j = 0; j< kids.size() ++j) {
            if ( nodesOfInterest[kids[j]] )
            {
                nodesOfInterest[node] = true;
                if (doneOne)
                {
                    //Two children of interest, it may be a node whose age we'd like to know
                    //...
                }
                else {
                    doneOne = true;
                }
            }
        }
        
    }
     
     */
    
    return;
    
}


void CheckNodeOrderConstraintsFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == tau)
    {
        tau = static_cast<const TypedDagNode<Tree>* >( newP );
    }
    else if (oldP == constraints)
    {
        constraints = static_cast<const TypedDagNode<RelativeNodeAgeConstraints>* >( newP );
        updateSetOfConstrainedNodes();
    }
}























