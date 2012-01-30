#import <Cocoa/Cocoa.h>
@class ToolDistanceMatrix;



@interface WindowControllerDistanceMatrix : NSWindowController {

    IBOutlet NSPopUpButton*       distanceTypeSelector;
    IBOutlet NSPopUpButton*       rateVariationSelector;
    IBOutlet NSPopUpButton*       baseFreqTreatmentSelector;
    IBOutlet NSTextField*         proportionInvariableSitesField;
    IBOutlet NSTextField*         gammaShapeLabel;
    IBOutlet NSTextField*         gammaShapeField;
	IBOutlet NSButton*            okButton;
	IBOutlet NSButton*            cancelButton;
    IBOutlet NSNumberFormatter*   gammaShapeFormatter;
    
    int                           distanceType;
    BOOL                          gammaRateVariation;
    int                           baseFreqTreatment;
    double                        proportionInvariableSites;
    double                        gammaShape;

    ToolDistanceMatrix*           myTool;
}

@property (readwrite) int    distanceType;
@property (readwrite) BOOL   gammaRateVariation;
@property (readwrite) int    baseFreqTreatment;
@property (readwrite) double proportionInvariableSites;
@property (readwrite) double gammaShape;

- (IBAction)cancelButtonAction:(id)sender;
- (id)initWithTool:(ToolDistanceMatrix*)t;
- (IBAction)okButtonAction:(id)sender;
- (IBAction)rateVariationModified:(id)sender;

@end
