#import "AnalysisLogView.h"



@implementation AnalysisLogView

- (id)initWithFrame:(NSRect)frame {

    if ( (self = [super initWithFrame:frame]) ) 
		{
		}
    return self;
}

- (void)drawRect:(NSRect)dirtyRect {

	NSRect bounds = [self bounds];
	[[NSColor lightGrayColor] set];
	[NSBezierPath fillRect:bounds];
	
	NSColorList* ColorListToUse = [NSColorList colorListNamed:@"Crayons"];
	[[ColorListToUse colorWithKey:@"Silver"] set];
	[NSBezierPath fillRect:bounds];
}

@end
