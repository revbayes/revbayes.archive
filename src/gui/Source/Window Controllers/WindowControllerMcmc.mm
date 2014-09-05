#import "SettingsMcmc.h"
#import "ToolMcmc.h"
#import "WindowControllerMcmc.h"



@implementation WindowControllerMcmc

@synthesize mcmcType;
@synthesize numRuns;
@synthesize numChains;
@synthesize numCycles;
@synthesize statusFreq;
@synthesize sampleFreq;
@synthesize temperature;
@synthesize initSeed;

- (IBAction)helpButtonAction:(id)sender {

}

- (id)init {

    return [self initWithTool:nil andSettings:nil];
}

- (id)initWithTool:(ToolMcmc*)t andSettings:(SettingsMcmc*)s {

	if ( (self = [super initWithWindowNibName:@"ControlWindowMcmc"]) )
        {
        myTool        = t;
        settings      = s;
        mcmcType      = [settings mcmcType];
        numRuns       = [settings numRuns];
        numChains     = [settings numChains];
        numCycles     = [settings numCycles];
        statusFreq    = [settings statusFreq];
        sampleFreq    = [settings sampleFreq];
        temperature   = [settings temperature];
        initSeed      = [settings initSeed];
        }
	return self;
}

- (IBAction)okButtonAction:(id)sender {

    [settings setMcmcType:mcmcType];
    [settings setNumRuns:numRuns];
    [settings setNumChains:numChains];
    [settings setNumCycles:numCycles];
    [settings setStatusFreq:statusFreq];
    [settings setSampleFreq:sampleFreq];
    [settings setTemperature:temperature];
    [settings setInitSeed:initSeed];
    [myTool closeControlPanel];
}

- (void)windowDidLoad {

}

@end
