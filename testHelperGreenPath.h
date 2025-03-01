#pragma once

/*
This class is used in order to verfiy the correctness of the
HelperGreenPath object. Its functions (tests) should run only
in "testing mode".
Ideally, it will also NOT be part of the final executable but
composed into a different one.
*/
class TestHelperGreenPath
{
    public:
        TestHelperGreenPath();
        ~TestHelperGreenPath();
        
        /*
        This will be invoked in order to run the tests. If
        and only if ALL tests passed, it will return 0, otherwise
        it will return non-zero value. It is "equivalent" to GTest's
        testing::init() function
        */
        int TestHelperGreenPathRunTests(); 
        int testFindPathWhenSnakeExpandToFour();
};