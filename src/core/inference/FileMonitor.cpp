/**
 * @file
 * This file contains the implementation of FileMonitor, used to save
 * information to file about the monitoring of a variable DAG node.
 *
 * @brief Implementation of FileMonitor
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

#include "DagNodeContainer.h"
#include "Ellipsis.h"
#include "Integer.h"
#include "FileMonitor.h"
#include "RbException.h"
#include "RbUtil.h"
#include "ValueRule.h"
#include "VariableNode.h"
#include "VectorString.h"

#include <sstream>


/** Constructor */
FileMonitor::FileMonitor(void) : Monitor(getMemberRules() ),
    filename( NULL ),
    separator( NULL ) {
    
}

/** Copy Constructor */
FileMonitor::FileMonitor(const FileMonitor &x) : Monitor(x),
filename( x.filename ),
separator( x.separator ) {
    
}

FileMonitor::~FileMonitor() {
   
}


/** Clone the object */
FileMonitor* FileMonitor::clone(void) const {

    return new FileMonitor(*this);
}


/** Get class name of object */
const std::string& FileMonitor::getClassName(void) { 
    
    static std::string rbClassName = "File monitor";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& FileMonitor::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Monitor::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& FileMonitor::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}

/** Return member rules */
const MemberRules& FileMonitor::getMemberRules( void ) const {
    
    static MemberRules memberRules = MemberRules();
    static bool        rulesSet = false;
    
    if (!rulesSet) 
    {
        memberRules.push_back( new ValueRule( "filename"  , RbString::getClassTypeSpec()         ) );
        memberRules.push_back( new ValueRule( "printgen"  , Integer::getClassTypeSpec(),  new Integer(1)    ) );
        memberRules.push_back( new ValueRule( "separator" , RbString::getClassTypeSpec(), new RbString("\t") ) );
        memberRules.push_back( new ValueRule( "variable"  , RbLanguageObject::getClassTypeSpec() ) );
        memberRules.push_back( new Ellipsis (               RbLanguageObject::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return memberRules;
}


/** Monitor value unconditionally */
void FileMonitor::monitor(void) {

    for (std::vector<RbVariablePtr>::const_iterator it=nodes.begin(); it!=nodes.end(); it++) {
        // add a separator before every new element except the first element
        if ( it != nodes.begin() )
            outStream << static_cast<RbString&>( separator->getValue() ).getValue();
        
        // print the value
        (*it)->printValue(outStream);
    }
    
    outStream << std::endl;
    
    
}


/** Monitor value at generation gen */
void FileMonitor::monitor(int gen) {

    // get the printing frequency
    int samplingFrequency = dynamic_cast<const Integer&>( printgen->getValue() ).getValue();
    
    if (gen % samplingFrequency == 0) {
        // print the iteration number first
        outStream << gen;
        
        for (std::vector<RbVariablePtr>::const_iterator it=nodes.begin(); it!=nodes.end(); it++) {
            // add a separator before every new element
            outStream << static_cast<RbString&>( separator->getValue() ).getValue();
            
            // print the value
            (*it)->printValue(outStream);
        }

	outStream << std::endl;
    }
}


/** open the file stream for printing */
void FileMonitor::openStream(void) {

    // get the filename
    std::string f = dynamic_cast<const RbString&>( filename->getValue() ).getValue();
    
    // open the stream to the file
    outStream.open(f.c_str());
    
}

/** Print header for monitored values */
void FileMonitor::printHeader() {
  
    // print one column for the iteration number
    outStream << "Sample";
    
    for (std::vector<RbVariablePtr>::const_iterator it=nodes.begin(); it!=nodes.end(); it++) {
        // add a separator before every new element
        outStream << static_cast<RbString&>( separator->getValue() ).getValue();
        
         const DAGNode* theNode = static_cast<const DAGNode*>((*it)->getDagNode());
        
        // print the header
        if (theNode->getName() != "")
            outStream << theNode->getName();
        else
            outStream << "Unnamed " << theNode->getType();
    }
    
    outStream << std::endl;
}


/** Print value for user */
void FileMonitor::printValue(std::ostream& o) const {
    
    // get the printing frequency
    int samplingFrequency = dynamic_cast<const Integer&>( printgen->getValue() ).getValue();
    
    o << "Monitor: interval = " << samplingFrequency;
}


void FileMonitor::setMemberVariable(std::string const &name, Variable* var) {
    
    // catch setting of the variables 
    if (name == "separator") {
        separator = var;
        
    } 
    else if (name == "filename") {
        filename = var;
        
    }
    else {
        // call parent class to set member variable
        Monitor::setMemberVariable( name, var );
    }
}


