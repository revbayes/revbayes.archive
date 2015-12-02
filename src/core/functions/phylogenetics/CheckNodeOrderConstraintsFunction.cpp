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
    try {
    // add the constraints parameter as a parent
    addParameter( tau );
    addParameter( constraints );
    updateSetOfConstrainedNodes();
    
    update();
}
catch (RbException e)
{
    std::cerr << e.getMessage() << std::endl;
    }

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
        if ( nodeAges.at(constra[i].first) <  nodeAges.at(constra[i].second) ) {
            return false;
        }
    }
    return true;
}


void CheckNodeOrderConstraintsFunction::update( void )
{
    (*value) = Boolean(false);
    try {
        updateMapOfNodeAges();
    }
    catch (RbException e)
    {
        std::cerr << e.getMessage() << std::endl;
    }

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


bool CheckNodeOrderConstraintsFunction::isInVector(std::vector<TopologyNode> nv, const TopologyNode n)
{
    for (const auto& elem: nv) {
        if (elem.equals ( n ) )
        {
            return true;
        }
    }
    return false;
}


void CheckNodeOrderConstraintsFunction::climbUpTheTree(const TopologyNode& node, boost::unordered_set <const TopologyNode* >& pathFromNodeToRoot) {
    try {
        if (! (node.isRoot( ) ) ) {
            pathFromNodeToRoot.insert(&node);
            climbUpTheTree(node.getParent(), pathFromNodeToRoot);
        }
    }
    catch (RbException e)
    {
        throw e;
    }

    return;
}


double CheckNodeOrderConstraintsFunction::getAgeOfMRCARecursive(const TopologyNode& node, boost::unordered_set <const TopologyNode* >& pathFromOtherNodeToRoot) {
    try {

    if ( node.isRoot() || pathFromOtherNodeToRoot.find(&node) != pathFromOtherNodeToRoot.end() ) {
        return node.getAge();
    }
    else {
        return getAgeOfMRCARecursive( node.getParent(), pathFromOtherNodeToRoot );
    }
    }
    catch (RbException e)
    {
        throw e;
    }

    
}



double CheckNodeOrderConstraintsFunction::getAgeOfMRCA(std::string first, std::string second) {
    
        const TopologyNode &node1 = tau->getValue().getTipNodeWithName(first) ;

        const TopologyNode &node2 = tau->getValue().getTipNodeWithName(second) ;
        
        if (! (node2.equals( node1 ) ) )
        {
            boost::unordered_set <const TopologyNode* > pathFromNode1ToRoot;
            climbUpTheTree(node1, pathFromNode1ToRoot);
            
            double age = getAgeOfMRCARecursive(node2, pathFromNode1ToRoot);
            return age;
        }
        else {
            return node1.getAge();
        }

    return 0.4;
}



//Here we compute node ages from the current tree.
void CheckNodeOrderConstraintsFunction::updateMapOfNodeAges()
{
  
    
    nodeAges.clear();
    for (const auto& elem: constrainedNodes) {
        nodeAges[elem] = getAgeOfMRCA(elem.first, elem.second);
    }
    
    
    
    
    
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

















