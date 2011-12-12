#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>
#import "RbView.h"
@class InOutlet;
@class Tool;

#define UP      0
#define DOWN    1
#define LEFT    2
#define RIGHT   3



@interface AnalysisView : RbView {

}

- (NSPoint)centerPointBetweenPoint:(NSPoint)p1 andPoint:(NSPoint)p2;
- (NSPoint)centerPointBetweenTool:(Tool*)t1 andTool:(Tool*)t2;
- (NSPoint)centerPointBetweenRect:(NSRect)r1 andRect:(NSRect)r2;
- (float)distanceOfPoint:(NSPoint)a0 fromLineDefinedByPoint:(NSPoint)a1 andPoint:(NSPoint)a2;
- (float)distanceFromPoint:(NSPoint)a toPoint:(NSPoint)b;
- (NSPoint)findClosestPointOnEdgeOfRect:(NSRect)r fromPoint:(NSPoint)p2;
- (void)getBezierPath:(NSBezierPath*)bezy forRect:(NSRect)r startPoint:(NSPoint)sp secondPoint:(NSPoint)p2 andEndPoint:(NSPoint)ep;
- (void)getBoundingRectForToolWithRect:(NSRect*)r1 andRect:(NSRect*)r2 andCenterPoint:(NSPoint*)cp withPosition:(NSPoint)p1 initializingPoint:(NSPoint*)s1 andPosition:(NSPoint)p2 initializingPoint:(NSPoint*)s2;
- (BOOL)isPoint:(NSPoint)p inLineSegmentDefinedByPoint:(NSPoint)a andPoint:(NSPoint)b;
- (void)mouseDown:(NSEvent*)event;
- (NSPoint)newPointDistance:(float)d betweenPoint:(NSPoint)p1 andPoint:(NSPoint)p2;
- (BOOL)readFromPasteboard:(NSPasteboard*)pb;
- (void)removeAllFocusRings;
- (void)removeSelectedConnections;
- (void)scaleFactorChanged:(NSNotification*)notification;
- (IBAction)selectAll:(id)sender;
- (void)selectAllItems;
- (ItemSelector)selectItem:(NSPoint)forPoint;
- (void)selectItemsInSweepArea;
- (void)updateBackgroundColor:(NSNotification*)notification;
- (void)writeToPasteboard:(NSPasteboard*)pb;

@end
