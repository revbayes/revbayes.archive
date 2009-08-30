#ifndef DistributionFactory_H
#define DistributionFactory_H

template<typename Dist>
class DistributionFactory {

public:

	virtual Dist* createDistribution() = 0;

	/// Loads a plugin
	    MYENGINE_API void loadPlugin(const std::string &sFilename) {
	      if(m_LoadedPlugins.find(sFilename) == m_LoadedPlugins.end())
	        m_LoadedPlugins.insert(PluginMap::value_type(
	          sFilename,
	          Plugin(sFilename)
	        )).first->second.registerPlugin(*this);
	    }

private:

};

#endif
