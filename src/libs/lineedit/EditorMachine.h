#ifndef EDITORMACHINE_H
#define	EDITORMACHINE_H

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

#include "EditorState.h"
#include "lineeditUtils.h"
#include "EditorMachineObserver.h"



typedef std::vector<EditorState*> StatePointer;
typedef std::vector<std::string> StringVector;

class EditorMachine {
public:

    EditorMachine() {
        defaultState = new StateIdle();
        queuedStates = new StatePointer();
        nl = "\n\r";
        this->reset();
    }

    /**
     * Run this method for each new context
     */
    void reset(void) { // resets to default state (new line)
        linePos.clear();
        linePos.push_back(0);
        cmd = "";
        queuedStates->clear();
        queuedStates->push_back(defaultState);
        message = "";
        writeMessage("Reset", defaultState);
    }

    void deleteChar() {
        if (cmd.size() <= 0) {
            reset();
            return;
        }
        const char c = cmd.at(cmd.size() - 1);
        StateType type = queuedStates->back()->getType();

        // cancel state
        if (queuedStates->back()->tryCancel(c, type)) {
            cancelState(queuedStates->back());
            linePos.pop_back();
        }

        cmd = cmd.substr(0, cmd.size() - 1);

    }

    /**
     * Processes the whole command buffer and changes the state of machine accordingly
     * @param buf user input, must be null terminated
     * @return boolean weather the input has added a new state
     */
    bool processInput(std::string buf) {

        if (buf.size() <= 0) {
            return false;
        }

        // extract the unprocessed string
        cmd = buf.substr(linePos.back(), buf.size());
        std::string subject = LineEditUtils().extractSubject(cmd);


        StateType type = (queuedStates->back())->getType();
        bool stateReleased = false;

        // release state
        if (queuedStates->back()->tryRelease(cmd, type)) {
            releaseState(queuedStates->back());
            stateReleased = true;
        }

        bool stateTriggered = true;
        // set new state
        if (stateInAssignment.tryHook(cmd, type)) {
            addState(new StateInAssignment(), subject);

        } else if (stateInBrackets.tryHook(cmd, type)) {
            addState(new StateInBrackets(), subject);

        } else if (stateInString.tryHook(cmd, type)) {
            addState(new StateInString(), subject);

        } else if (stateListingMembers.tryHook(cmd, type)) {
            addState(new StateListingMembers(), subject);

        } else {
            stateTriggered = false;
        }

        // update linePos if any change in state happened
        if (stateReleased || stateTriggered) {
            linePos.push_back(buf.size());
        }

        // return true only if new state was added
        return stateTriggered;
    }

    std::string getCmd() const {
        return cmd;
    }

    unsigned int getLinePos() const {
        return linePos.back();
    }

    StatePointer* getStateQueue() const {
        return queuedStates;
    }

    EditorState* getCurrentState() {
        return queuedStates->back();
    }

    std::string getMessage() {
        return message;
    }
    
    void setObserver(EditorMachineObserver *observer){
        this->observer = observer;
    }

private:
    EditorMachineObserver *observer;
    std::string nl;
    std::string message;
    std::vector<int> linePos; // position on command line that was last processed
    std::string cmd; // the part of the command line that are currently being processed

    StatePointer *queuedStates; // states can overlap, and are stored in this queue

    EditorState* defaultState;

    // the states this machine can be in
    StateIdle stateIdle;
    StateInAssignment stateInAssignment;
    StateInBrackets stateInBrackets;
    StateInString stateInString;
    StateListingMembers stateListingMembers;

    void writeMessage(std::string m, EditorState *state) {
        std::string tab = "";
        for (int i = 0; i < queuedStates->size(); i++) {
            tab.append("..");
        }
        //message.append(tab).append(state->getMessage()).append(nl);
        message.append(tab).append(m).append("-").append(state->getDescription()).append(nl);
        message.append("Current state:").append(queuedStates->back()->getDescription()).append(nl);
    }

    void addState(EditorState* e, std::string subject) {
        e->setSubject(subject);
        queuedStates->push_back(e);
        writeMessage("add", e);
        observer->eventStateChanged();
    }

    void releaseState(EditorState* e) {        
        queuedStates->pop_back();
        writeMessage("release", e);
        observer->eventStateChanged();
    }
    
    void cancelState(EditorState* e) {        
        queuedStates->pop_back();
        writeMessage("cancel", e);
        observer->eventStateChanged();
    }
};

#endif	/* EDITORMACHINE_H */

