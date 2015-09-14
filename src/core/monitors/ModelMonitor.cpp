#include "ModelMonitor.h"
#include "DagNode.h"
#include "Model.h"
#include "Monitor.h"
#include "RbFileManager.h"
#include "StochasticNode.h"

using namespace RevBayesCore;

/* Constructor */
ModelMonitor::ModelMonitor(unsigned long g, const std::string &fname, const std::string &del) : AbstractFileMonitor(std::vector<DagNode *>(),g,fname,del),
    stochasticNodesOnly( false )
{
    
}


/**
 * Destructor.
 */
ModelMonitor::~ModelMonitor()
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself 
 */
ModelMonitor* ModelMonitor::clone(void) const 
{
    
    return new ModelMonitor(*this);
}



/**
 * Reset the currently monitored DAG nodes by extracting the DAG nodes from the model again 
 * and store this in the set of DAG nodes.
 */
void ModelMonitor::resetDagNodes( void )
{
    
    // for savety we empty our dag nodes
    nodes.clear();
    
    if ( model != NULL )
    {
        // we only want to have each nodes once
        // this should by default happen by here we check again
        std::set<std::string> varNames;
        
        const std::vector<DagNode*> &n = model->getDagNodes();
        for (std::vector<DagNode*>::const_iterator it = n.begin(); it != n.end(); ++it) 
        {
            
            DagNode *theNode = *it;
            
            // only simple numeric variable can be monitored (i.e. only integer and real numbers)
            if ( theNode->isSimpleNumeric() && !theNode->isClamped())
            {
                if ( (!stochasticNodesOnly && !theNode->isConstant() && theNode->getName() != "" && !theNode->isHidden() && !theNode->isElementVariable() ) || ( theNode->isStochastic() && !theNode->isClamped() && theNode->isHidden() == false  && theNode->isElementVariable() == false ) )
                {
                    const std::string &name = theNode->getName();
                    if ( varNames.find( name ) == varNames.end() )
                    {
                        nodes.push_back( theNode );
                        varNames.insert( name );
                    }
                    else
                    {
                        /*
                        std::cerr << "Trying to add variable with name '" << name << "' twice." << std::endl;
                         */
                    }
                }
            }
        
        }
    }
    
}


/**
 * Set the model from which this monitor will extract the variables.
 * This will automatically result into a reseting of the currently monitored variables.
 *
 * \param[in]   m    The new model.
 */
void ModelMonitor::setModel(Model *m)
{
    
    // delegate call to super class
    Monitor::setModel(m);
    
    // reset the DAG nodes that should be monitored
    resetDagNodes();
    
    sortNodesByName();
}


/**
 * Set flag about whether to monitor only stochastic DAG nodes or deterministic ones as well. 
 *
 * \param[in]   tf   Flag if only stochastic nodes should be monitored.
 */
void ModelMonitor::setStochasticNodesOnly(bool tf) 
{
    
    stochasticNodesOnly = tf;
    
    // reset the DAG nodes that should be monitored
    resetDagNodes();
    
}

