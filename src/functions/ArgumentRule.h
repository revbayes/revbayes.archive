/**
 * @file
 * This file contains the declaration of ArgumentRule, which is
 * the base class for objects used to describe rules for
 * arguments passed to functions.
 *
 * @brief Declaration of ArgumentRule
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 *
 * @todo More complex rules are probably better implemented
 *       in argument rule classes derived from this class,
 *       for instance MinMaxRule, OptionRule, etc. -- Fredrik
 *
 * $Id$
 */

#ifndef ArgumentRule_H
#define ArgumentRule_H

#include "RbInternal.h"

#include <string>

class DAGNode;
class RbObject;
class StringVector;

class ArgumentRule : public RbInternal {

    public:
                                    ArgumentRule(const std::string& argName, const std::string& valType, int varDim=0);                 //!< Constructor of rule without default value
                                    ArgumentRule(const std::string& argName, const std::string& valType, int varDim, RbObject* defVal); //!< Constructor of rule with default value
                                    ArgumentRule(const std::string& argName, RbObject* defVal);                                         //!< Constructor of rule with default value and implicit type and dim
                                    ArgumentRule(const std::string& argName, DAGNode* defVar);                                          //!< Constructor of rule with default variable and implicit type and dim
		                            ArgumentRule(const ArgumentRule& a);                                                                //!< Copy constructor
        virtual                    ~ArgumentRule(void);                                                                                 //!< Destructor

        // Assignment operator
        ArgumentRule&               operator=(const ArgumentRule& x);                                                                   //!< Assignment operator

        // Basic utility functions
        virtual ArgumentRule*       clone(void) const { return new ArgumentRule(*this); }                                               //!< Clone object
        virtual const StringVector& getClass(void) const;                                                                               //!< Get class vector
        void                        printValue(std::ostream& o) const;                                                                  //!< Print value for user
        std::string                 toString(void) const;                                                                               //!< General info on object

        // ArgumentRule functions
        RbObject*                   getDefaultValue(void) const;                                                                        //!< Get default val (copy)
        DAGNode*                    getDefaultVariable(void) const;                                                                     //!< Get default node (copy)
        DAGNode*                    getDefaultVariablePtr(void) const;                                                                  //!< Get default node (ptr)
        int                         getDim(void) const { return dim; }                                                                  //!< Get argument dim
        std::string                 getLabel(void) const { return label; }                                                              //!< Get label of argument
        const std::string&          getValueType(void) const { return valueType; }                                                      //!< Get value type
        bool                        hasDefault(void) const;                                                                             //!< Has default?
        virtual bool                isArgValid(DAGNode* var) const;                                                                     //!< Is var valid argument?
        bool                        isWrapperRule(void) const { return wrapperRule; }                                                   //!< Is '&' argument
        void                        setWrapperRule(bool val) { wrapperRule = val; }                                                     //!< Set wrapperRule flag

    protected:
        std::string                 label;                                                                                              //!< Label of argument
        std::string                 valueType;                                                                                          //!< Type of argument value
        int                         dim;                                                                                                //!< Dim of argument
        DAGNode*                    defaultVariable;                                                                                    //!< Default variable
        bool                        wrapperRule;                                                                                        //!< Is wrapper argument?
};

#endif
