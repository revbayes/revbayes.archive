/**
 * @file
 * This file contains the declaration of RbSettings, which 
 * contains the settings for many of the variables that are
 * potentially tunable by the user.
 *
 * @brief Declaration of RbSettings
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since version 1.0 2009-09-02
 *
 * $Id$
 */

#ifndef RbSettings_H
#define RbSettings_H

#include <string>

class RbSettings {

    public:
        static RbSettings&          userSettings(void)                                  //!< Get a reference to the singleton RbSettings object
		                               {
                                       static RbSettings settings = RbSettings();
									   return settings;
                                       }
        
        // Access functions
        bool                        getPrintNodeIndex(void) const;                      //!< Retrieve the flag whether we should print node indices 
        double                      getTolerance(void) const;                           //!< Retrieve the tolerance for comparing doubles
        
        // setters
        void                        setPrintNodeIndex(bool tf);                         //!< Set the flag whether we should print node indices
        void                        setTolerance(double t);                             //!< Set the tolerance for comparing double
        void                        initializeUserSettings(void);                       //!< Initialize the user settings to default values

    private:
                                    RbSettings(void);                                   //!< Default constructor
                                    RbSettings(const RbSettings& s) {}                  //!< Prevent copy
                                    RbSettings(std::string& defaultFileName);           //!< Constructor taking a default file name
                                   ~RbSettings(void) {}                                 //!< Delete function table
        RbSettings&                 operator=(const RbSettings& s);                     //! Prevent assignment


		// Variables that have user settings
        double                      tolerance;                                          //!< Tolerance for comparison of doubles
        bool                        printNodeIndex;                                     //!< Should the node index of a tree be printed as a comment?
};

#endif

