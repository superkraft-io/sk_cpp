//
//  sk_cpp_macosUITestsLaunchTests.m
//  sk_cpp_macosUITests
//
//  Created by Splitter AI on 23/12/2024.
//

#import <XCTest/XCTest.h>

@interface sk_cpp_macosUITestsLaunchTests : XCTestCase

@end

@implementation sk_cpp_macosUITestsLaunchTests

+ (BOOL)runsForEachTargetApplicationUIConfiguration {
    return YES;
}

- (void)setUp {
    self.continueAfterFailure = NO;
}

- (void)testLaunch {
    XCUIApplication *app = [[XCUIApplication alloc] init];
    [app launch];

    // Insert steps here to perform after app launch but before taking a screenshot,
    // such as logging into a test account or navigating somewhere in the app

    XCTAttachment *attachment = [XCTAttachment attachmentWithScreenshot:XCUIScreen.mainScreen.screenshot];
    attachment.name = @"Launch Screen";
    attachment.lifetime = XCTAttachmentLifetimeKeepAlways;
    [self addAttachment:attachment];
}

@end
