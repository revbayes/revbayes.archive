#import "ParmConstantRealPos.h"
#import "ParmTree.h"
#import "RevBayes.h"
#import "ValueRealPos.h"



@implementation ParmConstantRealPos

- (Value*)allocateValue {
    
    return ([[ValueRealPos alloc] initWithNumber:0.0]);
}

- (void)dealloc {
    
	[super dealloc];
}

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
        parmType = PARM_CONSTANT_REALPOS;
        domain   = POSITIVE_REAL_NUMBER;
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
        [(ValueRealPos*)v setValue:x];
        }
}

- (NSString*)xibName {
    
	NSString* xn = @"ControlWindowConstant";
	return xn;
}

@end
