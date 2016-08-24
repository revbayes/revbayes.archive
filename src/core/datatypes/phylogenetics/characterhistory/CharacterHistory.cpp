#include "CharacterHistory.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "Tree.h"

using namespace RevBayesCore;


CharacterHistory::CharacterHistory(Tree *t, size_t nc, size_t ns, bool rb ) :
    tree( t ),
    histories(),
    n_branches(),
    n_character( nc ),
    n_events( 0 ),
    n_states( ns ),
    useRootBranch( rb )
{
    
    if ( tree != NULL )
    {
        n_branches = tree->getNumberOfNodes() - 1 + ( useRootBranch ? 1 : 0 );
        // create a branch history object for each branch
        for (size_t i=0; i<n_branches; ++i)
        {
            histories.push_back( BranchHistory(n_character, n_states, i) );
        }
        
    }
    
}



/**
 * Dummy destructor.
 */
CharacterHistory::~CharacterHistory()
{
    
}


/**
 * Index operator.
 */
const BranchHistory& CharacterHistory::operator[](size_t i) const
{
    if ( i > histories.size() )
    {
        throw RbException("Index out of bounds in character history.");
    }
    
    return histories[i];
}


/**
 * Add an event onto the tree.
 * Here we will also
 */
void CharacterHistory::addEvent( CharacterEvent *e, size_t branch_index)
{
    
    BranchHistory &bh = histories[ branch_index ];
    bh.addEvent( e );
    
    ++n_events;
}



/**
 * Create a deep copy.
 */
CharacterHistory* CharacterHistory::clone( void ) const
{
    
    return new CharacterHistory( *this );
}


/**
 * Get the number of branches of the tree.
 */
size_t CharacterHistory::getNumberBranches( void ) const
{
    return n_branches;
}



/**
 * Get the number of total events over the tree.
 */
size_t CharacterHistory::getNumberEvents( void ) const
{
    return n_events;
}


/**
 * Get the number of states for this character.
 */
size_t CharacterHistory::getNumberStates( void ) const
{
    return n_states;
}

bool CharacterHistory::hasRootBranch( void ) const
{
    return useRootBranch;
}

/**
 * Pick a random event.
 */
CharacterEvent* CharacterHistory::pickRandomEvent( size_t &branch_index )
{
    
    RandomNumberGenerator *rng = GLOBAL_RNG;
    
    // randomly pick an index for the event
    size_t event_index = size_t( std::floor(n_events * rng->uniform01()) );
    for (size_t i=0; i<n_branches; ++i)
    {
        BranchHistory& bh = histories[i];
        if ( bh.getNumberEvents() > event_index )
        {
            branch_index = i;
            return bh.getEvent(event_index);
        }
        else
        {
            event_index -= bh.getNumberEvents();
        }
        
    }
    
    throw RbException("Could not pick a random event because I think there are more events than there actually are.");
}
 


/**
 * Remove the given event from our histories.
 */
void CharacterHistory::removeEvent( CharacterEvent *e, size_t branch_index)
{
    
    BranchHistory &bh = histories[ branch_index ];
    bh.removeEvent( e );
    
    --n_events;
}


/**
 * Set the new tree.
 */
void CharacterHistory::setTree(Tree *t)
{
    
    tree = t;
    if ( tree != NULL )
    {
        histories.clear();
        n_events = 0;
        n_branches = tree->getNumberOfNodes() - 1  + (useRootBranch ? 1 : 0);
        
        // create a branch history object for each branch
        for (size_t i=0; i<n_branches; ++i)
        {
            histories.push_back( BranchHistory(n_character, n_states, i) );
        }
        
    }
    
}





