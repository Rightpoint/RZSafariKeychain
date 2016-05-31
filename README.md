RZSafariKeychain
================

RZSafariKeychain is a collection of class methods to make getting access to the user credentials in the keychain easier.

###Installation
Easiest way to install is using cocopods, add this to your pod file to get it working.

```ruby
pod 'RZSafariKeychain', '~> 0.1'
```

Alternatively you can just add the contents of the source folder to your project.


### Use

##### Check for Access
If you are building for iOS 7 + 8 you need to make sure that the system has access.  There is a class method to do this `+ (BOOL)isSafariKeychainAvailable;`

##### Ask for credentials
If the device has access to safari keychain you can then prompt the user for their credentials.  Here is an example call
```objc
[RZSafariKeychain getAccountKeychainCredentialsWithCompletion:^(NSString *accountName, NSString *accountPassword, NSError *error) {
        if ( error ) {
            NSLog(@"Error getting safari Credentials: %@", error);
        }
        else {
            if ( accountName != nil && accountPassword != nil ) {
                self.emailAddressTextField.text = accountName;
                self.passwordTextField.text = accountPassword;
            }
            else {
                NSLog( @"Got back an invalid account name or password" );
            }
        }
    }];

```

Setup Walkthrough
=======
** The remainder of the readme is just a walkthrough to setting up the server/configuration side of Safari Keychain. **


###Server Implementation
#####  Create your web credentials JSON file.
This will look something like this:
 
```json
{
    "webcredentials": {
    	"apps": [    "YWBN8XTPBJ.com.example.myApp",
                 	"YWBN8XTPBJ.com.example.myOtherApp" ]
    }
}
```
Note that the service that is being used is 'webcredentials' and the value used for apps is in the form `App-ID-Prefix`.`bundleID`

**An Important Note:  The value above for YWBN8XTPBJ is the App ID Prefix.  This is generally the same as your TeamID but this is not always the case!  See here for more details: <https://developer.apple.com/library/ios/technotes/tn2311/_index.html>**

#####Sign the json file.
Next you need to sign the json file to create an apple-app-site-association file.  This can be done with the following script.

```sh
cat json.txt | openssl smime -sign -inkey example.com.key
                             -signer example.com.pem
                             -certfile intermediate.pem
                             -noattr -nodetach
                             -outform DER > apple-app-site-association
```

You will need to get the key and certificate for your webserver to pass in above as the `-sign` and `-signer` parameters.

Note that your actual command may be a bit different and most will not have a `-certfile` unless you have intermediate certificates.

#####Host the file.
The last step for the server side setup is to actually host the file.  The location for it should be `https://example.com/apple-app-site-association`.

Also make sure that the `Content-Type` is `application/pkcs7-mime`

###App Configuration

This is generally the easy part.
##### Add capabilities.
In XCode go to your `Project File` -> `Capabilities` -> `Associated Domains` and turn it on.

##### Add Associated Domains
Inline in xCode hit the `+` and add your domain in the following format:  `service`:`Domain`  An example of this would be `webcredentials:www.example.com`.  This will update your entitlements.plist to look something like this.

```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
	<key>com.apple.developer.associated-domains</key>
	<array>
		<string>webcredentials:www.example.com</string>
	</array>
</dict>
</plist>

```

For most people you are done.  Easy.

If something goes wrong there is the option to configure you app ID through the Developer portal which is also easy.  Just navigate to your app id and add Associated Domains.  After this you will also need to add your domains in the same format as above.

**If you use an enterprise cert or if there is any other reason that you use a wildcard app id (anything with a * in it), this will be annoying...  You will not be able to have an entitlements plist with these keys in it.  To get around this you can have multiple entitlements plist files, one for Production and one for Enterprise/Wildcard.  There is an `code signing entitlements` property on a projects build settings that you can use to differentiate based off of a build configuration**

## Usefull links
This is just a collection of links that I found helpful while implementing safari keychain.

- <https://developer.apple.com/library/prerelease/ios/documentation/UserExperience/Conceptual/Handoff/AdoptingHandoff/AdoptingHandoff.html#//apple_ref/doc/uid/TP40014338-CH2-SW10> ( check out the section titled 'Web Browser-to-Native App Handoff' )
- <https://developer.apple.com/videos/wwdc/2014/#506> ( WWDC video )
- <https://www.openssl.org/docs/apps/smime.html> ( OpenSSL reference for smime, helpful if you have a complex web setup. )
- <https://developer.apple.com/library/ios/technotes/tn2311/_index.html> ( Apple Docs for Managing a different App ID Prefix and Team ID )
