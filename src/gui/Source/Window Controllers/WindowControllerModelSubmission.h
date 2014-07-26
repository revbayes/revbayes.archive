#import <Cocoa/Cocoa.h>


@interface WindowControllerModelSubmission : NSWindowController {

    IBOutlet NSTextField*   modelNameField;
    IBOutlet NSTextField*   creatorNameField;
    IBOutlet NSTextField*   notesField;
	IBOutlet NSButton*      submitModelButton;
	BOOL                    submitModelToPublic;
	NSString*               modelName;
	NSString*               creatorName;
	NSString*               notes;
}

@property (nonatomic)        BOOL      submitModelToPublic;
@property (nonatomic,strong) NSString* modelName;
@property (nonatomic,strong) NSString* creatorName;
@property (nonatomic,strong) NSString* notes;

- (IBAction)cancelAction:(id)sender;
- (IBAction)changedModelName:(id)sender;
- (void)cleanWindow;
- (BOOL)doesModelExist:(NSString*)mn;
- (IBAction)helpButtonAction:(id)sender;
- (IBAction)okAction:(id)sender;

@end
