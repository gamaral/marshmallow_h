static const char * s_test_format = "[%s] : %s \"%s\"\n";
static const char * s_str_passed  = "PASS";
static const char * s_str_failed  = "FAIL";
static bool s_passed = true;

#define TEST(x, y) fprintf(stderr, s_test_format, y ? s_str_passed : s_str_failed, __FUNCTION__, x); s_passed &= (y)
#define TEST_EXITCODE s_passed ? 0 : -1
