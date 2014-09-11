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
     * children, checking for cycles in the graph, and printing of parents
     * and children.
     */

    class DagNode {
    
    public:

        virtual                                     ~DagNode(void);             //!< Virtual destructor

        // Basic functionality (pure virtual functions)
        virtual DagNode*                            clone(void) const = 0;
        virtual DagNode*                            cloneDAG(std::map<const DagNode*, DagNode*> &nodesMap) const = 0;                           //!< Clone the entire DAG which is connected to this node
        virtual size_t                              getNumberOfElements(void) const = 0;                                                        //!< Get the number of elements for this value
        virtual void                                printName(std::ostream &o, const std::string &sep, int l=-1, bool left=true) const = 0;     //!< Monitor/Print this variable
        virtual void                                printStructureInfo(std::ostream &o, bool verbose=false) const = 0;                          //!< Print the structural information (e.g. name, value-type, distribution/function, children, parents, etc.)
        virtual void                                printValue(std::ostream &o, const std::string &sep, int l=-1, bool left=true) const = 0;    //!< Monitor/Print this variable

        // Stochastic node functions (default implementation for other nodes here)
        virtual double                              getLnProbability(void) { return 0.0; }
        virtual double                              getLnProbabilityRatio(void) { return 0.0; }
        virtual void                                redraw(void) {}                                                                             //!< Redraw value (applies only to stochastic nodes)

        // DAG construction and manipulation functions
        void                                        addChild(DagNode *child) const;                                                             //!< Add a new child node
        void                                        removeChild(DagNode *child) const;
        void                                        replace(DagNode *n);                                                                        //!< Replace this node with node p.
        virtual void                                setName(const std::string &n);                                                              //!< Set the name of this variable for identification purposes.

        // Public functions for using the DAG
        void                                        getAffectedStochasticNodes(std::set<DagNode*>& affected);                                   //!< Get affected downstream stochastic nodes (not including this one)
        const std::set<DagNode*>&                   getChildren(void) const;                                                                    //!< Get the set of parents
        DagNode*                                    getFirstChild(void) const;                                                                  //!< Get the first child from a our set
        const std::string&                          getName(void) const;                                                                        //!< Get the of the node
        bool                                        isAssignable(void) const;                                                                   //!< Is this DAG node modifiable by user?
        virtual bool                                isClamped(void) const;                                                                      //!< Is this node clamped? Only stochastic nodes might be clamped.
        virtual bool                                isComposite(void) const;                                                                    //!< Is this DAG node composite?
        virtual bool                                isConstant(void) const;                                                                     //!< Is this DAG node constant?
        virtual bool                                isNAValue(void) const;                                                                      //!< Is this an NA value node?
        virtual bool                                isSimpleNumeric(void) const;                                                                //!< Is this variable a simple numeric variable? Currently only integer and real number are.
        virtual bool                                isStochastic(void) const;                                                                   //!< Is this DAG node stochastic?
        virtual void                                setPriorOnly(bool tf);                                                                      //!< Set whether we want to have the probability of the prior only.

        // Partial update functions
        void                                        addTouchedElementIndex(size_t i);                                                           //!< Add the index of an element that has been touch (usually for vector-like values)
        void                                        clearTouchedElementIndices(void);
        const std::set<size_t>&                     getTouchedElementIndices(void) const;                                                       //!< Get the indices of the touches elements. If the set is empty, then all elements might have changed.

        // Smart pointer / reference counting support
        size_t                                      decrementReferenceCount(void) const;                                                        //!< Decrement the reference count for reference counting in smart pointers
        size_t                                      getReferenceCount(void) const;                                                              //!< Get the reference count for reference counting in smart pointers
        void                                        incrementReferenceCount(void) const;                                                        //!< Increment the reference count for reference counting in smart pointers

        // Reinitialization functions
        virtual void                                reInitialized(void);                                                                        //!< The DAG was re-initialized so maybe you want to reset some stuff
        virtual void                                reInitializeAffected(void);                                                                 //!< The DAG was re-initialized so maybe you want to reset some stuff
        virtual void                                reInitializeMe(void);                                                                       //!< The DAG was re-initialized so maybe you want to reset some stuff


        // Parent management functions that nodes with parents need to override
        virtual std::set<const DagNode*>            getParents(void) const;                                                                     //!< Get the set of parents (empty set here)
        virtual void                                swapParent(const DagNode *oldP, const DagNode *newP);                                       //!< Exchange the parent node which includes setting myself as a child of the new parent and removing myself from my old parents children list
        
    protected:
                                                    DagNode(const std::string &n);                                                              //!< Constructor
                                                    DagNode(const DagNode &n);                                                                  //!< Copy Constructor
    
        DagNode&                                    operator=(const DagNode &d);                                                                //!< Overloaded assignment operator
        
        // Helper functions maintaining consistent DAG state
        virtual void                                getAffected(std::set<DagNode*>& affected, DagNode* affecter);                               //!< Get affected stochastic nodes (including this one, if applicable)
        virtual void                                keep(DagNode* affecter) = 0;                                                                //!< Keep value / state
        virtual void                                restore(DagNode *restorer) = 0;                                                             //!< Restore value / state
        virtual void                                touch(DagNode *toucher) = 0;                                                                //!< Touch me and dispatch message, if applicable
    
        // Helper functions for priting
        void                                        printChildren(std::ostream& o, size_t indent, size_t lineLen, bool verbose=false) const;    //!< Print children DAG nodes
        void                                        printParents(std::ostream& o, size_t indent, size_t lineLen, bool verbose=false) const;     //!< Print children DAG nodes
        
        // Member variables
        mutable std::set<DagNode*>                  children;                                                                                   //!< Children of this node in the model DAG
        std::string                                 name;                                                                                       //!< Name of this node
        std::set<size_t>                            touchedElements;                                                                            //!< Touched element indices (for containers)
        bool                                        priorOnly;                                                                                  //!< Calculate only prior probability
    
    private:

        // Reference count
        mutable size_t                              refCount;                                                                                   //!< Reference count of this node
    };

}

#endif

