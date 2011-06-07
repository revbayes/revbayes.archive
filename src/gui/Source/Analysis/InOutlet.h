#import <Cocoa/Cocoa.h>
@class Tool;



@interface InOutlet : NSObject <NSCoding> {

    BOOL           amInlet;         // YES: inlet, NO: outlet
	BOOL           isSelected;      // has this connection been selected
    NSColor*       toolColor;       // the color for this inlet/outlet
	Tool*          toolOwner;       // the tool that "owns" this inlet/outlet
	NSRect         inOutletRect;    // position of inlet/outlet with respect to the tool owner
	NSPoint        inOutletPoint;   // position of the middle of the inlet/outlet with respect to the tool owner
	InOutlet*      partner;         // the other inlet/outlet to which this one is connected
}

@property (readwrite) BOOL amInlet;
@property (readwrite) BOOL isSelected;
@property (readwrite) NSRect inOutletRect;
@property (assign) InOutlet* partner;
@property (assign) NSColor* toolColor;
@property (assign) Tool* toolOwner;

- (void)flipSelected;
- (NSPoint)getInOutletPointFromPoint:(NSPoint)p;
- (NSRect)getInOutletRectFromPoint:(NSPoint)p;
- (id)initWithTool:(Tool*)t;

@end
