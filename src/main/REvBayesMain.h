/**
 * @file
 * This file contains the declaration of the main class in REvBayes.
 * The main class is responsible to start and execute the program, load and manage the resources, and delegate the tasks to the corresponding instances.
 *
 * @brief Declaration of the abstract class REvBayesMain
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The REvBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-14, version 1.0
 * @extends RbObject
 *
 * $Id$
 */

#ifndef REvBayesMain_H
#define REvBayesMain_H

#include <string>

#include "RbObject.h"
#include "../modelLanguage/RbCommand.h"
#include "../ui/UserInterface.h"

class REvBayesMain: public RbObject {

public:
	REvBayesMain(void);                           //!< default constructor

	void run(void);                               //!< run the program
	void performCommand(RbCommand c);             //!< external interface for the UI to call the parser/main program to perform a command entered by the user

private:
	void initialize(void);                        //!< initializes the instances predefined for the program
	void startUserInteraction(void);              //!< start the UI and wait for commands from user
	void finishProgram(void);                     //!< finish execution and clean up

	std::string settingsFile;                     //!< the filename for the settings of REvBayes
	std::string libraryPath;                      //!< the path name for libraries used in the program, e.g. BEAGLE
	UserInterface* ui;                            //!< the user interface
	EvoMoDeLParser* parser;                       //!< the parser
};

#endif REvBayesMain_H
