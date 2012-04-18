/**
 * @file
 * This file contains the declaration of ParserDistribution, which specifies the
 * interface for ParserDistributions in RevBayes. A ParserDistribution is taken in
 * the statistical sense.
 *
 * @brief Declaration of ParserDistribution
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-03-19 09:25:05 +0100 (Mon, 19 Mar 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 * @interface ParserDistribution
 *
 * $Id: ParserDistribution.h 1353 2012-03-19 08:25:05Z hoehna $
 */

#ifndef ParserDistribution_H
#define ParserDistribution_H

#include "Distribution.h"
#include "Real.h"
#include "RealPos.h"

#include <set>
#include <string>

class RandomNumberGenerator;


class ParserDistribution: public Distribution {
    
public:
    virtual                            ~ParserDistribution(void) {}                                                                       //!< Destructor
    
    // Basic utility functions
    virtual ParserDistribution*         clone(void) const = 0;                                                              //!< Clone object
    static const std::string&           getClassName(void);                                                                 //!< Get class name
    static const TypeSpec&              getClassTypeSpec(void);                                                             //!< Get class type spec
    
    // Member object function you have to override
    virtual const MemberRules&          getMemberRules(void) const = 0;                                                     //!< Get member rules
    
    // Member object functions you may want to override
    virtual const MethodTable&          getMethods(void) const;                                                             //!< Get member methods
    
    
    // ParserDistribution functions you have to override
    virtual const TypeSpec&             getVariableType(void) const = 0;                                                    //!< Get random variable type
    virtual double                      lnPdf( const RbLanguageObject& value) const = 0;                                    //!< Ln probability density
    virtual double                      pdf( const RbLanguageObject& value) const = 0;                                      //!< Probability density function
    virtual const RbLanguageObject&     rv(void) = 0;                                                                       //!< Generate a random draw
    
protected:
    ParserDistribution( const MemberRules& memberRules);                                      //!< Simple constructor
    
    virtual const RbLanguageObject&     executeOperationSimple(const std::string& name, const std::vector<Argument>& args); //!< Map member methods to internal functions
    virtual void                        setMemberVariable(const std::string& name, const Variable* var);          //!< Set member variable
    
    std::vector<const Variable*>        parameters;
    
private:
    
    Real                                functionValueLnPdf;
    RealPos                             functionValuePdf;
};

#endif

