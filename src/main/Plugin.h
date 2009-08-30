/*
 * Plugin.h
 *
 *  Created on: 25 aug 2009
 *      Author: Sebastian
 */

#ifndef PLUGIN_H
#define PLUGIN_H

///////////////////////////////////////////////////////////////////////////////
// Plugin architecture example                                               //
//                                                                           //
// This code serves as an example to the plugin architecture discussed in    //
// the article and can be freely used                                        //
///////////////////////////////////////////////////////////////////////////////

#include <string>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

//TODO assuming we defined PLUGIN_EXPORTS
// exporting the dll functions
#ifdef PLUGIN_EXPORTS
#define PLUGIN_API __declspec(dllexport)
#else
#define PLUGIN_API __declspec(dllimport)
#endif

class Kernel;

/// Representation of a plugin
class Plugin {
  public:
    /// Initialize and load plugin
	PLUGIN_API Plugin(const std::string &sFilename);
    /// Copy existing plugin instance
	PLUGIN_API Plugin(const Plugin &Other);
    /// Unload a plugin
	PLUGIN_API ~Plugin();

  //
  // Plugin implementation
  //
  public:
    /// Query the plugin for its expected engine version
	PLUGIN_API int getEngineVersion() const {
      return m_pfnGetEngineVersion();
    }

    /// Register the plugin to a kernel
	PLUGIN_API void registerPlugin(Kernel &K) {
      m_pfnRegisterPlugin(K);
    }

  private:
    /// Too lazy to this now...
    Plugin &operator =(const Plugin &Other);

    /// Signature for the version query function
    typedef int  fnGetEngineVersion();
    /// Signature for the plugin's registration function
    typedef void fnRegisterPlugin(Kernel &);

    HMODULE             m_hDLL;                ///< Win32 DLL handle
    size_t             *m_pDLLRefCount;        ///< Number of references to the DLL
    fnGetEngineVersion *m_pfnGetEngineVersion; ///< Version query function
    fnRegisterPlugin   *m_pfnRegisterPlugin;   ///< Plugin registration function
};

#endif /* PLUGIN_H_ */
