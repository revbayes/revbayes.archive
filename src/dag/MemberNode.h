/**
 * @file
 * This file contains the declaration of MemberNode, which is used to
 * hold RbComplex values, i.e., types with member variables.
 *
 * @brief Declaration of MemberNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2010-01-12 21:51:54 +0100 (Tis, 12 Jan 2010) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since 2010-09-28
 *
 * $Id: MemberNode.h 221 2010-01-12 20:51:54Z Hoehna $
 */

#ifndef MemberNode_H
#define MemberNode_H


class MemberNode {

    public:


        virtual                        ~MemberNode(void) {}                                                //!< Destructor

        // Regular functions
        MemberNode*                clone(void) const;                                                      //!< Clone this node
        const VectorString&     getClass(void) const;                                                       //!< Get class vector
        const RbObject*         getValElement(const VectorInteger& index) const = 0;                //!< Get value element
        const RbObject*         getStoredValue(void) = 0;                                           //!< Get stored value
        const RbObject*         getValue(void) = 0;                                                 //!< Get value
        virtual const RbObject*         getValue(void) const = 0;                                           //!< Get value (const)
        virtual void                    printValue(std::ostream& o) const = 0;                              //!< Print value for user
        virtual void                    printStruct(std::ostream& o) const = 0;                             //!< Print struct for user
        virtual void                    setElement(const VectorInteger& index, RbObject*val) = 0;           //!< Set value element
        virtual std::string             toString(void) const = 0;                                           //!< Complete info about object

        // Functions you may want to override
        virtual int                     getDim(void) const { return 0; }                                    //!< Get wrapper dim
        virtual const MemberNode*          getVarElement(const VectorInteger& index) const;                    //!< Get variable element
        virtual void                    setElement(const VectorInteger& index, MemberNode* var);               //!< Set variable element

        // Functions you should not override
		const bool                      getIsDagExposed(void) { return isDagExposed; }                      //!< Get DAG exposure
        const std::string               getName(void) const;                                                //!< Get name from slot and children
        const std::string&              getType(void) const;                                                //!< Get wrapper type
        const std::string&              getValueType(void) const { return valueType; }                      //!< Get value type
        bool                            isType(const std::string& type) const;                              //!< Is wrapper of type?

        // DAG functions you should not override
        void                            addChildNode(VariableNode* c) { children.insert(c); }                   //!< Add child node
        void                            addReferringSlot(VariableSlot* s) { referringSlots.insert(s); }         //!< Add referring slot
        std::set<VariableNode*>&        getChildren(void) { return children; }                                  //!< Return children
		VariableSlot*                   getSlot(void) { return slot; }                                          //!< Return slot managing the variable
		std::set<MemberNode*>&             getParents(void) { return parents; }                                    //!< Return parents
		MemberNode*                        getReference(void) { return this; }                                     //!< Get reference (this node by default)
        std::set<VariableSlot*>&        getReferringSlots(void) { return referringSlots; }                      //!< Return referring slots
        bool                            isParentInDAG(const MemberNode* x, std::list<MemberNode*>& done) const;       //!< Is node x a parent of the caller in the DAG?
        int                             numChildren(void) const { return int(children.size()); }                //!< Number of children
        int                             numParents(void) const { return int(parents.size()); }                  //!< Number of parents
        int                             numReferringSlots(void) const { return int(referringSlots.size()); }    //!< Number of referring slots
        int                             numRefs(void) const;                                                    //!< Number of references
        void                            printChildren(std::ostream& o) const;                                   //!< Print children DAG nodes
        void                            printParents(std::ostream& o) const;                                    //!< Print children DAG nodes
        void                            removeChildNode(VariableNode* c) { children.erase(c); }                 //!< Remove a child node
        void                            removeReferringSlot(VariableSlot* s) { referringSlots.erase(s); }       //!< Remove a referring slot
		void                            setSlot(VariableSlot* s) { slot = s; }                                  //!< Set slot managing the variable
        void                            swapNodeTo(MemberNode* newNode);                                           //!< Swap node in DAG

        // DAG functions you have to override
        virtual MemberNode*                cloneDAG(std::map<MemberNode*, MemberNode*>& newNodes) const = 0;             //!< Clone graph
        virtual bool                    isMutableTo(const MemberNode* newNode) const = 0;                          //!< Is node mutable to newNode?
        virtual bool                    isMutableTo(const VectorInteger& index, const RbObject* newValue) const = 0;    //!< Is node mutable to contain newValue?
        virtual void                    mutateTo(MemberNode* newNode) = 0;                                         //!< Mutate to new node
        virtual MemberNode*                mutateTo(const VectorInteger& index, RbObject* newValue) = 0;           //!< Mutate to contain newValue
        virtual void                    touchAffected(void) = 0;                                                //!< Tell affected nodes value is reset

    protected:
                                        MemberNode(const std::string& valType);                                    //!< Constructor
                                        MemberNode(const MemberNode& x);                                              //!< Copy constructor

        std::set<VariableNode*>         children;                                                               //!< Set of children nodes
        std::set<MemberNode*>              parents;                                                                //!< Set of parent nodes
        VariableSlot*                   slot;                                                                   //!< Slot owning the node
        std::set<VariableSlot*>         referringSlots;                                                         //!< Set of slots referring to the node
        const std::string&              valueType;                                                              //!< Type of value
		bool                            isDagExposed;                                                           //!< Flag indicating whether DAG should be exposed to user
};

#endif
