//
//  MrcaFunction.h
//  revbayes_mlandis
//
//  Created by Michael Landis on 1/9/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

// This implementation is very simple but slow MRCA (aka LCA) calculator.
// For performance improvement, use a well-known LCA algorithm, e.g. Fischer-Heun, Berkman-Vishkin (see http://dx.doi.org/10.1007%2F11780441_5)

#ifndef __revbayes_mlandis__MrcaFunction__
#define __revbayes_mlandis__MrcaFunction__

#include "TypedDagNode.h"
#include "TypedFunction.h"
#include <vector>

namespace RevBayesCore {
    
    template<class treeType>
    class MrcaFunction : public TypedFunction< RbVector<std::vector<TopologyNode*> > >
    {
    public:
        MrcaFunction(const TypedDagNode<treeType>* t);
        MrcaFunction(const MrcaFunction &n);
        virtual ~MrcaFunction(void);
        
        MrcaFunction*                                       clone(void) const;                                                                        //!< Create an independent clone
        void                                                update(void);
        TopologyNode*                                       getMrca(const TopologyNode* p, const TopologyNode* q) const;
        TopologyNode*                                       getMrca(const size_t a, const size_t b) const;
        size_t                                              getNumTaxa(void);
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);
        
    private:
        void initializeMrcaMatrix(void);
        bool passDownIsInClade(TopologyNode* p, TopologyNode* q);
        
        const TypedDagNode<treeType>* tau;
        size_t numTaxa;
    };
    
    template<class treeType>
    std::ostream& operator<<(std::ostream& o, const MrcaFunction<treeType>& x);                                //!< Overloaded output operator

    std::ostream& operator<<(std::ostream& o, const std::vector<std::vector<TopologyNode*> >& x);
    std::ostream& operator<<(std::ostream& o, const std::vector<TopologyNode*>& x);

}

#include "MrcaFunction.h"
#include "Tree.h"
#include "TopologyNode.h"

using namespace RevBayesCore;

template<class treeType>
MrcaFunction<treeType>::MrcaFunction(const TypedDagNode<treeType>* t) :
    TypedFunction< RbVector<std::vector<TopologyNode*> > >(new std::vector<std::vector<TopologyNode*> >)
    ,tau(t)
{
    addParameter(tau);
    initializeMrcaMatrix();
    //update();
}

template<class treeType>
MrcaFunction<treeType>::MrcaFunction(const MrcaFunction& n) :
    TypedFunction< RbVector<std::vector<TopologyNode*> > >( n ),
    tau(n.tau),
    numTaxa(n.numTaxa)
{
    
}

template<class treeType>
MrcaFunction<treeType>::~MrcaFunction(void)
{
    ;
}

template<class treeType>
void MrcaFunction<treeType>::initializeMrcaMatrix(void)
{
    numTaxa = tau->getValue().getNumberOfTips();
    (*value).resize(numTaxa);
    for (size_t i = 0; i < numTaxa; i++)
    {
        (*value)[i].resize(numTaxa);
        for (size_t j = 0; j < numTaxa; j++)
        {
            (*value)[i][j] = NULL;
        }
    }
    
    //std::cout << "numTaxa\t" << numTaxa << "\n";
}

template<class treeType>
MrcaFunction<treeType>* MrcaFunction<treeType>::clone( void ) const {
    return new MrcaFunction( *this );
}


// Future improvement:
// keep track of which nodes need their mrca recomputed using dirty flags
template<class treeType>
void MrcaFunction<treeType>::update(void)
{
    // get tree variables
    size_t numNodes = tau->getValue().getNumberOfNodes();
    std::vector<TopologyNode*> nodes = tau->getValue().getNodes();
    const TopologyNode* root = &tau->getValue().getRoot();
    
    
    // quick fix, reset all mrca elts to NULL
    for (int i = 0; i < numTaxa; i++)
    {
        for (int j = 0; j < numTaxa; j++)
        {
            (*value)[i][j] = NULL;
        }
    }
    
    for (size_t i = 0; i < numNodes; i++)
    {
        TopologyNode* p = nodes[i];
        if (p->isTip())
        {
            // assign self as its own mrca
            size_t pIdx = p->getIndex();
            (*value)[pIdx][pIdx] = p;
            
            // for non-self mrca, travel towards root
            while (p != root)
            {
                p = &p->getParent();
                //std::cout << "p\t" << p->getIndex() << "\n";
                for (size_t j = i + 1; j < numNodes; j++)
                {
                    TopologyNode* q = nodes[j];
                    if (q->isTip())
                    {
                        bool found = passDownIsInClade(p,q);
                        size_t qIdx = q->getIndex();
                        //std::cout << "q\t" << q->getIndex() << "\n";

                        // if internal node is mrca of clade containing (original p) and q
                        // and if mrca has not yet been recorded
                        // then assign to symmetric mrca elements
                        
                        if (found && (*value)[pIdx][qIdx] == NULL) // use dirty flags instead of NULL (implicit uninitialized flag)
                        {
                            (*value)[pIdx][qIdx] = p;
                            (*value)[qIdx][pIdx] = p;
                        }
                    }
                }
            }
        }
    }
    
    
    /*
    std::cout << "MRCA\n";
    //std::cout << numTaxa <<"\n";
    for (int i = 0; i < numTaxa; i++)
    {
        for (int j = 0; j < numTaxa; j++)
        {
            std::cout << (*value)[i][j]->getIndex() << "\t";
        }
        std::cout << "\n";
    }
    std::cout << tau->getValue() << "\n";
    */
    
}

template<class treeType>
bool MrcaFunction<treeType>::passDownIsInClade(TopologyNode* p, TopologyNode* q)
{
    // returns true if is q in clade with MRCA p
    bool found = false;
    
    if (p != NULL)
    {
        size_t numChildren = p->getNumberOfChildren();
        
        //std::vector<TopologyNode*> ndeDescendants = p->getDescendants();
        //for (std::vector<Node*>::iterator it = ndeDescendants.begin(); it != ndeDescendants.end(); it++)
        for (size_t i = 0; i < numChildren; i++)
        {
            found = passDownIsInClade(&p->getChild(i), q);
            if (found) {
                //std::cout << "FOUND\n";
                return found;
            }
        }
        
        if (p == q)
        {
            //std::cout << p->getIndex() << " == " << q->getIndex() << "\n";
            found = true;
        }
        //else
            //std::cout << p->getIndex() << " != " << q->getIndex() << "\n";
    }
    
    return found;
    
}

template<class treeType>
void MrcaFunction<treeType>::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    if (oldP == tau) {
        tau = static_cast<const TypedDagNode< treeType >* >( newP );
    }
}

template<class treeType>
TopologyNode* MrcaFunction<treeType>::getMrca(const TopologyNode* p, const TopologyNode* q) const
{
    return (*value)[p->getIndex()][q->getIndex()];
}

template<class treeType>
TopologyNode* MrcaFunction<treeType>::getMrca(const size_t a, const size_t b) const
{
    return (*value)[a][b];
}

template<class treeType>
size_t MrcaFunction<treeType>::getNumTaxa(void)
{
    return numTaxa;
}

template<class treeType>
std::ostream& RevBayesCore::operator<<(std::ostream& o, const MrcaFunction<treeType>& x)
{
    size_t numTaxa = x.getNumTaxa();
    for (size_t i = 0; i < numTaxa; i++)
    {
        for (size_t j = 0; j < numTaxa; j++)
        {
            if (j != 0)
                o << "\t";
            o << x.getMrca(i,j)->getIndex();
        }
        o << "\n";
    }

    return o;
}
                
std::ostream& RevBayesCore::operator<<(std::ostream& o, const std::vector<std::vector<TopologyNode*> >& x)
{
    std::vector<std::vector<TopologyNode*> >::const_iterator it1 = x.begin();
    std::vector<std::vector<TopologyNode*> >::const_iterator it1_end = x.end();
    
    for ( ; it1 != it1_end; it1++)
    {
        std::vector<TopologyNode*>::const_iterator it2 = it1->begin();
        std::vector<TopologyNode*>::const_iterator it2_end = it1->end();
        for ( ; it2 != it2_end; it2++)
        {
            if (it2 != it1->begin())
                o << "\t";
            o << (*it2)->getIndex();
        }
        o << "\n";
    }
    
    return o;
}

std::ostream& RevBayesCore::operator<<(std::ostream& o, const std::vector<TopologyNode*>& x)
{
  
    std::vector<TopologyNode*>::const_iterator it = x.begin();
    std::vector<TopologyNode*>::const_iterator it_end = x.end();
    for ( ; it != it_end; it++)
    {
        if (it != x.begin())
            o << "\t";
        o << (*it)->getIndex();
    }
    o << "\n";

    return o;
}
                


#endif /* defined(__revbayes_mlandis__MrcaFunction__) */
