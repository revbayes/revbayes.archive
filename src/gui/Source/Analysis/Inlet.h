#import <Cocoa/Cocoa.h>
#import "InOutlet.h"
@class Connection;
@class Outlet;
@class Tool;



@interface Inlet : InOutlet <NSCoding> {

    Connection*    connection;      // the connection object indicating the outlet for this inlet
}

- (void)addConnection:(Connection*)c;
- (BOOL)amInlet;
- (BOOL)amOutlet;
- (Connection*)connection;
- (Connection*)connectionWithIndex:(int)idx;
- (Outlet*)getMyOutlet;
- (BOOL)hasConnection;
- (id)initWithTool:(Tool*)t;
- (int)numberOfConnections;
- (void)pointsForToolWithRect:(NSRect)r atVertex1:(NSPoint*)v1 andVertex2:(NSPoint*)v2 andVertex3:(NSPoint*)v3;
- (void)removeConnection;

@end
