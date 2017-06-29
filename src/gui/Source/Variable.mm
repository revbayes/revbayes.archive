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
    //[[NSColor blackColor] set];
    //[[NSBezierPath bezierPathWithOvalInRect:bounds] fill];
        
#   if 1
    
    // allocate a shadow object
    NSShadow* shadow = [[NSShadow alloc] init];
    [shadow setShadowOffset:NSMakeSize(3.0, -2.0)];
    [shadow setShadowBlurRadius:10.0];

    float borderWidth = bounds.size.width * 0.01;
    

    // draw a circular parameter
    
    // draw the circle background
    [NSGraphicsContext saveGraphicsState];
    [shadow set];
    [[NSColor lightGrayColor] set];
    [[NSBezierPath bezierPathWithOvalInRect:bounds] fill];
    [NSGraphicsContext restoreGraphicsState];
        
    // draw stripes indicating that the parameter is a constant
    
    
    // draw the black line around the circle
    [[NSColor blackColor] set];
    [NSBezierPath setDefaultLineWidth:borderWidth];
    [[NSBezierPath bezierPathWithOvalInRect:bounds] stroke];
    
    // draw the separator line
    NSPoint lp1 = NSMakePoint(bounds.origin.x + 0.1*bounds.size.width, bounds.origin.y + 0.33*bounds.size.width);
    NSPoint lp2 = NSMakePoint(bounds.origin.x + 0.9*bounds.size.width, bounds.origin.y + 0.33*bounds.size.width);
    NSBezierPath* separatorLine = [NSBezierPath bezierPath];
    [separatorLine setLineCapStyle:NSRoundLineCapStyle];
    [separatorLine moveToPoint:lp1];
    [separatorLine lineToPoint:lp2];
    [separatorLine setLineWidth:borderWidth];
    [[NSColor blackColor] set];
    [separatorLine stroke];
    
    // draw the text on the parameter
    
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
