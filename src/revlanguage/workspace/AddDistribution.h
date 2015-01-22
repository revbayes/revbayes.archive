#ifndef AddDistribution_h
#define AddDistribution_h

#include "DistributionFunctionPdf.h"
#include "DistributionFunctionRv.h"
#include "RlTypedDistribution.h"
#include "StringUtilities.h"
#include "Workspace.h"

/**
 * Generic function
 * @param value
 * @return
 */
template <typename T>
void AddDistribution(std::string name, RevLanguage::TypedDistribution<T> *dist)
{

    RevLanguage::Workspace::globalWorkspace().addDistribution("dn"+StringUtilities::firstCharToUpper(name), dist );
    RevLanguage::Workspace::globalWorkspace().addFunction("d"+name, new DistributionFunctionPdf<T>( dist->clone() ) );
    RevLanguage::Workspace::globalWorkspace().addFunction("r"+name, new DistributionFunctionRv<T>( dist->clone() ) );

}


#endif
