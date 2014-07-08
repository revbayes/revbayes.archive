/**
 * @file
 * This file contains the implementation of VariableSlot, which is
 * used to manage variables in frames and processed argument lists.
 *
 * @brief Implementation of VariableSlot
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-17, version 1.0
 *
 * $Id$
 */

#include "Argument.h"
#include "RbException.h"
#include "RlFunction.h"
#include "RbUtil.h"
#include "RbOptions.h"
#include "RevPtr.h"
#include "TypeSpec.h"
#include "Variable.h"
#include "VariableSlot.h"
#include "Workspace.h"

#include <cassert>

using namespace RevLanguage;

#if 0

// Declarations
std::ostream& operator<<(std::ostream& o, const VariableSlot& x);

/** Constructor of filled slot with type specification. */
VariableSlot::VariableSlot(const std::string &lbl, const TypeSpec& typeSp, const RevPtr<Variable>& var) : label(lbl) {
    
    variable.push_back( var );
    
}

/** Constructor of filled slot with type specification. */
VariableSlot::VariableSlot(const std::string &lbl, const RevPtr<Variable>& var) : label(lbl) {
    
    variable.push_back( var );
    
}


/** Constructor of empty slot based on type specification */
VariableSlot::VariableSlot(const std::string &lbl, const TypeSpec& typeSp) : label(lbl) {
    
    variable.push_back( NULL );
    
}


void VariableSlot::createVariable(const std::vector<int> &indices) {
    
    // test whether we need to update the dimensions
    if ( indices.size() > lengths.size() && lengths.size() > 0 ) {
        throw RbException("Cannot resize dimension of slot variable that already exists!");
    }
    else if ( indices.size() > lengths.size() ) {
        // this was a slot for a single variable before
        variable.clear();
        
        // fill with empty variables
        size_t nElements = 1;
        for (size_t i = 0; i < indices.size(); ++i) {
            nElements *= (indices[i]+1);
            lengths.push_back( indices[i]+1 );
        }
        for (size_t i = 0; i < nElements; ++i) {
            Variable *theVar = new Variable( NULL );
            theVar->setName( label );
            variable.push_back( theVar );
        }

    }
    else {
        std::vector<int> oldLengths = lengths;
        std::vector<RevPtr<Variable> > oldVars = variable;
        
        lengths.clear();
        variable.clear();
        
        // fill with empty variables
        size_t nElements = 1;
        for (size_t i = 0; i < indices.size(); ++i) {
            int n = (indices[i] > oldLengths[i]-1 ? indices[i] : oldLengths[i]-1) + 1;
            nElements *= (n);
            lengths.push_back( n );
        }
        for (size_t i = 0; i < nElements; ++i) {
            variable.push_back( new Variable( NULL ) );
        }
        
        std::vector<int> tmp;
        resetVariables(oldVars, oldLengths, tmp);
    }
    
    int index = 0;
    resetNames(label + "[", 0, index);
    
}


bool VariableSlot::doesVariableExist(const std::vector<int> &indices) const
{

    // test whether we need to update the dimensions
    
    size_t offset = 1;
    for (size_t i = 0; i < indices.size(); ++i)
    {
        if ( i > lengths.size() )
        {
            if ( indices[i] >= lengths[i] )
            {
                return false;
            }
            offset *= lengths[i];
        }
        else
        {
            
            return false;
//            if ( variable[offset-1]->getRevObject().hasSubElement() )
//            {
//            
//            }
        }
        
    }
    
    return true;
}


size_t VariableSlot::getDim( void ) const {
    
    return lengths.size();
}



/** Get the value of the variable */
const RevObject& VariableSlot::getRevObject( void ) const {
    
    return variable[0]->getRevObject();
}


/** Get the value of the variable */
RevObject& VariableSlot::getRevObject( void ) {
    
    return variable[0]->getRevObject();
}


//const Variable& VariableSlot::getVariable(void) const {
//    return *variable[0];
//}
//
//
//Variable& VariableSlot::getVariable(void) {
//    return *variable[0];
//}


RevPtr<const Variable> VariableSlot::getVariable(const std::vector<int> &indices) const {
    size_t index = 0;
    size_t elements = 1;
    
    if (indices.size() != lengths.size()) {
        throw RbException("Index out of bounds! Unequal indices for variables not supported (yet).");
    }
    
    for (int i = int(lengths.size())-1; i >= 0; --i) {
        // test for boundaries
        if (indices[i] >= lengths[i] || indices[i] <= 0) {
            throw RbException("Index out of bounds! Cannot access variable with index ...");
        }
        index += indices[i] * elements;
        elements *= lengths[i];
    }
    
    
    return RevPtr<const Variable>( variable[index] );
}


RevPtr<Variable> VariableSlot::getVariable(const std::vector<int> &indices) {
    
    size_t index = 0;
    size_t elements = 1;
    
    if (indices.size() < lengths.size()) 
    {
        
        RevPtr<Variable> var = getVectorizedVariable( indices );
        var->setName( label );
        
        return var;
    }
    else 
    {
    
        for (int i = int(lengths.size())-1; i >= 0; --i) 
        {
            // test for boundaries
            if (indices[i] >= lengths[i]) 
            {
                throw RbException("Index out of bounds! Cannot access variable with index ...");
            }
            index += indices[i] * elements;
            elements *= lengths[i];
        }
    }
    
    
    return variable[index];
}


//const RevPtr<Variable>& VariableSlot::getVariablePtr(void) const {
//    return variable[0];
//}


RevPtr<Variable> VariableSlot::getVectorizedVariable(const std::vector<int> &indices) const {
    
    int size = lengths[indices.size()];
    
    // Package arguments
    std::vector<Argument> args;
    
    if ( indices.size() + 1 == lengths.size() ) {
        size_t index = 0;
        size_t elements = size;
        for (int i = int(lengths.size())-2; i >= 0; --i) {
            // test for boundaries
            if (indices[i] >= lengths[i]) {
                throw RbException("Index out of bounds! Cannot access variable with index ...");
            }
            index += indices[i] * elements;
            elements *= lengths[i];
        }
        
        for (int i = 0; i < size; ++i) {
            args.push_back( Argument( variable[index+i], "") );
        }
    }
    else {
        std::vector<int> tmp_indices = indices;
        tmp_indices.push_back( 0 );
        for (int i = 0; i < size; ++i) {
            tmp_indices.pop_back();
            tmp_indices.push_back( i );
            args.push_back( Argument( getVectorizedVariable(tmp_indices), "") );
        }
    }
    
    Environment &env = Workspace::userWorkspace();
    Function *func = env.getFunction("v", args).clone();
    
    if (func == NULL)
        throw(RbException("Could not find function called 'v' taking specified arguments"));
    
    func->processArguments( args );
    func->setExecutionEnviroment( &env );
    
    RevObject* v = func->execute();
    delete func;
    
    return new Variable( v );
}


/** Is variable valid for the slot? Additional type checking here */
bool VariableSlot::isValidVariable( const RevObject& newVariable ) const {
    
    return true;    // No additional requirements here, but see MemberSlot
}



/* Print value of the slot variable */
void VariableSlot::printValue(std::ostream& o) const {
    
    if (variable[0] == NULL)
        o << "NULL";
    else
        variable[0]->printValue(o);
}


void VariableSlot::resetNames(std::string const &l, size_t level, int &index) {
    
    int size = lengths[level];
    
    // increase the level of depth
    level++;
    
    for (int i = 0; i < size; ++i) {
        if (lengths.size() == level ) {
            // we reached the depth of the slot
            
            // construct the name
            std::stringstream o;
            o << l << (i+1) << "]";
            variable[index]->setName( o.str() );
            ++index;
        }
        else {
            
            // construct the name
            std::stringstream o;
            o << l << (i+1) << ",";
            
            // recursive call
            resetNames(o.str(), level, index);
        }
    }
    
}


void VariableSlot::resetVariables(std::vector<RevPtr<Variable> > &v, std::vector<int> &l, std::vector<int> &indices) {
    
    int length = l[indices.size()];
    indices.push_back(0);
    for (int i = 0; i < length; ++i) {
        indices.pop_back();
        indices.push_back( i );
        
        // if we have reached the depth of the dimensions
        if ( indices.size() == l.size() ) {
            // compute the old position
            int oldIndex = 0;
            int elements = 1;
            for (int j = int(l.size())-1; j >= 0; --j) {
                oldIndex += indices[j] * elements;
                elements *= l[j];
            }
            
            // compute the new position
            int newIndex = 0;
            elements = 1;
            for (int j = int(l.size())-1; j >= 0; --j) {
                newIndex += indices[j] * elements;
                elements *= lengths[j];
            }
            
            variable[newIndex] = v[oldIndex];
        }
        else {
            resetVariables(v, l, indices);
        }
    }
    
    indices.pop_back();
}


/** Set variable */
void VariableSlot::setVariable(const RevPtr<Variable>& var) {
    
    // set the new variable
    variable[0] = var;
    
    // if our name is not empty we set the name of the DAG node
    if ( label != "" ) {
//        var->getValue().setName( label );
        throw RbException("Incomplete implementation");
    }
}


/** Make sure we can print to stream using << operator */
std::ostream& operator<<(std::ostream& o, const VariableSlot& x) {
    
    o << "<Slot>";
    if ( x.getLabel() != "" )
        o << " " << x.getLabel();
    o << " = ";
    x.getRevObject().printValue(o);

    return o;
}

#endif
