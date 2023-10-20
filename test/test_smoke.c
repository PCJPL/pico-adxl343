#include "unity.h"
#include "ADXL343.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_smoke_test(void) {
    adxl343_init();
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_smoke_test);
    return UNITY_END();
}
