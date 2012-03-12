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

#ifndef FileMonitor_H
#define FileMonitor_H

#include "MemberObject.h"
#include "Monitor.h"


#include <fstream>
#include <ostream>
#include <string>
#include <vector>

class VariableNode;


class FileMonitor : public Monitor {

public:
    // Constructors and Destructors
    FileMonitor();                                                                                              //!< Default Constructor
    FileMonitor(const FileMonitor &x);                                                                          //!< Copy Constructor
    virtual ~FileMonitor(void);                                                                                 //!< Destructor

    // Basic utility functions
    FileMonitor*                        clone(void) const;                                                  //!< Clone object
    static const std::string&           getClassName(void);                                                 //!< Get class name
    static const TypeSpec&              getClassTypeSpec(void);                                             //!< Get class type spec
    const TypeSpec&                     getTypeSpec(void) const;                                            //!< Get language type of the object
    void                                printValue(std::ostream& o) const;                                  //!< Print value (for user)

    // Member Object Functions
    const MemberRules&                  getMemberRules( void ) const;                                       //!< The member rules for a FileMonitor
    void                                setMemberVariable(const std::string &name, const Variable* var);    //!< Set a member variable. We catch here setting of variable nodes

    // Monitor functions
    void                                monitor(void);                                                      //!< Monitor unconditionally
    void                                monitor(int gen);                                                   //!< Monitor at generation gen

    // FileMonitor functions
    void                                closeStream(void);                                                  //!< Close stream after finish writing
    void                                openStream(void);                                                   //!< Open the stream for writing
    void                                printHeader(void);                                                  //!< Print header
    
private:
    std::ofstream                       outStream;

    // parameters
    RbConstVariablePtr                  filename;
    RbConstVariablePtr                  separator;
    
};

#endif

