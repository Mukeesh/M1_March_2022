#include "unity.h"

#define PROJECT_NAME "QUIZ_PROJECT"
/* Prototypes for all the test functions */

/**
 * @brief Testing function for Quiz_Project
 * 
 */
extern void test_quiz();
/* Required by the unity test framework */
void setUp(void){}
/* Required by the unity test framework */
void tearDown(void){}
/* Start of the application test */
int main(void){
    UnityBegin(NULL);

    RUN_TEST(test_quiz);

    return (UnityEnd());
}
