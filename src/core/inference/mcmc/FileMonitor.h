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

#include "ConstantMemberObject.h"
#include "Monitor.h"


#include <fstream>
#include <ostream>
#include <string>
#include <vector>

class RbObject;
class VectorString;
class VariableNode;

const std::string FileMonitor_name = "FileMonitor";

class FileMonitor : public Monitor {

public:
    // Constructors and Destructors
    FileMonitor();                                                                                              //!< Default Constructor
    FileMonitor(const FileMonitor &x);                                                                          //!< Copy Constructor
    virtual ~FileMonitor(void);                                                                                 //!< Destructor

    // Basic utility functions
    FileMonitor*                        clone(void) const;                                                  //!< Clone object
    const VectorString&                 getClass(void) const;                                               //!< Get class
    const TypeSpec&                     getTypeSpec(void) const;                                            //!< Get language type of the object
    void                                printValue(std::ostream& o) const;                                  //!< Print value (for user)

    // Member Object Functions
    const MemberRules&                  getMemberRules( void ) const;                                       //!< The member rules for a FileMonitor
    void                                setMemberVariable(const std::string &name, Variable* var);          //!< Set a member variable. We catch here setting of variable nodes

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
    RbVariablePtr                       filename;
    RbVariablePtr                       separator;
    
    static const TypeSpec               typeSpec;
};

#endif

