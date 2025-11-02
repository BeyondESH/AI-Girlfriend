#ifndef TEST_H
#define TEST_H

class Test
{
public:
    Test()=delete;
    static void test_threadMgr();
    static void test_http_get();
    static void test_post_chat();
    static void test_audioMgr();
    static void test_jsonMgr();
};

#endif // TEST_H
