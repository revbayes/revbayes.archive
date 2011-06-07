#import <Cocoa/Cocoa.h>
@class ParmRateMatrix;



@interface WindowControllerRateMatrix : NSWindowController {

	IBOutlet NSButton*       okButton;
    ParmRateMatrix*          myParm;
}

- (id)initWithParm:(ParmRateMatrix*)p;
- (IBAction)okButtonAction:(id)sender;

@end
