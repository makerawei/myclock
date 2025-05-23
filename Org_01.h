#pragma once
#include <Adafruit_GFX.h>

// Org_v01 by Orgdot (www.orgdot.com/aliasfonts).  A tiny,
// stylized font with all characters within a 6 pixel height.

const uint8_t Org_01Bitmaps[] PROGMEM = {
  0xE8, 0xE8, 0xA0, 0x57, 0xD5, 0xF5, 0x00, 0xFD, 0x3E, 0x5F, 0x80, 0x88, 
  0x88, 0x88, 0x80, 0xF4, 0xBF, 0x2E, 0x80, 0x80, 0x6A, 0x40, 0x95, 0x80, 
  0xAA, 0x80, 0x5D, 0x00, 0xC0, 0xF0, 0x80, 0x08, 0x88, 0x88, 0x00, 0xFC, 
  0x63, 0x1F, 0x80, 0x21, 0x08, 0x42, 0x00, 0xF8, 0x7F, 0x0F, 0x80, 0xF8, 
  0x7E, 0x1F, 0x80, 0x8C, 0x7E, 0x10, 0x80, 0xFC, 0x3E, 0x1F, 0x80, 0xFC, 
  0x3F, 0x1F, 0x80, 0xF8, 0x42, 0x10, 0x80, 0xFC, 0x7F, 0x1F, 0x80, 0xFC, 
  0x7E, 0x1F, 0x80, 0x90, 0xB0, 0x2A, 0x22, 0xF0, 0xF0, 0x88, 0xA8, 0xF8, 
  0x4E, 0x02, 0x00, 0xFD, 0x6F, 0x0F, 0x80, 0xFC, 0x7F, 0x18, 0x80, 0xF4, 
  0x7D, 0x1F, 0x00, 0xFC, 0x21, 0x0F, 0x80, 0xF4, 0x63, 0x1F, 0x00, 0xFC, 
  0x3F, 0x0F, 0x80, 0xFC, 0x3F, 0x08, 0x00, 0xFC, 0x2F, 0x1F, 0x80, 0x8C, 
  0x7F, 0x18, 0x80, 0xF9, 0x08, 0x4F, 0x80, 0x78, 0x85, 0x2F, 0x80, 0x8D, 
  0xB1, 0x68, 0x80, 0x84, 0x21, 0x0F, 0x80, 0xFD, 0x6B, 0x5A, 0x80, 0xFC, 
  0x63, 0x18, 0x80, 0xFC, 0x63, 0x1F, 0x80, 0xFC, 0x7F, 0x08, 0x00, 0xFC, 
  0x63, 0x3F, 0x80, 0xFC, 0x7F, 0x29, 0x00, 0xFC, 0x3E, 0x1F, 0x80, 0xF9, 
  0x08, 0x42, 0x00, 0x8C, 0x63, 0x1F, 0x80, 0x8C, 0x62, 0xA2, 0x00, 0xAD, 
  0x6B, 0x5F, 0x80, 0x8A, 0x88, 0xA8, 0x80, 0x8C, 0x54, 0x42, 0x00, 0xF8, 
  0x7F, 0x0F, 0x80, 0xEA, 0xC0, 0x82, 0x08, 0x20, 0x80, 0xD5, 0xC0, 0x54, 
  0xF8, 0x80, 0xF1, 0xFF, 0x8F, 0x99, 0xF0, 0xF8, 0x8F, 0x1F, 0x99, 0xF0, 
  0xFF, 0x8F, 0x6B, 0xA4, 0xF9, 0x9F, 0x10, 0x8F, 0x99, 0x90, 0xF0, 0x55, 
  0xC0, 0x8A, 0xF9, 0x90, 0xF8, 0xFD, 0x63, 0x10, 0xF9, 0x99, 0xF9, 0x9F, 
  0xF9, 0x9F, 0x80, 0xF9, 0x9F, 0x20, 0xF8, 0x88, 0x47, 0x1F, 0x27, 0xC8, 
  0x42, 0x00, 0x99, 0x9F, 0x99, 0x97, 0x8C, 0x6B, 0xF0, 0x96, 0x69, 0x99, 
  0x9F, 0x10, 0x2E, 0x8F, 0x2B, 0x22, 0xF8, 0x89, 0xA8, 0x0F, 0xE0
};

const GFXglyph Org_01Glyphs[] PROGMEM = {
  {     0,   0,   0,   6,    0,    1 },   // 0x20 ' '
  {     1,   1,   5,   2,    0,   -4 },   // 0x21 '!'
  {     2,   3,   1,   4,    0,   -4 },   // 0x22 '"'
  {     3,   5,   5,   6,    0,   -4 },   // 0x23 '#'
  {     7,   5,   5,   6,    0,   -4 },   // 0x24 '$'
  {    11,   5,   5,   6,    0,   -4 },   // 0x25 '%'
  {    15,   5,   5,   6,    0,   -4 },   // 0x26 '&'
  {    19,   1,   1,   2,    0,   -4 },   // 0x27 '''
  {    20,   2,   5,   3,    0,   -4 },   // 0x28 '('
  {    22,   2,   5,   3,    0,   -4 },   // 0x29 ')'
  {    24,   3,   3,   4,    0,   -3 },   // 0x2A '*'
  {    26,   3,   3,   4,    0,   -3 },   // 0x2B '+'
  {    28,   1,   2,   2,    0,    0 },   // 0x2C ','
  {    29,   4,   1,   5,    0,   -2 },   // 0x2D '-'
  {    30,   1,   1,   2,    0,    0 },   // 0x2E '.'
  {    31,   5,   5,   6,    0,   -4 },   // 0x2F '/'
  {    35,   5,   5,   6,    0,   -4 },   // 0x30 '0'
  {    39,   5,   5,   6,    0,   -4 },   // 0x31 '1'
  {    43,   5,   5,   6,    0,   -4 },   // 0x32 '2'
  {    47,   5,   5,   6,    0,   -4 },   // 0x33 '3'
  {    51,   5,   5,   6,    0,   -4 },   // 0x34 '4'
  {    55,   5,   5,   6,    0,   -4 },   // 0x35 '5'
  {    59,   5,   5,   6,    0,   -4 },   // 0x36 '6'
  {    63,   5,   5,   6,    0,   -4 },   // 0x37 '7'
  {    67,   5,   5,   6,    0,   -4 },   // 0x38 '8'
  {    71,   5,   5,   6,    0,   -4 },   // 0x39 '9'
  {    75,   1,   4,   5,    2,   -3 },   // 0x3A ':'
  {    76,   1,   4,   2,    0,   -3 },   // 0x3B ';'
  {    77,   3,   5,   4,    0,   -4 },   // 0x3C '<'
  {    79,   4,   3,   5,    0,   -3 },   // 0x3D '='
  {    81,   3,   5,   4,    0,   -4 },   // 0x3E '>'
  {    83,   5,   5,   6,    0,   -4 },   // 0x3F '?'
  {    87,   5,   5,   6,    0,   -4 },   // 0x40 '@'
  {    91,   5,   5,   6,    0,   -4 },   // 0x41 'A'
  {    95,   5,   5,   6,    0,   -4 },   // 0x42 'B'
  {    99,   5,   5,   6,    0,   -4 },   // 0x43 'C'
  {   103,   5,   5,   6,    0,   -4 },   // 0x44 'D'
  {   107,   5,   5,   6,    0,   -4 },   // 0x45 'E'
  {   111,   5,   5,   6,    0,   -4 },   // 0x46 'F'
  {   115,   5,   5,   6,    0,   -4 },   // 0x47 'G'
  {   119,   5,   5,   6,    0,   -4 },   // 0x48 'H'
  {   123,   5,   5,   6,    0,   -4 },   // 0x49 'I'
  {   127,   5,   5,   6,    0,   -4 },   // 0x4A 'J'
  {   131,   5,   5,   6,    0,   -4 },   // 0x4B 'K'
  {   135,   5,   5,   6,    0,   -4 },   // 0x4C 'L'
  {   139,   5,   5,   6,    0,   -4 },   // 0x4D 'M'
  {   143,   5,   5,   6,    0,   -4 },   // 0x4E 'N'
  {   147,   5,   5,   6,    0,   -4 },   // 0x4F 'O'
  {   151,   5,   5,   6,    0,   -4 },   // 0x50 'P'
  {   155,   5,   5,   6,    0,   -4 },   // 0x51 'Q'
  {   159,   5,   5,   6,    0,   -4 },   // 0x52 'R'
  {   163,   5,   5,   6,    0,   -4 },   // 0x53 'S'
  {   167,   5,   5,   6,    0,   -4 },   // 0x54 'T'
  {   171,   5,   5,   6,    0,   -4 },   // 0x55 'U'
  {   175,   5,   5,   6,    0,   -4 },   // 0x56 'V'
  {   179,   5,   5,   6,    0,   -4 },   // 0x57 'W'
  {   183,   5,   5,   6,    0,   -4 },   // 0x58 'X'
  {   187,   5,   5,   6,    0,   -4 },   // 0x59 'Y'
  {   191,   5,   5,   6,    0,   -4 },   // 0x5A 'Z'
  {   195,   2,   5,   3,    0,   -4 },   // 0x5B '['
  {   197,   5,   5,   6,    0,   -4 },   // 0x5C '\'
  {   201,   2,   5,   3,    0,   -4 },   // 0x5D ']'
  {   203,   3,   2,   4,    0,   -4 },   // 0x5E '^'
  {   204,   5,   1,   6,    0,    1 },   // 0x5F '_'
  {   205,   1,   1,   2,    0,   -4 },   // 0x60 '`'
  {   206,   4,   4,   5,    0,   -3 },   // 0x61 'a'
  {   208,   4,   5,   5,    0,   -4 },   // 0x62 'b'
  {   211,   4,   4,   5,    0,   -3 },   // 0x63 'c'
  {   213,   4,   5,   5,    0,   -4 },   // 0x64 'd'
  {   216,   4,   4,   5,    0,   -3 },   // 0x65 'e'
  {   218,   3,   5,   4,    0,   -4 },   // 0x66 'f'
  {   220,   4,   5,   5,    0,   -3 },   // 0x67 'g'
  {   223,   4,   5,   5,    0,   -4 },   // 0x68 'h'
  {   226,   1,   4,   2,    0,   -3 },   // 0x69 'i'
  {   227,   2,   5,   3,    0,   -3 },   // 0x6A 'j'
  {   229,   4,   5,   5,    0,   -4 },   // 0x6B 'k'
  {   232,   1,   5,   2,    0,   -4 },   // 0x6C 'l'
  {   233,   5,   4,   6,    0,   -3 },   // 0x6D 'm'
  {   236,   4,   4,   5,    0,   -3 },   // 0x6E 'n'
  {   238,   4,   4,   5,    0,   -3 },   // 0x6F 'o'
  {   240,   4,   5,   5,    0,   -3 },   // 0x70 'p'
  {   243,   4,   5,   5,    0,   -3 },   // 0x71 'q'
  {   246,   4,   4,   5,    0,   -3 },   // 0x72 'r'
  {   248,   4,   4,   5,    0,   -3 },   // 0x73 's'
  {   250,   5,   5,   6,    0,   -4 },   // 0x74 't'
  {   254,   4,   4,   5,    0,   -3 },   // 0x75 'u'
  {   256,   4,   4,   5,    0,   -3 },   // 0x76 'v'
  {   258,   5,   4,   6,    0,   -3 },   // 0x77 'w'
  {   261,   4,   4,   5,    0,   -3 },   // 0x78 'x'
  {   263,   4,   5,   5,    0,   -3 },   // 0x79 'y'
  {   266,   4,   4,   5,    0,   -3 },   // 0x7A 'z'
  {   268,   3,   5,   4,    0,   -4 },   // 0x7B '{'
  {   270,   1,   5,   2,    0,   -4 },   // 0x7C '|'
  {   271,   3,   5,   4,    0,   -4 },   // 0x7D '}'
  {   273,   5,   3,   6,    0,   -3 }    // 0x7E '~'
};

const GFXfont Org_01 PROGMEM = {(uint8_t *)Org_01Bitmaps,  
                                (GFXglyph *)Org_01Glyphs, 0x20, 0x7E,   7};

// Approx. 943 bytes
