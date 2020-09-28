/*
 * Unit tests for arbitrary-precision integer data type
 *
 * These tests are by no means comprehensive.  You will need to
 * add more tests of your own!  In particular, make sure that
 * you have tests for more challenging situations, such as
 *
 * - large values
 * - adding/subtracting/comparing values with different lengths
 * - special cases (carries when adding, borrows when subtracting, etc.)
 * - etc.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "apint.h"
#include "tctest.h"

typedef struct {
	ApInt *ap0;
	ApInt *ap1;
	ApInt *ap110660361;
	ApInt *max1;
  ApInt *ap123456789123456789123456789123456789123456789;
  ApInt *ap987654321987654321987654321987654321;
} TestObjs;

TestObjs *setup(void);
void cleanup(TestObjs *objs);

void testCreateFromU64(TestObjs *objs);
void testCreateFromHex(TestObjs *objs);
void testGetBits(TestObjs *objs);
void testHighestBitSet(TestObjs *objs);
void testLshiftN(TestObjs *objs);
void testCompare(TestObjs *objs);
void testFormatAsHex(TestObjs *objs);
void testAdd(TestObjs *objs);
void testSub(TestObjs *objs);
void testLshift(TestObjs *objs);


int main(int argc, char **argv) {
	TEST_INIT();

	if (argc > 1) {
		/*
		 * name of specific test case to execute was provided
		 * as a command line argument
		 */
		tctest_testname_to_execute = argv[1];
	}

	TEST(testCreateFromU64);
	TEST(testCreateFromHex);
	TEST(testGetBits);
	TEST(testHighestBitSet);
      	TEST(testLshiftN);
	TEST(testCompare);
	TEST(testFormatAsHex);
	TEST(testAdd);
	TEST(testSub);
	TEST(testLshift);
	TEST_FINI();
}

TestObjs *setup(void) {
	TestObjs *objs = malloc(sizeof(TestObjs));
	objs->ap0 = apint_create_from_u64(0UL);
	objs->ap1 = apint_create_from_u64(1UL);
	objs->ap110660361 = apint_create_from_u64(110660361UL);
	objs->max1 = apint_create_from_u64(0xFFFFFFFFFFFFFFFFUL);
	objs->ap123456789123456789123456789123456789123456789 = apint_create_from_hex("58936e53d139afefabb2683f150b684045f15");
	objs->ap987654321987654321987654321987654321 = apint_create_from_hex("be371e1eac64a448a691ab431812b1");
	return objs;
}

void cleanup(TestObjs *objs) {
	apint_destroy(objs->ap0);
	apint_destroy(objs->ap1);
	apint_destroy(objs->ap110660361);
	apint_destroy(objs->max1);
	apint_destroy(objs->ap123456789123456789123456789123456789123456789);
	apint_destroy(objs->ap987654321987654321987654321987654321);

	free(objs);
}

void testCreateFromU64(TestObjs *objs) {
	ASSERT(0UL == apint_get_bits(objs->ap0, 0));
	ASSERT(1UL == apint_get_bits(objs->ap1, 0));
	ASSERT(110660361UL == apint_get_bits(objs->ap110660361, 0));
	ASSERT(0xFFFFFFFFFFFFFFFFUL == apint_get_bits(objs->max1, 0));
}

void testCreateFromHex(TestObjs *objs){
  ASSERT(0x83F150B684045F15UL==apint_get_bits(objs->ap123456789123456789123456789123456789123456789,0));
  ASSERT(0xE53D139AFEFABB26UL==apint_get_bits(objs->ap123456789123456789123456789123456789123456789,1));
  ASSERT(0x58936UL==apint_get_bits(objs->ap123456789123456789123456789123456789123456789,2));
}

void testHighestBitSet(TestObjs *objs) {
	ASSERT(-1 == apint_highest_bit_set(objs->ap0));
	ASSERT(0 == apint_highest_bit_set(objs->ap1));
	ASSERT(26 == apint_highest_bit_set(objs->ap110660361));
	ASSERT(63 == apint_highest_bit_set(objs->max1));
	ASSERT(146 == apint_highest_bit_set(objs->ap123456789123456789123456789123456789123456789));
	ASSERT(119 == apint_highest_bit_set(objs->ap987654321987654321987654321987654321));
	
}
void testGetBits(TestObjs *objs){
  ASSERT(0xFFFFFFFFFFFFFFFFUL == apint_get_bits(objs->max1, 0));
  ASSERT(0UL == apint_get_bits(objs->max1, 1));
  ASSERT(0UL == apint_get_bits(objs->max1, 1000));
}
void testLshiftN(TestObjs *objs) {
	ApInt *result;
	char *s;
	result = apint_lshift_n(objs->ap0, 17);
	ASSERT(0UL == apint_get_bits(result, 0));
	ASSERT(0UL == apint_get_bits(result, 1));
	apint_destroy(result);

	result = apint_lshift_n(objs->ap1, 17);
	ASSERT(0x20000UL == apint_get_bits(result, 0));
	ASSERT(0UL == apint_get_bits(result, 1));
	apint_destroy(result);

	result = apint_lshift_n(objs->ap110660361, 17);
	ASSERT(0xD3116120000UL == apint_get_bits(result, 0));
	ASSERT(0UL == apint_get_bits(result, 1));
	apint_destroy(result);

	result = apint_lshift_n(objs->ap123456789123456789123456789123456789123456789, 17);
        
	ASSERT(0 == strcmp("b126dca7a2735fdf5764d07e2a16d0808be2a0000", (s = apint_format_as_hex(result))));
	free(s);
	apint_destroy(result);
	
}

void testCompare(TestObjs *objs) {
	/* 1 > 0 */
	ASSERT(apint_compare(objs->ap1, objs->ap0) > 0);
	/* 0 < 1 */
	ASSERT(apint_compare(objs->ap0, objs->ap1) < 0);
	/* 110660361 > 0 */
	ASSERT(apint_compare(objs->ap110660361, objs->ap0) > 0);
	/* 110660361 > 1 */
	ASSERT(apint_compare(objs->ap110660361, objs->ap1) > 0);
	/* 0 < 110660361 */
	ASSERT(apint_compare(objs->ap0, objs->ap110660361) < 0);
	/* 1 < 110660361 */
	ASSERT(apint_compare(objs->ap1, objs->ap110660361) < 0);
	ASSERT(apint_compare(objs->ap123456789123456789123456789123456789123456789, objs->ap987654321987654321987654321987654321) > 0);
	ASSERT(apint_compare(objs->ap987654321987654321987654321987654321, objs->ap123456789123456789123456789123456789123456789) < 0);

	
}

void testFormatAsHex(TestObjs *objs) {
	char *s;

	ASSERT(0 == strcmp("0", (s = apint_format_as_hex(objs->ap0))));
	free(s);

	ASSERT(0 == strcmp("1", (s = apint_format_as_hex(objs->ap1))));
	free(s);

	ASSERT(0 == strcmp("6988b09", (s = apint_format_as_hex(objs->ap110660361))));
	free(s);

	ASSERT(0 == strcmp("ffffffffffffffff", (s = apint_format_as_hex(objs->max1))));
	free(s);

	ASSERT(0 == strcmp("58936e53d139afefabb2683f150b684045f15", (s = apint_format_as_hex(objs->ap123456789123456789123456789123456789123456789))));
	free(s);
	ASSERT(0 == strcmp("be371e1eac64a448a691ab431812b1", (s = apint_format_as_hex(objs->ap987654321987654321987654321987654321))));
	free(s);
}

void testAdd(TestObjs *objs) {
	ApInt *sum;
	char *s;

	/* 0 + 0 = 0 */
	sum = apint_add(objs->ap0, objs->ap0);
	ASSERT(0 == strcmp("0", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);

	/* 1 + 0 = 1 */
	sum = apint_add(objs->ap1, objs->ap0);
	ASSERT(0 == strcmp("1", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);

	/* 1 + 1 = 2 */
	sum = apint_add(objs->ap1, objs->ap1);
	ASSERT(0 == strcmp("2", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);

	/* 110660361 + 1 = 110660362 */
	sum = apint_add(objs->ap110660361, objs->ap1);
	ASSERT(0 == strcmp("6988b0a", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);

	/* FFFFFFFFFFFFFFFF + 1 = 10000000000000000 */
	sum = apint_add(objs->max1, objs->ap1);
	ASSERT(0 == strcmp("10000000000000000", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);

	sum = apint_add(objs->ap123456789123456789123456789123456789123456789, objs->ap987654321987654321987654321987654321);
	ASSERT(0 == strcmp("58936e5fb4ab91da71fcacc97e261c71c71c6", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);
}

void testSub(TestObjs *objs) {
	ApInt *a, *b, *diff;
	char *s;

	/* subtracting 1 from ffffffffffffffff is fffffffffffffffe */
	diff = apint_sub(objs->max1, objs->ap1);
	ASSERT(0 == strcmp("fffffffffffffffe", (s = apint_format_as_hex(diff))));
	apint_destroy(diff);
	free(s);

	/* subtracting 0 from 1 is 1 */
	diff = apint_sub(objs->ap1, objs->ap0);
	ASSERT(0 == strcmp("1", (s = apint_format_as_hex(diff))));
	ASSERT(0 == apint_compare(diff, objs->ap1));
	apint_destroy(diff);
	free(s);

	/* subtracting 1 from 1 is 0 */
	diff = apint_sub(objs->ap1, objs->ap1);
	ASSERT(0 == strcmp("0", (s = apint_format_as_hex(diff))));
	ASSERT(0 == apint_compare(diff, objs->ap0));
	apint_destroy(diff);
	free(s);

	/* subtracting 1 from 0 can't be represented because it is negative */
	ASSERT(NULL == apint_sub(objs->ap0, objs->ap1));

	/* test involving larger values */
	a = apint_create_from_hex("7e35207519b6b06429378631ca460905c19537644f31dc50114e9dc90bb4e4ebc43cfebe6b86d");
	b = apint_create_from_hex("9fa0fb165441ade7cb8b17c3ab3653465e09e8078e09631ec8f6fe3a5b301dc");
	diff = apint_sub(a, b);
	
	ASSERT(0 == strcmp("7e35207519b6afc4883c6fdd8898213a367d73b918de95f20766963b0251c622cd3ec4633b691",
		(s = apint_format_as_hex(diff))));
	apint_destroy(diff);
	apint_destroy(b);
	apint_destroy(a);
	free(s);

	diff= apint_sub(objs->ap123456789123456789123456789123456789123456789, objs->ap987654321987654321987654321987654321);
	ASSERT(0 == strcmp("58936e47edc7ce04e56823b4abf0b40ec4c64", (s = apint_format_as_hex(diff))));
	apint_destroy(diff);
	free(s);
}

void testLshift(TestObjs *objs) {
	ApInt *result;
	char *s;
	result = apint_lshift(objs->ap0);
	ASSERT(0UL == apint_get_bits(result, 0));
	ASSERT(0UL == apint_get_bits(result, 1));
	apint_destroy(result);

	result = apint_lshift(objs->ap1);
	ASSERT(0x2UL == apint_get_bits(result, 0));
	ASSERT(0UL == apint_get_bits(result, 1));
	apint_destroy(result);

	result = apint_lshift(objs->ap110660361);
	ASSERT(0xD311612UL == apint_get_bits(result, 0));
	ASSERT(0UL == apint_get_bits(result, 1));
	apint_destroy(result);

	result = apint_lshift(objs->max1);
	ASSERT(0xFFFFFFFFFFFFFFFEUL == apint_get_bits(result, 0));
	ASSERT(1UL == apint_get_bits(result, 1));
	apint_destroy(result);

	result = apint_lshift(objs->ap123456789123456789123456789123456789123456789);
	ASSERT(0 == strcmp("b126dca7a2735fdf5764d07e2a16d0808be2a", s = apint_format_as_hex(result)));
	free(s);
	apint_destroy(result);

	
}
