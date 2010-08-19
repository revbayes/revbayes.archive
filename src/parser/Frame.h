/**
 * @file
 * This file contains the declaration of Frame, which is
 * used to hold information about an evaluation or execution
 * frame. Each frame has a pointer to the enclosing (parent)
 * frame. A frame and its parents constitute an evaluation
 * environment. The base environment is the global workspace.
 * It is a special type of frame, which is described in the
 * class Workspace, derived from Frame.
 *
 * A frame essentially consists of a variable table. The workspace
 * also contains a function table, and a class (type) table.
 *
 * @brief Declaration of Frame
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The REvBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-17, version 1.0
 *
 * $Id$
 */

#ifndef Frame_H 
#define Frame_H

#include <map>
#include <string>

class DAGNode;
class VectorInteger;
class RbObject;

typedef struct {
    std::string type;
    int         dim;
    bool        reference;
    bool        temp;
    DAGNode*    variable;
} ObjectSlot;

typedef std::map<std::string, ObjectSlot> VariableTable;

class Frame {

    public:
                                    Frame(void);                                                                        //!< Constructor of frame with NULL parent
                                    Frame(Frame* parentFr);                                                             //!< Constructor of frame with parent
                                    Frame(const Frame& x);                                                              //!< Copy constructor
        virtual                    ~Frame(void);                                                                        //!< Destroy table of frame, not entire environment

        // Assignment and equals operator
        Frame&                      operator=(const Frame& x);                                                          //!< Assignment
        bool                        operator==(const Frame& x) const;                                                   //!< Equals comparison
        bool                        operator!=(const Frame& x) const;                                                   //!< Not equals comparison

        // Basic utility functions
        virtual Frame*              clone(void) const { return new Frame(*this); }                                      //!< Clone frame
        virtual Frame*              cloneEnvironment(void) const;                                                       //!< Clone environment
        virtual void                printValue(std::ostream& o) const;                                                  //!< Print table for user
        virtual std::string         toString(void) const;                                                               //!< Complete info to string

        // Regular functions
	    void                        addReference(const std::string& name, DAGNode* varPtr);                             //!< Add a variable reference
	    void                        addReference(const std::string& name, const std::string& type, int dim=0);          //!< Add empty reference
	    void                        addVariable(const std::string& name, RbObject* value);                              //!< Add a const variable
	    void                        addVariable(const std::string& name, DAGNode* var);                                 //!< Add a variable
	    void                        addVariable(const std::string& name, const VectorInteger& index, DAGNode* var);         //!< Add container variable
	    void                        addVariable(const std::string& name, const std::string& type, int dim=0);           //!< Add declared but empty slot
        void                        eraseVariable(const std::string& name);                                             //!< Erase a variable
        bool                        existsVariable(const std::string& name) const;                                      //!< Does variable exist?
        int                         getDim(const std::string& name) const;                                              //!< Get dim of variable slot
        const std::string&          getFrameName(void) const { return frameName; }                                      //!< Get base name of frame
        Frame*                      getParentFrame(void) const { return parentFrame; }                                  //!< Get parent frame
        const std::string&          getType(const std::string& name) const;                                             //!< Get type of variable slot
        const RbObject*             getValue(const std::string& name);                                                  //!< Get value
        const DAGNode*              getVariable(const std::string& name) const ;                                        //!< Get variable
        DAGNode*                    getVariable(const std::string& name);                                               //!< Get non-const variable
        const Frame*                getVariableFrame(const std::string& name) const;                                    //!< Get frame of variable
        const VariableTable&        getVariableTable(void) const { return variableTable; }                              //!< Return variable table
        const RbObject*             getValElement(const std::string& name, const VectorInteger& index) const;               //!< Get val elem
        const DAGNode*              getVarElement(const std::string& name, const VectorInteger& index) const;               //!< Get var elem
        DAGNode*                    getVarElement(const std::string& name, const VectorInteger& index);                     //!< Get non-const var elem
        void                        setFrameName(const std::string& name);                                              //!< Set base name of frame
        void                        setValue(const std::string& name, RbObject* val);                                   //!< Set variable value
        void                        setVariable(const std::string& name, DAGNode* var);                                 //!< Set variable
        void                        setValElement(const std::string& name, const VectorInteger& index, RbObject* value);    //!< Set element of an object
        void                        setVarElement(const std::string& name, const VectorInteger& index, DAGNode* variable);  //!< Set element of a DAG node container
        int                         getDim(void) const { return 1; }                                                    //!< Get subscript dimensions
	
    protected:
        Frame*                      parentFrame;                                                                        //!< Pointer to enclosing frame
        VariableTable               variableTable;                                                                      //!< Variable table
        std::string                 frameName;                                                                          //!< Name of frame
};

#endif
