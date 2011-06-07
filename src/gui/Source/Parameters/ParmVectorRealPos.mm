#import "ParmVectorRealPos.h"
#import "RevBayes.h"



@implementation ParmVectorRealPos

- (void)dealloc {
    
	[super dealloc];
}

- (void)encodeWithCoder:(NSCoder *)aCoder {
    
	[super encodeWithCoder:aCoder];
}

- (id)init {
    
    self = [self initWithScaleFactor:1.0 andTool:nil];
    return self;
}

- (id)initWithScaleFactor:(float)sf andTool:(ToolModel*)t {
    
    if ( (self = [super initWithScaleFactor:sf andTool:t]) ) 
		{
		// initialize the parameter image
		[self initializeImage];
        [self setImageWithSize:itemSize];
        
        // initialize some variables
        parmType   = PARM_VECTOR_REALPOS;
        domain     = VECTOR_POSITIVE_REAL_NUMBER + POSITIVE_REAL_NUMBER;
        isVector   = YES;
        dimensions = 2;
        }
    return self;
}

- (id)initWithCoder:(NSCoder*)aDecoder {
    
    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
		// initialize the parameter image
		[self initializeImage];
        [self setImageWithSize:itemSize];
		}
	return self;
}

- (NSString*)xibName {
    
	NSString* xn = [NSString stringWithString:@"ControlWindowRV"];
	return xn;
}


@end
