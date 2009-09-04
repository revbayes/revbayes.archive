/*
 * Factory.h
 *
 *  Created on: 1 sep 2009
 *      Author: Sebastian
 */

#ifndef FACTORY_H_
#define FACTORY_H_

#include <string>
#include <map>

#include "../main/Plugin.h"
#include "../main/RbObject.h"
#include "DistributionFactory.h"

/// The engine's system core
class Factory {
  public:
    /// Access the distribution factory
	DistributionFactory& getDistributionFactory(void) { return m_DistributionFactory; }

    /// Loads a plugin
	//void loadPlugin(const std::string &sFilename) {
    //  if(m_LoadedPlugins.find(sFilename) == m_LoadedPlugins.end())
    //    m_LoadedPlugins.insert(PluginMap::value_type(
    //      sFilename,
    //      Plugin(sFilename)
    //    )).first->second.registerPlugin(*this);
    //}

  private:
    /// Map of plugins by their associated file names
    //typedef std::map<std::string, Plugin> PluginMap;
    typedef std::map<std::string, RbObject> PluginMap;

    //PluginMap              m_LoadedPlugins;          ///< All plugins currently loaded
    DistributionFactory    m_DistributionFactory;    ///< The distribution factory
};


#endif /* FACTORY_H_ */
