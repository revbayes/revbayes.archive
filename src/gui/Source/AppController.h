#import <Cocoa/Cocoa.h>
@class WindowControllerPreferences;
@class WindowControllerRegistration;



@interface AppController : NSObject {

	IBOutlet NSMenuItem*               preferencesMenu;
    IBOutlet NSMenuItem*               newDocumentWindowMenu;
    IBOutlet NSMenuItem*               parameterPaletteMenu;
	WindowControllerPreferences*       preferencesController;
	WindowControllerRegistration*      registrationController;
}

- (void)checkExistenceOfSupportFolder;
+ (void)initialize;
- (IBAction)showPreferencesPanel:(id)sender;
- (IBAction)showRegistrationPanel:(id)sender;
- (BOOL)validateMenuItem:(NSMenuItem*)item;

@end
