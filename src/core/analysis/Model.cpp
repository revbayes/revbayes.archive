#include "Model.h"

#include "DagNode.h"
#include "RbException.h"

using namespace RevBayesCore;

Model::Model(const DagNode *source) {
    
    // add this node and build model graph
    addSourceNode( source );
    DagNode* newSource = nodesMap.find( source )->second;
    sources.insert( newSource );
    
}


Model::Model(const std::set<const DagNode*> s) : sources() {
    
    // iterate over all sources
    for (std::set<const DagNode*>::const_iterator it = s.begin(); it != s.end(); ++it) 
    {
        // add this node and build model graph
        addSourceNode( *it );
        DagNode* newSource = nodesMap.find( *it )->second;
        sources.insert( newSource );
    }
    
}


Model::Model(const Model &m) : sources() {
    
    // iterate over all sources
    for (std::set<const DagNode*>::const_iterator it = m.sources.begin(); it != m.sources.end(); ++it) 
    {
        // add this node and build model graph
        addSourceNode( *it );
        DagNode* newSource = nodesMap.find( *it )->second;
        sources.insert( newSource );
    }
    
}


Model::~Model( void ) {
    
    for (std::vector<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it) 
    {
        DagNode *theNode = *it;
        delete theNode;
    }
    
}


void Model::addSourceNode(const DagNode *sourceNode) {
    
    if (sourceNode == NULL)
        throw RbException("Cannot instantiate a model with a NULL DAG node.");
    
    sourceNode->cloneDAG(nodesMap);
    
    // add the source node to our set of sources
//    sourceNodes.insert( (DAGNode*)nodesMap[theSourceNode] );
        
    /* insert new nodes into direct access vector */
    std::map<const DagNode*, DagNode* >::iterator i = nodesMap.begin();
    
    nodes.clear();
    while ( i != nodesMap.end() ) 
    {
        
        DagNode* theNewNode = (*i).second;
        
        // increment the iterator;
        ++i;
            
        // insert in direct access vector
        nodes.push_back( theNewNode );
    }
    
}


Model* Model::clone( void ) const {
    return new Model( *this );
}



const std::vector<DagNode *>& Model::getDagNodes( void ) const {
    
    return nodes;
}


std::vector<DagNode *>& Model::getDagNodes( void ) {
    
    return nodes;
}


const std::map<const DagNode*, DagNode*>& Model::getNodesMap( void ) const {
    
    return nodesMap;
}


const std::map<const DagNode*, std::set<DagNode*> >& Model::getReplacementMap( void ) const {
    
    return replacementMap;
}
