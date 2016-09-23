#import "RevBayes.h"
#import "ToolNumericalMcmcOutput.h"




@implementation ToolNumericalMcmcOutput

- (BOOL)checkForExecute:(NSMutableDictionary*)errors {

    // TO DO
    return YES;
}

- (BOOL)checkForWarning:(NSMutableDictionary*)warnings {

    return YES;
}

- (BOOL)execute {

    NSLog(@"Executing %@", [self className]);
    usleep(2000000);

    return [super execute];
}

- (id)initWithScaleFactor:(float)sf {

    if ( (self = [super initWithScaleFactor:sf]) ) 
		{
		// initialize the tool image
		[self initializeImage];
        [self setImageWithSize:itemSize];
        
		// initialize the inlet/outlet information
		[self addInletOfColor:[NSColor orangeColor]];
		[self addOutletOfColor:[NSColor purpleColor]];
        [self setInletLocations];
        [self setOutletLocations];
        
        // initialize the settings
        //settings = [[SettingsMcmc alloc] init];
		
		// initialize the control window
		//controlWindow = [[WindowControllerMcmc alloc] initWithTool:self andSettings:settings];
		}
    return self;
}

- (id)initWithCoder:(NSCoder*)aDecoder {

    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
		// initialize the tool image
		[self initializeImage];
        [self setImageWithSize:itemSize];
        
        // initialize the settings
        //settings = [aDecoder decodeObjectForKey:@"settings"];
        
		// initialize the control window
		//controlWindow = [[WindowControllerMcmc alloc] initWithTool:self andSettings:settings];
		}
	return self;
}

- (void)initializeImage {

    itemImage[0] = [NSImage imageNamed:@"ToolNumberSet25.icns"];
    itemImage[1] = [NSImage imageNamed:@"ToolNumberSet50.icns"];
    itemImage[2] = [NSImage imageNamed:@"ToolNumberSet75.icns"];
    itemImage[3] = [NSImage imageNamed:@"ToolNumberSet100.icns"];
    itemImage[4] = [NSImage imageNamed:@"ToolNumberSet125.icns"];
    itemImage[5] = [NSImage imageNamed:@"ToolNumberSet150.icns"];
    itemImage[6] = [NSImage imageNamed:@"ToolNumberSet200.icns"];
    itemImage[7] = [NSImage imageNamed:@"ToolNumberSet400.icns"];

	float s[8] = { 0.25, 0.50, 0.75, 1.0, 1.25, 1.50, 2.0, 4.0 };
	for (int i=0; i<8; i++)
        [itemImage[i] setSize:NSMakeSize(ITEM_IMAGE_SIZE*s[i], ITEM_IMAGE_SIZE*s[i])];
}

- (void)prepareForExecution {

}

- (void)updateForChangeInParent {

}

@end
