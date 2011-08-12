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

#include <fstream>
#include <ostream>
#include <string>
#include <vector>

class RbObject;
class VectorString;
class VariableNode;

class Monitor : public ConstantMemberObject {

public:
    // Constructors and Destructors
    Monitor();                                                                                      //!< Default Constructor
    Monitor(const Monitor &x);                                                                      //!< Copy Constructor
    virtual ~Monitor(void);                                                                         //!< Destructor

    // Basic utility functions
    Monitor*                    clone(void) const;                                                  //!< Clone object
    RbLanguageObject*           convertTo(const std::string &type) const;                           //!< Convert to type
    const VectorString&         getClass(void) const;                                               //!< Get class
    bool                        isConvertibleTo(const std::string& type, bool once) const;          //!< Is convertible to type?
    void                        printValue(std::ostream& o) const;                                  //!< Print value (for user)
    std::string                 richInfo(void) const;                                               //!< Complete info about object

    // Member Object Functions
    const MemberRules&          getMemberRules( void ) const;                                       //!< The member rules for a monitor
    void                        setMemberVariable(const std::string &name, Variable *var);          //!< Set a member variable. We catch here setting of variable nodes

    // Monitor functions
    void                        closeStream(void);                                                  //!< Close stream after finish writing
    std::vector<VariableNode*>& getDagNodes(void) { return nodes;}                                  //!< Get the nodes vector
    void                        monitor(void);                                                      //!< Monitor unconditionally
    void                        monitor(int gen);                                                   //!< Monitor at generation gen
    void                        openStream(void);                                                   //!< Open the stream for writing
    void                        printHeader(void);                                                  //!< Print header
    void                        replaceDagNodes(std::vector<VariableNode*> &n);                     //!< Set the nodes vector
    
private:
    std::vector<VariableNode*>  nodes;                                                              //!< Vector of nodes which this monitors prints
    std::ofstream               outStream;
};

#endif

