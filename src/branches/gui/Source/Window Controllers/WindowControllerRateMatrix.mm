#import "ParmRateMatrix.h"
#import "WindowControllerRateMatrix.h"



@implementation WindowControllerRateMatrix

- (id)init {

    return [self initWithParm:nil];
}

- (id)initWithParm:(ParmRateMatrix*)p {

	if ( (self = [super initWithWindowNibName:@"ControlWindowRateMatrix"]) )
        {
        myParm = p;

        }
	return self;
}

- (IBAction)okButtonAction:(id)sender {

    //[self setWindowVariables];
    [myParm closeControlPanel];
}

@end
