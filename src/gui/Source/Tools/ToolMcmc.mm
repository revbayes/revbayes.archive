#import "InOutlet.h"
#import "RevBayes.h"
#import "SettingsMcmc.h"
#import "ToolMcmc.h"
#import "WindowControllerMcmc.h"



@implementation ToolMcmc

- (void)awakeFromNib {

}

- (BOOL)checkForExecute:(NSMutableDictionary*)errors {

    // TO DO
    return YES;
}

- (BOOL)checkForWarning:(NSMutableDictionary*)warnings {

    return YES;
}

- (void)closeControlPanel {

    [NSApp stopModal];
	[controlWindow close];
    [self setInletsAndOutlets];
}

- (void)encodeWithCoder:(NSCoder *)aCoder {

	[super encodeWithCoder:aCoder];
    [aCoder encodeObject:settings forKey:@"settings"];
}

- (BOOL)execute {

    NSLog(@"Executing %@", [self className]);
    usleep(2000000);

    return [super execute];
}

- (id)init {

    self = [self initWithScaleFactor:1.0];
    return self;
}

- (id)initWithScaleFactor:(float)sf {

    if ( (self = [super initWithScaleFactor:sf]) ) 
		{
		// initialize the tool image
		[self initializeImage];
        [self setImageWithSize:itemSize];
        
		// initialize the inlet/outlet information
		[self addInletOfColor:[NSColor blueColor]];
		[self addOutletOfColor:[NSColor redColor]];
		[self addOutletOfColor:[NSColor orangeColor]];
        [self setInletLocations];
        [self setOutletLocations];
        
        // initialize the settings
        settings = [[SettingsMcmc alloc] init];
		
		// initialize the control window
		controlWindow = [[WindowControllerMcmc alloc] initWithTool:self andSettings:settings];
		}
    return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder {

    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
		// initialize the tool image
		[self initializeImage];
        [self setImageWithSize:itemSize];
        
        // initialize the settings
        settings = [aDecoder decodeObjectForKey:@"settings"];
        
		// initialize the control window
		controlWindow = [[WindowControllerMcmc alloc] initWithTool:self andSettings:settings];
		}
	return self;
}

- (void)initializeImage {

    itemImage[0] = [NSImage imageNamed:@"Tool_Mcmc25.icns"];
    itemImage[1] = [NSImage imageNamed:@"Tool_Mcmc50.icns"];
    itemImage[2] = [NSImage imageNamed:@"Tool_Mcmc75.icns"];
    itemImage[3] = [NSImage imageNamed:@"Tool_Mcmc100.icns"];
    itemImage[4] = [NSImage imageNamed:@"Tool_Mcmc125.icns"];
    itemImage[5] = [NSImage imageNamed:@"Tool_Mcmc150.icns"];
    itemImage[6] = [NSImage imageNamed:@"Tool_Mcmc200.icns"];
    itemImage[7] = [NSImage imageNamed:@"Tool_Mcmc400.icns"];

	float s[8] = { 0.25, 0.50, 0.75, 1.0, 1.25, 1.50, 2.0, 4.0 };
	for (int i=0; i<8; i++)
        [itemImage[i] setSize:NSMakeSize(ITEM_IMAGE_SIZE*s[i], ITEM_IMAGE_SIZE*s[i])];
}

- (void)prepareForExecution {

}

- (NSMutableAttributedString*)sendTip {

    NSString* myTip = @" MCMC Tool ";
    if ([self isFullyConnected] == YES)
        myTip = [myTip stringByAppendingString:@"\n Fully Connected "];
    else 
        myTip = [myTip stringByAppendingString:@"\n Missing Connections "];

    NSDictionary *attr = [NSDictionary 
                 dictionaryWithObjects:[NSArray arrayWithObjects:[NSFont fontWithName:@"Lucida Grande Bold" size:14.0], [[NSColor whiteColor] colorWithAlphaComponent:1.0], nil] 
                               forKeys: [NSArray arrayWithObjects:NSFontAttributeName,NSForegroundColorAttributeName, nil]];

    NSMutableAttributedString* attrString = [[NSMutableAttributedString alloc] initWithString:myTip attributes:attr];

    return attrString;
}

- (void)setInletsAndOutlets {

    if ( [controlWindow mcmcType] == 0 )
        {
        if ( [self numInlets] != 1 && [self numOutlets] != 2 )
            {
            [self removeAllInletsAndOutlets];
            [self addInletOfColor:[NSColor blueColor]];
            [self addOutletOfColor:[NSColor redColor]];
            [self addOutletOfColor:[NSColor orangeColor]];
            }
        }
    else
        {
        if ( [self numInlets] != 2 && [self numOutlets] != 1 )
            {
            [self removeAllInletsAndOutlets];
            [self addInletOfColor:[NSColor blueColor]];
            [self addInletOfColor:[NSColor blueColor]];
            [self addOutletOfColor:[NSColor purpleColor]];
            }
        }
}

- (void)showControlPanel {

    NSPoint p = [self originForControlWindow:[controlWindow window]];
    [[controlWindow window] setFrameOrigin:p];
	[controlWindow showWindow:self];    
	[[controlWindow window] makeKeyAndOrderFront:nil];
    [NSApp runModalForWindow:[controlWindow window]];
}

- (NSString*)toolName {

    return @"MCMC";
}

- (void)updateForChangeInParent {

}

@end
