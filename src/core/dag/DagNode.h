/**
 * @file
 * This file contains the declaration of the DAG node class, which is our base class for all DAG node is in the model graph.
 * It is merely used as a common base class to connect the entire graph.
 *
 * @brief Declaration of the base class DagNode.
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-06-17, version 1.0
 * @interface DagNode
 *
 * $Id$
 */



#ifndef DagNode_H
#define DagNode_H

#include <map>
#include <set>
#include <string>
#include <vector>

namespace RevBayesCore {
    
    class Monitor;
    class Move;

    class DagNode {
    
    public:
        
        enum DagNodeTypes { CONSTANT, DETERMINISTIC, STOCHASTIC };
    
        virtual                                                    ~DagNode(void);                                                                                      //!< Virtual destructor

        // pure virtual methods
        virtual DagNode*                                            clone(void) const = 0;
        virtual DagNode*                                            cloneDAG(std::map<const DagNode*, DagNode*> &nodesMap, std::map<std::string, const DagNode* > &names) const = 0; //!< Clone the entire DAG which is connected to this node
        virtual double                                              getLnProbability(void) = 0;
        virtual double                                              getLnProbabilityRatio(void) = 0;
        virtual size_t                                              getNumberOfElements(void) const = 0;                                                        //!< Get the number of elements for this value
        virtual void                                                printName(std::ostream &o, const std::string &sep, int l=-1, bool left=true) const = 0;     //!< Monitor/Print this variable
        virtual void                                                printStructureInfo(std::ostream &o, bool verbose=false) const = 0;                          //!< Print the structural information (e.g. name, value-type, distribution/function, children, parents, etc.)
        virtual void                                                printValue(std::ostream &o, int l=-1, bool left=true) const = 0;                            //!< Monitor/Print this variable
        virtual void                                                redraw(void) = 0;                                                                           //!< Redraw the current value of the node (applies only to stochastic nodes)
        
        // public member functions
        void                                                        addChild(DagNode *child) const;                                                             //!< Add a new child node
        void                                                        addMonitor(Monitor *m);                                                                     //!< Add a new monitor on this node
        void                                                        addMove(Move *m);                                                                           //!< Add a new move on this node
        void                                                        addTouchedElementIndex(size_t i);                                                           //!< Add the index of an element that has been touch (usually for vector-like values)
        void                                                        clearTouchedElementIndices(void);
        DagNode*                                                    cloneDownstreamDag(std::map<const DagNode*, DagNode*> &nodesMap) const;                     //!< Clone the DAG which is downstream to this node (all children)
        size_t                                                      decrementReferenceCount(void) const;                                                        //!< Decrement the reference count for reference counting in smart pointers
        void                                                        getAffectedNodes(std::set<DagNode *>& affected);                                            //!< get affected nodes
        const std::set<DagNode*>&                                   getChildren(void) const;                                                                    //!< Get the set of children
        std::string                                                 getDagNodeType(void) const;
        DagNode*                                                    getFirstChild(void) const;                                                                  //!< Get the first child from a our set
        const std::set<Monitor*>&                                   getMonitors(void) const;                                                                    //!< Get the set of monitors
        const std::set<Move*>&                                      getMoves(void) const;                                                                       //!< Get the set of moves
        const std::string&                                          getName(void) const;                                                                        //!< Get the of the node
        size_t                                                      getNumberOfChildren(void) const;                                                            //!< Get the number of children for this node
        virtual std::set<const DagNode*>                            getParents(void) const;                                                                     //!< Get the set of parents (empty set here)
        size_t                                                      getReferenceCount(void) const;                                                              //!< Get the reference count for reference counting in smart pointers
        const std::set<size_t>&                                     getTouchedElementIndices(void) const;                                                       //!< Get the indices of the touches elements. If the set is empty, then all elements might have changed.
        void                                                        incrementReferenceCount(void) const;                                                        //!< Increment the reference count for reference counting in smart pointers
        bool                                                        isAssignable(void) const;                                                                   //!< Is this DAG node modifiable by user?
        virtual bool                                                isClamped(void) const;                                                                      //!< Is this node clamped? Only stochastic nodes might be clamped.
        virtual bool                                                isConstant(void) const;                                                                     //!< Is this DAG node constant?
        virtual bool                                                isHidden(void) const;                                                                       //!< Is this DAG node hidden from the autogenerated graphviz model graph? (true for Element-lookup and Type-converter nodes)
        virtual bool                                                isSimpleNumeric(void) const;                                                                //!< Is this variable a simple numeric variable? Currently only integer and real number are.
        virtual bool                                                isStochastic(void) const;                                                                   //!< Is this DAG node stochastic?
        void                                                        keep(void);
        virtual void                                                keepAffected(void);                                                                         //!< Keep value of affected nodes
        virtual void                                                printValueElements(std::ostream &o, const std::string &sep, int l=-1, bool left=true) const;//!< Monitor/Print this variable by printing its elements separated by this separator
        virtual void                                                reInitialized(void);                                                                        //!< The DAG was re-initialized so maybe you want to reset some stuff
        virtual void                                                reInitializeAffected(void);                                                                 //!< The DAG was re-initialized so maybe you want to reset some stuff
        virtual void                                                reInitializeMe(void);                                                                       //!< The DAG was re-initialized so maybe you want to reset some stuff
        void                                                        removeChild(DagNode *child) const;                                                          //!< Remove this child node from our set of children.
        void                                                        removeMonitor(Monitor *m);                                                                  //!< Remove this monitor from our set.
        void                                                        removeMove(Move *m);                                                                        //!< Remove this move from our set.
        void                                                        replace(DagNode *n);                                                                        //!< Replace this node with node p.
        void                                                        restore(void);
        virtual void                                                restoreAffected(void);                                                                      //!< Restore value of affected nodes recursively
        void                                                        setHidden(bool tf);                                                                         //!< Set if this variable is hidden from printing.
        virtual void                                                setName(const std::string &n);                                                              //!< Set the name of this variable for identification purposes.
        void                                                        setParentNamePrefix(const std::string &p);
        virtual void                                                setPriorOnly(bool tf);                                                                      //!< Set whether we want to have the probability of the prior only.
        virtual void                                                swapParent(const DagNode *oldP, const DagNode *newP);                                       //!< Exchange the parent node which includes setting myself as a child of the new parent and removing myself from my old parents children list
        void                                                        touch(void);
        virtual void                                                touchAffected(void);                                                                        //!< Touch affected nodes (flag for recalculation)

        
    protected:
                                                                    DagNode(const std::string &n);                                                              //!< Constructor
                                                                    DagNode(const DagNode &n);                                                                  //!< Copy Constructor
    
        DagNode&                                                    operator=(const DagNode &d);                                                                //!< Overloaded assignment operator
        
        virtual void                                                getAffected(std::set<DagNode *>& affected, DagNode* affecter) = 0;                          //!< get affected nodes
        virtual void                                                keepMe(DagNode* affecter) = 0;                                                              //!< Keep value of myself
        virtual void                                                restoreMe(DagNode *restorer) = 0;                                                           //!< Restore value of this nodes
        virtual void                                                touchMe(DagNode *toucher) = 0;                                                              //!< Touch myself (flag for recalculation)
    
        // helper functions
        void                                                        getPrintableChildren(std::set<DagNode*> &c) const;
        void                                                        getPrintableParents(std::set<const DagNode*> &p) const;
        void                                                        printChildren(std::ostream& o, size_t indent, size_t lineLen, bool verbose=false) const;    //!< Print children DAG nodes
        void                                                        printParents(std::ostream& o, size_t indent, size_t lineLen, bool verbose=false) const;     //!< Print children DAG nodes
        
        // members
        mutable std::set<DagNode*>                                  children;                                                                                   //!< The children in the model graph of this node
        bool                                                        hidden;
        std::set<Monitor*>                                          monitors;
        std::set<Move*>                                             moves;
        std::string                                                 name;
        bool                                                        priorOnly;
        std::set<size_t>                                            touchedElements;
        DagNodeTypes                                                type;

    
    private:
        
        mutable size_t                                              refCount;
    };

}

#endif

