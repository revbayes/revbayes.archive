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
#include "SimpleMemberFunction.h"

#include <fstream>
#include <ostream>
#include <set>
#include <string>
#include <vector>

class InferenceMonitor;


class ParserMonitor : public MemberObject {
    
public:
    // Constructors and Destructors
    ParserMonitor(InferenceMonitor* m, const std::string &n, const MemberRules &mr, const std::set<std::string> &a);    //!< Default Constructor
    ParserMonitor(const ParserMonitor &x);                                                                              //!< Copy Constructor
    virtual ~ParserMonitor(void);                                                                                 //!< Destructor
 
    // overloaded operators
    ParserMonitor&                          operator=(const ParserMonitor& m);
    
    // Basic utility functions
    ParserMonitor*                          clone(void) const;                                                  //!< Clone object
    static const std::string&               getClassName(void);                                                 //!< Get class name
    static const TypeSpec&                  getClassTypeSpec(void);                                             //!< Get class type spec
    const TypeSpec&                         getTypeSpec(void) const;                                            //!< Get language type of the object
    void                                    printValue(std::ostream& o) const;                                  //!< Print value (for user)
    
    // Member Object Functions
    const MemberRules&                      getMemberRules(void) const;                                                 //!< The member rules for a ParserMonitor
    void                                    setMemberVariable(const std::string &name, const RbPtr<Variable> &var);     //!< Set a member variable. We catch here setting of variable nodes
    
    // access functions
    const InferenceMonitor*                 getLeanMonitor(void) const;
    const std::vector<const DAGNode *>&     getMonitorArgumgents(void) const;
//    void                                addDagNode(DAGNode* d);                                             //!< Add a DAG node to this ParserMonitor
    //    std::vector<RbConstVariablePtr>&    getDagNodes(void) { return nodes;}                                  //!< Get the nodes vector
//    virtual void                        monitor(void) = 0;                                                  //!< ParserMonitor unconditionally
//    virtual void                        monitor(int gen) = 0;                                               //!< ParserMonitor at generation gen
    //    void                                replaceDagNodes(std::vector<VariableNode*> &n);                     //!< Set the nodes vector
//    bool                                monitorsVariable(const RbString& varName);                          //!< Tell whether the variable with name is ParserMonitored by this ParserMonitor
      
    protected:
    void                                    setSimpleMemberValue(const std::string& name, const RbPtr<const RbLanguageObject> &var);        //!< Set member variable

private:
    std::set<std::string>                   attributeNames;
    std::vector<const DAGNode*>             nodes;
    MemberRules                             memberRules;
    InferenceMonitor*                       monitor;
    std::string                             name;
    TypeSpec                                typeSpec;
};

#endif

