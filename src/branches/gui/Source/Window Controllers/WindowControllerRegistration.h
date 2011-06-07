#import <Cocoa/Cocoa.h>


@interface WindowControllerRegistration : NSWindowController {

    IBOutlet NSPopUpButton*  countrySelector;
	IBOutlet NSButton*       registerNowButton;
	IBOutlet NSButton*       registerLaterButton;
	IBOutlet NSTextField*    firstNameField;
	IBOutlet NSTextField*    lastNameField;
	IBOutlet NSTextField*    emailAddressField;
	IBOutlet NSTextField*    institutionField;
	NSArray*                 countryList;
    BOOL                     hasRegistered;
}

- (void)openWindow;
- (IBAction)registerNowAction:(id)sender;
- (IBAction)registerLaterAction:(id)sender;
- (void)registerUserEntriesInDefaults;

@end
