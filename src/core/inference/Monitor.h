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

#include "ConstantMemberObject.h"
#include "RbString.h"

#include <fstream>
#include <ostream>
#include <string>
#include <vector>

class RbObject;
class VectorString;
class VariableNode;

const std::string Monitor_name = "Monitor";

class Monitor : public ConstantMemberObject {
    
public:
    // Constructors and Destructors
    Monitor();                                                                                              //!< Default Constructor
    Monitor(const MemberRules& rules ) ;                                                                    //!< Constructor
    Monitor(const Monitor &x);                                                                              //!< Copy Constructor
    virtual ~Monitor(void);                                                                                 //!< Destructor
    
    // Basic utility functions
    virtual Monitor*                    clone(void) const = 0;                                                  //!< Clone object
    virtual const VectorString&         getClass(void) const;                                               //!< Get class
    virtual const TypeSpec&             getTypeSpec(void) const;                                            //!< Get language type of the object
    virtual void                        printValue(std::ostream& o) const;                                  //!< Print value (for user)
    
    // Member Object Functions
    virtual const MemberRules&          getMemberRules( void ) const;                                       //!< The member rules for a monitor
    virtual void                        setMemberDagNode(const std::string &name, DAGNode* var);            //!< Set a member variable. We catch here setting of variable nodes
    
    std::vector<RbDagNodePtr>&          getDagNodes(void) { return nodes;}                                  //!< Get the nodes vector
    virtual void                        monitor(void) = 0;                                                  //!< Monitor unconditionally
    virtual void                        monitor(int gen) = 0;                                               //!< Monitor at generation gen
    void                                replaceDagNodes(std::vector<VariableNode*> &n);                     //!< Set the nodes vector
    bool                                monitorsVariable(const RbString& varName);                          //! Tell whether the variable with name is monitored by this monitor
    
protected:
    std::vector<RbDagNodePtr>           nodes;                                                              //!< Vector of nodes that this monitor monitors    

    // parameters
    Variable                            printgen;
    Variable                            variables;
   
private:
    static const TypeSpec               typeSpec;
};

#endif

