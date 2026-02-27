#include <cstdio>
#include <cstdlib>

// Minimal test framework — no external dependencies
static int g_tests_run = 0;
static int g_tests_passed = 0;
static int g_tests_failed = 0;

void test_report(const char* name, bool passed) {
	g_tests_run++;
	if (passed) {
		g_tests_passed++;
		printf("  PASS: %s\n", name);
	} else {
		g_tests_failed++;
		printf("  FAIL: %s\n", name);
	}
}

// Declarations of test suites
void run_ar_envelope_tests();
void run_adsr_envelope_tests();
void run_oscillator_tests();
void run_polivoks_filter_tests();

int main() {
	printf("=== Stellar Debris DSP Tests ===\n\n");

	printf("[AR Envelope]\n");
	run_ar_envelope_tests();
	printf("\n");

	printf("[ADSR Envelope]\n");
	run_adsr_envelope_tests();
	printf("\n");

	printf("[Oscillator]\n");
	run_oscillator_tests();
	printf("\n");

	printf("[Polivoks Filter]\n");
	run_polivoks_filter_tests();
	printf("\n");

	printf("=== Results: %d/%d passed", g_tests_passed, g_tests_run);
	if (g_tests_failed > 0) {
		printf(", %d FAILED", g_tests_failed);
	}
	printf(" ===\n");

	return g_tests_failed > 0 ? 1 : 0;
}
