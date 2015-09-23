//#include "Tree.h"
//#include "RbException.h"
//#include "RbOptions.h"
//#include "TreeChangeEventListener.h"
//#include "Topology.h"
//#include "TopologyNode.h"
//
//using namespace RevBayesCore;
//
///* Default constructor */
//TimeTree::TimeTree(void) : Tree()
//{
//    
//}
//
//
///* Destructor */
//TimeTree::~TimeTree(void)
//{
//    
//}
//
//
///* Clone function */
//TimeTree* TimeTree::clone(void) const
//{
//    
//    return new TimeTree(*this);
//}
//
//
//double TimeTree::getAge(size_t idx) const
//{
//    return ages[idx];
//}
//
//
//
//double TimeTree::getBranchLength(size_t idx) const
//{
//    
//    const TopologyNode &n = topology->getNode( idx );
//    if ( n.isRoot() )
//    {
//        return 0.0;
//    }
//    else
//    {
//        size_t parentIdx = n.getParent().getIndex();
//        return ages[parentIdx] - ages[idx];
//    }
//}
//
//
//
//double TimeTree::getTime(size_t idx) const
//{
//    
//    const TopologyNode &n = topology->getNode( idx );
//    if ( n.isRoot() )
//    {
//        return 0.0;
//    }
//    else
//    {
//        size_t parentIdx = n.getParent().getIndex();
//        return ages[parentIdx] - ages[idx] + getTime(parentIdx);
//    }
//}
//
//
//double TimeTree::getTreeLength(void) const
//{
//    double treeLength = 0;
//    const std::vector<TopologyNode*> &nodes = topology->getNodes();
//    for (size_t i = 0; i < nodes.size(); i++)
//    {
//        treeLength += nodes[i]->getBranchLength();
//    }
//    
//    return treeLength;
//}
//
//
//void TimeTree::resizeElementVectors(size_t n)
//{
//    // remove all elements
//    ages.clear();
////    times.clear();
//    
//    // resize to new dimension
//    ages.resize( n );
////    times.resize( n );
//}
//
//
//void TimeTree::setAge(size_t idx, double a)
//{
//    
//    // fire a tree change event
//    const std::set<TreeChangeEventListener*> &listeners = changeEventHandler.getListeners();
//    const TopologyNode &n = topology->getNode(idx);
//    
//    for (std::set<TreeChangeEventListener*>::iterator it = listeners.begin(); it != listeners.end(); ++it)
//    {
//
//        for (size_t i = 0; i < n.getNumberOfChildren(); ++i)
//        {
//            (*it)->fireTreeChangeEvent( n.getChild(i) );
//        }
//        
//        // just to be save
//        (*it)->fireTreeChangeEvent( n );
//
//    }
//    
//    // flag the newick string as invalid
//    const std::vector<TopologyNode*> &children = n.getChildren();
//    topology->getNode(idx).flagNewickRecomputation();
//    for (std::vector<TopologyNode*>::const_iterator it = children.begin(); it != children.end(); ++it)
//    {
//        (*it)->flagNewickRecomputation();
//    }
//    
//    ages[idx] = a;
//}
//
//
//std::ostream& RevBayesCore::operator<<(std::ostream& o, const TimeTree& x) {
//    
//    o << x.getNewickRepresentation();
//    
//    return o;
//}
