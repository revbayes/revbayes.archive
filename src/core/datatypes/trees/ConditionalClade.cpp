#include "ConditionalClade.h"


using namespace RevBayesCore;



/**
 * Default constructor that simply allocates the object.
 *
 * \param[in]    p    The parental/ancestral clade.
 * \param[in]    c    The child/descendant clade.
 */
ConditionalClade::ConditionalClade(const Clade &p, const Clade &c) :
    child( c ),
    parent( p )
{
    
}


/**
 * Get the child clade.
 *
 * \return    The child clade.
 */
const Clade& ConditionalClade::getChild( void ) const
{
    return child;
}


/**
 * Get the parent clade.
 *
 * \return    The parent clade.
 */
const Clade& ConditionalClade::getParent( void ) const
{
    return parent;
}

