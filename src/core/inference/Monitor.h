/**
 * @file
 * This file contains the declaration of Monitor, used to hold information
 * about the monitoring of a variable DAG node.
 *
 * @brief Declaration of Monitor
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id$
 */

#ifndef Monitor_H
#define Monitor_H

#include "MemberObject.h"
#include "RbString.h"

#include <fstream>
#include <ostream>
#include <string>
#include <vector>


class Monitor : public MemberObject {
    
public:
    // Constructors and Destructors
    Monitor();                                                                                              //!< Default Constructor
    Monitor(const MemberRules& rules ) ;                                                                    //!< Constructor
    
    // Basic utility functions
    virtual Monitor*                    clone(void) const = 0;                                              //!< Clone object
    static const std::string&           getClassName(void);                                                 //!< Get class name
    static const TypeSpec&              getClassTypeSpec(void);                                             //!< Get class type spec
    virtual const TypeSpec&             getTypeSpec(void) const;                                            //!< Get language type of the object
    virtual void                        printValue(std::ostream& o) const;                                  //!< Print value (for user)
    
    // Member Object Functions
    virtual const MemberRules&          getMemberRules( void ) const;                                       //!< The member rules for a monitor
    virtual void                        setMemberVariable(const std::string &name, const Variable* var);    //!< Set a member variable. We catch here setting of variable nodes
    
    void                                addDagNode(DAGNode* d);                                             //!< Add a DAG node to this monitor
//    std::vector<RbConstVariablePtr>&    getDagNodes(void) { return nodes;}                                  //!< Get the nodes vector
    virtual void                        monitor(void) = 0;                                                  //!< Monitor unconditionally
    virtual void                        monitor(int gen) = 0;                                               //!< Monitor at generation gen
//    void                                replaceDagNodes(std::vector<VariableNode*> &n);                     //!< Set the nodes vector
    bool                                monitorsVariable(const RbString& varName);                          //!< Tell whether the variable with name is monitored by this monitor
    
protected:

    // parameters
    std::set<DAGNode*>                  nodes;
   
};

#endif

