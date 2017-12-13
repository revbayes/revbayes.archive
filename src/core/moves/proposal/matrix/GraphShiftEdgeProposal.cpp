#include "GraphShiftEdgeProposal.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbVector.h"
#include "TypedDagNode.h"

#include <cmath>
#include <iostream>

using namespace RevBayesCore;

/**
 * Constructor
 *
 * Here we simply allocate and initialize the Proposal object.
 */
GraphShiftEdgeProposal::GraphShiftEdgeProposal( StochasticNode<MatrixReal> *n, const RbVector<long>& v, double l, bool s) : Proposal(),
array(NULL),
matrix( n ),
vertices( v ),
sampling_probability( l ),
storedValue( 0.0 ),
symmetric( s ),
undo_needed( false )
{
    // build the complete edge list if no edges are provided
    vertex_list_length = vertices.size();
    if (vertex_list_length == 0) {
        
        for (size_t i = 0; i < n->getValue().size(); i++) {
            vertices.push_back(i+1);
        }
        vertex_list_length = vertices.size();
    }
    
    // tell the base class to add the node
    addNode( matrix );
    
}

/**
 * The cleanProposal function may be called to clean up memory allocations after AbstractMove
 * decides whether to accept, reject, etc. the proposed value.
 *
 */
void GraphShiftEdgeProposal::cleanProposal( void )
{
    ; // do nothing
}

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the proposal.
 */
GraphShiftEdgeProposal* GraphShiftEdgeProposal::clone( void ) const
{
    
    return new GraphShiftEdgeProposal( *this );
}


/**
 * Get Proposals' name of object
 *
 * \return The Proposals' name.
 */
const std::string& GraphShiftEdgeProposal::getProposalName( void ) const
{
    static std::string name = "MatrixRealSingleElementSlideMove";
    
    return name;
}


/**
 * Perform the proposal.
 *
 * A sliding proposal draws a random uniform number u ~ unif (-0.5,0.5)
 * and MatrixRealSingleElementSlidings the current vale by
 * delta = lambda * u
 * where lambda is the tuning parameter of the proposal to influence the size of the proposals.
 *
 * \return The hastings ratio.
 */
double GraphShiftEdgeProposal::doProposal( void )
{
    
    // undo not necessarily needed
    undo_needed = false;
    
    // clear touched edge elements set
    touched_edge_elements = std::set<size_t>();
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    MatrixReal& v = matrix->getValue();
    
    // Find all vertices with >1 edge
    std::vector<size_t> valid_vertices;
    std::vector< std::vector<size_t> > edge_list( vertices.size() );
    
    // Store all edges
    if (symmetric)
    {
        for (size_t i = 0; i < vertices.size(); i++)
        {
            size_t idx = vertices[i]-1;
            for (size_t j = 0; j < vertices.size(); j++)
            {
                size_t jdx = vertices[j]-1;
                if ( v[idx][jdx] > 0 ) {
                    valid_vertices.push_back( idx );
                    break;
                }
            }
        }
    }
    
    // Sample one vertex among vertices of degree > 0
    size_t vertex_sample_pos = size_t( rng->uniform01() * valid_vertices.size() );
    from_idx = valid_vertices[ vertex_sample_pos ];
    
    // Collect all the present and absent edges adjacent to the sampled vertex
    std::vector<size_t> present_edge;
    std::vector<size_t> absent_edge;
    for (size_t i = 0; i < vertices.size(); i++)
    {
        size_t to_idx = vertices[i]-1;
        if (v[from_idx][to_idx] == 0)
        {
            absent_edge.push_back(to_idx);
        }
        else
        {
            present_edge.push_back(to_idx);
        }
    }
    
    // abort if no shiftable edges
    if (present_edge.size() == 0 || absent_edge.size() == 0)
    {
        return 0.0;
    }
    
    // moving forward, undoo is needed
    undo_needed = true;
    
    // Sample one present edge to shift to the position of an absent edge
    present_switch_idx = present_edge[ size_t( rng->uniform01() * present_edge.size() ) ];
    absent_switch_idx = absent_edge[ size_t( rng->uniform01() * absent_edge.size() ) ];
    
    
//    std::cout << "before\n";
//    std::cout << v << "\n";
    // Switch sampled values
    v[ from_idx ][ present_switch_idx ] = 0;
    v[ from_idx ][ absent_switch_idx ] = 1;
    if (symmetric) {
        v[ present_switch_idx ][ from_idx ] = 0;
        v[ absent_switch_idx ][ from_idx ] = 1;
    }
    
    
//    std::cout << "after\n";
//    std::cout << v << "\n";
    
    matrix->addTouchedElementIndex( from_idx * v.getNumberOfRows() + present_switch_idx );
    matrix->addTouchedElementIndex( from_idx * v.getNumberOfRows() + absent_switch_idx );
    if (symmetric) {
        matrix->addTouchedElementIndex( absent_switch_idx * v.getNumberOfRows() + from_idx );
        matrix->addTouchedElementIndex( present_switch_idx * v.getNumberOfRows() + from_idx );
    }

//    // sample the vertex to update w.p. 1
//    size_t first_index = size_t( rng->uniform01() * vertex_list_length );
//    touched_edge_elements.insert( first_index );
//    
//    // sample additional edges to update w.p. p
//    for (size_t i = 0; i < edge_list_length; i++) {
//        if (rng->uniform01() < sampling_probability) {
//            touched_edge_elements.insert( i );
//        }
//    }
//    
//    // flip and touch all sampled edges
//    for (std::set<size_t>::iterator it = touched_edge_elements.begin(); it != touched_edge_elements.end(); it++) {
//        size_t i = edges[ *it ][ 0 ] - 1;
//        size_t j = edges[ *it ][ 1 ] - 1;
//        v[i][j] = ( v[i][j] == 1 ? 0 : 1 );
//        if (symmetric) {
//            v[j][i] = v[i][j];
//        }
//        matrix->addTouchedElementIndex( i * v.getNumberOfRows() + j );
//        
//    }
    
    // symmetric proposal
    return 0;
}


/**
 *
 */
void GraphShiftEdgeProposal::prepareProposal( void )
{
    
}


/**
 * Print the summary of the Proposal.
 *
 * The summary just contains the current value of the tuning parameter.
 * It is printed to the stream that it passed in.
 *
 * \param[in]     o     The stream to which we print the summary.
 */
void GraphShiftEdgeProposal::printParameterSummary(std::ostream &o) const
{
    
    o << "p = " << sampling_probability;
    
}


/**
 * Reject the Proposal.
 *
 * Since the Proposal stores the previous value and it is the only place
 * where complex undo operations are known/implement, we need to revert
 * the value of the variable/DAG-node to its original value.
 */
void GraphShiftEdgeProposal::undoProposal( void )
{
    
    if (!undo_needed) {
        return;
    }
    
    MatrixReal& v = matrix->getValue();
    
    v[ from_idx ][ present_switch_idx ] = 1;
    v[ from_idx ][ absent_switch_idx ] = 0;
    if (symmetric) {
        v[ present_switch_idx ][ from_idx ] = 1;
        v[ absent_switch_idx ][ from_idx ] = 0;
    }

    // clear all touched elements
    matrix->clearTouchedElementIndices();
    
    // clear touched edge elements set
    touched_edge_elements = std::set<size_t>();
}


/**
 * Swap the current variable for a new one.
 *
 * \param[in]     oldN     The old variable that needs to be replaced.
 * \param[in]     newN     The new RevVariable.
 */
void GraphShiftEdgeProposal::swapNodeInternal(DagNode *oldN, DagNode *newN)
{
    if (oldN == array)
    {
        array = static_cast< StochasticNode<RbVector<RbVector<double> > >* >(newN) ;
    }
    else
    {
        matrix = static_cast< StochasticNode<MatrixReal>* >(newN) ;
    }
    
}


/**
 * Tune the Proposal to accept the desired acceptance ratio.
 *
 * The acceptance ratio for this Proposal should be around 0.44.
 * If it is too large, then we increase the proposal size,
 * and if it is too small, then we decrease the proposal size.
 */
void GraphShiftEdgeProposal::tune( double rate )
{
    
    if ( rate > 0.44 )
    {
        sampling_probability *= (1.0 + ((rate-0.44)/0.56) );
    }
    else
    {
        sampling_probability /= (2.0 - rate/0.44 );
    }
    
    // bound value s.t. 0 < switch_probability <= 1.0
    if (sampling_probability > 1.0)
    {
        sampling_probability = 1.0;
    }
    else if (sampling_probability < 1.0 / vertex_list_length)
    {
        sampling_probability = 1.0 / vertex_list_length;
    }
}

