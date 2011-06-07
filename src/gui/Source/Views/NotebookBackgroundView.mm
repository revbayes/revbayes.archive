#import "NotebookBackgroundView.h"



@implementation NotebookBackgroundView

- (id)initWithFrame:(NSRect)frame {

    if ( (self = [super initWithFrame:frame]) ) 
		{
		}
    return self;
}

- (void)drawRect:(NSRect)dirtyRect  {

	// get the size of the view
	NSRect bounds = [self bounds];
	
	// fill in the background color
	[[NSColor whiteColor] set];
	[NSBezierPath fillRect:bounds];
	
	// fill in the grid
	float y = bounds.size.height;
	float gridSize = 20.0;
	while (y > 0.0)
		{
		y -= gridSize;
		NSPoint p1 = NSMakePoint(0.0, y);
		NSPoint p2 = NSMakePoint(bounds.size.width, y);
		NSBezierPath* connection = [NSBezierPath bezierPath];
		[connection moveToPoint:p1];
		[connection lineToPoint:p2];
		[[NSColor blueColor] set];
		[connection setLineWidth:0.2];
		[connection stroke];
		}
	float x = 0.0;
	while (x < bounds.size.width)
		{
		x += gridSize;
		NSPoint p1 = NSMakePoint(x, 0.0);
		NSPoint p2 = NSMakePoint(x, bounds.size.height);
		NSBezierPath* connection = [NSBezierPath bezierPath];
		[connection moveToPoint:p1];
		[connection lineToPoint:p2];
		[[NSColor blueColor] set];
		[connection setLineWidth:0.2];
		[connection stroke];
		}
}

@end
