#import "Variable.h"



@implementation Variable

@synthesize name;
@synthesize dimensions;
@synthesize interfaceName;
@synthesize interfaceSymbol;

- (void)drawRect:(NSRect)dirtyRect {

    [super drawRect:dirtyRect];
    
    // drawing code here
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
        name = [aDecoder decodeObjectForKey:@"name"];
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
