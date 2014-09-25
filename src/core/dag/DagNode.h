#ifndef DagNode_H
#define DagNode_H

#include <map>
#include <set>
#include <string>
#include <vector>

namespace RevBayesCore {
    
    /**
     * @brief DAG node base class
     *
     * This is the base class for all DAG nodes in the model graph. Tasks that
     * are handled at this level include connecting the graph by adding
     * children, and printing of parents and children.
     */
    
    class DagNode {
        
    public:
        
        enum DagNodeTypes { CONSTANT, DETERMINISTIC, STOCHASTIC };              //!< Basic DAG node types
        
        virtual                                     ~DagNode(void);             //!< Virtual destructor
        
        // pure virtual methods
        virtual DagNode*                            clone(void) const = 0;
        virtual DagNode*                            cloneDAG(std::map<const DagNode*, DagNode*> &nodesMap) const = 0;                           //!< Clone the entire DAG which is connected to this node
        virtual size_t                              getNumberOfElements(void) const = 0;                                                        //!< Get the number of elements for this value
        virtual void                                printName(std::ostream &o, const std::string &sep, int l=-1, bool left=true) const = 0;     //!< Monitor/Print this variable
        virtual void                                printStructureInfo(std::ostream &o, bool verbose=false) const = 0;                          //!< Print the structural information (e.g. name, value-type, distribution/function, children, parents, etc.)
        virtual void                                printValue(std::ostream &o, const std::string &sep, int l=-1, bool left=true) const = 0;    //!< Monitor/Print this variable
        
        virtual double                              getLnProbability(void) { return 0.0; }
        virtual double                              getLnProbabilityRatio(void) { return 0.0; }
        virtual void                                redraw(void) {}                                                                             //!< Redraw the current value of the node (applies only to stochastic nodes)
        
        // public member functions
        void                                        addChild(DagNode *child) const;                                                             //!< Add a new child node
        void                                        addTouchedElementIndex(size_t i);                                                           //!< Add the index of an element that has been touch (usually for vector-like values)
        void                                        clearTouchedElementIndices(void);
        DagNode*                                    cloneDownstreamDag(std::map<const DagNode*, DagNode*> &nodesMap) const;                     //!< Clone the DAG which is downstream to this node (all children)
        void                                        collectDownstreamGraph(std::set<DagNode*> &nodes);                                          //!< Collect all nodes downstream from this node (incl the node)
        size_t                                      decrementReferenceCount(void) const;                                                        //!< Decrement the reference count for reference counting in smart pointers
        void                                        getAffectedNodes(std::set<DagNode*>& affected);                                             //!< Get affected downstream stochastic nodes (not including this one)
        const std::set<DagNode*>&                   getChildren(void) const;                                                                    //!< Get the set of parents
        std::string                                 getDagNodeType(void) const;
        DagNode*                                    getFirstChild(void) const;                                                                  //!< Get the first child from a our set
        const std::string&                          getName(void) const;                                                                        //!< Get the of the node
        size_t                                      getNumberOfChildren(void) const;                                                            //!< Get the number of children for this node
        size_t                                      getReferenceCount(void) const;                                                              //!< Get the reference count for reference counting in smart pointers
        const std::set<size_t>&                     getTouchedElementIndices(void) const;                                                       //!< Get the indices of the touches elements. If the set is empty, then all elements might have changed.
        void                                        incrementReferenceCount(void) const;                                                        //!< Increment the reference count for reference counting in smart pointers
        bool                                        isAssignable(void) const;                                                                   //!< Is this DAG node modifiable by user?
        virtual bool                                isClamped(void) const;                                                                      //!< Is this node clamped? Only stochastic nodes might be clamped.
        virtual bool                                isComposite(void) const;                                                                    //!< Is this DAG node composite?
        virtual bool                                isConstant(void) const;                                                                     //!< Is this DAG node constant?
        virtual bool                                isNAValue(void) const;                                                                      //!< Is this an NA value node?
        virtual bool                                isSimpleNumeric(void) const;                                                                //!< Is this variable a simple numeric variable? Currently only integer and real number are.
        virtual bool                                isStochastic(void) const;                                                                   //!< Is this DAG node stochastic?
        void                                        keep(void);
        virtual void                                keepAffected(void);                                                                         //!< Keep value of affected nodes
        virtual void                                reInitialized(void);                                                                        //!< The DAG was re-initialized so maybe you want to reset some stuff
        virtual void                                reInitializeAffected(void);                                                                 //!< The DAG was re-initialized so maybe you want to reset some stuff
        virtual void                                reInitializeMe(void);                                                                       //!< The DAG was re-initialized so maybe you want to reset some stuff
        void                                        removeChild(DagNode *child) const;
        void                                        replace(DagNode *n);                                                                        //!< Replace this node with node p.
        void                                        restore(void);
        virtual void                                restoreAffected(void);                                                                      //!< Restore value of affected nodes recursively
        virtual void                                setName(const std::string &n);                                                              //!< Set the name of this variable for identification purposes.
        virtual void                                setPriorOnly(bool tf);                                                                      //!< Set whether we want to have the probability of the prior only.
        void                                        touch(void);
        virtual void                                touchAffected(void);                                                                        //!< Touch affected nodes (flag for recalculation)
        
        // Parent management functions that nodes with parents need to override. Here we just return an empty set or throw an error.
        virtual std::set<const DagNode*>            getParents(void) const;                                                                     //!< Get the set of parents (empty set here)
        virtual void                                swapParent(const DagNode *oldP, const DagNode *newP);                                       //!< Exchange the parent node which includes setting myself as a child of the new parent and removing myself from my old parents children list
        
    protected:
        DagNode(const std::string &n);                                                              //!< Constructor
        DagNode(const DagNode &n);                                                                  //!< Copy Constructor
        
        DagNode&                                    operator=(const DagNode &d);                                                                //!< Overloaded assignment operator
        
        virtual void                                getAffected(std::set<DagNode*>& affected, DagNode* affecter);                               //!< Get affected stochastic nodes (including this one, if applicable)
        virtual void                                keepMe(DagNode* affecter) = 0;                                                              //!< Keep value of myself
        virtual void                                restoreMe(DagNode *restorer) = 0;                                                           //!< Restore value of this nodes
        virtual void                                touchMe(DagNode *toucher) = 0;                                                              //!< Touch myself (flag for recalculation)
        
        // helper functions
        void                                        printChildren(std::ostream& o, size_t indent, size_t lineLen, bool verbose=false) const;    //!< Print children DAG nodes
        void                                        printParents(std::ostream& o, size_t indent, size_t lineLen, bool verbose=false) const;     //!< Print children DAG nodes
        
        // members
        mutable std::set<DagNode*>                  children;                                                                                   //!< The children in the model graph of this node
        std::string                                 name;
        std::set<size_t>                            touchedElements;
        bool                                        priorOnly;
        DagNodeTypes                                type;
        
        
    private:
        
        mutable size_t                              refCount;
    };
    
}

#endif

