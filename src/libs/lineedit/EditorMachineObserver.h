/* 
 * File:   EditorMachineObserver.h
 * Author: johadunf
 *
 * Created on May 19, 2014, 8:27 AM
 */

#ifndef EDITORMACHINEOBSERVER_H
#define	EDITORMACHINEOBSERVER_H

class EditorMachineObserver{
public:
    virtual void eventStateChanged(void) = 0;
};

#endif	/* EDITORMACHINEOBSERVER_H */

