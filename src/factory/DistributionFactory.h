/*
 * DistributionContainer.h
 *
 *  Created on: 1 sep 2009
 *      Author: Sebastian
 */

#ifndef DISTRIBUTIONCONTAINER_H_
#define DISTRIBUTIONCONTAINER_H_

#include <list>

#include "../main/Plugin.h"
#include "../main/Distribution.h"

/// Manages the distributions
class DistributionContainer {
  public:

    /// Destructor
    ~DistributionContainer() {
      for(DistributionList::reverse_iterator It = m_Distribution.rbegin();
          It != m_Distribution.rend();
          ++It)
        delete *It;
    }

    /// Allows plugins to add new distributions
    void addDistributions(Distribution* dist) {
      m_Distribution.push_back(dist.release());
    }

    /// finds a distribution matching the specified name
    Distribution* getDistribution(const std::string &sName) {
      for(DistributionList::iterator It = m_Distribution.begin();
          It != m_Distribution.end();
          ++It)
        if((*It)->getType()->equals(sName)){
        	// found the distribution with the given name
        	// create an instance
        	// TODO need to add the parameters
          return (*It)->createInstance();
        }

      throw std::runtime_error("Invalid or unsupported distribution type");
    }

  private:
    /// List of distributions
    typedef std::list<Distribution *> DistributionList;

    DistributionList m_Distribution; ///< All available distributions
};

#endif /* DISTRIBUTIONCONTAINER_H_ */
