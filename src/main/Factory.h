/*
 * Factory.h
 *
 *  Created on: 1 sep 2009
 *      Author: Sebastian
 */

#ifndef FACTORY_H_
#define FACTORY_H_


#include "plugin.h"

/// The engine's system core
class Factory {
  public:
    /// Access the distribution container
	PLUGIN_API DistributionContainer &getDistributionContainer() { return m_DistributionContainer; }

    /// Loads a plugin
	PLUGIN_API void loadPlugin(const std::string &sFilename) {
      if(m_LoadedPlugins.find(sFilename) == m_LoadedPlugins.end())
        m_LoadedPlugins.insert(PluginMap::value_type(
          sFilename,
          Plugin(sFilename)
        )).first->second.registerPlugin(*this);
    }

  private:
    /// Map of plugins by their associated file names
    typedef std::map<std::string, Plugin> PluginMap;

    PluginMap              m_LoadedPlugins;          ///< All plugins currently loaded
    DistributionContainer  m_DistributionContainer;  ///< The distribution container
};


#endif /* FACTORY_H_ */
