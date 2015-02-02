#include "LogicalAndFunction.h"

using namespace RevBayesCore;

LogicalAndFunction::LogicalAndFunction(const TypedDagNode<unsigned int> *l, const TypedDagNode<unsigned int> *r) : TypedFunction<unsigned int>( new unsigned(false) ),
    left( l ),
    right( r )
{
    // add the parameters as parents
    this->addParameter( left );
    this->addParameter( right );
    
    update();
}


LogicalAndFunction::~LogicalAndFunction( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



LogicalAndFunction* LogicalAndFunction::clone( void ) const {
    return new LogicalAndFunction( *this );
}


void LogicalAndFunction::update( void ) {
    
    *this->value = (left->getValue() && right->getValue());
    
}



void LogicalAndFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    
    if ( oldP == left ) {
        left = static_cast<const TypedDagNode<unsigned int>* >( newP );
    }
    if ( oldP == right ) {
        right = static_cast<const TypedDagNode<unsigned int>* >( newP );
    }
    
}

