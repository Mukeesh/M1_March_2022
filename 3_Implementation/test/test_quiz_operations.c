#include "unity.h"
#include "quiz_operations.h"
#define PROJECT_NAME "QUIZ_PROJECT"
void test_initArray();
void test_insertArray();
void test_freeArray();
void test_initArrayRound();
void test_insertArrayRound();
void test_freeArrayRound();
void test_waitForClients();
void test_game();
void test_XMLParser();
void test_printRound();
int test_printResult();
void setUp(){}
void tearDown(){}
int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_initArray);
    RUN_TEST(test_insertArray);
    RUN_TEST(test_freeArray);
    RUN_TEST(test_initArrayRound);
    RUN_TEST(test_insertArrayRound);
    RUN_TEST(test_freeArrayRound);
    RUN_TEST(test_waitForClients);
    RUN_TEST(test_game);
    RUN_TEST(test_XMLParser);
    RUN_TEST(test_printRound);
    RUN_TEST(test_printResult);
    return UNITY_END;
    
}