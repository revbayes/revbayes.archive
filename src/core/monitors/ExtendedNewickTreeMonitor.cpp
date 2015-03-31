#include "ExtendedNewickTreeMonitor.h"
#include "DagNode.h"
#include "Model.h"
#include "Monitor.h"
#include "RbException.h"

using namespace RevBayesCore;


/* Constructor */
ExtendedNewickTreeMonitor::ExtendedNewickTreeMonitor(TypedDagNode<TimeTree> *t, const std::set<DagNode*> &n, bool np, unsigned long g, const std::string &fname, const std::string &del, bool pp, bool l, bool pr, bool ap) : AbstractFileMonitor(t,g,fname,del,pp,l,pr,ap),
    isNodeParameter( np ),
    tree( t ),
    nodeVariables( n )
{
//    this->nodes.insert( tree );
    
    for (std::set<DagNode*>::iterator it = nodeVariables.begin(); it != nodeVariables.end(); ++it)
    {
        this->nodes.push_back( *it );
        
        // tell the node that we have a reference to it (avoids deletion)
        (*it)->incrementReferenceCount();
    }
}


/* Clone the object */
ExtendedNewickTreeMonitor* ExtendedNewickTreeMonitor::clone(void) const
{
    
    return new ExtendedNewickTreeMonitor(*this);
}


/** Monitor value at generation gen */
void ExtendedNewickTreeMonitor::monitorVariables(unsigned long gen)
{
    
    outStream << separator;
    
    tree->getValue().clearParameters();
    for (std::set<DagNode*>::iterator it = nodeVariables.begin(); it != nodeVariables.end(); ++it)
    {
        const std::string &name = (*it)->getName();
//        Container *c = dynamic_cast<Container *>( (*it)->getValue() );
        size_t numParams = (*it)->getNumberOfElements();

        std::stringstream ss;
        (*it)->printValueElements(ss,"\t");
        std::string concatenatedValues = ss.str();
        std::vector<std::string> values;
        StringUtilities::stringSplit(concatenatedValues, "\t", values);
        for (size_t i = 0; i < numParams; ++i)
        {
            TopologyNode &node = tree->getValue().getNode( i );
            if ( isNodeParameter == true )
            {
                node.addNodeParameter( name, values[i]);
            }
            else
            {
                node.addBranchParameter( name, values[i]);
            }
            
        }
    }
            
    outStream << tree->getValue();
    
}


/** Print header for monitored values */
void ExtendedNewickTreeMonitor::printFileHeader()
{
    
    // add a separator tree
    outStream << separator << "Tree";
        
    // end line of stream
    outStream << std::endl;
}


void ExtendedNewickTreeMonitor::swapNode(DagNode *oldN, DagNode *newN)
{
    
    TypedDagNode< RbVector<double> >* nodeVar = dynamic_cast< TypedDagNode< RbVector<double> > *>(oldN);
    if ( oldN == tree )
    {
        tree = static_cast< TypedDagNode< TimeTree > *>( newN );
    }
    else if ( nodeVar != NULL )
    {
        // error catching
        if ( nodeVariables.find(nodeVar) == nodeVariables.end() )
        {
            throw RbException("Cannot replace DAG node with name\"" + oldN->getName() + "\" in this extended newick monitor because the monitor doesn't hold this DAG node.");
        }
        
        nodeVariables.erase( nodeVar );
        nodeVariables.insert( static_cast< TypedDagNode< RbVector<double> > *>(newN) );
    }
    
    // delegate to base class
    AbstractFileMonitor::swapNode(oldN, newN);
}


