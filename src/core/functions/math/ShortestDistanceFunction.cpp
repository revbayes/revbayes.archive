//
//  ShortestDistanceFunction.cpp
//  revbayes-branch-proj
//
//  Created by Michael Landis on 2/15/17.
//  Copyright © 2017 Michael Landis. All rights reserved.
//

#include "ShortestDistanceFunction.h"
#include "RbConstants.h"
#include "RbVector.h"
#include "TypedFunction.h"
#include <cmath>
#include <set>
#include <vector>

using namespace RevBayesCore;

ShortestDistanceFunction::ShortestDistanceFunction(const TypedDagNode<RbVector<RbVector<int> > >* adj, const TypedDagNode<RbVector<RbVector<double> > >* dist) : TypedFunction<RbVector<RbVector<double> > >( new RbVector<RbVector<double> >() ),
num_nodes( adj->getValue().size() ),
adjacencies( adj ),
distances( dist )

{
    
    *value = RbVector<RbVector<double> >(num_nodes, RbVector<double>(num_nodes, 1));
   
    addParameter( adjacencies );
    addParameter( distances );
}


ShortestDistanceFunction* ShortestDistanceFunction::clone( void ) const
{
    return new ShortestDistanceFunction(*this);
}


std::vector<std::set<size_t> > ShortestDistanceFunction::createAdjacencySets(const RbVector<RbVector<int> >& adj)
{

    std::vector<std::set<size_t> > s;
    for (size_t i = 0; i < num_nodes; i++)
    {
        std::set<size_t> t;
        for (size_t j = 0; j < num_nodes; j++)
        {
            if (i != j && adj[i][j] > 0)
            {
                t.insert(j);
            }
        }
        s.push_back(t);
    }
    
    return s;
}

RbVector<RbVector<double> > ShortestDistanceFunction::findShortestPaths(const RbVector<RbVector<int> >& adj, const RbVector<RbVector<double> >& dist)
{
    
    std::vector<std::set<size_t> > adj_set = createAdjacencySets(adj);
    
    // 0. Keep track of final distances for all origin nodes under Dijkstra's
    RbVector<RbVector<double> > return_distances(num_nodes, std::vector<double>(num_nodes, RbConstants::Double::inf));
    
    // run Dijkstra's for each origin node
    for (size_t i = 0; i < num_nodes; i++) {
        
        size_t current_node = i;
        
        // 1. Set the initial node as current.
        ; // done by the for loop
        
        // 2. Assign to every node a tentative distance value: set it to zero for our initial node and to infinity for all other nodes.
        std::vector<std::vector<double> > tmp_distances(num_nodes, std::vector<double>(num_nodes, RbConstants::Double::inf));
        tmp_distances[i][i] = 0.0;

        // Mark all other nodes unvisited.
        std::vector<bool> visited_list(num_nodes, false);
        visited_list[i] = true;
        
        // Create a set of all the unvisited nodes called the unvisited set.
        std::set<size_t> unvisited_nodes;
        for (size_t j = 0; j < num_nodes; j++)
        {
            if (j != i) {
                unvisited_nodes.insert(j);
            }
        }

        bool stop = false;
        while (!stop) {
        
            // 3. For the current node, consider all of its unvisited neighbors and calculate their tentative distances.
            //    Compare the newly calculated tentative distance to the current assigned value and assign the smaller one.
            //    For example, if the current node A is marked with a distance of 6, and the edge connecting it with a neighbor B has length 2, then the distance to B (through A) will be 6 + 2 = 8.
            //    If B was previously marked with a distance greater than 8 then change it to 8. Otherwise, keep the current value.
        
            for (size_t j = 0; j < adj_set.size(); j++)
            {
                for (std::set<size_t>::iterator it = adj_set[current_node].begin(); it != adj_set[current_node].end(); it++)
                {
                    size_t k = *it;
                    
                    // do not revisit visited nodes
                    if (visited_list[k])
                        continue;
                    
                    if (dist[j][k] < tmp_distances[j][k])
                    {
                        tmp_distances[j][k] = dist[j][k];
                    }
                }
                
                
                // 4. When we are done considering all of the neighbors of the current node, mark the current node as visited and remove it from the unvisited set. A visited node will never be checked again.
                visited_list[j] = true;
                unvisited_nodes.erase(j);
                
                
                // 5. If the destination node has been marked visited (when planning a route between two specific nodes) or,
                //    if the smallest tentative distance among the nodes in the unvisited set is infinity (when planning a complete traversal; occurs when there is no connection between the initial node and remaining unvisited nodes), then stop. The algorithm has finished.
                double smallest_value = RbConstants::Double::inf;
                size_t small_node = RbConstants::Integer::neginf;
                bool found_smaller = false;
                for (std::set<size_t>::iterator jt = unvisited_nodes.begin(); jt != unvisited_nodes.end(); jt++)
                {
                    size_t j = *jt;
                    for (std::set<size_t>::iterator kt = adj_set[current_node].begin(); kt != adj_set[current_node].end(); kt++)
                    {
                        size_t k = *kt;
                        if (tmp_distances[j][k] < smallest_value)
                        {
                            found_smaller = true;
                            small_node = k;
                            break;
                        }
                        
                    }
                    if (found_smaller)
                    {
                        break;
                    }
                }
                
                if (!found_smaller)
                {
                    // this is where we report the shortest path for origin node i to all remaining nodes
                    stop = true;
                }
                
                // 6. Otherwise, select the unvisited node that is marked with the smallest tentative distance, set it as the new "current node", and go back to step 3.
                current_node = small_node;
            }
        }
        
        // reconcile the different shortest-path distances between area-pairs to store into return distances
        
    }
    
    return return_distances;
    
}

void ShortestDistanceFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == adjacencies)
    {
        adjacencies = static_cast<const TypedDagNode<RbVector<RbVector<int> > >* >( newP );
    }
    else if (oldP == distances)
    {
        distances = static_cast<const TypedDagNode<RbVector<RbVector<double> > >* >( newP );
    }
    
}

void ShortestDistanceFunction::update( void )
{
    // get the new values
    const RbVector<RbVector<int> >& adj = adjacencies->getValue();
    const RbVector<RbVector<double> >& dist = distances->getValue();
    
    // find all the shortest paths
    *value = findShortestPaths(adj, dist);
    
}
