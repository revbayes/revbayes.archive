#ifndef AddContinuousDistribution_h
#define AddContinuousDistribution_h

#include "RlContinuousDistribution.h"
#include "DistributionFunctionCdf.h"
#include "DistributionFunctionPdf.h"
#include "DistributionFunctionQuantileContinuous.h"
#include "DistributionFunctionQuantilePositiveContinuous.h"
#include "DistributionFunctionRv.h"
#include "RlPositiveContinuousDistribution.h"
#include "RlTypedDistribution.h"
#include "StringUtilities.h"
#include "Workspace.h"

/**
 * Generic function
 * @param value
 * @return
 */
template <typename T>
void AddContinuousDistribution(std::string name, RevLanguage::ContinuousDistribution *dist)
{
    
    RevLanguage::Workspace::globalWorkspace().addDistribution("dn"+StringUtilities::firstCharToUpper(name), dist );
    RevLanguage::Workspace::globalWorkspace().addFunction("d"+name, new DistributionFunctionPdf<T>( dist->clone() ) );
    RevLanguage::Workspace::globalWorkspace().addFunction("p"+name, new DistributionFunctionCdf( dist->clone() ) );
    RevLanguage::Workspace::globalWorkspace().addFunction("q"+name, new DistributionFunctionQuantileContinuous( dist->clone() ) );
    RevLanguage::Workspace::globalWorkspace().addFunction("r"+name, new DistributionFunctionRv<T>( dist->clone() ) );
    
}


/**
 * Generic function
 * @param value
 * @return
 */
template <typename T>
void AddContinuousDistribution(std::string name, RevLanguage::PositiveContinuousDistribution *dist)
{
    
    RevLanguage::Workspace::globalWorkspace().addDistribution("dn"+StringUtilities::firstCharToUpper(name), dist );
    RevLanguage::Workspace::globalWorkspace().addFunction("d"+name, new DistributionFunctionPdf<T>( dist->clone() ) );
    RevLanguage::Workspace::globalWorkspace().addFunction("p"+name, new DistributionFunctionCdf( dist->clone() ) );
    RevLanguage::Workspace::globalWorkspace().addFunction("q"+name, new DistributionFunctionQuantilePositiveContinuous( dist->clone() ) );
    RevLanguage::Workspace::globalWorkspace().addFunction("r"+name, new DistributionFunctionRv<T>( dist->clone() ) );
    
}


#endif
