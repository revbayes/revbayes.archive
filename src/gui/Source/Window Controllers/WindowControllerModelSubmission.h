#import <Cocoa/Cocoa.h>


@interface WindowControllerModelSubmission : NSWindowController {

    IBOutlet NSTextField*    modelNameField;
    IBOutlet NSTextField*    creatorNameField;
    IBOutlet NSTextField*    notesField;
	IBOutlet NSButton*       submitModelButton;
	BOOL                     submitModelToPublic;
	NSString*                modelName;
	NSString*                creatorName;
	NSString*                notes;
}

@property (readwrite) BOOL submitModelToPublic;
@property (readwrite,assign) NSString* modelName;
@property (readwrite,assign) NSString* creatorName;
@property (readwrite,assign) NSString* notes;

- (IBAction)cancelAction:(id)sender;
- (IBAction)changedModelName:(id)sender;
- (void)cleanWindow;
- (BOOL)doesModelExist:(NSString*)mn;
- (IBAction)helpButtonAction:(id)sender;
- (IBAction)okAction:(id)sender;

@end
