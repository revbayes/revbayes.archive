/**
 * @file
 * This file contains the declaration of ParserMonitor, used to hold information
 * about the monitoring of a variable DAG node.
 *
 * @brief Declaration of ParserMonitor
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-10 10:31:21 +0200 (Tue, 10 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id: ParserMonitor.h 1370 2012-04-10 08:31:21Z hoehna $
 */

#ifndef ParserMonitor_H
#define ParserMonitor_H

#include "MemberObject.h"
#include "RbString.h"

#include <fstream>
#include <ostream>
#include <string>
#include <vector>


class ParserMonitor : public MemberObject {
    
public:
    // Constructors and Destructors
    ParserMonitor();                                                                                              //!< Default Constructor
    ParserMonitor(const MemberRules& rules ) ;                                                                    //!< Constructor
    ParserMonitor(const ParserMonitor &x);                                                                              //!< Copy Constructor
    virtual ~ParserMonitor(void);                                                                                 //!< Destructor
    
    // Basic utility functions
    virtual ParserMonitor*              clone(void) const = 0;                                              //!< Clone object
    static const std::string&           getClassName(void);                                                 //!< Get class name
    static const TypeSpec&              getClassTypeSpec(void);                                             //!< Get class type spec
    virtual const TypeSpec&             getTypeSpec(void) const;                                            //!< Get language type of the object
    virtual void                        printValue(std::ostream& o) const;                                  //!< Print value (for user)
    
    // Member Object Functions
    virtual const MemberRules&          getMemberRules( void ) const;                                       //!< The member rules for a ParserMonitor
    virtual void                        setMemberVariable(const std::string &name, const Variable* var);    //!< Set a member variable. We catch here setting of variable nodes
    
    void                                addDagNode(DAGNode* d);                                             //!< Add a DAG node to this ParserMonitor
    //    std::vector<RbConstVariablePtr>&    getDagNodes(void) { return nodes;}                                  //!< Get the nodes vector
    virtual void                        monitor(void) = 0;                                                  //!< ParserMonitor unconditionally
    virtual void                        monitor(int gen) = 0;                                               //!< ParserMonitor at generation gen
    //    void                                replaceDagNodes(std::vector<VariableNode*> &n);                     //!< Set the nodes vector
    bool                                monitorsVariable(const RbString& varName);                          //!< Tell whether the variable with name is ParserMonitored by this ParserMonitor
    
protected:
    
    // parameters
    RbConstVariablePtr                  printgen;
    std::set<DAGNode*>                  nodes;
    
private:
    
};

#endif

