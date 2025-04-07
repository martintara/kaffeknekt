#include <Arduino.h>
#include <unity.h>
#include "hello_logic.h"

void test_hello_message_should_be_correct() {
    TEST_ASSERT_EQUAL_STRING("Hello from loop", get_hello_message().c_str());
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    UNITY_BEGIN();
    RUN_TEST(test_hello_message_should_be_correct);
    UNITY_END();
}

void loop() {
    // not used
}
