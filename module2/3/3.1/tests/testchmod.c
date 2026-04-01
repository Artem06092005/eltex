
#include "../src/chmod.h"
#include "../unity/unity.h"

void setUp(void) {}

void tearDown(void) {}

void test_getBitMask(void) {
	TEST_ASSERT_EQUAL_HEX16(0755, getBitMask("755"));
	TEST_ASSERT_EQUAL_HEX16(0777, getBitMask("777"));
	TEST_ASSERT_EQUAL_HEX16(0000, getBitMask("000"));

	TEST_ASSERT_EQUAL_HEX16(0777, getBitMask("rwxrwxrwx"));
	TEST_ASSERT_EQUAL_HEX16(0755, getBitMask("rwxr-xr-x"));
	TEST_ASSERT_EQUAL_HEX16(0644, getBitMask("rw-r--r--"));
	TEST_ASSERT_EQUAL_HEX16(0000, getBitMask("---------"));
}

void test_getBitMaskError(void) {
	TEST_ASSERT_EQUAL_HEX16(0xFFFF, getBitMask("jjkk"));
	TEST_ASSERT_EQUAL_HEX16(0xFFFF, getBitMask("999"));
	TEST_ASSERT_EQUAL_HEX16(0xFFFF, getBitMask("rwx"));
	TEST_ASSERT_EQUAL_HEX16(0xFFFF, getBitMask("rwxrwxrwxrwxrwxrwxrwxr"));
}

void test_checkFileMode(void) {
	char resStr[10];
	int mode = getStatString("./src/main.c", resStr);
	TEST_ASSERT_NOT_EQUAL(-1, mode);

	TEST_ASSERT_EQUAL_HEX16(0664, mode);
	TEST_ASSERT_EQUAL_STRING("rw-rw-r--", resStr);
}

void test_simulateChmod(void) {
	char *testFile = "src/chmod.c";

	TEST_ASSERT_EQUAL_HEX16(0700, simulateChmod("700", testFile));
	TEST_ASSERT_EQUAL_HEX16(0444, simulateChmod("r--r--r--", testFile));

	TEST_ASSERT_EQUAL_HEX16(0665, simulateChmod("o+x", testFile));

	TEST_ASSERT_EQUAL_HEX16(0775, simulateChmod("a+x", testFile));

	TEST_ASSERT_EQUAL_HEX16(0245, simulateChmod("o+x,g-w,u-r", testFile));

	TEST_ASSERT_EQUAL_HEX16(0000, simulateChmod("a-rwx", testFile));
	TEST_ASSERT_EQUAL_HEX16(0777, simulateChmod("a+rwx", testFile));

	TEST_ASSERT_EQUAL_HEX16(0674, simulateChmod("g-rw,g+rwx", testFile));
}

void test_simulateChmodError(void) {
	TEST_ASSERT_EQUAL_HEX16(0xFFFF,
							simulateChmod("755", "agsgniujdngdgidu.txt"));
}

int main(void) {
	UNITY_BEGIN();
	RUN_TEST(test_getBitMask);
	RUN_TEST(test_getBitMaskError);
	RUN_TEST(test_checkFileMode);
	RUN_TEST(test_simulateChmod);
	RUN_TEST(test_simulateChmodError);
	return UNITY_END();
}