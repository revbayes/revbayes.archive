/**
 * @file
 * This file contains the declaration of InferenceMonitor, used to hold information
 * about the monitoring of a variable DAG node.
 *
 * @brief Declaration of InferenceMonitor
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-10 10:31:21 +0200 (Tue, 10 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id: InferenceMonitor.h 1370 2012-04-10 08:31:21Z hoehna $
 */

#ifndef InferenceMonitor_H
#define InferenceMonitor_H

#include <fstream>
#include <ostream>
#include <string>
#include <vector>

class InferenceDagNode;
class RbLanguageObject;


class InferenceMonitor {
    
public:
    // Constructors and Destructors
    InferenceMonitor();                                                                                     //!< Default Constructor
    InferenceMonitor(const InferenceMonitor &x);                                                            //!< Copy Constructor
    virtual ~InferenceMonitor(void);                                                                        //!< Destructor
    
    // Basic utility functions
    virtual InferenceMonitor*           clone(void) const = 0;                                              //!< Clone object
        
//    void                                addDagNode(DAGNode* d);                                             //!< Add a DAG node to this InferenceMonitor
    //    std::vector<RbConstVariablePtr>&    getDagNodes(void) { return nodes;}                                  //!< Get the nodes vector
    virtual void                        monitor(void) = 0;                                                  //!< InferenceMonitor unconditionally
    virtual void                        monitor(int gen) = 0;                                               //!< InferenceMonitor at generation gen
    //    void                                replaceDagNodes(std::vector<VariableNode*> &n);                     //!< Set the nodes vector
//    bool                                monitorsVariable(const std::string &varName);                       //!< Tell whether the variable with name is InferenceMonitored by this InferenceMonitor
    
protected:
    
    // parameters
    size_t                              printgen;
    std::vector<InferenceDagNode*>      nodes;
    std::vector<RbLanguageObject*>      templateObject;
    
private:
    
};

#endif

