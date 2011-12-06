#import <Cocoa/Cocoa.h>
@class Connection;
@class Tool;



@interface InOutlet : NSObject <NSCoding> {

    NSColor*       toolColor;       // the color for this inlet/outlet
	Tool*          toolOwner;       // the tool that "owns" this inlet/outlet
    NSPoint        position;        // x,y position of the middle relative to bottom-left corner of tool, in a (0,1) coordinate system
}

@property (assign) NSColor* toolColor;
@property (assign) Tool* toolOwner;
@property (readwrite) NSPoint position;

- (BOOL)amInlet;
- (BOOL)amOutlet;
- (id)initWithTool:(Tool*)t;
- (int)numberOfConnections;
- (void)pointsForToolWithRect:(NSRect)r atVertex1:(NSPoint*)v1 andVertex2:(NSPoint*)v2 andVertex3:(NSPoint*)v3;
- (NSRect)rectForToolWithRect:(NSRect)r;

@end
