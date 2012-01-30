#import "ToolDistanceMatrix.h"
#import "WindowControllerDistanceMatrix.h"




@implementation WindowControllerDistanceMatrix

@synthesize distanceType;
@synthesize gammaRateVariation;
@synthesize baseFreqTreatment;
@synthesize proportionInvariableSites;
@synthesize gammaShape;

- (void)awakeFromNib {

    [self rateVariationModified:self];
}

- (IBAction)cancelButtonAction:(id)sender {

    [myTool closeControlPanel];
}

- (id)init {

    return [self initWithTool:nil];
}

- (id)initWithTool:(ToolDistanceMatrix*)t {

	if ( (self = [super initWithWindowNibName:@"ControlWindowDistanceMatrix"]) )
        {
        myTool = t;

		distanceType              = [myTool distanceType];
		gammaRateVariation        = [myTool gammaRateVariation];
		baseFreqTreatment         = [myTool baseFreqTreatment];
		proportionInvariableSites = [myTool proportionInvariableSites];
		gammaShape                = [myTool gammaShape];
        }
	return self;
}

- (IBAction)okButtonAction:(id)sender {

    [myTool setDistanceType:distanceType];
    [myTool setGammaRateVariation:gammaRateVariation];
    [myTool setBaseFreqTreatment:baseFreqTreatment];
    [myTool setProportionInvariableSites:proportionInvariableSites];
    [myTool setGammaShape:gammaShape];
    
    [myTool closeControlPanel];
    
    [myTool calculateDistances];
}

- (IBAction)rateVariationModified:(id)sender {

    if ( [rateVariationSelector selectedTag] == GAMMA_RATES )
        {
        NSString* sName = [NSString stringWithFormat:@"%lf", gammaShape];
        [gammaShapeField setTextColor:[NSColor blackColor]];
        [gammaShapeField setStringValue:sName];
        [gammaShapeLabel setEnabled:YES];
        [gammaShapeField setEnabled:YES];
        [gammaShapeField setDrawsBackground:YES];
        [gammaShapeField setEditable:YES];
        }
    else
        {
        unichar inf = 0x221E; // infinity symbol
        NSString* sName = [[NSString alloc] initWithCharacters:&inf length:1];
        [gammaShapeField setTextColor:[NSColor blueColor]];
        [gammaShapeField setStringValue:sName];
        [gammaShapeLabel setEnabled:NO];
        [gammaShapeField setEnabled:NO];
        [gammaShapeField setDrawsBackground:NO];
        [gammaShapeField setEditable:NO];
        }
}

- (void)windowDidLoad {

    [super windowDidLoad];
}

@end
