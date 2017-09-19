//
//  PhylogeneticDistanceRateModifier.h
//  revbayes-branch-proj
//
//  Created by Michael Landis on 2/24/17.
//  Copyright © 2017 Michael Landis. All rights reserved.
//

#ifndef PhylogeneticDistanceRateModifier_h
#define PhylogeneticDistanceRateModifier_h


#include "CharacterHistoryRateModifier.h"
#include "StochasticNode.h"
#include "TopologyNode.h"

#include <set>
#include <string>
#include <vector>


namespace RevBayesCore
{
    
    
    
    class PhylogeneticDistanceRateModifier : public CharacterHistoryRateModifier
    {
    public:
        PhylogeneticDistanceRateModifier(size_t ns, size_t nc);
        PhylogeneticDistanceRateModifier(const PhylogeneticDistanceRateModifier& g);
        
        
        PhylogeneticDistanceRateModifier&   assign(const Assignable &m);
        double                              computeRateMultiplier(std::vector<CharacterEvent*> currState, CharacterEvent* newState, double age=0.0);
        double                              computeSiteRateMultiplier(const TopologyNode& node, CharacterEvent* currState, CharacterEvent* newState, double age=0.0);
        double                              computeSiteRateMultiplier(const TopologyNode& node, unsigned currState, unsigned newState, unsigned charIdx=0, double age=0.0);
        
        void                                setTree(const Tree& t);
        void                                setScale(const double& s);
        
        void                                update(void);
        PhylogeneticDistanceRateModifier*   clone(void) const;
        
    protected:
        
        
    private:
        Tree                                tau;
        double                              scale;
        size_t                              num_branches;
        std::vector<std::vector<double> >   node_phylo_adjacencies;
        std::vector<std::vector<double> >   node_phylo_distances;
    };
    
    std::ostream& operator<<(std::ostream& o, const PhylogeneticDistanceRateModifier& x);
}

#endif /* PhylogeneticDistanceRateModifier_h */
