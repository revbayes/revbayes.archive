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


#include "FileMonitor.h"
#include "InferenceDagNode.h"
#include "InferenceMonitor.h"
#include "RbLanguageObject.h"


/** Constructor */
FileMonitor::FileMonitor(void) : InferenceMonitor(), outStream() {

    filename = "";
    separator = "";
    
}


FileMonitor::FileMonitor(const FileMonitor &f) : InferenceMonitor( f ), outStream() {
    
    filename = f.filename;
    separator = f.separator;
}


/** Clone the object */
FileMonitor* FileMonitor::clone(void) const {

    return new FileMonitor(*this);
}


void FileMonitor::closeStream() {
    outStream.close();
}


/** Monitor value unconditionally */
void FileMonitor::monitor(void) {

    // print the iteration number first
    outStream << "-";
    
    for (size_t i = 0; i < nodes.size(); i++) {
        // add a separator before every new element
        outStream << separator;
        
        // get the node
        InferenceDagNode *node = nodes[i];
        
        // get the template object
        RbLanguageObject *tmpObject = templateObject[i];
        
        // reset the value of the template object
        tmpObject->setLeanValue( node->getValue() );
        
        // print the value
        tmpObject->printValue(outStream);
    }
    
    outStream << std::endl;
    
}


/** Monitor value at generation gen */
void FileMonitor::monitor(long gen) {

    // get the printing frequency
    int samplingFrequency = printgen;
    
    if (gen % samplingFrequency == 0) {
        // print the iteration number first
        outStream << gen;
        
        for (size_t i = 0; i < nodes.size(); i++) {
            // add a separator before every new element
            outStream << separator;
            
            // get the node
            InferenceDagNode *node = nodes[i];
            
            // get the template object
            RbLanguageObject *tmpObject = templateObject[i];
            
            // reset the value of the template object
            tmpObject->setLeanValue( node->getValue() );
            
            // print the value
            tmpObject->printValue(outStream);
        }

        outStream << std::endl;
    
    }
}


/** open the file stream for printing */
void FileMonitor::openStream(void) {
    
    // open the stream to the file
    outStream.open( filename.c_str(), std::fstream::out );
    
}

/** Print header for monitored values */
void FileMonitor::printHeader() {
  
    // print one column for the iteration number
    outStream << "Sample";
    
    for (std::vector<InferenceDagNode *>::const_iterator it=nodes.begin(); it!=nodes.end(); it++) {
        // add a separator before every new element
        outStream << separator;
        
         const InferenceDagNode* theNode = *it;
        
        // print the header
        if (theNode->getName() != "")
            outStream << theNode->getName();
        else
            outStream << "Unnamed";
    }
    
    outStream << std::endl;
}


void FileMonitor::setAttribute(std::string const &name, const RbValue<void*> &a) {
    
    if ( name == "printgen" ) {
        printgen = static_cast<int *>( a.value )[0];
    }
    else if ( name == "separator" ) {
        separator = static_cast<std::string *>( a.value )[0];
    }
    else if ( name == "filename" ) {
        filename = static_cast<std::string *>( a.value )[0];
    }
}


