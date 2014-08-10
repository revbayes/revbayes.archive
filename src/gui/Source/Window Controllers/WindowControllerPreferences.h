#import <Cocoa/Cocoa.h>

extern NSString* const RB_AnalysisBgrndColorKey;
extern NSString* const RB_AnalysisGridColorKey;
extern NSString* const RB_ModelBgrndColorKey;
extern NSString* const RB_ModelGridColorKey;
extern NSString* const RB_UserHasRegisteredKey;
extern NSString* const RB_UserFirstNameKey;
extern NSString* const RB_UserLastNameKey;
extern NSString* const RB_UserEmailKey;
extern NSString* const RB_UserCountryKey;
extern NSString* const RB_UserInstitutionKey;
extern NSString* const RB_NotebookFontKey;


@interface WindowControllerPreferences : NSWindowController {

    IBOutlet NSTabView*     preferencesSelectorButton;
	IBOutlet NSButton*      cancelButton;
	IBOutlet NSButton*      okButton;
	IBOutlet NSButton*      restoreButton;
	IBOutlet NSColorWell*   analysisBkgrndColor;
	IBOutlet NSColorWell*   analysisGridColor;
	IBOutlet NSColorWell*   modelBkgrndColor;
	IBOutlet NSColorWell*   modelGridColor;
    IBOutlet NSButton*      selectNotebookFont;
    IBOutlet NSTextField*   defaultFontTextField;
    NSColor*                startingAnalysisBkgndColor;     // the analysis window background on window opening
    NSColor*                startingAnalysisGridColor;      // the analysis window grid color on window opening
    NSColor*                startingModelBkgndColor;        // the model window background on window opening
    NSColor*                startingModelGridColor;         // the model window grid color on window opening
    NSFont*                 notebookFont;
    NSColor*                currentAnalysisBkgndColor;      // the current analysis window background
	NSColor*                currentAnalysisGridColor;       // the current analysis grid color
    NSColor*                currentModelBkgndColor;         // the current model window background
    NSColor*                currentModelGridColor;          // the current model window grid color
    NSFontPanel*            theFontPanel;
}

@property (nonatomic,strong) NSFont* notebookFont;

- (NSColor*)analysisBkgrndColor;
- (NSColor*)analysisGridColor;
- (IBAction)cancelButtonAction:(id)sender;
- (IBAction)changeAnalysisBkgrndColor:(id)sender;
- (IBAction)changeAnalysisGridColor:(id)sender;
- (IBAction)changeModelBkgrndColor:(id)sender;
- (IBAction)changeModelGridColor:(id)sender;
- (IBAction)openFontSelector:(id)sender;
- (NSColor*)modelBkgrndColor;
- (NSColor*)modelGridColor;
- (IBAction)okButtonAction:(id)sender;
- (void)openWindow;
- (IBAction)restoreButtonAction:(id)sender;
- (void)restoreRegistration;

@end
