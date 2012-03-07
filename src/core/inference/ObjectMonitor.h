/**
 * @file
 * This file contains the declaration of Monitor, used to save information
 * to a file about the monitoring of a variable DAG node.
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

#ifndef ObjectMonitor_H
#define ObjectMonitor_H

//#include "MemberObject.h"
#include "Monitor.h"
#include "Vector.h"
#include "RbString.h"


#include <fstream>
#include <ostream>
#include <string>
#include <vector>


class ObjectMonitor : public Monitor {
    
public:
    // Constructors and Destructors
    ObjectMonitor();                                                                                                            //!< Default Constructor
    ObjectMonitor(const ObjectMonitor &x);                                                                                      //!< Copy Constructor
    virtual ~ObjectMonitor(void);                                                                                               //!< Destructor
    
    // Basic utility functions
    ObjectMonitor*                      clone(void) const;                                                                      //!< Clone object
    static const std::string&           getClassName(void);                                                                     //!< Get class name
    static const TypeSpec&              getClassTypeSpec(void);                                                                 //!< Get class type spec
    const TypeSpec&                     getTypeSpec(void) const;                                                                //!< Get language type of the object
    void                                printValue(std::ostream& o) const;                                                      //!< Print value (for user)
    
    // Member Object Functions
    const MemberRules&                  getMemberRules( void ) const;                                                           //!< The member rules for an ObjectMonitor
    virtual const MethodTable&          getMethods(void) const;                                                                 //!< Get methods
    const RbLanguageObject&             executeOperationSimple(const std::string& name, const std::vector<Argument>& args);     //!< Execute method
    void                                setMemberVariable(const std::string &name, Variable* var);                              //!< Set a member variable. We catch here setting of variable nodes

    // Monitor functions
    void                                monitor(void);                                                                          //!< Monitor unconditionally
    void                                monitor(int gen);                                                                       //!< Monitor at generation gen

    // ObjectMonitor function
    Vector&                             getValues(const RbString& varName);                                                     //!< returns the values contained in the values vector for variable with name varName
    
private:
    std::map<RbString, Vector >         values;                                                                                 //!< Vector of values from the monitored node
    
};

#endif

