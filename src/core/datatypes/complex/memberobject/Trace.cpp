//
//  Trace.m
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 3/24/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "Trace.h"

#include "RbUtil.h"
#include "VectorString.h"
#include "XmlDocument.h"
#include "XmlElementReference.h"
#include "XmlElementInstance.h"

#include <sstream>
#include <string>
#include <vector>


// Definition of the static type spec member
const TypeSpec Trace::typeSpec(Trace_name);

Trace::Trace()
{
    invalidate();
}

/**
 * Copy constructor
 *
 */
Trace::Trace(const Trace& t) {
    burnin                          = t.burnin;
    ess                             = t.ess;
    mean                            = t.mean;
    median                          = t.median;
    sem                             = t.sem;
    stepSize                        = t.stepSize;
    
    converged                       = t.converged;
    passedStationarityTest          = t.passedStationarityTest;
    passedGewekeTest                = t.passedGewekeTest;
    //    passedHeidelbergerWelchStatistic = NOT_CHECKED;
    //    passedRafteryLewisStatistic = NOT_CHECKED;
    passedEssThreshold              = t.passedEssThreshold;
    passedSemThreshold              = t.passedSemThreshold;
    passedIidBetweenChainsStatistic = t.passedIidBetweenChainsStatistic;
    passedGelmanRubinTest           = t.passedGelmanRubinTest;
    
    values                          = t.values;
}

Trace::Trace(const XmlDocument* doc, const XmlElementAttributed* element) {
    burnin                              = element->getAttributeInt("burnin");
    ess                                 = element->getAttributeInt("ess");
    mean                                = element->getAttributeDouble("mean");
    median                              = element->getAttributeDouble("median");
    sem                                 = element->getAttributeDouble("sem");
    stepSize                            = element->getAttributeInt("stepSize");
    converged                           = element->getAttributeInt("converged");
    passedStationarityTest              = element->getAttributeInt("passedStationarityTest");
    passedGewekeTest                    = element->getAttributeInt("passedGewekeTest");
    passedEssThreshold                  = element->getAttributeInt("passedEssThreshold");
    passedSemThreshold                  = element->getAttributeInt("passedSemThreshold");
    passedIidBetweenChainsStatistic     = element->getAttributeInt("passedIidBetweenChainsStatistic");
    passedGelmanRubinTest               = element->getAttributeInt("passedGelmanRubinTest");
    parmName                            = element->getAttribute("parmName");
    fileName                            = element->getAttribute("fileName");
    values                              = element->getAttributeVectorDouble("values");
    
}

Trace::~Trace() {
    values.clear();
}


void Trace::addObject(double d)
{
    values.push_back(d);
    
    // invalidate for recalculation of meta data
    invalidate();
}

/** Clone function */
MemberObject* Trace::clone() const {
    
    return new Trace(*this);
}


/** Get class vector describing type of object */
const VectorString& Trace::getClass() const {
    
    static VectorString rbClass = VectorString(Trace_name) + MemberObject::getClass();
    return rbClass;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Trace::getTypeSpec(void) const {
    return typeSpec;
}


XmlElement* Trace::encode(XmlDocument* doc, const std::string& name) {    
    // check if the document already contains this trace
    if (doc->hasXmlElementWithId((uintptr_t) this)) {
//        return doc->getXmlElement((uintptr_t)this);
        return new XmlElementReference(name,(uintptr_t)this);
    }
    
    XmlElementInstance *element = new XmlElementInstance(name,(uintptr_t)this);
    element->setAttribute("burnin",burnin);
    element->setAttribute("ess",ess);
    element->setAttribute("mean",mean);
    element->setAttribute("median",median);
    element->setAttribute("sem",sem);
    element->setAttribute("stepSize",stepSize);
    element->setAttribute("converged",converged);
    element->setAttribute("passedStationarityTest",passedStationarityTest);
    element->setAttribute("passedGewekeTest",passedGewekeTest);
    element->setAttribute("passedEssThreshold",passedEssThreshold);
    element->setAttribute("passedSemThreshold",passedSemThreshold);
    element->setAttribute("passedIidBetweenChainsStatistic",passedIidBetweenChainsStatistic);
    element->setAttribute("passedGelmanRubinTest",passedGelmanRubinTest);
    element->setAttribute("parmName",parmName);
    element->setAttribute("fileName",fileName);
    element->setAttribute("values", values);
    
    return element;
}


void Trace::invalidate() {
    // set values to defaults and mark for recalculation
    burnin                          = -1;
    ess                             = -1;
    mean                            = 0.0;
    median                          = 0.0;
    sem                             = -1;
    stepSize                        = 1;
    
    converged                       = NOT_CHECKED;
    passedStationarityTest          = NOT_CHECKED;
    passedGewekeTest                = NOT_CHECKED;
    //    passedHeidelbergerWelchStatistic = NOT_CHECKED;
    //    passedRafteryLewisStatistic = NOT_CHECKED;
    passedEssThreshold              = NOT_CHECKED;
    passedSemThreshold              = NOT_CHECKED;
    passedIidBetweenChainsStatistic = NOT_CHECKED;
    passedGelmanRubinTest           = NOT_CHECKED;
    
    
}

/** Print value for user */
void Trace::printValue(std::ostream &o) const {
    
    o << "Trace values to be printed ...";
}


void Trace::removeObjectAtIndex (int index)
{
    // create a iterator for the vector
    std::vector<double>::iterator it = values.begin();
    
    //jump to the position to remove
    it += index;
    
    // remove the element
    values.erase(it);
    
    // invalidate for recalculation of meta data
    invalidate();
}

void Trace::removeLastObject()
{
    // remove object from list
    values.pop_back();
    
    // invalidate for recalculation of meta data
    invalidate();
}


