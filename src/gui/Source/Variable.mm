#import "Variable.h"



@implementation Variable

@synthesize name;
@synthesize dimensions;
@synthesize interfaceName;
@synthesize interfaceSymbol;

- (void)drawRect:(NSRect)dirtyRect {

    [super drawRect:dirtyRect];
    
	// set the background of the palette window
	NSRect bounds = [self bounds];
	[[NSColor whiteColor] set];
	[NSBezierPath fillRect:bounds];
    
    // draw the parameter
    [[NSColor blackColor] set];
    [[NSBezierPath bezierPathWithOvalInRect:bounds] fill];
    
    return;
    
#   if 0
    
    // find the lower left corner of the parameter and transform it to the view's coordinate system
    //[self transformToBottomLeftCoordinates:&(r.origin)];
    
    // draw the parameter
        // draw the circle background
        [NSGraphicsContext saveGraphicsState];
        if (pPtr != nil)
            [shadow set];
        [[NSColor lightGrayColor] set];
        [[NSBezierPath bezierPathWithOvalInRect:r] fill];
        [NSGraphicsContext restoreGraphicsState];
            
        // draw stripes indicating that the parameter is a constant
        BOOL drawStripes = NO;
        if (pPtr == nil)
            {
            if ([self parmId] == 1)
                drawStripes = YES;
            }
        else
            {
            if ([pPtr isConstant] == YES)
                drawStripes = YES;
            }
        
        // draw the black line around the circle
        [[NSColor blackColor] set];
        [NSBezierPath setDefaultLineWidth:borderWidth];
        [[NSBezierPath bezierPathWithOvalInRect:r] stroke];
        
        // draw the separator line
        NSPoint lp1 = NSMakePoint(r.origin.x + 0.1*r.size.width, r.origin.y + 0.33*r.size.width);
        NSPoint lp2 = NSMakePoint(r.origin.x + 0.9*r.size.width, r.origin.y + 0.33*r.size.width);
        NSBezierPath* separatorLine = [NSBezierPath bezierPath];
        [separatorLine setLineCapStyle:NSRoundLineCapStyle];
        [separatorLine moveToPoint:lp1];
        [separatorLine lineToPoint:lp2];
        [separatorLine setLineWidth:borderWidth];
        [[NSColor blackColor] set];
        [separatorLine stroke];
#   endif
}

- (void)encodeWithCoder:(NSCoder*)aCoder {

	[super encodeWithCoder:aCoder];
    [aCoder encodeInt:(int)dimensions forKey:@"dimensions"];
    [aCoder encodeObject:name         forKey:@"name"];
}

- (id)initWithCoder:(NSCoder*)aDecoder {

    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
        dimensions = (size_t)([aDecoder decodeIntForKey:@"dimensions"]);
        name       = [aDecoder decodeObjectForKey:@"name"];
        parmSize   = 100.0;
		}
	return self;
}

- (id)initWithFrame:(NSRect)frame {

    if ( (self = [super initWithFrame:frame]) ) 
        {
        dimensions = 0;
        name       = @"";
        parmSize   = 100.0;
        }
    return self;
}

- (void)print {

    NSLog(@"%@", name);
    NSLog(@"   dim    = %zu", dimensions);
    NSLog(@"   name   = %@", interfaceName);
    NSLog(@"   symbol = %@", interfaceSymbol);
}

@end
