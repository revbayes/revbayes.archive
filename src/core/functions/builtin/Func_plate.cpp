/**
 * @file
 * This file contains the implementation of Func_plate.
 *
 * @brief Implementation of Func_sqrt
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-03-10 12:55:11 +0100 (Sat, 10 Mar 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id: Func_plate.cpp 1327 2012-03-10 11:55:11Z hoehna $
 */

#include "DAGNode.h"
#include "DagNodeContainer.h"
#include "DeterministicNode.h"
#include "Ellipsis.h"
#include "Func_plate.h"
#include "Parser.h"
#include "ParserDistribution.h"
#include "RbException.h"
#include "RbNullObject.h"
#include "RbUtil.h"
#include "RbVector.h"
#include "StochasticNode.h"
#include "TypeSpec.h"
#include "ValueRule.h"

#include <cassert>
#include <cmath>
#include <sstream>


Func_plate::Func_plate(void) : RbFunction() {
    
}


/** Clone object */
Func_plate* Func_plate::clone( void ) const {
    
    return new Func_plate( *this );
}


/** Execute function */
const RbLanguageObject& Func_plate::execute( void ) {
    
    // get the command string
    std::string command = static_cast<const RbString &>( args[0].getVariable().getValue() ).getValue();
    
    std::cerr << command << std::endl;
    
    plate.clear();
    
    std::vector<const Container *> ranges;
    std::vector<std::string> names;
    for (std::vector<Argument>::const_iterator i = args.begin()+1; i != args.end(); ++i) {
        ranges.push_back( static_cast<const Container *>( i->getVariable().getValue().clone() ) );
        names.push_back( i->getLabel() );
        std::cerr << names[names.size() - 1] << ":\t\t";
        ranges[ranges.size()-1]->printValue( std::cerr );
        std::cerr << std::endl;
    }
    
    fillPlate(command, ranges, names, 0);
    
    // now we need to extract the DAG nodes and replace them with the merged ones
    for ( size_t i = 0; i < plate.size(); ++i ) {
        const std::string &varname = plate.getName( i );
        const DAGNode *node = plate.getDagNode( varname );
        
        // if this is a DAG node container, then we replace it
        if ( node->getValue().isTypeSpec( DagNodeContainer::getClassTypeSpec() ) ) {
            std::cerr << "Merging variable:\t\t" << varname << std::endl;
            DAGNode* newNode = replaceVariable( static_cast<const DagNodeContainer &>( node->getValue() ) );
            
//            plate.remove( varname );
//            plate.addVariable( varname, newNode );
        }
    }
    
    return RbNullObject::getInstance();
}


void Func_plate::fillPlate(std::string const &c, const std::vector<const Container *> &ranges, const std::vector<std::string> &names, size_t level) {
    
    // get the container for this level
    const Container *con = ranges[level];
    
    for (size_t i = 0; i < con->size(); ++i) {
        // get the element for this index
        const RbObject &element = con->getElement( i );
        std::stringstream out;
        element.printValue(out);
        std::string stringValue = out.str();
        
        // replace the command string
        std::string replaceCommand = replaceIndex(c,names[level], stringValue);
        
        // call the fill plate recursively if necessary
        if ( ranges.size() > level+1 ) {
            fillPlate(replaceCommand, ranges, names, level+1);
        }
        else {
            Parser &p = Parser::getParser();
            
            std::cerr << "Executing:\t" << replaceCommand << std::endl;
            p.processCommand(replaceCommand, &plate);
        }
        
    }
}


/** Get argument rules */
const ArgumentRules& Func_plate::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if (!rulesSet) 
    {
        argumentRules.push_back( new ValueRule( "command", RbString::getClassTypeSpec() ) );
        // add the rule for the ranges and enforce at least one
//        argumentRules.push_back( new ValueRule( "", Container::getClassTypeSpec() ) );
        argumentRules.push_back( new Ellipsis( Container::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
const std::string& Func_plate::getClassName(void) { 
    
    static std::string rbClassName = "plate function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_plate::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_plate::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = TypeSpec(RbVoid_name);
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_plate::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RealPos::getClassTypeSpec();
    return returnTypeSpec;
}


std::string Func_plate::replaceIndex(std::string const &c, std::string const &n, std::string const &i) const {
    std::string replacedCommand = c;
    
    std::cerr << "String before replacement:\t" << c << std::endl;
    
    std::string marker = "<" + n + ">";
    size_t pos = 0;
    while ( ( pos = replacedCommand.find(marker, pos) ) != std::string::npos ) {
        replacedCommand = replacedCommand.replace(pos, marker.size(), i);
    }
    
    std::cerr << "String after replacement:\t\t" << replacedCommand << std::endl;
    
    return replacedCommand;
}

DAGNode* Func_plate::replaceVariable(const DagNodeContainer &orgNodes) const  {
    
    DAGNode* newNode = NULL;
    
    const DAGNode& first = static_cast<const DAGNode &>( orgNodes.getElement( 0 ) );
    
    if ( first.isTypeSpec( StochasticNode::getClassTypeSpec() ) ) {
        newNode = new StochasticNode( static_cast<const StochasticNode &>( first ).getDistribution().clone() );
    }
    else {
        newNode = new DeterministicNode();
    }
    
    return newNode;
}
