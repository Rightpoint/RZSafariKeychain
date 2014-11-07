//
//  RZSafariKeychain.m
//  Raizlabs
//
//  Created by alex.rouse on 9/17/14.
//  Copyright (c) 2014 Raizlabs. All rights reserved.
//
 
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
 
#import "RZSafariKeychain.h"
@import Security.SecSharedCredential;
 
NSInteger const kRZSafariKeychainErrorCodeNoAccount = -25300; //errKCItemNotFound in MacErrors.h
NSInteger const kRZSafariKeychainErrorCodeNoDomain = -50; //paramErr in MacErrors.h
 
@implementation RZSafariKeychain
 
+ (void)getAccountKeychainCredentialsWithCompletion:(RZSafariKeychainGetAccountCompletionBlock)completion
{
    SecRequestSharedWebCredential(NULL, NULL, ^(CFArrayRef credentials, CFErrorRef error) {
        
        if ( error == nil ) {
            // At this point we should only have 1 object in credentials although the API supports more than one.
            if ( CFArrayGetCount(credentials) > 0 ) {
                NSDictionary *userCreds = CFArrayGetValueAtIndex(credentials, 0);
                NSString *userName = userCreds[(__bridge __strong id)(kSecAttrAccount)];
                NSString *userPassword = userCreds[(__bridge __strong id)(kSecSharedPassword)];
 
                if ( completion != nil ) {
                    dispatch_async(dispatch_get_main_queue(), ^{
                        completion(userName, userPassword, nil);
                    });
                }
            }
            else {
                // The user hit no thanks.
                if ( completion != nil ) {
                    dispatch_async(dispatch_get_main_queue(), ^{
                        completion(nil, nil, nil);
                    });
                }
            }
        }
        else {
            if ( completion != nil ) {
                NSError *arcError = (__bridge NSError *)(error);
                dispatch_async(dispatch_get_main_queue(), ^{
                    completion(nil, nil, arcError);
                });
            }
        }
    });
}
 
+ (void)setAccountName:(NSString *)accountName password:(NSString *)password forURLString:(NSString *)urlString completion:(RZSafariKeychainSetAccountCompletionBlock)completion;
{
    NSParameterAssert(accountName);
    NSParameterAssert(urlString);
    
    SecAddSharedWebCredential((__bridge CFStringRef)(urlString), (__bridge CFStringRef)(accountName), (__bridge CFStringRef)(password), ^(CFErrorRef error) {
        if ( completion != nil ) {
            dispatch_async(dispatch_get_main_queue(), ^{
                completion((__bridge NSError *)(error));
            });
        }
    });
}
 
+ (BOOL)isSafariKeychainAvailable
{
    return ( SecRequestSharedWebCredential != NULL );
}
 
@end