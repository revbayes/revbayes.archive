#import <Cocoa/Cocoa.h>
#import "InOutlet.h"
@class Connection;
@class Inlet;
@class Tool;



@interface Outlet : InOutlet <NSCoding> {

    NSMutableArray* connections;    // a mutable array holding the connections to the inlets for this outlet
}

- (void)addConnectionWithInlet:(Inlet*)theInlet;
- (BOOL)amInlet;
- (BOOL)amOutlet;
- (Connection*)connectionWithIndex:(int)idx;
- (id)initWithTool:(Tool*)t;
- (int)numberOfConnections;
- (void)pointsForToolWithRect:(NSRect)r atVertex1:(NSPoint*)v1 andVertex2:(NSPoint*)v2 andVertex3:(NSPoint*)v3;
- (void)removeConnection:(Connection*)c;

@end
