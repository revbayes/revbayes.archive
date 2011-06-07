#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>
#import "RbView.h"
@class InOutlet;



@interface AnalysisView : RbView {

}

- (void)mouseDown:(NSEvent*)event;
- (BOOL)readFromPasteboard:(NSPasteboard*)pb;
- (void)removeAllFocusRings;
- (void)removeConnectionWithOutlet:(InOutlet*)ol;
- (void)removeSelectedConnections;
- (void)scaleFactorChanged:(NSNotification*)notification;
- (IBAction)selectAll:(id)sender;
- (void)selectAllItems;
- (ItemSelector)selectItem:(NSPoint)forPoint;
- (void)selectItemsInSweepArea;
- (void)updateBackgroundColor:(NSNotification*)notification;
- (void)writeToPasteboard:(NSPasteboard*)pb;

@end
