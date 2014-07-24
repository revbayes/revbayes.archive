#import "RevBayes.h"
#import "ServerComm.h"



@implementation ServerComm

@synthesize nickname;
@synthesize serverHostname;

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication*)app {

	if (proxy)
		{
		[proxy unsubscribeClient:self];
		[[proxy connectionForProxy] invalidate];
		}
    return NSTerminateNow;
}

- (void)cleanup {

    if (localModelNames != nil)
        [localModelNames release];
    localModelNames = nil;
    if (curatedModelNames != nil)
        [curatedModelNames release];
    curatedModelNames = nil;
    if (modelsToTransfer != nil)
        [modelsToTransfer release];
    modelsToTransfer = nil;
   
	NSConnection* connection = [proxy connectionForProxy];
	[[NSNotificationCenter defaultCenter] removeObserver:self];
	[connection invalidate];
	[proxy release];
	proxy = nil;
    connectedToServer = NO;
}

- (BOOL)connect {

	BOOL successful;
	
	NSSocketPort* sendPort = [[NSSocketPort alloc] initRemoteWithTCPPort:8081 host:serverHostname];
	//NSSocketPort* sendPort = [[NSSocketPort alloc] initRemoteWithTCPPort:8081 host:nil];
	
	NSConnection* connection = [NSConnection connectionWithReceivePort:nil sendPort:sendPort];
	[connection setRequestTimeout:10.0];
	[connection setReplyTimeout:10.0];
	[sendPort release];
	
	@try 
		{
		proxy = [[connection rootProxy] retain];
		
		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(connectionDown:) name:NSConnectionDidDieNotification object:connection];
		
		[proxy setProtocolForProxy:@protocol(RevBayesServing)];
		
		successful = [proxy subscribeClient:self];
		if (successful)
            {

            }
		else 
			{
            NSLog(@"Nickname not available");
			[self cleanup];
            return NO;
			}

		}
	@catch (NSException* e) 
		{
        NSLog(@"Unable to connect %@", e);
		[self cleanup];
        return NO;
		}
    return YES;
}

- (BOOL)connectToServer {

    if ([self subscribe] == NO)
        {
        NSLog(@" * Unable to establish a connection with %@", serverHostname);
        connectedToServer = NO;
        return NO;
        }
    NSLog(@" * Established connection with %@", serverHostname);
    connectedToServer = YES;
    return YES;
}

- (void)connectionDown:(NSNotification*)note {

	NSLog(@"connection down:");
	[self cleanup];
}

- (void)checkForNewModels {
 
    // check that we are connected with the server
    if (connectedToServer == NO)
        return;
    
    // get the list of models on the client
    [self getListOfClientsModels];
    
    // get the list of curated models on the server
    [self getCuratedModelNames];
    
    // get the list of curated models to download to the client
    [self getListOfModelsToDownload];
    
    // download the curated models to the client
    [self downloadCuratedModels];
}

- (BOOL)checkForNewVersion {

    // check that we are connected with the server
    if (connectedToServer == NO)
        return NO;
        
    // get the current version of this client
    float myVersionNumber = VERSION_NUMBER;
    
    float vn = [self getMostRecentProgramVersionNumber];
    NSLog(@" * Retrieved number for most recent RevBayes version: %1.2f", vn);
    
    if ( (int)(myVersionNumber*1000.0) < (int)(vn*1000.0) )    
        return YES;
    return NO;
}

- (void)curatedModelNamed:(in bycopy NSString*)modelName withData:(in bycopy NSData*)data {

    // write the file to ~/Library/Application Support/RevBayes/Curated Models/<Model Name>
    NSString* localLocation  = @"~/Library/Application Support/RevBayes/Curated Models/";
    localLocation = [localLocation stringByExpandingTildeInPath];
    NSString *localFile = [NSString stringWithString:localLocation];
    localFile = [localFile stringByAppendingPathComponent:modelName];
    NSError* error;
    [data writeToFile:localFile options:NSDataWritingAtomic error:&error];
}

- (void)curatedModelNames:(in bycopy NSMutableArray*)message {

    // get the curated models
    if (curatedModelNames != nil)
        [curatedModelNames release];
    curatedModelNames = [[NSMutableArray alloc] initWithArray:message];
    
    // print the list of curated models
    NSLog(@" * Gathered list of curated models located on the server");
    id element;
    NSEnumerator* modelListEnum = [curatedModelNames objectEnumerator];
    int i = 1;
    while ( (element = [modelListEnum nextObject]) )
        NSLog(@" *    Curated model %d: = \"%@\"", i++, element);
}

- (void)dealloc {

	[super dealloc];
}

- (void)disconnectFromServer {

    if (connectedToServer == YES)
        [self unsubscribe];
    connectedToServer = NO;
}

- (void)downloadCuratedModels {

    id element;
    NSEnumerator* modelListEnum = [modelsToTransfer objectEnumerator];
    while ( (element = [modelListEnum nextObject]) )
        {
        NSLog(@" *    Downloading model: = \"%@\"", element);
        [self downloadCuratedModelNamed:element];
        }

}

- (void)downloadCuratedModelNamed:(NSString*)modelName {

    if (connectedToServer == NO)
        return;

	if (!proxy)
		{
		[self connect];
		if (!proxy)
			return;
		}
	
	@try 
		{
		[proxy getCuratedModelNamed:modelName forClient:self];
		}
	@catch (NSException* e) 
		{
		[self cleanup];
        return;
		}
}

- (void)getCuratedModelNames {

    if (connectedToServer == NO)
        return;
        
	if (!proxy)
		{
		[self connect];
		if (!proxy)
			return;
		}
	
	@try 
		{
		[proxy getCuratedModelNamesForClient:self];
		}
	@catch (NSException* e) 
		{
		[self cleanup];
        return;
		}
}

- (void)getListOfClientsModels {

    // allocate a mutable array that holds the names of the models
    if (localModelNames != nil)
        [localModelNames release];
	localModelNames = [[NSMutableArray alloc] init];

    // read the names of the files in the support folder for RevBayes
    NSString* localLocation  = @"~/Library/Application Support/RevBayes/Curated Models/";
    localLocation = [localLocation stringByExpandingTildeInPath];
    NSError* error = nil;
    NSArray* tempLocalModels = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:localLocation error:&error];
    if (error != nil)
		return;
        
    // initialize the list of local models
	id element;
    NSEnumerator* modelListEnum = [tempLocalModels objectEnumerator];
    while ( (element = [modelListEnum nextObject]) )
		{
		if ( [element characterAtIndex:0] != '.' )
			[localModelNames addObject:element];
		}
    
    // print the list of local models
    NSLog(@" * Gathered list of models on this client");
    modelListEnum = [localModelNames objectEnumerator];
    int i = 1;
    while ( (element = [modelListEnum nextObject]) )
        NSLog(@" *    Client model %d: = \"%@\"", i++, element);
}

- (void)getListOfModelsToDownload {

	// initialize a list of models that need to be transferred from the remote location
    if (modelsToTransfer != nil)
        [modelsToTransfer release];
	modelsToTransfer = [[NSMutableArray alloc] initWithCapacity:0];
    id element;
    NSEnumerator* modelListEnum = [curatedModelNames objectEnumerator];
    while ( (element = [modelListEnum nextObject]) )
		{
		BOOL isModelFoundLocally = NO;
		id element2;
		NSEnumerator* localEnum = [localModelNames objectEnumerator];
		while ( (element2 = [localEnum nextObject]) )
			{
			BOOL res = [element isEqualToString:element2];
			if (res == YES)
				{
				isModelFoundLocally = YES;
				break;
				}
			}
		if (isModelFoundLocally == NO)
			[modelsToTransfer addObject:element];
		}
    NSLog(@" * Determined list of models to be downloaded to the client");
    modelListEnum = [modelsToTransfer objectEnumerator];
    int i = 1;
    while ( (element = [modelListEnum nextObject]) )
        NSLog(@" *    Model to download %d: = \"%@\"", i++, element);
}

- (float)getMostRecentProgramVersionNumber {

    if (connectedToServer == NO)
        return VERSION_NUMBER;

    float vn = VERSION_NUMBER;
	if (!proxy)
		{
		[self connect];
		if (!proxy)
			return VERSION_NUMBER;
		}
	
	@try 
		{
		vn = [proxy getVersionNumberForClient:self];
		}
	@catch (NSException* e) 
		{
        return VERSION_NUMBER;
		}

    return vn;
}

- (id)init {

    if ( (self = [super init]) ) 
		{
		// set the address of the server
        [self setServerHostname:@"wright.berkeley.edu"];
		
		// set the client's name (nickname)
		NSArray* hna = [[NSHost currentHost] names];
		NSString* hns = @"(";
		id element;
		NSEnumerator* myEnumerator = [hna objectEnumerator];
		int i = 0;
		while ( (element = [myEnumerator nextObject]) )
			{
			if (i != 0)
				hns = [hns stringByAppendingString:@", "];
			hns = [hns stringByAppendingFormat:@"%@", element];
			i++;
			}
		hns = [hns stringByAppendingString:@")"];
        NSString* nn = NSFullUserName();
        nn = [nn stringByAppendingFormat:@" : %@", hns];
        [self setNickname:nn];

        connectedToServer = NO;
        curatedModelNames = nil;
        localModelNames   = nil;
        modelsToTransfer  = nil;
		}
	return self;
}

- (void)submitModelWithInfo:(NSArray*)modelInfo andData:data {

    if (connectedToServer == NO)
        return;

    NSLog(@"submit a model with info %@ with data %@", modelInfo, data);
	if (!proxy)
		{
		[self connect];
		if (!proxy)
			return;
		}
	
	@try 
		{
		[proxy submitModelWithInfo:modelInfo andData:data fromClient:self];
		}
	@catch (NSException* e) 
		{
		[self cleanup];
        return;
		}
}

- (void)submitRegistrationInfo:(NSArray*)userInfo {

    if (connectedToServer == NO)
        return;

	if (!proxy)
		{
		[self connect];
		if (!proxy)
			return;
		}
	
	@try 
		{
		[proxy submitRegistrationInfo:userInfo fromClient:self];
		}
	@catch (NSException* e) 
		{
		[self cleanup];
        return;
		}
}

- (BOOL)subscribe {

	if (proxy)
        [self unsubscribe];
    BOOL isSuccessful = [self connect];
    return isSuccessful;
}

- (void)unsubscribe {

	@try 
		{
		[proxy unsubscribeClient:self];
        NSLog(@" * Disconnected from server");
		[self cleanup];
		}
	@catch (NSException * e) 
		{
        NSLog(@"Error unsubscribing");
		}
}

@end
