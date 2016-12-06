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
   
        void                        initializeUserSettings(void);                       //!< Initialize the user settings to default values
    
    
        // Access functions
        const std::string&          getModuleDir(void) const;                           //!< Retrieve the module directory name
        size_t                      getLineWidth(void) const;                           //!< Retrieve the line width that will be used for the screen width when printing
        bool                        getUseScaling(void) const;                          //!< Retrieve the flag whether we should scale the likelihood in CTMC models
        size_t                      getScalingDensity(void) const;                      //!< Retrieve the scaling density that determines how often to scale the likelihood in CTMC models
        std::string                 getOption(const std::string &k) const;              //!< Retrieve a user option
        bool                        getPrintNodeIndex(void) const;                      //!< Retrieve the flag whether we should print node indices
        double                      getTolerance(void) const;                           //!< Retrieve the tolerance for comparing doubles
        const std::string&          getWorkingDirectory(void) const;                    //!< Retrieve the current working directory
    
        // setters
        void                        setModuleDir(const std::string &md);                //!< Set the module directory name
        void                        setUseScaling(bool s);                              //!< Set the flag whether we should scale the likelihood in CTMC models
        void                        setScalingDensity(size_t w);                        //!< Set the scaling density n, where CTMC likelihoods are scaled every n-th node (min 1)
        void                        setLineWidth(size_t w);                             //!< Set the line width that will be used for the screen width when printing
        void                        setOption(const std::string &k, const std::string &v, bool write);  //!< Set the key value pair.
        void                        setPrintNodeIndex(bool tf);                         //!< Set the flag whether we should print node indices
        void                        setTolerance(double t);                             //!< Set the tolerance for comparing double
        void                        setWorkingDirectory(const std::string &wd);         //!< Set the current working directory
    
    private:
                                    RbSettings(void);                                   //!< Default constructor
                                    RbSettings(const RbSettings& s) {}                  //!< Prevent copy
                                   ~RbSettings(void) {}                                 //!< Delete function table
        RbSettings&                 operator=(const RbSettings& s);                     //!< Prevent assignment


        void                        writeUserSettings(void);                            //!< Write the current settings into a file.
    
		// Variables that have user settings
        size_t                      lineWidth;
        bool                        useScaling;
        size_t                      scalingDensity;
        std::string                 moduleDir;
        double                      tolerance;                                          //!< Tolerance for comparison of doubles
        bool                        printNodeIndex;                                     //!< Should the node index of a tree be printed as a comment?
        std::string                 workingDirectory;
    
};

#endif

