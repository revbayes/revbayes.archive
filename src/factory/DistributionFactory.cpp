/*
 * DistributionFactory.cpp
 *
 *  Created on: 4 sep 2009
 *      Author: Sebastian
 */

#include <list>
#include <stdexcept>

#include "DistributionFactory.h"

DistributionFactory::DistributionFactory(void) {
	// TODO Auto-generated constructor stub

}

DistributionFactory::~-DistributionFactory(void) {
    for(DistributionList::reverse_iterator It = m_Distribution.rbegin();
        It != m_Distribution.rend();
        ++It)
      delete *It;
}

DistributionFactory::addDistributions(Distribution* dist){
    m_Distribution.push_back(dist);
}

DistributionFactory::getDistribution(const std::string& name){
    for(DistributionList::iterator It = m_Distribution.begin();
        It != m_Distribution.end();
        ++It)
      if((*It)->getDistributionName().compare(sName) == 0){
      	// found the distribution with the given name
      	// create an instance
      	// TODO need to add the parameters
        return (*It)->createInstance();
        return NULL;
      }

    throw std::runtime_error("Invalid or unsupported distribution type");
}
