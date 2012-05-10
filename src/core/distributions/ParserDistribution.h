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
#include "MemberFunction.h"
#include "RbValue.h"
#include "Real.h"
#include "RealPos.h"

#include <set>
#include <string>

class RandomNumberGenerator;
class Distribution;


class ParserDistribution : public MemberObject  {
    
public:
    virtual                            ~ParserDistribution(void) {}                                                                       //!< Destructor
    
//    ParserDistribution&                 operator=(const ParserDistribution& p);
    
    // Basic utility functions
    virtual ParserDistribution*         clone(void) const = 0;                                                              //!< Clone object
    static const std::string&           getClassName(void);                                                                 //!< Get class name
    static const TypeSpec&              getClassTypeSpec(void);                                                             //!< Get class type spec
    
    // Member object function you have to override
    virtual const MemberRules&          getMemberRules(void) const = 0;                                                     //!< Get member rules
    
    // Member object functions you may want to override
    virtual void                        clear(void);                                                                        //!< Clear the arguments
    virtual const MethodTable&          getMethods(void) const;                                                             //!< Get member methods
    const std::vector<Argument>&        getParameters() const;
    virtual const TypeSpec&             getVariableType(void) const;                                                        //!< Get random variable type
    virtual const RbLanguageObject&     getTemplateRandomVariable(void) const;                                              //!< Get the template ranom variable
    virtual void                        setMember(const std::string& name, const RbPtr<const Variable> &var);                            //!< Set member variable
    
    // functions you have to override
    virtual Distribution*      getLeanDistribution(void) const = 0;                                                //!< Get the lean distribution
    virtual double                      jointLnPdf( const RbLanguageObject& value) const = 0;                               //!< Ln probability density
    virtual double                      lnPdf( const RbLanguageObject& value) const = 0;                                    //!< Ln probability density
    virtual double                      pdf( const RbLanguageObject& value) const = 0;                                      //!< Probability density function
    virtual void                        rv(void) = 0;                                                                       //!< Generate a random draw
    virtual void                        setParameters(const std::vector<RbValue<void*> > &p) = 0;                           //!< Set the pointers to the variables of the distribution. The last one is always the random value.
    virtual void                        setValue(const RbValue<void*> &v) = 0;                                              //!< Set the pointers to the value of the distribution.
    
protected:
    ParserDistribution( const std::string &n, const MemberRules& memberRules, RbLanguageObject* rv);                        //!< Simple constructor
    ParserDistribution( const ParserDistribution &p);                                                                       //!< Copy constructor
    
    virtual RbPtr<RbLanguageObject>     executeOperationSimple(const std::string& name, const std::vector<Argument>& args); //!< Map member methods to internal functions
    virtual void                        setMemberVariable(const std::string& name, const Variable* var);                    //!< Set member variable
    
    std::string                         name;
    MemberRules                         memberRules;
    std::vector<Argument>               params;
    RbPtr<RbLanguageObject>             randomValue;

};

#endif

