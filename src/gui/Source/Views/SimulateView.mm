#import "SimulateView.h"
#import "WindowControllerSimulate.h"




@implementation SimulateView

- (id)initWithFrame:(NSRect)frame {

    if ( (self = [super initWithFrame:frame]) ) 
        {
        }
    return self;
}

- (void)drawRect:(NSRect)dirtyRect {

    // draw the background
	NSRect bounds = [self bounds];
    [[NSColor whiteColor] set];
	[NSBezierPath fillRect:bounds];
}

@end
