#include "LogicalUnotFunction.h"

using namespace RevBayesCore;

LogicalUnotFunction::LogicalUnotFunction(const TypedDagNode<unsigned int> *l) : TypedFunction<unsigned int>( new unsigned(false) ),
    left( l )
{
    // add the parameters as parents
    this->addParameter( left );
    
    update();
}


LogicalUnotFunction::~LogicalUnotFunction( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



LogicalUnotFunction* LogicalUnotFunction::clone( void ) const
{

    return new LogicalUnotFunction( *this );
}


void LogicalUnotFunction::update( void )
{
    
    *(this->value) = !( left->getValue() );
    
}



void LogicalUnotFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    
    if ( oldP == left ) {
        left = static_cast<const TypedDagNode<unsigned int>* >( newP );
    }
    
}

