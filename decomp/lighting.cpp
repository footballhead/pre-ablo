#include "lighting.h"

//
// Initialized variables
//

DWORD dword_4ACBF8 = 0; // unused
BOOL lightflag = FALSE;

// TODO: From Devilution, verify!
char CrawlTable[2749] = {
    1,
    0, 0,
    4,
    0, 1, 0, -1, -1, 0, 1, 0,
    16,
    0, 2, 0, -2, -1, 2, 1, 2,
    -1, -2, 1, -2, -1, 1, 1, 1,
    -1, -1, 1, -1, -2, 1, 2, 1,
    -2, -1, 2, -1, -2, 0, 2, 0,
    24,
    0, 3, 0, -3, -1, 3, 1, 3,
    -1, -3, 1, -3, -2, 3, 2, 3,
    -2, -3, 2, -3, -2, 2, 2, 2,
    -2, -2, 2, -2, -3, 2, 3, 2,
    -3, -2, 3, -2, -3, 1, 3, 1,
    -3, -1, 3, -1, -3, 0, 3, 0,
    32,
    0, 4, 0, -4, -1, 4, 1, 4,
    -1, -4, 1, -4, -2, 4, 2, 4,
    -2, -4, 2, -4, -3, 4, 3, 4,
    -3, -4, 3, -4, -3, 3, 3, 3,
    -3, -3, 3, -3, -4, 3, 4, 3,
    -4, -3, 4, -3, -4, 2, 4, 2,
    -4, -2, 4, -2, -4, 1, 4, 1,
    -4, -1, 4, -1, -4, 0, 4, 0,
    40,
    0, 5, 0, -5, -1, 5, 1, 5,
    -1, -5, 1, -5, -2, 5, 2, 5,
    -2, -5, 2, -5, -3, 5, 3, 5,
    -3, -5, 3, -5, -4, 5, 4, 5,
    -4, -5, 4, -5, -4, 4, 4, 4,
    -4, -4, 4, -4, -5, 4, 5, 4,
    -5, -4, 5, -4, -5, 3, 5, 3,
    -5, -3, 5, -3, -5, 2, 5, 2,
    -5, -2, 5, -2, -5, 1, 5, 1,
    -5, -1, 5, -1, -5, 0, 5, 0,
    48,
    0, 6, 0, -6, -1, 6, 1, 6,
    -1, -6, 1, -6, -2, 6, 2, 6,
    -2, -6, 2, -6, -3, 6, 3, 6,
    -3, -6, 3, -6, -4, 6, 4, 6,
    -4, -6, 4, -6, -5, 6, 5, 6,
    -5, -6, 5, -6, -5, 5, 5, 5,
    -5, -5, 5, -5, -6, 5, 6, 5,
    -6, -5, 6, -5, -6, 4, 6, 4,
    -6, -4, 6, -4, -6, 3, 6, 3,
    -6, -3, 6, -3, -6, 2, 6, 2,
    -6, -2, 6, -2, -6, 1, 6, 1,
    -6, -1, 6, -1, -6, 0, 6, 0,
    56,
    0, 7, 0, -7, -1, 7, 1, 7,
    -1, -7, 1, -7, -2, 7, 2, 7,
    -2, -7, 2, -7, -3, 7, 3, 7,
    -3, -7, 3, -7, -4, 7, 4, 7,
    -4, -7, 4, -7, -5, 7, 5, 7,
    -5, -7, 5, -7, -6, 7, 6, 7,
    -6, -7, 6, -7, -6, 6, 6, 6,
    -6, -6, 6, -6, -7, 6, 7, 6,
    -7, -6, 7, -6, -7, 5, 7, 5,
    -7, -5, 7, -5, -7, 4, 7, 4,
    -7, -4, 7, -4, -7, 3, 7, 3,
    -7, -3, 7, -3, -7, 2, 7, 2,
    -7, -2, 7, -2, -7, 1, 7, 1,
    -7, -1, 7, -1, -7, 0, 7, 0,
    64,
    0, 8, 0, -8, -1, 8, 1, 8,
    -1, -8, 1, -8, -2, 8, 2, 8,
    -2, -8, 2, -8, -3, 8, 3, 8,
    -3, -8, 3, -8, -4, 8, 4, 8,
    -4, -8, 4, -8, -5, 8, 5, 8,
    -5, -8, 5, -8, -6, 8, 6, 8,
    -6, -8, 6, -8, -7, 8, 7, 8,
    -7, -8, 7, -8, -7, 7, 7, 7,
    -7, -7, 7, -7, -8, 7, 8, 7,
    -8, -7, 8, -7, -8, 6, 8, 6,
    -8, -6, 8, -6, -8, 5, 8, 5,
    -8, -5, 8, -5, -8, 4, 8, 4,
    -8, -4, 8, -4, -8, 3, 8, 3,
    -8, -3, 8, -3, -8, 2, 8, 2,
    -8, -2, 8, -2, -8, 1, 8, 1,
    -8, -1, 8, -1, -8, 0, 8, 0,
    72,
    0, 9, 0, -9, -1, 9, 1, 9,
    -1, -9, 1, -9, -2, 9, 2, 9,
    -2, -9, 2, -9, -3, 9, 3, 9,
    -3, -9, 3, -9, -4, 9, 4, 9,
    -4, -9, 4, -9, -5, 9, 5, 9,
    -5, -9, 5, -9, -6, 9, 6, 9,
    -6, -9, 6, -9, -7, 9, 7, 9,
    -7, -9, 7, -9, -8, 9, 8, 9,
    -8, -9, 8, -9, -8, 8, 8, 8,
    -8, -8, 8, -8, -9, 8, 9, 8,
    -9, -8, 9, -8, -9, 7, 9, 7,
    -9, -7, 9, -7, -9, 6, 9, 6,
    -9, -6, 9, -6, -9, 5, 9, 5,
    -9, -5, 9, -5, -9, 4, 9, 4,
    -9, -4, 9, -4, -9, 3, 9, 3,
    -9, -3, 9, -3, -9, 2, 9, 2,
    -9, -2, 9, -2, -9, 1, 9, 1,
    -9, -1, 9, -1, -9, 0, 9, 0,
    80,
    0, 10, 0, -10, -1, 10, 1, 10,
    -1, -10, 1, -10, -2, 10, 2, 10,
    -2, -10, 2, -10, -3, 10, 3, 10,
    -3, -10, 3, -10, -4, 10, 4, 10,
    -4, -10, 4, -10, -5, 10, 5, 10,
    -5, -10, 5, -10, -6, 10, 6, 10,
    -6, -10, 6, -10, -7, 10, 7, 10,
    -7, -10, 7, -10, -8, 10, 8, 10,
    -8, -10, 8, -10, -9, 10, 9, 10,
    -9, -10, 9, -10, -9, 9, 9, 9,
    -9, -9, 9, -9, -10, 9, 10, 9,
    -10, -9, 10, -9, -10, 8, 10, 8,
    -10, -8, 10, -8, -10, 7, 10, 7,
    -10, -7, 10, -7, -10, 6, 10, 6,
    -10, -6, 10, -6, -10, 5, 10, 5,
    -10, -5, 10, -5, -10, 4, 10, 4,
    -10, -4, 10, -4, -10, 3, 10, 3,
    -10, -3, 10, -3, -10, 2, 10, 2,
    -10, -2, 10, -2, -10, 1, 10, 1,
    -10, -1, 10, -1, -10, 0, 10, 0,
    88,
    0, 11, 0, -11, -1, 11, 1, 11,
    -1, -11, 1, -11, -2, 11, 2, 11,
    -2, -11, 2, -11, -3, 11, 3, 11,
    -3, -11, 3, -11, -4, 11, 4, 11,
    -4, -11, 4, -11, -5, 11, 5, 11,
    -5, -11, 5, -11, -6, 11, 6, 11,
    -6, -11, 6, -11, -7, 11, 7, 11,
    -7, -11, 7, -11, -8, 11, 8, 11,
    -8, -11, 8, -11, -9, 11, 9, 11,
    -9, -11, 9, -11, -10, 11, 10, 11,
    -10, -11, 10, -11, -10, 10, 10, 10,
    -10, -10, 10, -10, -11, 10, 11, 10,
    -11, -10, 11, -10, -11, 9, 11, 9,
    -11, -9, 11, -9, -11, 8, 11, 8,
    -11, -8, 11, -8, -11, 7, 11, 7,
    -11, -7, 11, -7, -11, 6, 11, 6,
    -11, -6, 11, -6, -11, 5, 11, 5,
    -11, -5, 11, -5, -11, 4, 11, 4,
    -11, -4, 11, -4, -11, 3, 11, 3,
    -11, -3, 11, -3, -11, 2, 11, 2,
    -11, -2, 11, -2, -11, 1, 11, 1,
    -11, -1, 11, -1, -11, 0, 11, 0,
    96,
    0, 12, 0, -12, -1, 12, 1, 12,
    -1, -12, 1, -12, -2, 12, 2, 12,
    -2, -12, 2, -12, -3, 12, 3, 12,
    -3, -12, 3, -12, -4, 12, 4, 12,
    -4, -12, 4, -12, -5, 12, 5, 12,
    -5, -12, 5, -12, -6, 12, 6, 12,
    -6, -12, 6, -12, -7, 12, 7, 12,
    -7, -12, 7, -12, -8, 12, 8, 12,
    -8, -12, 8, -12, -9, 12, 9, 12,
    -9, -12, 9, -12, -10, 12, 10, 12,
    -10, -12, 10, -12, -11, 12, 11, 12,
    -11, -12, 11, -12, -11, 11, 11, 11,
    -11, -11, 11, -11, -12, 11, 12, 11,
    -12, -11, 12, -11, -12, 10, 12, 10,
    -12, -10, 12, -10, -12, 9, 12, 9,
    -12, -9, 12, -9, -12, 8, 12, 8,
    -12, -8, 12, -8, -12, 7, 12, 7,
    -12, -7, 12, -7, -12, 6, 12, 6,
    -12, -6, 12, -6, -12, 5, 12, 5,
    -12, -5, 12, -5, -12, 4, 12, 4,
    -12, -4, 12, -4, -12, 3, 12, 3,
    -12, -3, 12, -3, -12, 2, 12, 2,
    -12, -2, 12, -2, -12, 1, 12, 1,
    -12, -1, 12, -1, -12, 0, 12, 0,
    104,
    0, 13, 0, -13, -1, 13, 1, 13,
    -1, -13, 1, -13, -2, 13, 2, 13,
    -2, -13, 2, -13, -3, 13, 3, 13,
    -3, -13, 3, -13, -4, 13, 4, 13,
    -4, -13, 4, -13, -5, 13, 5, 13,
    -5, -13, 5, -13, -6, 13, 6, 13,
    -6, -13, 6, -13, -7, 13, 7, 13,
    -7, -13, 7, -13, -8, 13, 8, 13,
    -8, -13, 8, -13, -9, 13, 9, 13,
    -9, -13, 9, -13, -10, 13, 10, 13,
    -10, -13, 10, -13, -11, 13, 11, 13,
    -11, -13, 11, -13, -12, 13, 12, 13,
    -12, -13, 12, -13, -12, 12, 12, 12,
    -12, -12, 12, -12, -13, 12, 13, 12,
    -13, -12, 13, -12, -13, 11, 13, 11,
    -13, -11, 13, -11, -13, 10, 13, 10,
    -13, -10, 13, -10, -13, 9, 13, 9,
    -13, -9, 13, -9, -13, 8, 13, 8,
    -13, -8, 13, -8, -13, 7, 13, 7,
    -13, -7, 13, -7, -13, 6, 13, 6,
    -13, -6, 13, -6, -13, 5, 13, 5,
    -13, -5, 13, -5, -13, 4, 13, 4,
    -13, -4, 13, -4, -13, 3, 13, 3,
    -13, -3, 13, -3, -13, 2, 13, 2,
    -13, -2, 13, -2, -13, 1, 13, 1,
    -13, -1, 13, -1, -13, 0, 13, 0,
    112,
    0, 14, 0, -14, -1, 14, 1, 14,
    -1, -14, 1, -14, -2, 14, 2, 14,
    -2, -14, 2, -14, -3, 14, 3, 14,
    -3, -14, 3, -14, -4, 14, 4, 14,
    -4, -14, 4, -14, -5, 14, 5, 14,
    -5, -14, 5, -14, -6, 14, 6, 14,
    -6, -14, 6, -14, -7, 14, 7, 14,
    -7, -14, 7, -14, -8, 14, 8, 14,
    -8, -14, 8, -14, -9, 14, 9, 14,
    -9, -14, 9, -14, -10, 14, 10, 14,
    -10, -14, 10, -14, -11, 14, 11, 14,
    -11, -14, 11, -14, -12, 14, 12, 14,
    -12, -14, 12, -14, -13, 14, 13, 14,
    -13, -14, 13, -14, -13, 13, 13, 13,
    -13, -13, 13, -13, -14, 13, 14, 13,
    -14, -13, 14, -13, -14, 12, 14, 12,
    -14, -12, 14, -12, -14, 11, 14, 11,
    -14, -11, 14, -11, -14, 10, 14, 10,
    -14, -10, 14, -10, -14, 9, 14, 9,
    -14, -9, 14, -9, -14, 8, 14, 8,
    -14, -8, 14, -8, -14, 7, 14, 7,
    -14, -7, 14, -7, -14, 6, 14, 6,
    -14, -6, 14, -6, -14, 5, 14, 5,
    -14, -5, 14, -5, -14, 4, 14, 4,
    -14, -4, 14, -4, -14, 3, 14, 3,
    -14, -3, 14, -3, -14, 2, 14, 2,
    -14, -2, 14, -2, -14, 1, 14, 1,
    -14, -1, 14, -1, -14, 0, 14, 0,
    120,
    0, 15, 0, -15, -1, 15, 1, 15,
    -1, -15, 1, -15, -2, 15, 2, 15,
    -2, -15, 2, -15, -3, 15, 3, 15,
    -3, -15, 3, -15, -4, 15, 4, 15,
    -4, -15, 4, -15, -5, 15, 5, 15,
    -5, -15, 5, -15, -6, 15, 6, 15,
    -6, -15, 6, -15, -7, 15, 7, 15,
    -7, -15, 7, -15, -8, 15, 8, 15,
    -8, -15, 8, -15, -9, 15, 9, 15,
    -9, -15, 9, -15, -10, 15, 10, 15,
    -10, -15, 10, -15, -11, 15, 11, 15,
    -11, -15, 11, -15, -12, 15, 12, 15,
    -12, -15, 12, -15, -13, 15, 13, 15,
    -13, -15, 13, -15, -14, 15, 14, 15,
    -14, -15, 14, -15, -14, 14, 14, 14,
    -14, -14, 14, -14, -15, 14, 15, 14,
    -15, -14, 15, -14, -15, 13, 15, 13,
    -15, -13, 15, -13, -15, 12, 15, 12,
    -15, -12, 15, -12, -15, 11, 15, 11,
    -15, -11, 15, -11, -15, 10, 15, 10,
    -15, -10, 15, -10, -15, 9, 15, 9,
    -15, -9, 15, -9, -15, 8, 15, 8,
    -15, -8, 15, -8, -15, 7, 15, 7,
    -15, -7, 15, -7, -15, 6, 15, 6,
    -15, -6, 15, -6, -15, 5, 15, 5,
    -15, -5, 15, -5, -15, 4, 15, 4,
    -15, -4, 15, -4, -15, 3, 15, 3,
    -15, -3, 15, -3, -15, 2, 15, 2,
    -15, -2, 15, -2, -15, 1, 15, 1,
    -15, -1, 15, -1, -15, 0, 15, 0,
    (char)128,
    0, 16, 0, -16, -1, 16, 1, 16,
    -1, -16, 1, -16, -2, 16, 2, 16,
    -2, -16, 2, -16, -3, 16, 3, 16,
    -3, -16, 3, -16, -4, 16, 4, 16,
    -4, -16, 4, -16, -5, 16, 5, 16,
    -5, -16, 5, -16, -6, 16, 6, 16,
    -6, -16, 6, -16, -7, 16, 7, 16,
    -7, -16, 7, -16, -8, 16, 8, 16,
    -8, -16, 8, -16, -9, 16, 9, 16,
    -9, -16, 9, -16, -10, 16, 10, 16,
    -10, -16, 10, -16, -11, 16, 11, 16,
    -11, -16, 11, -16, -12, 16, 12, 16,
    -12, -16, 12, -16, -13, 16, 13, 16,
    -13, -16, 13, -16, -14, 16, 14, 16,
    -14, -16, 14, -16, -15, 16, 15, 16,
    -15, -16, 15, -16, -15, 15, 15, 15,
    -15, -15, 15, -15, -16, 15, 16, 15,
    -16, -15, 16, -15, -16, 14, 16, 14,
    -16, -14, 16, -14, -16, 13, 16, 13,
    -16, -13, 16, -13, -16, 12, 16, 12,
    -16, -12, 16, -12, -16, 11, 16, 11,
    -16, -11, 16, -11, -16, 10, 16, 10,
    -16, -10, 16, -10, -16, 9, 16, 9,
    -16, -9, 16, -9, -16, 8, 16, 8,
    -16, -8, 16, -8, -16, 7, 16, 7,
    -16, -7, 16, -7, -16, 6, 16, 6,
    -16, -6, 16, -6, -16, 5, 16, 5,
    -16, -5, 16, -5, -16, 4, 16, 4,
    -16, -4, 16, -4, -16, 3, 16, 3,
    -16, -3, 16, -3, -16, 2, 16, 2,
    -16, -2, 16, -2, -16, 1, 16, 1,
    -16, -1, 16, -1, -16, 0, 16, 0,
    (char)136,
    0, 17, 0, -17, -1, 17, 1, 17,
    -1, -17, 1, -17, -2, 17, 2, 17,
    -2, -17, 2, -17, -3, 17, 3, 17,
    -3, -17, 3, -17, -4, 17, 4, 17,
    -4, -17, 4, -17, -5, 17, 5, 17,
    -5, -17, 5, -17, -6, 17, 6, 17,
    -6, -17, 6, -17, -7, 17, 7, 17,
    -7, -17, 7, -17, -8, 17, 8, 17,
    -8, -17, 8, -17, -9, 17, 9, 17,
    -9, -17, 9, -17, -10, 17, 10, 17,
    -10, -17, 10, -17, -11, 17, 11, 17,
    -11, -17, 11, -17, -12, 17, 12, 17,
    -12, -17, 12, -17, -13, 17, 13, 17,
    -13, -17, 13, -17, -14, 17, 14, 17,
    -14, -17, 14, -17, -15, 17, 15, 17,
    -15, -17, 15, -17, -16, 17, 16, 17,
    -16, -17, 16, -17, -16, 16, 16, 16,
    -16, -16, 16, -16, -17, 16, 17, 16,
    -17, -16, 17, -16, -17, 15, 17, 15,
    -17, -15, 17, -15, -17, 14, 17, 14,
    -17, -14, 17, -14, -17, 13, 17, 13,
    -17, -13, 17, -13, -17, 12, 17, 12,
    -17, -12, 17, -12, -17, 11, 17, 11,
    -17, -11, 17, -11, -17, 10, 17, 10,
    -17, -10, 17, -10, -17, 9, 17, 9,
    -17, -9, 17, -9, -17, 8, 17, 8,
    -17, -8, 17, -8, -17, 7, 17, 7,
    -17, -7, 17, -7, -17, 6, 17, 6,
    -17, -6, 17, -6, -17, 5, 17, 5,
    -17, -5, 17, -5, -17, 4, 17, 4,
    -17, -4, 17, -4, -17, 3, 17, 3,
    -17, -3, 17, -3, -17, 2, 17, 2,
    -17, -2, 17, -2, -17, 1, 17, 1,
    -17, -1, 17, -1, -17, 0, 17, 0,
    (char)144,
    0, 18, 0, -18, -1, 18, 1, 18,
    -1, -18, 1, -18, -2, 18, 2, 18,
    -2, -18, 2, -18, -3, 18, 3, 18,
    -3, -18, 3, -18, -4, 18, 4, 18,
    -4, -18, 4, -18, -5, 18, 5, 18,
    -5, -18, 5, -18, -6, 18, 6, 18,
    -6, -18, 6, -18, -7, 18, 7, 18,
    -7, -18, 7, -18, -8, 18, 8, 18,
    -8, -18, 8, -18, -9, 18, 9, 18,
    -9, -18, 9, -18, -10, 18, 10, 18,
    -10, -18, 10, -18, -11, 18, 11, 18,
    -11, -18, 11, -18, -12, 18, 12, 18,
    -12, -18, 12, -18, -13, 18, 13, 18,
    -13, -18, 13, -18, -14, 18, 14, 18,
    -14, -18, 14, -18, -15, 18, 15, 18,
    -15, -18, 15, -18, -16, 18, 16, 18,
    -16, -18, 16, -18, -17, 18, 17, 18,
    -17, -18, 17, -18, -17, 17, 17, 17,
    -17, -17, 17, -17, -18, 17, 18, 17,
    -18, -17, 18, -17, -18, 16, 18, 16,
    -18, -16, 18, -16, -18, 15, 18, 15,
    -18, -15, 18, -15, -18, 14, 18, 14,
    -18, -14, 18, -14, -18, 13, 18, 13,
    -18, -13, 18, -13, -18, 12, 18, 12,
    -18, -12, 18, -12, -18, 11, 18, 11,
    -18, -11, 18, -11, -18, 10, 18, 10,
    -18, -10, 18, -10, -18, 9, 18, 9,
    -18, -9, 18, -9, -18, 8, 18, 8,
    -18, -8, 18, -8, -18, 7, 18, 7,
    -18, -7, 18, -7, -18, 6, 18, 6,
    -18, -6, 18, -6, -18, 5, 18, 5,
    -18, -5, 18, -5, -18, 4, 18, 4,
    -18, -4, 18, -4, -18, 3, 18, 3,
    -18, -3, 18, -3, -18, 2, 18, 2,
    -18, -2, 18, -2, -18, 1, 18, 1,
    -18, -1, 18, -1, -18, 0, 18, 0};
// Unused
// TODO: From Devilution, verify!
char *pCrawlTable[19] = {
    CrawlTable,
    CrawlTable + 3,
    CrawlTable + 12,
    CrawlTable + 45,
    CrawlTable + 94,
    CrawlTable + 159,
    CrawlTable + 240,
    CrawlTable + 337,
    CrawlTable + 450,
    CrawlTable + 579,
    CrawlTable + 724,
    CrawlTable + 885,
    CrawlTable + 1062,
    CrawlTable + 1255,
    CrawlTable + 1464,
    CrawlTable + 1689,
    CrawlTable + 1930,
    CrawlTable + 2187,
    CrawlTable + 2460};
// TODO: From Devilution, verify!
BYTE vCrawlTable[23][30] = {
    {1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 8, 0, 9, 0, 10, 0, 11, 0, 12, 0, 13, 0, 14, 0, 15, 0},
    {1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 8, 1, 9, 1, 10, 1, 11, 1, 12, 1, 13, 1, 14, 1, 15, 1},
    {1, 0, 2, 0, 3, 0, 4, 1, 5, 1, 6, 1, 7, 1, 8, 1, 9, 1, 10, 1, 11, 1, 12, 2, 13, 2, 14, 2, 15, 2},
    {1, 0, 2, 0, 3, 1, 4, 1, 5, 1, 6, 1, 7, 1, 8, 2, 9, 2, 10, 2, 11, 2, 12, 2, 13, 3, 14, 3, 15, 3},
    {1, 0, 2, 1, 3, 1, 4, 1, 5, 1, 6, 2, 7, 2, 8, 2, 9, 3, 10, 3, 11, 3, 12, 3, 13, 4, 14, 4, 0, 0},
    {1, 0, 2, 1, 3, 1, 4, 1, 5, 2, 6, 2, 7, 3, 8, 3, 9, 3, 10, 4, 11, 4, 12, 4, 13, 5, 14, 5, 0, 0},
    {1, 0, 2, 1, 3, 1, 4, 2, 5, 2, 6, 3, 7, 3, 8, 3, 9, 4, 10, 4, 11, 5, 12, 5, 13, 6, 14, 6, 0, 0},
    {1, 1, 2, 1, 3, 2, 4, 2, 5, 3, 6, 3, 7, 4, 8, 4, 9, 5, 10, 5, 11, 6, 12, 6, 13, 7, 0, 0, 0, 0},
    {1, 1, 2, 1, 3, 2, 4, 2, 5, 3, 6, 4, 7, 4, 8, 5, 9, 6, 10, 6, 11, 7, 12, 7, 12, 8, 13, 8, 0, 0},
    {1, 1, 2, 2, 3, 2, 4, 3, 5, 4, 6, 5, 7, 5, 8, 6, 9, 7, 10, 7, 10, 8, 11, 8, 12, 9, 0, 0, 0, 0},
    {1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 5, 7, 6, 8, 7, 9, 8, 10, 9, 11, 9, 11, 10, 0, 0, 0, 0, 0, 0},
    {1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 9, 11, 10, 11, 0, 0, 0, 0, 0, 0},
    {1, 1, 2, 2, 2, 3, 3, 4, 4, 5, 5, 6, 5, 7, 6, 8, 7, 9, 7, 10, 8, 10, 8, 11, 9, 12, 0, 0, 0, 0},
    {1, 1, 1, 2, 2, 3, 2, 4, 3, 5, 4, 6, 4, 7, 5, 8, 6, 9, 6, 10, 7, 11, 7, 12, 8, 12, 8, 13, 0, 0},
    {1, 1, 1, 2, 2, 3, 2, 4, 3, 5, 3, 6, 4, 7, 4, 8, 5, 9, 5, 10, 6, 11, 6, 12, 7, 13, 0, 0, 0, 0},
    {0, 1, 1, 2, 1, 3, 2, 4, 2, 5, 3, 6, 3, 7, 3, 8, 4, 9, 4, 10, 5, 11, 5, 12, 6, 13, 6, 14, 0, 0},
    {0, 1, 1, 2, 1, 3, 1, 4, 2, 5, 2, 6, 3, 7, 3, 8, 3, 9, 4, 10, 4, 11, 4, 12, 5, 13, 5, 14, 0, 0},
    {0, 1, 1, 2, 1, 3, 1, 4, 1, 5, 2, 6, 2, 7, 2, 8, 3, 9, 3, 10, 3, 11, 3, 12, 4, 13, 4, 14, 0, 0},
    {0, 1, 0, 2, 1, 3, 1, 4, 1, 5, 1, 6, 1, 7, 2, 8, 2, 9, 2, 10, 2, 11, 2, 12, 3, 13, 3, 14, 3, 15},
    {0, 1, 0, 2, 0, 3, 1, 4, 1, 5, 1, 6, 1, 7, 1, 8, 1, 9, 1, 10, 1, 11, 2, 12, 2, 13, 2, 14, 2, 15},
    {0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 1, 8, 1, 9, 1, 10, 1, 11, 1, 12, 1, 13, 1, 14, 1, 15},
    {0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 8, 0, 9, 0, 10, 0, 11, 0, 12, 0, 13, 0, 14, 0, 15}};
// Unused
BYTE byte_4AD9C8[18][18] = {
    {0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
    {0, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
    {0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
    {0, 0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
    {0, 0, 1, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
    {0, 0, 1, 1, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
    {0, 0, 0, 1, 1, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
    {0, 0, 0, 1, 1, 1, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
    {0, 0, 0, 0, 1, 1, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3},
    {0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3},
    {0, 0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3},
    {0, 0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3},
    {0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3},
    {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3},
    {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3},
    {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3},
    {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3},
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2}};
// TODO: From Devilution, verify!
BYTE RadiusAdj[23] = {0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 4, 3, 2, 2, 2, 1, 1, 1, 0, 0, 0, 0};

// SetLightFX    000000000042E530

// DoLighting    000000000042E5EF
void DoLighting(int nXPos, int nYPos, int nRadius, int Lnum)
{
    // TODO
}

// DoUnLight    000000000042EB01
// DoUnVision    000000000042EBF3
// DoVision    000000000042ECE8

// .text:0042F267
void MakeLightTable()
{
    // TODO
}

// ResetLight    000000000042F8E5
// ToggleLight    000000000042FA32
// InitLightMax    000000000042FB92
// InitLighting    000000000042FBC2
// AddLight    000000000042FBFA
int AddLight(int x, int y, int r)
{
    // TODO
    return 0;
}

// AddUnLight    000000000042FCF9
void AddUnLight(int i)
{
    // TODO
}

// ChangeLightRadius    000000000042FD84
// ChangeLightXY    000000000042FE70
// ChangeLightOff    000000000042FF6D
// ChangeLight    000000000043006A
// ProcessLightList    0000000000430178
// SavePreLighting    0000000000430362
// InitVision    00000000004303DF
// AddVision    00000000004304FD
// AddUnVision    00000000004305B8
// ChangeVisionRadius    0000000000430622
// ChangeVisionXY    00000000004306ED
// ChangeVision    00000000004307CA
// ProcessVisionList    00000000004308B7
