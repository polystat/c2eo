#include "stdio.h"

short func0() {
	return 0;
}
static short func1() {
	return 0;
}
extern short func2() {
	return 0;
}
const short func3() {
	return 0;
}
static const short func4() {
	return 0;
}
extern const short func5() {
	return 0;
}
volatile short func6() {
	return 0;
}
static volatile short func7() {
	return 0;
}
extern volatile short func8() {
	return 0;
}
const volatile short func9() {
	return 0;
}
static const volatile short func10() {
	return 0;
}
extern const volatile short func11() {
	return 0;
}
inline __attribute__((always_inline)) short func12() {
	return 0;
}
static inline __attribute__((always_inline)) short func13() {
	return 0;
}
extern inline __attribute__((always_inline)) short func14() {
	return 0;
}
const inline __attribute__((always_inline)) short func15() {
	return 0;
}
static const inline __attribute__((always_inline)) short func16() {
	return 0;
}
extern const inline __attribute__((always_inline)) short func17() {
	return 0;
}
volatile inline __attribute__((always_inline)) short func18() {
	return 0;
}
static volatile inline __attribute__((always_inline)) short func19() {
	return 0;
}
extern volatile inline __attribute__((always_inline)) short func20() {
	return 0;
}
const volatile inline __attribute__((always_inline)) short func21() {
	return 0;
}
static const volatile inline __attribute__((always_inline)) short func22() {
	return 0;
}
extern const volatile inline __attribute__((always_inline)) short func23() {
	return 0;
}

int main() {
	printf("%d\n", func0());
	printf("%d\n", func1());
	printf("%d\n", func2());
	printf("%d\n", func3());
	printf("%d\n", func4());
	printf("%d\n", func5());
	printf("%d\n", func6());
	printf("%d\n", func7());
	printf("%d\n", func8());
	printf("%d\n", func9());
	printf("%d\n", func10());
	printf("%d\n", func11());
	printf("%d\n", func12());
	printf("%d\n", func13());
	printf("%d\n", func14());
	printf("%d\n", func15());
	printf("%d\n", func16());
	printf("%d\n", func17());
	printf("%d\n", func18());
	printf("%d\n", func19());
	printf("%d\n", func20());
	printf("%d\n", func21());
	printf("%d\n", func22());
	printf("%d\n", func23());
	return 0;
}
