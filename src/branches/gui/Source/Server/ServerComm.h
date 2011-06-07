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

- (void)checkForNewModels;
- (BOOL)checkForNewVersion;
- (BOOL)connectToServer;
- (void)disconnectFromServer;
- (void)downloadCuratedModels;
- (void)downloadCuratedModelNamed:(NSString*)modelName;
- (void)getCuratedModelNames;
- (void)getListOfClientsModels;
- (void)getListOfModelsToDownload;
- (void)setNickname:(NSString*)s;
- (float)getMostRecentProgramVersionNumber;
- (void)setServerHostname:(NSString*)s;
- (void)submitModelWithInfo:(NSArray*)modelInfo andData:data;
- (void)submitRegistrationInfo:(NSArray*)userInfo;
- (BOOL)subscribe;
- (void)unsubscribe;

@end
