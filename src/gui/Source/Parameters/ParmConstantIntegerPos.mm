#import "ParmConstantIntegerPos.h"
#import "ParmTree.h"
#import "RevBayes.h"
#import "ValueIntegerPos.h"



@implementation ParmConstantIntegerPos

- (Value*)allocateValue {
    
    return ([[ValueIntegerPos alloc] initWithNumber:0]);
}

/* JPHARC - (void)dealloc {
    
	[super dealloc];
} */

- (void)encodeWithCoder:(NSCoder*)aCoder {
    
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
        parmType = PARM_CONSTANT_INTEGERPOS;
        domain   = POSITIVE_INTEGER;
        [self initializeValue];
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

- (void)setValueFromString:(NSString*)str forValue:(Value*)v {
    
    if (v != nil)
        {
        int x = [str intValue];
        [(ValueIntegerPos*)v setValue:x];
        }
}

- (NSString*)xibName {
    
	NSString* xn = @"ControlWindowConstant";
	return xn;
}

@end
