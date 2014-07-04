#include "ModuloFunction.h"

using namespace RevBayesCore;

ModuloFunction::ModuloFunction(const TypedDagNode<int> *l, const TypedDagNode<int> *r) : TypedFunction<int>( new int(0) ),
    left( l ),
    right( r )
{
    // add the parameters as parents
    this->addParameter( left );
    this->addParameter( right );
    
    update();
}


ModuloFunction::~ModuloFunction( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



ModuloFunction* ModuloFunction::clone( void ) const
{
    return new ModuloFunction( *this );
}


void ModuloFunction::update( void )
{
    
    *this->value = (left->getValue() % right->getValue());
    
}



void ModuloFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if ( oldP == left )
    {
        left = static_cast<const TypedDagNode<int>* >( newP );
    }
    
    if ( oldP == right )
    {
        right = static_cast<const TypedDagNode<int>* >( newP );
    }
    
}

