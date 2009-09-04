/*
 * DistributionContainer.h
 *
 *  Created on: 1 sep 2009
 *      Author: Sebastian
 */

#ifndef DistributionFactory_H
#define DistributionFactory_H

#include "../main/Distribution.h"

/// Manages the distributions
class DistributionFactory {
  public:

    /// Destructor
    ~DistributionFactory();

    /// Allows plugins to add new distributions
    void addDistributions(Distribution* dist);

    /// finds a distribution matching the specified name
    Distribution* getDistribution(const std::string &sName);

  private:
    /// List of distributions
    typedef std::list<Distribution *> DistributionList;

    DistributionList m_Distribution; ///< All available distributions
};

#endif /* DistributionFactory_H */
