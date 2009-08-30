/*
 * Plugin.cpp
 *
 *  Created on: 25 aug 2009
 *      Author: Sebastian
 */

#include "Plugin.h"

///////////////////////////////////////////////////////////////////////////////
// Plugin architecture example                                               //
//                                                                           //
// This code serves as an example to the plugin architecture discussed in    //
// the article and can be freely used.                                       //
///////////////////////////////////////////////////////////////////////////////
#include <stdexcept>

/** Loads the specified plugin as a dynamic library and locates
    the plugin's exported functions

    @param  sFilename  Filename of the plugin to load
*/
Plugin::Plugin(const std::string &sFilename) :
  m_hDLL(0),
  m_pfnGetEngineVersion(0),
  m_pfnRegisterPlugin(0),
  m_pDLLRefCount(0) {

  // Try to load the plugin as a dynamic library
  m_hDLL = ::LoadLibrary(sFilename.c_str());
  if(!m_hDLL)
    throw runtime_error(string("Could not load '") + sFilename + "'");

  // Locate the plugin's exported functions
  try {
    m_pfnGetEngineVersion = reinterpret_cast<fnGetEngineVersion *>(
      ::GetProcAddress(m_hDLL, "getEngineVersion")
    );
    m_pfnRegisterPlugin = reinterpret_cast<fnRegisterPlugin *>(
      ::GetProcAddress(m_hDLL, "registerPlugin")
    );

    // If the functions aren't found, we're going to assume this is
    // a plain simple DLL and not one of our plugins
    if(!m_pfnGetEngineVersion || ! m_pfnRegisterPlugin)
      throw runtime_error(string("'") + sFilename + "' is not a valid plugin");

    // Initialize a new DLL reference counter
    m_pDLLRefCount = new size_t(1);
  }
  catch(...) {
    ::FreeLibrary(m_hDLL);
    throw;
  }
}

/** Creates a copy of a plugin that has already been loaded.
    Required to provide correct semantics for storing plugins in
    an STL map container.

    @param  Other  Other plugin instance to copy
*/
Plugin::Plugin(const Plugin &Other) :
  m_hDLL(Other.m_hDLL),
  m_pfnGetEngineVersion(Other.m_pfnGetEngineVersion),
  m_pfnRegisterPlugin(Other.m_pfnRegisterPlugin),
  m_pDLLRefCount(Other.m_pDLLRefCount) {
  /// Increase DLL reference counter
  ++*m_pDLLRefCount;
}

/** Destroys the plugin, unloading its library when no more references
    to it exist.
*/
Plugin::~Plugin() {
  // Only unload the DLL if there are no more references to it
  if(!--*m_pDLLRefCount) {
    delete m_pDLLRefCount;
    ::FreeLibrary(m_hDLL);
  }
}

