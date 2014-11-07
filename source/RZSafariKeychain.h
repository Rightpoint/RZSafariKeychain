//
//  RZSafariKeychain.h
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
 
@import Foundation;
 
/**
 *  RZSafariKeychain is a lightweight object useful in abstracting some of the lower level libraries required
 *  to use Safari Keychain.  Although the functions used to create its methods
 *  have much more capability than is provided through this class, it is intended to hit the general use case.
 *
 *  If more functionality is required it is recommended that you implement the methods yourself.
 *
 *  It is required to set up your app's entitlements plist and grant access for your app id before using Safari
 *  keychain.  You also must create an apple-app-site-association file signed by your website's CA.  More notes
 *  on doing that can be found here:https://developer.apple.com/library/prerelease/ios/documentation/UserExperience/Conceptual/Handoff/AdoptingHandoff/AdoptingHandoff.html under the `Web Browser–to–Native App Handoff` section.
 *  @note Your service must be `webcredentials` in order to use
 *
 *  @todo implement Safari password creation method.
 */
 
#pragma mark - Defines
/**
 *  Error code for when there are no accounts associated with the domain
 */
OBJC_EXTERN NSInteger const kRZSafariKeychainErrorCodeNoAccount;
 
/**
 *  Error code for when the entitlements aren't setup correctly.
 */
OBJC_EXTERN NSInteger const kRZSafariKeychainErrorCodeNoDomain;
 
/**
 *  The safari keychain account completion block.  This is called with either the valid information
 *  about a user's account of the error that was recieved during the inquiry about their account
 *
 *  @param accountName     The account name for the selected account.  nil if there is an error or one
 *                         isn't chosen.
 *  @param accountPassword The account password for the selected account.  nil if there is an error or
 *                         one isn't chosen.
 *  @param error           The error that occured while selecting account.  nil if it succeeded or if the
 *                          user didn't select an account (hit `No Thanks`)
 */
typedef void(^RZSafariKeychainGetAccountCompletionBlock)(NSString *accountName, NSString *accountPassword, NSError *error);
 
/**
 *  The Safari keychain account update/create completion block.  This is called with the state of the
 *  account update/create request.  If error is nil, then the request succeeded.
 *
 *  @param error The error that occured while updating/creating an account.  nil if it succeeded.
 */
typedef void(^RZSafariKeychainSetAccountCompletionBlock)(NSError *error);
 
#pragma mark - RZSafariKeychain
 
NS_CLASS_AVAILABLE_IOS(8_0) @interface RZSafariKeychain : NSObject
 
/**
 *  Gets the user's information from safari keychain or safari autofill.  
 *  
 *  The user will be prompted with an alert showing a list of their accounts that are associated with your url.
 *  The url will be read from the entitlements file in the Associated Domains capabilities object.  You can add 
 *  multiple domains to the list and this will check for all of them.  Note that these domains must be fully qualified.
 *  ie.  include `www.` when needed.
 *
 *  @note No method for specifying a specific user to restrict it to.
 *  @note No method for specifying a specific domain.  May want to add this one.
 *
 *  @param completion A completion notifying the user of the success/failure of the request.  This is always called on
 *                      the main thread.
 */
+ (void)getAccountKeychainCredentialsWithCompletion:(RZSafariKeychainGetAccountCompletionBlock)completion;
 
/**
 *  Stores an account and password into the safari keychain.  This will allow it to be asseccible in safari and
 *  other apps that use the same URL as an associated domain.  If a password already exists, it will be updated.
 *
 *  In order to remove a password from the keychain, just pass in `NULL` as the password and it will remove that
 *  password.
 *  If you are removing a password, iOS will prompt the user with a dialog to confirm that it is intended.
 *
 *  @param accountName The account name that is attempting to be modified.  cannot be nil.
 *  @param password    The password that will be stored with the associated account.  Pass in nil to remove the
 *                      password from the account.
 *  @param urlsString  The fully qualified (includes www if needed) string representation of the website that
 *                      is updating or storing the password for.  This cannot be nil and also must be in the apps
 *                      associated domains entitlements.
 *  @param completion  A completion notifying the user of the success/failure of the request.  This is always called
 *                      on the main thread.
 */
+ (void)setAccountName:(NSString *)accountName password:(NSString *)password forURLString:(NSString *)urlString completion:(RZSafariKeychainSetAccountCompletionBlock)completion;
 
/**
 *  Helper method to see if the methods used by RZSafariKeychain are available on the current os.
 *
 *  @return State if security keychain methods are available.
 */
+ (BOOL)isSafariKeychainAvailable;
 
@end