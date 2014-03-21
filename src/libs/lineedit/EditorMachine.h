#ifndef EDITORMACHINE_H
#define	EDITORMACHINE_H

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

#include "EditorState.h"
#include "lineeditUtils.h"


typedef std::vector<EditorState*> StatePointer;
typedef std::vector<std::string> StringVector;

class EditorMachine {
    unsigned int linePos; // position on command line that was last processed
    const char* cmd; // the part of the command line that are currently being processed

    StatePointer queuedStates; // states can overlap, and are stored in this queue

    EditorState* defaultState;

    // the states this machine can be in
    StateIdle stateIdle;
    StateInAssignment stateInAssignment;
    StateInBrackets stateInBrackets;
    StateInString stateInString;
    StateListingMembers stateListingMembers;

public:

    EditorMachine() {

        defaultState = new StateIdle();

        this->reset();
    }

    /**
     * Run this method for each new context ( new line )
     */
    void reset(void) { // resets to default state (new line)
        linePos = 0;
        cmd = 0;
        queuedStates.clear();
        queuedStates.push_back(defaultState);
    }

    /**
     * Processes the whole command buffer and changes the state of machine accordingly
     * @param buf user input, must be null terminated
     * @return boolean weather the input has added a new state
     */
    bool processInput(const char* buf) {
        // todo: consider that user has deleted all or part of command line
        if (Utils().charLen(buf) <= 0) {
            return false;
        }
        
        std::string _cmd = Utils().getTail(buf, linePos); // previously unprocessed string
        this->cmd = _cmd.c_str();
        std::string subject = Utils().extractSubject(_cmd.c_str()); // might be handy
        linePos = Utils().charLen(buf); // update line position

        StateType type = (queuedStates.back())->getType();

        // release state
        if ((queuedStates.back())->tryRelease(_cmd.c_str(), type)) {
            queuedStates.pop_back();
            //return false;  
        }

        // set new state
        if (stateInAssignment.tryHook(_cmd.c_str(), type)) {
            addState(new StateInAssignment(), subject);

        } else if (stateInBrackets.tryHook(_cmd.c_str(), type)) {
            addState(new StateInBrackets(), subject);

        } else if (stateInString.tryHook(_cmd.c_str(), type)) {
            addState(new StateInString(), subject);

        } else if (stateListingMembers.tryHook(_cmd.c_str(), type)) {
            addState(new StateListingMembers(), subject);

        } else {
            return false;
        }

        return true;
    }

    const char* getCmd() const {
        return cmd;
    }

    unsigned int getLinePos() const {
        return linePos;
    }

    StatePointer getStateQueue() const {
        return queuedStates;
    }
    
    EditorState* getCurrentState(){
        return queuedStates.back();
    }

private:

    void addState(EditorState* e, std::string subject) {
        e->setSubject(subject);
        queuedStates.push_back(e);
    }


};

#endif	/* EDITORMACHINE_H */

