#import "ParmRateMatrix.h"
#import "RevBayes.h"



@implementation ParmRateMatrix

- (void)awakeFromNib {

}

- (void)dealloc {

    NSLog(@"dealloc ParmRateMatrix %@", self);
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
		// initialize the tool image
		[self initializeImage];
        [self setImageWithSize:itemSize];

        // initialize some variables
        parmType         = PARM_QMATRIX;
		}
    return self;
}

- (id)initWithCoder:(NSCoder*)aDecoder {

    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
		// initialize the parameter image
		[self initializeImage];
        [self setImageWithSize:itemSize];

		// initialize some variables
		}
	return self;
}

- (void)initializeImage {

	float s[8] = { 0.25, 0.50, 0.75, 1.0, 1.25, 1.50, 2.0, 4.0 };
	for (int i=0; i<8; i++)
        [itemImage[i] setSize:NSMakeSize(ITEM_IMAGE_SIZE*s[i], ITEM_IMAGE_SIZE*s[i])];
}

@end
