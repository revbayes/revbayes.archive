#include "Model.h"

#include "DagNode.h"
#include "RbException.h"

using namespace RevBayesCore;


/**
 * Constructor from a single DAG node.
 * The model graph is extracted by obtaining all DAG nodes connected to the provide source node.
 * The entire model graph is copied and a map between the pointers to the original DAG nodes and
 * the copied DAG nodes is created for convenient access.
 *
 * \param[in]    source    The DAG node from which the model graph is extracted.
 */
Model::Model(const DagNode *source) 
{
    
    // add this node to the source nodes and build model graph
    addSourceNode( source );
    
}


/**
 * Constructor from a set of DAG nodes.
 * The model graph is extracted by obtaining all DAG nodes that are connected to either of the provide source nodes.
 * The entire model graph is copied and a map between the pointers to the original DAG nodes and
 * the copied DAG nodes is created for convenient access.
 *
 * \param[in]    sources    The set of DAG nodes from which the model graph is extracted.
 */
Model::Model(const std::set<const DagNode*> &s) :
    sources() 
{
    
    // iterate over all sources
    for (std::set<const DagNode*>::const_iterator it = s.begin(); it != s.end(); ++it) 
    {
        // add this node and build model graph
        addSourceNode( *it );
    }
    
}


/**
 * Copy constructor. We instantiate the model from the previously stored source nodes. 
 *
 * \param[in]    m    The model object to copy.
 */
Model::Model(const Model &m) :
    sources()
{
    
    // iterate over all sources
    for (std::set<const DagNode*>::const_iterator it = m.sources.begin(); it != m.sources.end(); ++it) 
    {
        // add this node and build model graph
        addSourceNode( *it );
    }
    
}

/**
 * Destructor.
 * We have created new copied of the DAG nodes so we need to delete these here again.
 */
Model::~Model( void ) 
{
    
    // delete each DAG node from the copied model graph.
    for (std::vector<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it) 
    {
        DagNode *theNode = *it;
        if ( theNode->decrementReferenceCount() == 0 )
        {
            delete theNode;
        }
    }
    
    while ( !sources.empty() ) 
    {
        std::set<const DagNode*>::iterator theNode = sources.begin();
        sources.erase( *theNode );
        
        if ( (*theNode)->decrementReferenceCount() == 0)
        {
            delete *theNode;
        }
    }
    
}


/**
 * Assignment operator.
 * We have stored previously all the source nodes so we can simply construct the model graph by extracting
 * the DAG again from these source nodes. Note that we are acutally making a copy of the original graph again
 * which may cause that we get a different model graph (e.g. if the graph has change in the meanwhile).
 * The reason behind this approach is that we keep a valid map from the pointers of the original DAG nodes to
 * the pointers to the copied DAG nodes.
 *
 * \todo Check that this copy constructor is good or if we should use a different mechanism for the map between the
 * original DAG nodes and the copied DAG nodes.
 *
 * \param[in]    m    The model object to copy.
 */
Model& Model::operator=(const Model &x) 
{
    
    if ( this != &x )
    {
        // first remove all DAG nodes
        for (std::vector<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it) 
        {
            DagNode *theNode = *it;
            if ( theNode->decrementReferenceCount() == 0 )
            {
                delete theNode;
            }
        }
        
        // empty the source nodes
        while ( !sources.empty() ) 
        {
            std::set<const DagNode*>::iterator theNode = sources.begin();
            sources.erase( theNode );
            
            if ( (*theNode)->decrementReferenceCount() == 0)
            {
                delete *theNode;
            }
        }
        
        // iterate over all sources
        for (std::set<const DagNode*>::const_iterator it = x.sources.begin(); it != x.sources.end(); ++it) 
        {
            // add this node and build model graph
            addSourceNode( *it );
        }
    }
    
    return *this;
}



/**
 * Add a new source node.
 * We extract the model graph from the source node by calling cloneDAG on it.
 * cloneDAG clones the entire connected graph containing the given node.
 * Then we insert the copied source node in our set of source nodes so that we can use it
 * later to construct the model graph again in the copy constructor.
 * At the same time we fill the nodes map between the pointers of the nodes to the original DAG
 * and the the pointer to the cloned DAG nodes and fill also the vector of DAG nodes contained 
 * in this model.
 * 
 *
 * \param[in]    sourceNode    The new source node.
 */
void Model::addSourceNode(const DagNode *sourceNode) 
{
    
    // check that the source node is a valid pointer
    if (sourceNode == NULL)
    {
        throw RbException("Cannot instantiate a model with a NULL DAG node.");
    }
    
    // copy the entire graph connected to the source node
    // only if the node is not contained already in the nodesMap will it be copied.
    std::map<std::string, const DagNode* > names;
    sourceNode->cloneDAG(nodesMap, names);
    
    // add the source node to our set of sources
    DagNode *theNewSource = nodesMap[sourceNode];
    theNewSource->incrementReferenceCount();
    sources.insert( theNewSource );
    
    // we don't really know which nodes are new in our nodes map.
    // therefore we empty the nodes map and fill it again.
    for (std::vector<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it) 
    {
        
        DagNode *theNode = *it;
        theNode->decrementReferenceCount();
        
    }
    nodes.clear();
    
    /* insert new nodes into direct access vector */
    std::map<const DagNode*, DagNode* >::iterator i = nodesMap.begin();
    
    while ( i != nodesMap.end() ) 
    {
        // get the copied node
        DagNode* theNewNode = (*i).second;
        
        // increment the reference count to the new node
        theNewNode->incrementReferenceCount();
            
        // insert in direct access vector
        nodes.push_back( theNewNode );

        // increment the iterator;
        ++i;
    }
    
}

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself 
 */
Model* Model::clone( void ) const 
{
    
    return new Model( *this );
}


/**
 * Constant getter function for the vector of DAG nodes of the model.
 *
 * \return Vector of DAG nodes constituting to this model.
 */
const std::vector<DagNode *>& Model::getDagNodes( void ) const 
{
    
    return nodes;
}


/**
 * Non-constant getter function for the vector of DAG nodes of the model.
 *
 * \return Vector of DAG nodes constituting to this model.
 */
std::vector<DagNode *>& Model::getDagNodes( void ) 
{
    
    return nodes;
}


/**
 * Constant getter function for the map between the pointers of the original DAG nodels
 * to the copied DAG nodes of the model.
 *
 * \return Map between pointers from original to copied DAG nodes.
 */
const std::map<const DagNode*, DagNode*>& Model::getNodesMap( void ) const 
{
    
    return nodesMap;
}


/**
 * Set the number of processes available to this specific model object.
 * If there is more than one process available, then we can use these
 * to compute the likelihood in parallel. Yeah!
 */
void Model::setNumberOfProcesses(size_t n, size_t offset)
{
    
    // delegate the call to each DAG node
    for (std::vector<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
    {
        
        DagNode *theNode = *it;
        theNode->setNumberOfProcesses(n,offset);
        
    }
}
