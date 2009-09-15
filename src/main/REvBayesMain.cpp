/**
 * @file
 * This file contains the implementation of the REvBayesMain class.
 *
 * @brief Implementation of the class REvBayesMain
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

#include <iostream>

#include "REvBayesMain.h"
#include "../modelLanguage/RbCommand.h"


/**
 * @brief default constructor
 *
 * This is the default constructor with no parameters.
 *
 */
REvBayesMain::REvBayesMain(void){

}

/**
 * @brief run the program
 *
 * This function runs the program, i.e. it is equvialent to the main function of REvBayes.
 *
 */
void REvBayesMain::run(void){
	// initialize the program
	initialize();

	// start the user interaction
	startUserInteraction();

	// finish the execution
	finishProgram();
}

/**
 * @brief perform command
 *
 * This function performs a command of the REvBayes language. It interacts with the parser.
 *
 * @param c          the command to execute
 */
void REvBayesMain::perfomCommand(RbCommand c){
	parser->parseCommand(c);
}

/**
 * @brief initialize the program
 *
 * This function initializes the program. All instances are created here, the default behaivour is loaded, ...
 *
 */
void REvBayesMain::initialize(void){
	// TODO implement this
	// read the default settings

	// create instaces for:
	// the user interface

	// the parser

	// the workspace

	// load functions, i.e. call function to add RbFunctions to the workspace/symbol table
}

/**
 * @brief do user interaction
 *
 * This function calls the user interface and waits for commands
 *
 */
void REvBayesMain::startUserInteraction(void){
	ui->startInteraction();
}

/**
 * @brief finish execution
 *
 * This function finishs the execution of the program and cleans up.
 *
 */
void REvBayesMain::finishProgram(void){
	//TODO implement this
	//might save settings

	//might save current workspace

	//release instances

}


int main(void) {
	// get an instance of REvBayes
	REvBayesMain main;

	// run the program
	main.run();
}
