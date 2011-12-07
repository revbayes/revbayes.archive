#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>
#import "RbView.h"
@class InOutlet;
@class Tool;

#define UP      0
#define DOWN    1
#define LEFT    2
#define RIGHT   3
typedef struct {

    NSPoint location;
    int     direction;
} RbAutomaton;



@interface AnalysisView : RbView {

}

- (NSPoint)centerPointBetweenPoint:(NSPoint)p1 andPoint:(NSPoint)p2;
- (NSPoint)centerPointBetweenTool:(Tool*)t1 andTool:(Tool*)t2;
- (float)distanceFromPoint:(NSPoint)a toPoint:(NSPoint)b;
- (NSPoint)findClosestPointOnEdgeOfRect:(NSRect)r fromPoint:(NSPoint)p2;
- (NSRect)getBoundsRectForInOutlet:(InOutlet*)iolet withCenterPoint:(NSPoint)cp whileInitializingAutomaton:(RbAutomaton*)a;
- (NSRect)getBoundingRectForToolWithRect:(NSRect)r connectionOriginationPoint:(NSPoint)op andGoalPoint:(NSPoint)cp;
- (void)mouseDown:(NSEvent*)event;
- (NSPoint)newPointDistance:(float)d betweenPoint:(NSPoint)p1 andPoint:(NSPoint)p2;
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
