#import <Cocoa/Cocoa.h>
@class Connection;
@class Tool;



@interface InOutlet : NSObject <NSCoding> {

    NSColor*                        toolColor;       // the color for this inlet/outlet
	Tool*                           toolOwner;       // the tool that "owns" this inlet/outlet
    NSPoint                         position;        // x,y position of the middle relative to bottom-left corner of tool, in a (0,1) coordinate system
}

@property (nonatomic,weak) NSColor* toolColor;
@property (nonatomic,weak) Tool*    toolOwner;
@property (nonatomic)      NSPoint  position;

- (BOOL)amInlet;
- (BOOL)amOutlet;
- (NSPoint)getDrawingPositionForToolWithRect:(NSRect)tr andBoundedRect:(NSRect)b;
- (NSRect)getTextRectForToolWithRect:(NSRect)r;
- (id)initWithTool:(Tool*)t;
- (BOOL)isOnLeftEdge;
- (BOOL)isOnRightEdge;
- (BOOL)isOnUpperEdge;
- (BOOL)isOnLowerEdge;
- (int)numberOfConnections;
- (NSPoint)offsetPosition;
- (void)pointsForToolWithRect:(NSRect)r atVertex1:(NSPoint*)v1 andVertex2:(NSPoint*)v2 andVertex3:(NSPoint*)v3;
- (NSPoint)pointForToolWithRect:(NSRect)r;
- (NSRect)rectForToolWithRect:(NSRect)r;

@end
