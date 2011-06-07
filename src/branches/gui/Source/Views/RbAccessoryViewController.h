#import <Cocoa/Cocoa.h>



@interface RbAccessoryViewController : NSViewController <NSPrintPanelAccessorizing> {

	IBOutlet NSButton*        fitHorizontalCheckbox;
	IBOutlet NSButton*        fitVerticalCheckbox;
	IBOutlet NSTextField*     tMarginTextField;
	IBOutlet NSTextField*     bMarginTextField;
	IBOutlet NSTextField*     lMarginTextField;
	IBOutlet NSTextField*     rMarginTextField;
	NSArray*                  localizedSummaryItems;
	float                     rbUnitConversionConstant; // convert points to desired units, currently set at inches
	float                     bMarginConverted;
	float                     tMarginConverted;
	float                     lMarginConverted;
	float                     rMarginConverted;
}

#pragma mark FACTORY METHOD

+ (RbAccessoryViewController*)sharedController;

#pragma mark ACTION METHODS

- (IBAction)changeFitHorizontal:(id)sender;
- (IBAction)changeFitVertical:(id)sender;
- (IBAction)changeTMargin:(id)sender;
- (IBAction)changeBMargin:(id)sender;
- (IBAction)changeLMargin:(id)sender;
- (IBAction)changeRMargin:(id)sender;

#pragma mark ACCESSOR METHODS

- (NSButton*)fitHorizontalCheckbox;
- (NSButton*)fitVerticalCheckbox;
- (NSTextField*)tMarginTextField;
- (NSTextField*)bMarginTextField;
- (NSTextField*)lMarginTextField;
- (NSTextField*)rMarginTextField;


- (void)setFitHorizontal:(int)h;
- (int)fitHorizontal;

- (void)setFitVertical:(int)v;
- (int)fitVertical;

- (void)setTMargin:(float)t;
- (float)tMargin;

- (void)setBMargin:(float)b;
- (float)bMargin;

- (void)setLMargin:(float)l;
- (float)lMargin;

- (void)setRMargin:(float)r;
- (float)rMargin;

- (float)tMarginConverted:(float)tc;
- (float)bMarginConverted:(float)bc;
- (float)lMarginConverted:(float)lc;
- (float)rMarginConverted:(float)rc;

#pragma mark PROTOCOL METHODS

- (NSArray*)localizedSummaryItems;
- (NSSet*)keyPathsForValuesAffectingPreview;

#pragma mark VIEW MANAGEMENT

- (void)updateView;

@end
