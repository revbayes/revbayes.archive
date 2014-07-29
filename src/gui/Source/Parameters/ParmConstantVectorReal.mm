#import "ParmConstantVectorReal.h"
#import "ParmTree.h"
#import "RevBayes.h"
#import "ValueReal.h"
#import "ValueVectorReal.h"



@implementation ParmConstantVectorReal

- (Value*)allocateValue {
    
    return ([[ValueVectorReal alloc] initWithDimensions:dimensions]);
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
        parmType   = PARM_CONSTANT_VECTOR_REAL;
        domain     = VECTOR_REAL_NUMBER + REAL_NUMBER;
        isVector   = YES;
        dimensions = 2;
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
        float x = [str floatValue];
        [(ValueReal*)v setValue:x];
        }
}

- (NSString*)xibName {
    
	NSString* xn = @"ControlWindowConstant";
	return xn;
}

@end
