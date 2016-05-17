#ifndef MultispeciesCoalescent_H
#define MultispeciesCoalescent_H

#include "RbVector.h"
#include "Tree.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    class Clade;
    
    class MultispeciesCoalescent : public TypedDistribution<Tree> {
        
    public:
        MultispeciesCoalescent(const TypedDagNode<Tree> *st, const std::vector<Taxon> &t);
        virtual                                            ~MultispeciesCoalescent(void);                                                                       //!< Virtual destructor
        
        // public member functions
        MultispeciesCoalescent*                             clone(void) const;                                                                                  //!< Create an independent clone
        double                                              computeLnProbability(void);
        void                                                redrawValue(void);
        void                                                setNes(TypedDagNode<RbVector<double> >* inputNes);
        void                                                setNe(TypedDagNode<double>* inputNe);

    protected:
        // Parameter management functions
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter
        
    private:
        
        double                                              getNe(size_t index) const;
        
        // helper functions
        void                                                attachTimes(Tree *psi, std::vector<TopologyNode *> &tips, size_t index, const std::vector<double> &times);
        void                                                buildRandomBinaryTree(std::vector<TopologyNode *> &tips);
        void                                                simulateTree(void);
        
        // members
   //     std::map<std::string, std::string>                  gene2species;
        std::vector<Taxon>                                  taxa;
        const TypedDagNode<Tree>*                           speciesTree;
        const TypedDagNode<RbVector<double> >*              Nes;
        const TypedDagNode<double >*                        Ne;
        size_t                                              num_taxa;
        double                                              logTreeTopologyProb;
        
      //  std::map<std::string, TopologyNode * > speciesNames2speciesNodes;
       // std::map< std::string, TopologyNode*> individualNames2geneTreeTips;
       // std::map< const TopologyNode *, std::set< TopologyNode* > > individualsPerBranch;
       // std::map<double, TopologyNode*> speciesAge2speciesNodes;*/
    };
    
}

#endif
