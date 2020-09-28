# Arbitrary_Precision_Integer

<!-- TABLE OF CONTENTS -->
## Table of Contents

* [About the Project](#about-the-project)
  * [Built With](#built-with)
* [Getting Started](#getting-started)
  * [Installation](#installation)
* [Usage](#usage)


<!-- ABOUT THE PROJECT -->
## About The Project

Most programming languages' (such as C, C++, and Java) primitive numeric types (int, long, double, etc.) are implemented by hardware-supported numeric data types. Therefore, inevitably, only a finite set of values can be directly represented using those data types. In this project, I want to implement a data type that could store infinitely large integers and supports integer arithmetic such as add, subtract, compare, and bit shift.

There are two obvious advantages of this data type
1. Although theoretically we can't really store infinitely large number because our computer memory is limited. We could use this data type to store enormously large number compared with primitive numeric types. Some applications, such as cryptographic applications, need to represent values that the machine integer data types can’t be used directly. 
2. We don't need to worry about integer overflow, which is a common but troublesome behavior in primitive numeric types. 

### Built With
* [C](https://www.gnu.org/software/gnu-c-manual/gnu-c-manual.html)
* [TCTest](https://github.com/daveho/tctest)



<!-- GETTING STARTED -->
## Getting Started

Here is how you set up this project locally

### Installation

1. Install the C language [GCC compiler](https://gcc.gnu.org/install/index.html)
2. Clone the repo
```sh
git clone https://github.com/tottiliyt/ArbitraryPrecisionInteger.git
```
3. Makefile
```sh
make all
```


## Usage
Here is the functionality of each function (Checkout the apint.c file for implementation details):

**apint_create_from_u64**: Returns a pointer to an ApInt instance whose value is specified by the val parameter, which is a 64-bit unsigned value.

**apint_create_from_hex**: Returns a pointer to an ApInt instance whose value is specified by the hex parameter, which is an arbitrary sequence of hexadecimal (base 16) digits. This function should accept both the lower-case letters a through f and the upper-case letters A through F as the hex digits with values 10 through 15.

**apint_destroy**: Deallocates the memory used by the ApInt instance pointed-to by the ap parameter.

**apint_get_bits**: Returns a uint64_t value containing 64 bits of the binary representation of the ApInt instance pointed to by ap. The parameter n indicates which bits to return. If n is 0, bits 0..63 are returned, if n is 1 bits 64..127 are returned, etc. The function should be prepared to handle arbitrarily large values of n.

**apint_highest_bit_set**: Returns the position of the most significant bit set to 1 in representation of the ApInt pointed to by ap. As a special case, returns -1 if the ApInt instance pointed to by ap represents the value 0.

**apint_lshift**: Returns a pointer to an ApInt instance formed by shifting each bit of the ApInt instance pointed to by ap one bit position to the left.

**apint_lshift_n**: Returns a pointer to an ApInt instance formed by shifting each bit of the ApInt instance pointed to by ap n bit positions to the left. Important: your implementation of this function should not involve calling apint_lshift in a loop.

**apint_format_as_hex**: Returns a pointer to a dynamically-allocated C character string containing the hexadecimal (base 16) digits of the representation of the ApInt instance pointed to by ap. Note that the hex digits representing the values 10 through 15 should be lower-case a through f. The string returned should not have any leading zeroes, except in the special case of the ApInt instance representing the value 0, in which case the returned string should consist of a single 0 digit.

**apint_add**: Computes the sum a plus b, and returns a pointer to an ApInt instance representing the sum.

**apint_sub**: Computes the difference a minus b, and returns a pointer to an ApInt instance representing the difference. As a special case, if b is greater than a, returns NULL (since the ApInt data type cannot represent a negative value.)

**apint_compare**: Compares the values represented by the ApInt instances pointed-to by the parameters left and right. Returns a negative value if left is less that right, a positive value if left is greater than right, and 0 if the values are equal.


To use these function, you could write a main function that initialize ApInt data type and call these function, or simply edit the apintTests.c to print the value of ApInt after calling functions.

For example, if you wonder what's the result of 

123456789123456789123456789123456789123456789 - 987654321987654321987654321987654321

```C
a = apint_create_from_hex("58936e53d139afefabb2683f150b684045f15"); /*hex string of 123456789123456789123456789123456789123456789*/ 
	b = apint_create_from_hex("be371e1eac64a448a691ab431812b1"); /*hex string of 987654321987654321987654321987654321*/ 
	diff = apint_sub(a, b);
 fprintf("the resulf of 123456789123456789123456789123456789123456789 - 987654321987654321987654321987654321
 is %s", apint_format_as_hex(diff));
```
Example outcome


```Linux
make all
./apintTests
the resulf of 123456789123456789123456789123456789123456789 - 987654321987654321987654321987654321
 is 58936e5fb4ab91da71fcacc97e261c71c71c6 /*hex string of 123456790111111111111111111111111111111111110*/ 
```
