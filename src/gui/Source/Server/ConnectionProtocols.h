#import <Foundation/Foundation.h>


@protocol RevBayesUsing

- (bycopy NSString*)nickname;
- (void)curatedModelNamed:(in bycopy NSString*)modelName withData:(in bycopy NSData*)data;
- (void)curatedModelNames:(in bycopy NSMutableArray*)message;

@end


@protocol RevBayesServing

- (void)getCuratedModelNamed:(in bycopy NSString*)modelName forClient:(in byref id <RevBayesUsing>)client;
- (void)getCuratedModelNamesForClient:(in byref id <RevBayesUsing>)client;
- (float)getVersionNumberForClient:(in byref id <RevBayesUsing>)client;
- (void)submitModelWithInfo:(in bycopy NSArray*)modelInfo andData:(in bycopy NSData*)data fromClient:(in byref id <RevBayesUsing>)client;
- (void)submitRegistrationInfo:(in bycopy NSArray*)modelInfo fromClient:(in byref id <RevBayesUsing>)client;
- (BOOL)subscribeClient:(in byref id <RevBayesUsing>)newClient;
- (void)unsubscribeClient:(in byref id <RevBayesUsing>)client;

@end
