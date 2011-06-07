#import <Cocoa/Cocoa.h>
@class WindowControllerModel;



@interface WindowControllerModelBrowser : NSWindowController {

    WindowControllerModel*        modelWindowPtr;
    NSMutableArray*               curatedModels;
    NSMutableArray*               userModels;
	IBOutlet NSArrayController*   curatedModelController;
	IBOutlet NSArrayController*   userModelController;
    IBOutlet NSButton*            okButton;
    IBOutlet NSButton*            removeUserModelButton;
    IBOutlet NSTabView*           tabView;
    BOOL                          userSelectedCuratedModel;
    BOOL                          userDidCancel;
}

@property (readwrite,retain) NSMutableArray* curatedModels;
@property (readwrite) BOOL userSelectedCuratedModel;
@property (readwrite) BOOL userDidCancel;
@property (readwrite,retain) NSMutableArray* userModels;

- (IBAction)cancelButtonAction:(id)sender;
- (IBAction)helpButtonAction:(id)sender;
- (id)initWithModelWindow:(WindowControllerModel*)mc;
- (IBAction)okButtonAction:(id)sender;
- (void)readCuratedModels;
- (void)readUserModels;
- (IBAction)removeUserModelAction:(id)sender;
- (NSString*)selectedModel;

@end
