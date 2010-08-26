/**
 * @file
 * This file contains the declaration of Workspace, which is
 * used to hold the global workspace, the mother of all frames.
 * It is also used for the user workspace, which is the next
 * descendant frame, containing all variables, types and
 * functions defined by the user.
 *
 * @brief Declaration of Workspace
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes development core team
 * @license GPL version 3
 * @extends Frame
 * @package parser
 * @version 1.0
 * @since version 1.0 2009-09-02
 *
 * $Id$
 */

#ifndef RbSettings_H
#define RbSettings_H

#include <string>

/**
 * @brief Workspace
 *
 *
 */
class RbSettings {

    public:

        /** Get global workspace */
        static RbSettings& userSettings() {
                static RbSettings settings = RbSettings();
                return settings;
        }
 
        void                    initializeUserSettings(void);
        
        // Access functions
        double                  getTolerance(void) { return tolerance; }                            //!< Retrieve the tolerance for comparing doubles
        
    private:
                                RbSettings(void);                                                   //!< Default constructor
                                RbSettings(const RbSettings& s) {}                                  //!< Prevent copy
                                RbSettings(std::string& defaultFileName);                           //!< Constructor taking a default file name
                               ~RbSettings(void) {}                                                 //!< Delete function table
        RbSettings&             operator=(const RbSettings& s);                                     //! Prevent assignment

        double                  tolerance;                                                          //!< Tolerance for comparison of doubles
};

#endif

