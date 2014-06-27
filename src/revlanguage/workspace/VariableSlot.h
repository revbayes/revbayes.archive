/**
 * @file
 * This file contains the declaration of VariableSlot, which is
 * used to manage variables in frames and processed argument lists.
 *
 * @brief Declaration of VariableSlot
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2010-09-06, version 1.0
 *
 * $Id$
 */

#ifndef VariableSlot_H 
#define VariableSlot_H

#include "RbPtr.h"
#include "TypeSpec.h"

#include <string>

namespace RevLanguage {

class Variable;


class VariableSlot {
    
public:
    VariableSlot(const std::string &label, const TypeSpec& typeSp, const RbPtr<Variable>& var);                     //!< Constructor of filled slot
    VariableSlot(const std::string &label, const TypeSpec& typeSp);                                                 //!< Constructor of empty slot
    VariableSlot(const std::string &label, const RbPtr<Variable>& var);                                             //!< Constructor with filled slot
    virtual                                ~VariableSlot(void) {}
    
    void                                    createVariable(const std::vector<int> &indices);                        //!< Create a variable at the index (and if necessary resize).
    bool                                    doesVariableExist(const std::vector<int> &indices) const;               //!< Does a variable exist for this index?
    size_t                                  getDim(void) const;                                                     //!< Get the number of dimension
    const std::string&                      getLabel(void) const { return label; }                                  //!< Get the label of the slot
    //    const Variable&                         getVariable(void) const;                                                //!< Get the variable
    //    Variable&                               getVariable(void);                                                      //!< Get the variable (non-const to return non-const variable)
    const RevObject&                 getValue(void) const;                                                   //!< Get the value of the variable
    RevObject&                       getValue(void);                                                         //!< Get the value of the variable (non-const to return non-const value)
    RbPtr<const Variable>                   getVariable(const std::vector<int> &indices) const;                     //!< Get the variable
    RbPtr<Variable>                         getVariable(const std::vector<int> &indices);                           //!< Get the variable (non-const to return non-const variable)
    virtual bool                            isValidVariable(const RevObject& newVariable ) const;            //!< Is newVariable valid for the slot?
    void                                    printValue(std::ostream& o) const;                                      //!< Print value of slot
    void                                    setLabel(const std::string l) { label = l; }
    void                                    setVariable(const RbPtr<Variable>& var);                                //!< Set a slot with a variable
    
private:
    // helper methods
    RbPtr<Variable>                         getVectorizedVariable(const std::vector<int> &indices) const;
    void                                    resetNames(const std::string &l, size_t level, int &index);
    void                                    resetVariables(std::vector<RbPtr<Variable> > &v, std::vector<int> &l, std::vector<int> &i);

    
    // Member variables
    std::string                             label;                                                                  //!< the label for this slot. The label should correspond to the name uner which this slot is stored in variable table. However, the label does not have to correspond to the variable name, e.g. a argument could have the label mean but the name of the variable is mu.
    std::vector<int>                        lengths;
    std::vector<RbPtr<Variable> >           variable;                                                               //!< the argument living in the slot 

};

/* Global functions using the class */
std::ostream&                               operator<<(std::ostream& o, const VariableSlot& x);                     //!< Overloaded output operator

}

#endif
