#import <Cocoa/Cocoa.h>
#import "ConnectionProtocols.h"



@interface ServerComm : NSObject <RevBayesUsing> {

	NSString*               nickname;
	NSString*               serverHostname;
	id                      proxy;
    NSMutableArray*         localModelNames;
    NSMutableArray*         curatedModelNames;
    NSMutableArray*         modelsToTransfer;
    BOOL                    connectedToServer;
}

@property (nonatomic,strong) NSString* nickname;
@property (nonatomic,strong) NSString* serverHostname;

- (void)checkForNewModels;
- (BOOL)checkForNewVersion;
- (BOOL)connectToServer;
- (void)disconnectFromServer;
- (void)downloadCuratedModels;
- (void)downloadCuratedModelNamed:(NSString*)modelName;
- (void)getCuratedModelNames;
- (void)getListOfClientsModels;
- (void)getListOfModelsToDownload;
- (float)getMostRecentProgramVersionNumber;
- (void)submitModelWithInfo:(NSArray*)modelInfo andData:data;
- (void)submitRegistrationInfo:(NSArray*)userInfo;
- (BOOL)subscribe;
- (void)unsubscribe;

@end
