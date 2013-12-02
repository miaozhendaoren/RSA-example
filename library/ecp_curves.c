/*
 *  Elliptic curves over GF(p): curve-specific data and functions
 *
 *  Copyright (C) 2006-2013, Brainspark B.V.
 *
 *  This file is part of PolarSSL (http://www.polarssl.org)
 *  Lead Maintainer: Paul Bakker <polarssl_maintainer at polarssl.org>
 *
 *  All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "polarssl/config.h"

#if defined(POLARSSL_ECP_C)

#include "polarssl/ecp.h"

#if defined(_MSC_VER) && !defined(inline)
#define inline _inline
#else
#if defined(__ARMCC_VERSION) && !defined(inline)
#define inline __inline
#endif /* __ARMCC_VERSION */
#endif /*_MSC_VER */

/*
 * Domain parameters for secp192r1
 */
static unsigned char secp192r1_p[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
static unsigned char *secp192r1_a = NULL;
static unsigned char secp192r1_b[] = {
    0x64, 0x21, 0x05, 0x19, 0xE5, 0x9C, 0x80, 0xE7, 0x0F, 0xA7, 0xE9, 0xAB,
    0x72, 0x24, 0x30, 0x49, 0xFE, 0xB8, 0xDE, 0xEC, 0xC1, 0x46, 0xB9, 0xB1 };
static unsigned char secp192r1_gx[] = {
    0x18, 0x8D, 0xA8, 0x0E, 0xB0, 0x30, 0x90, 0xF6, 0x7C, 0xBF, 0x20, 0xEB,
    0x43, 0xA1, 0x88, 0x00, 0xF4, 0xFF, 0x0A, 0xFD, 0x82, 0xFF, 0x10, 0x12 };
static unsigned char secp192r1_gy[] = {
    0x07, 0x19, 0x2B, 0x95, 0xFF, 0xC8, 0xDA, 0x78, 0x63, 0x10, 0x11, 0xED,
    0x6B, 0x24, 0xCD, 0xD5, 0x73, 0xF9, 0x77, 0xA1, 0x1E, 0x79, 0x48, 0x11 };
static unsigned char secp192r1_n[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x99, 0xDE, 0xF8, 0x36, 0x14, 0x6B, 0xC9, 0xB1, 0xB4, 0xD2, 0x28, 0x31 };

/*
 * Domain parameters for secp224r1
 */
static unsigned char secp224r1_p[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01 };
static unsigned char *secp224r1_a = NULL;
static unsigned char secp224r1_b[] = {
    0xB4, 0x05, 0x0A, 0x85, 0x0C, 0x04, 0xB3, 0xAB, 0xF5, 0x41, 0x32, 0x56,
    0x50, 0x44, 0xB0, 0xB7, 0xD7, 0xBF, 0xD8, 0xBA, 0x27, 0x0B, 0x39, 0x43,
    0x23, 0x55, 0xFF, 0xB4 };
static unsigned char secp224r1_gx[] = {
    0xB7, 0x0E, 0x0C, 0xBD, 0x6B, 0xB4, 0xBF, 0x7F, 0x32, 0x13, 0x90, 0xB9,
    0x4A, 0x03, 0xC1, 0xD3, 0x56, 0xC2, 0x11, 0x22, 0x34, 0x32, 0x80, 0xD6,
    0x11, 0x5C, 0x1D, 0x21 };
static unsigned char secp224r1_gy[] = {
    0xBD, 0x37, 0x63, 0x88, 0xB5, 0xF7, 0x23, 0xFB, 0x4C, 0x22, 0xDF, 0xE6,
    0xCD, 0x43, 0x75, 0xA0, 0x5A, 0x07, 0x47, 0x64, 0x44, 0xD5, 0x81, 0x99,
    0x85, 0x00, 0x7E, 0x34 };
static unsigned char secp224r1_n[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0x16, 0xA2, 0xE0, 0xB8, 0xF0, 0x3E, 0x13, 0xDD, 0x29, 0x45,
    0x5C, 0x5C, 0x2A, 0x3D };

/*
 * Domain parameters for secp256r1
 */
static unsigned char secp256r1_p[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
static unsigned char *secp256r1_a = NULL;
static unsigned char secp256r1_b[] = {
    0x5A, 0xC6, 0x35, 0xD8, 0xAA, 0x3A, 0x93, 0xE7, 0xB3, 0xEB, 0xBD, 0x55,
    0x76, 0x98, 0x86, 0xBC, 0x65, 0x1D, 0x06, 0xB0, 0xCC, 0x53, 0xB0, 0xF6,
    0x3B, 0xCE, 0x3C, 0x3E, 0x27, 0xD2, 0x60, 0x4B };
static unsigned char secp256r1_gx[] = {
    0x6B, 0x17, 0xD1, 0xF2, 0xE1, 0x2C, 0x42, 0x47, 0xF8, 0xBC, 0xE6, 0xE5,
    0x63, 0xA4, 0x40, 0xF2, 0x77, 0x03, 0x7D, 0x81, 0x2D, 0xEB, 0x33, 0xA0,
    0xF4, 0xA1, 0x39, 0x45, 0xD8, 0x98, 0xC2, 0x96 };
static unsigned char secp256r1_gy[] = {
    0x4F, 0xE3, 0x42, 0xE2, 0xFE, 0x1A, 0x7F, 0x9B, 0x8E, 0xE7, 0xEB, 0x4A,
    0x7C, 0x0F, 0x9E, 0x16, 0x2B, 0xCE, 0x33, 0x57, 0x6B, 0x31, 0x5E, 0xCE,
    0xCB, 0xB6, 0x40, 0x68, 0x37, 0xBF, 0x51, 0xF5 };
static unsigned char secp256r1_n[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xBC, 0xE6, 0xFA, 0xAD, 0xA7, 0x17, 0x9E, 0x84,
    0xF3, 0xB9, 0xCA, 0xC2, 0xFC, 0x63, 0x25, 0x51 };

/*
 * Domain parameters for secp384r1
 */
static unsigned char secp384r1_p[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF };
static unsigned char *secp384r1_a = NULL;
static unsigned char secp384r1_b[] = {
    0xB3, 0x31, 0x2F, 0xA7, 0xE2, 0x3E, 0xE7, 0xE4, 0x98, 0x8E, 0x05, 0x6B,
    0xE3, 0xF8, 0x2D, 0x19, 0x18, 0x1D, 0x9C, 0x6E, 0xFE, 0x81, 0x41, 0x12,
    0x03, 0x14, 0x08, 0x8F, 0x50, 0x13, 0x87, 0x5A, 0xC6, 0x56, 0x39, 0x8D,
    0x8A, 0x2E, 0xD1, 0x9D, 0x2A, 0x85, 0xC8, 0xED, 0xD3, 0xEC, 0x2A, 0xEF };
static unsigned char secp384r1_gx[] = {
    0xAA, 0x87, 0xCA, 0x22, 0xBE, 0x8B, 0x05, 0x37, 0x8E, 0xB1, 0xC7, 0x1E,
    0xF3, 0x20, 0xAD, 0x74, 0x6E, 0x1D, 0x3B, 0x62, 0x8B, 0xA7, 0x9B, 0x98,
    0x59, 0xF7, 0x41, 0xE0, 0x82, 0x54, 0x2A, 0x38, 0x55, 0x02, 0xF2, 0x5D,
    0xBF, 0x55, 0x29, 0x6C, 0x3A, 0x54, 0x5E, 0x38, 0x72, 0x76, 0x0A, 0xB7 };
static unsigned char secp384r1_gy[] = {
    0x36, 0x17, 0xDE, 0x4A, 0x96, 0x26, 0x2C, 0x6F, 0x5D, 0x9E, 0x98, 0xBF,
    0x92, 0x92, 0xDC, 0x29, 0xF8, 0xF4, 0x1D, 0xBD, 0x28, 0x9A, 0x14, 0x7C,
    0xE9, 0xDA, 0x31, 0x13, 0xB5, 0xF0, 0xB8, 0xC0, 0x0A, 0x60, 0xB1, 0xCE,
    0x1D, 0x7E, 0x81, 0x9D, 0x7A, 0x43, 0x1D, 0x7C, 0x90, 0xEA, 0x0E, 0x5F };
static unsigned char secp384r1_n[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xC7, 0x63, 0x4D, 0x81, 0xF4, 0x37, 0x2D, 0xDF, 0x58, 0x1A, 0x0D, 0xB2,
    0x48, 0xB0, 0xA7, 0x7A, 0xEC, 0xEC, 0x19, 0x6A, 0xCC, 0xC5, 0x29, 0x73 };

/*
 * Domain parameters for secp521r1
 */
static unsigned char secp521r1_p[] = {
    0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
static unsigned char *secp521r1_a = NULL;
static unsigned char secp521r1_b[] = {
    0x00, 0x51, 0x95, 0x3E, 0xB9, 0x61, 0x8E, 0x1C, 0x9A, 0x1F, 0x92, 0x9A,
    0x21, 0xA0, 0xB6, 0x85, 0x40, 0xEE, 0xA2, 0xDA, 0x72, 0x5B, 0x99, 0xB3,
    0x15, 0xF3, 0xB8, 0xB4, 0x89, 0x91, 0x8E, 0xF1, 0x09, 0xE1, 0x56, 0x19,
    0x39, 0x51, 0xEC, 0x7E, 0x93, 0x7B, 0x16, 0x52, 0xC0, 0xBD, 0x3B, 0xB1,
    0xBF, 0x07, 0x35, 0x73, 0xDF, 0x88, 0x3D, 0x2C, 0x34, 0xF1, 0xEF, 0x45,
    0x1F, 0xD4, 0x6B, 0x50, 0x3F, 0x00 };
static unsigned char secp521r1_gx[] = {
    0x00, 0xC6, 0x85, 0x8E, 0x06, 0xB7, 0x04, 0x04, 0xE9, 0xCD, 0x9E, 0x3E,
    0xCB, 0x66, 0x23, 0x95, 0xB4, 0x42, 0x9C, 0x64, 0x81, 0x39, 0x05, 0x3F,
    0xB5, 0x21, 0xF8, 0x28, 0xAF, 0x60, 0x6B, 0x4D, 0x3D, 0xBA, 0xA1, 0x4B,
    0x5E, 0x77, 0xEF, 0xE7, 0x59, 0x28, 0xFE, 0x1D, 0xC1, 0x27, 0xA2, 0xFF,
    0xA8, 0xDE, 0x33, 0x48, 0xB3, 0xC1, 0x85, 0x6A, 0x42, 0x9B, 0xF9, 0x7E,
    0x7E, 0x31, 0xC2, 0xE5, 0xBD, 0x66 };
static unsigned char secp521r1_gy[] = {
    0x01, 0x18, 0x39, 0x29, 0x6A, 0x78, 0x9A, 0x3B, 0xC0, 0x04, 0x5C, 0x8A,
    0x5F, 0xB4, 0x2C, 0x7D, 0x1B, 0xD9, 0x98, 0xF5, 0x44, 0x49, 0x57, 0x9B,
    0x44, 0x68, 0x17, 0xAF, 0xBD, 0x17, 0x27, 0x3E, 0x66, 0x2C, 0x97, 0xEE,
    0x72, 0x99, 0x5E, 0xF4, 0x26, 0x40, 0xC5, 0x50, 0xB9, 0x01, 0x3F, 0xAD,
    0x07, 0x61, 0x35, 0x3C, 0x70, 0x86, 0xA2, 0x72, 0xC2, 0x40, 0x88, 0xBE,
    0x94, 0x76, 0x9F, 0xD1, 0x66, 0x50 };
static unsigned char secp521r1_n[] = {
    0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFA, 0x51, 0x86,
    0x87, 0x83, 0xBF, 0x2F, 0x96, 0x6B, 0x7F, 0xCC, 0x01, 0x48, 0xF7, 0x09,
    0xA5, 0xD0, 0x3B, 0xB5, 0xC9, 0xB8, 0x89, 0x9C, 0x47, 0xAE, 0xBB, 0x6F,
    0xB7, 0x1E, 0x91, 0x38, 0x64, 0x09 };

/*
 * Domain parameters for brainpoolP256r1 (RFC 5639 3.4)
 */
static unsigned char brainpoolP256r1_p[] = {
    0xA9, 0xFB, 0x57, 0xDB, 0xA1, 0xEE, 0xA9, 0xBC, 0x3E, 0x66, 0x0A, 0x90,
    0x9D, 0x83, 0x8D, 0x72, 0x6E, 0x3B, 0xF6, 0x23, 0xD5, 0x26, 0x20, 0x28,
    0x20, 0x13, 0x48, 0x1D, 0x1F, 0x6E, 0x53, 0x77 };
static unsigned char brainpoolP256r1_a[] = {
    0x7D, 0x5A, 0x09, 0x75, 0xFC, 0x2C, 0x30, 0x57, 0xEE, 0xF6, 0x75, 0x30,
    0x41, 0x7A, 0xFF, 0xE7, 0xFB, 0x80, 0x55, 0xC1, 0x26, 0xDC, 0x5C, 0x6C,
    0xE9, 0x4A, 0x4B, 0x44, 0xF3, 0x30, 0xB5, 0xD9 };
static unsigned char brainpoolP256r1_b[] = {
    0x26, 0xDC, 0x5C, 0x6C, 0xE9, 0x4A, 0x4B, 0x44, 0xF3, 0x30, 0xB5, 0xD9,
    0xBB, 0xD7, 0x7C, 0xBF, 0x95, 0x84, 0x16, 0x29, 0x5C, 0xF7, 0xE1, 0xCE,
    0x6B, 0xCC, 0xDC, 0x18, 0xFF, 0x8C, 0x07, 0xB6 };
static unsigned char brainpoolP256r1_gx[] = {
    0x8B, 0xD2, 0xAE, 0xB9, 0xCB, 0x7E, 0x57, 0xCB, 0x2C, 0x4B, 0x48, 0x2F,
    0xFC, 0x81, 0xB7, 0xAF, 0xB9, 0xDE, 0x27, 0xE1, 0xE3, 0xBD, 0x23, 0xC2,
    0x3A, 0x44, 0x53, 0xBD, 0x9A, 0xCE, 0x32, 0x62 };
static unsigned char brainpoolP256r1_gy[] = {
    0x54, 0x7E, 0xF8, 0x35, 0xC3, 0xDA, 0xC4, 0xFD, 0x97, 0xF8, 0x46, 0x1A,
    0x14, 0x61, 0x1D, 0xC9, 0xC2, 0x77, 0x45, 0x13, 0x2D, 0xED, 0x8E, 0x54,
    0x5C, 0x1D, 0x54, 0xC7, 0x2F, 0x04, 0x69, 0x97 };
static unsigned char brainpoolP256r1_n[] = {
    0xA9, 0xFB, 0x57, 0xDB, 0xA1, 0xEE, 0xA9, 0xBC, 0x3E, 0x66, 0x0A, 0x90,
    0x9D, 0x83, 0x8D, 0x71, 0x8C, 0x39, 0x7A, 0xA3, 0xB5, 0x61, 0xA6, 0xF7,
    0x90, 0x1E, 0x0E, 0x82, 0x97, 0x48, 0x56, 0xA7 };

/*
 * Domain parameters for brainpoolP384r1 (RFC 5639 3.6)
 */
static unsigned char brainpoolP384r1_p[] = {
    0x8C, 0xB9, 0x1E, 0x82, 0xA3, 0x38, 0x6D, 0x28, 0x0F, 0x5D, 0x6F, 0x7E,
    0x50, 0xE6, 0x41, 0xDF, 0x15, 0x2F, 0x71, 0x09, 0xED, 0x54, 0x56, 0xB4,
    0x12, 0xB1, 0xDA, 0x19, 0x7F, 0xB7, 0x11, 0x23, 0xAC, 0xD3, 0xA7, 0x29,
    0x90, 0x1D, 0x1A, 0x71, 0x87, 0x47, 0x00, 0x13, 0x31, 0x07, 0xEC, 0x53 };
static unsigned char brainpoolP384r1_a[] = {
    0x7B, 0xC3, 0x82, 0xC6, 0x3D, 0x8C, 0x15, 0x0C, 0x3C, 0x72, 0x08, 0x0A,
    0xCE, 0x05, 0xAF, 0xA0, 0xC2, 0xBE, 0xA2, 0x8E, 0x4F, 0xB2, 0x27, 0x87,
    0x13, 0x91, 0x65, 0xEF, 0xBA, 0x91, 0xF9, 0x0F, 0x8A, 0xA5, 0x81, 0x4A,
    0x50, 0x3A, 0xD4, 0xEB, 0x04, 0xA8, 0xC7, 0xDD, 0x22, 0xCE, 0x28, 0x26 };
static unsigned char brainpoolP384r1_b[] = {
    0x04, 0xA8, 0xC7, 0xDD, 0x22, 0xCE, 0x28, 0x26, 0x8B, 0x39, 0xB5, 0x54,
    0x16, 0xF0, 0x44, 0x7C, 0x2F, 0xB7, 0x7D, 0xE1, 0x07, 0xDC, 0xD2, 0xA6,
    0x2E, 0x88, 0x0E, 0xA5, 0x3E, 0xEB, 0x62, 0xD5, 0x7C, 0xB4, 0x39, 0x02,
    0x95, 0xDB, 0xC9, 0x94, 0x3A, 0xB7, 0x86, 0x96, 0xFA, 0x50, 0x4C, 0x11 };
static unsigned char brainpoolP384r1_gx[] = {
    0x1D, 0x1C, 0x64, 0xF0, 0x68, 0xCF, 0x45, 0xFF, 0xA2, 0xA6, 0x3A, 0x81,
    0xB7, 0xC1, 0x3F, 0x6B, 0x88, 0x47, 0xA3, 0xE7, 0x7E, 0xF1, 0x4F, 0xE3,
    0xDB, 0x7F, 0xCA, 0xFE, 0x0C, 0xBD, 0x10, 0xE8, 0xE8, 0x26, 0xE0, 0x34,
    0x36, 0xD6, 0x46, 0xAA, 0xEF, 0x87, 0xB2, 0xE2, 0x47, 0xD4, 0xAF, 0x1E };
static unsigned char brainpoolP384r1_gy[] = {
    0x8A, 0xBE, 0x1D, 0x75, 0x20, 0xF9, 0xC2, 0xA4, 0x5C, 0xB1, 0xEB, 0x8E,
    0x95, 0xCF, 0xD5, 0x52, 0x62, 0xB7, 0x0B, 0x29, 0xFE, 0xEC, 0x58, 0x64,
    0xE1, 0x9C, 0x05, 0x4F, 0xF9, 0x91, 0x29, 0x28, 0x0E, 0x46, 0x46, 0x21,
    0x77, 0x91, 0x81, 0x11, 0x42, 0x82, 0x03, 0x41, 0x26, 0x3C, 0x53, 0x15 };
static unsigned char brainpoolP384r1_n[] = {
    0x8C, 0xB9, 0x1E, 0x82, 0xA3, 0x38, 0x6D, 0x28, 0x0F, 0x5D, 0x6F, 0x7E,
    0x50, 0xE6, 0x41, 0xDF, 0x15, 0x2F, 0x71, 0x09, 0xED, 0x54, 0x56, 0xB3,
    0x1F, 0x16, 0x6E, 0x6C, 0xAC, 0x04, 0x25, 0xA7, 0xCF, 0x3A, 0xB6, 0xAF,
    0x6B, 0x7F, 0xC3, 0x10, 0x3B, 0x88, 0x32, 0x02, 0xE9, 0x04, 0x65, 0x65 };

/*
 * Domain parameters for brainpoolP512r1 (RFC 5639 3.7)
 */
static unsigned char brainpoolP512r1_p[] = {
    0xAA, 0xDD, 0x9D, 0xB8, 0xDB, 0xE9, 0xC4, 0x8B, 0x3F, 0xD4, 0xE6, 0xAE,
    0x33, 0xC9, 0xFC, 0x07, 0xCB, 0x30, 0x8D, 0xB3, 0xB3, 0xC9, 0xD2, 0x0E,
    0xD6, 0x63, 0x9C, 0xCA, 0x70, 0x33, 0x08, 0x71, 0x7D, 0x4D, 0x9B, 0x00,
    0x9B, 0xC6, 0x68, 0x42, 0xAE, 0xCD, 0xA1, 0x2A, 0xE6, 0xA3, 0x80, 0xE6,
    0x28, 0x81, 0xFF, 0x2F, 0x2D, 0x82, 0xC6, 0x85, 0x28, 0xAA, 0x60, 0x56,
    0x58, 0x3A, 0x48, 0xF3 };
static unsigned char brainpoolP512r1_a[] = {
    0x78, 0x30, 0xA3, 0x31, 0x8B, 0x60, 0x3B, 0x89, 0xE2, 0x32, 0x71, 0x45,
    0xAC, 0x23, 0x4C, 0xC5, 0x94, 0xCB, 0xDD, 0x8D, 0x3D, 0xF9, 0x16, 0x10,
    0xA8, 0x34, 0x41, 0xCA, 0xEA, 0x98, 0x63, 0xBC, 0x2D, 0xED, 0x5D, 0x5A,
    0xA8, 0x25, 0x3A, 0xA1, 0x0A, 0x2E, 0xF1, 0xC9, 0x8B, 0x9A, 0xC8, 0xB5,
    0x7F, 0x11, 0x17, 0xA7, 0x2B, 0xF2, 0xC7, 0xB9, 0xE7, 0xC1, 0xAC, 0x4D,
    0x77, 0xFC, 0x94, 0xCA };
static unsigned char brainpoolP512r1_b[] = {
    0x3D, 0xF9, 0x16, 0x10, 0xA8, 0x34, 0x41, 0xCA, 0xEA, 0x98, 0x63, 0xBC,
    0x2D, 0xED, 0x5D, 0x5A, 0xA8, 0x25, 0x3A, 0xA1, 0x0A, 0x2E, 0xF1, 0xC9,
    0x8B, 0x9A, 0xC8, 0xB5, 0x7F, 0x11, 0x17, 0xA7, 0x2B, 0xF2, 0xC7, 0xB9,
    0xE7, 0xC1, 0xAC, 0x4D, 0x77, 0xFC, 0x94, 0xCA, 0xDC, 0x08, 0x3E, 0x67,
    0x98, 0x40, 0x50, 0xB7, 0x5E, 0xBA, 0xE5, 0xDD, 0x28, 0x09, 0xBD, 0x63,
    0x80, 0x16, 0xF7, 0x23 };
static unsigned char brainpoolP512r1_gx[] = {
    0x81, 0xAE, 0xE4, 0xBD, 0xD8, 0x2E, 0xD9, 0x64, 0x5A, 0x21, 0x32, 0x2E,
    0x9C, 0x4C, 0x6A, 0x93, 0x85, 0xED, 0x9F, 0x70, 0xB5, 0xD9, 0x16, 0xC1,
    0xB4, 0x3B, 0x62, 0xEE, 0xF4, 0xD0, 0x09, 0x8E, 0xFF, 0x3B, 0x1F, 0x78,
    0xE2, 0xD0, 0xD4, 0x8D, 0x50, 0xD1, 0x68, 0x7B, 0x93, 0xB9, 0x7D, 0x5F,
    0x7C, 0x6D, 0x50, 0x47, 0x40, 0x6A, 0x5E, 0x68, 0x8B, 0x35, 0x22, 0x09,
    0xBC, 0xB9, 0xF8, 0x22 };
static unsigned char brainpoolP512r1_gy[] = {
    0x7D, 0xDE, 0x38, 0x5D, 0x56, 0x63, 0x32, 0xEC, 0xC0, 0xEA, 0xBF, 0xA9,
    0xCF, 0x78, 0x22, 0xFD, 0xF2, 0x09, 0xF7, 0x00, 0x24, 0xA5, 0x7B, 0x1A,
    0xA0, 0x00, 0xC5, 0x5B, 0x88, 0x1F, 0x81, 0x11, 0xB2, 0xDC, 0xDE, 0x49,
    0x4A, 0x5F, 0x48, 0x5E, 0x5B, 0xCA, 0x4B, 0xD8, 0x8A, 0x27, 0x63, 0xAE,
    0xD1, 0xCA, 0x2B, 0x2F, 0xA8, 0xF0, 0x54, 0x06, 0x78, 0xCD, 0x1E, 0x0F,
    0x3A, 0xD8, 0x08, 0x92 };
static unsigned char brainpoolP512r1_n[] = {
    0xAA, 0xDD, 0x9D, 0xB8, 0xDB, 0xE9, 0xC4, 0x8B, 0x3F, 0xD4, 0xE6, 0xAE,
    0x33, 0xC9, 0xFC, 0x07, 0xCB, 0x30, 0x8D, 0xB3, 0xB3, 0xC9, 0xD2, 0x0E,
    0xD6, 0x63, 0x9C, 0xCA, 0x70, 0x33, 0x08, 0x70, 0x55, 0x3E, 0x5C, 0x41,
    0x4C, 0xA9, 0x26, 0x19, 0x41, 0x86, 0x61, 0x19, 0x7F, 0xAC, 0x10, 0x47,
    0x1D, 0xB1, 0xD3, 0x81, 0x08, 0x5D, 0xDA, 0xDD, 0xB5, 0x87, 0x96, 0x82,
    0x9C, 0xA9, 0x00, 0x69 };

/*
 * Import an ECP group from binary
 */
static int ecp_group_read_binary( ecp_group *grp,
                                  const unsigned char *p,  size_t plen,
                                  const unsigned char *a,  size_t alen,
                                  const unsigned char *b,  size_t blen,
                                  const unsigned char *gx, size_t gxlen,
                                  const unsigned char *gy, size_t gylen,
                                  const unsigned char *n,  size_t nlen)
{
    int ret;

    MPI_CHK( mpi_read_binary( &grp->P, p, plen ) );
    if( a != NULL )
        MPI_CHK( mpi_read_binary( &grp->A, a, alen ) );
    else
        MPI_CHK( mpi_sub_int( &grp->A, &grp->P, 3 ) );
    MPI_CHK( mpi_read_binary( &grp->B, b, blen ) );
    MPI_CHK( mpi_read_binary( &grp->N, n, nlen ) );

    MPI_CHK( mpi_read_binary( &grp->G.X, gx, gxlen ) );
    MPI_CHK( mpi_read_binary( &grp->G.Y, gy, gylen ) );
    MPI_CHK( mpi_lset( &grp->G.Z, 1 ) );

    grp->pbits = mpi_msb( &grp->P );
    grp->nbits = mpi_msb( &grp->N );

cleanup:
    if( ret != 0 )
        ecp_group_free( grp );

    return( ret );
}

#if defined(POLARSSL_ECP_NIST_OPTIM)
/* Forward declarations */
static int ecp_mod_p192( mpi * );
static int ecp_mod_p224( mpi * );
static int ecp_mod_p256( mpi * );
static int ecp_mod_p384( mpi * );
static int ecp_mod_p521( mpi * );

#define NIST_MODP( P )      grp->modp = ecp_mod_ ## P;
#else
#define NIST_MODP( P )
#endif

#define LOAD_GROUP( G )     ecp_group_read_binary( grp,     \
                            G ## _p,  sizeof( G ## _p  ),   \
                            G ## _a,  sizeof( G ## _a  ),   \
                            G ## _b,  sizeof( G ## _b  ),   \
                            G ## _gx, sizeof( G ## _gx ),   \
                            G ## _gy, sizeof( G ## _gy ),   \
                            G ## _n,  sizeof( G ## _n  ) )

/*
 * Set a group using well-known domain parameters
 */
int ecp_use_known_dp( ecp_group *grp, ecp_group_id id )
{
    grp->id = id;

    switch( id )
    {
#if defined(POLARSSL_ECP_DP_SECP192R1_ENABLED)
        case POLARSSL_ECP_DP_SECP192R1:
            NIST_MODP( p192 );
            return( LOAD_GROUP( secp192r1 ) );
#endif /* POLARSSL_ECP_DP_SECP192R1_ENABLED */

#if defined(POLARSSL_ECP_DP_SECP224R1_ENABLED)
        case POLARSSL_ECP_DP_SECP224R1:
            NIST_MODP( p224 );
            return( LOAD_GROUP( secp224r1 ) );
#endif /* POLARSSL_ECP_DP_SECP224R1_ENABLED */

#if defined(POLARSSL_ECP_DP_SECP256R1_ENABLED)
        case POLARSSL_ECP_DP_SECP256R1:
            NIST_MODP( p256 );
            return( LOAD_GROUP( secp256r1 ) );
#endif /* POLARSSL_ECP_DP_SECP256R1_ENABLED */

#if defined(POLARSSL_ECP_DP_SECP384R1_ENABLED)
        case POLARSSL_ECP_DP_SECP384R1:
            NIST_MODP( p384 );
            return( LOAD_GROUP( secp384r1 ) );
#endif /* POLARSSL_ECP_DP_SECP384R1_ENABLED */

#if defined(POLARSSL_ECP_DP_SECP521R1_ENABLED)
        case POLARSSL_ECP_DP_SECP521R1:
            NIST_MODP( p521 );
            return( LOAD_GROUP( secp521r1 ) );
#endif /* POLARSSL_ECP_DP_SECP521R1_ENABLED */

#if defined(POLARSSL_ECP_DP_BP256R1_ENABLED)
        case POLARSSL_ECP_DP_BP256R1:
            return( LOAD_GROUP( brainpoolP256r1 ) );
#endif /* POLARSSL_ECP_DP_BP256R1_ENABLED */

#if defined(POLARSSL_ECP_DP_BP384R1_ENABLED)
        case POLARSSL_ECP_DP_BP384R1:
            return( LOAD_GROUP( brainpoolP384r1 ) );
#endif /* POLARSSL_ECP_DP_BP384R1_ENABLED */

#if defined(POLARSSL_ECP_DP_BP512R1_ENABLED)
        case POLARSSL_ECP_DP_BP512R1:
            return( LOAD_GROUP( brainpoolP512r1 ) );
#endif /* POLARSSL_ECP_DP_BP512R1_ENABLED */

        default:
            ecp_group_free( grp );
            return( POLARSSL_ERR_ECP_FEATURE_UNAVAILABLE );
    }
}

#if defined(POLARSSL_ECP_NIST_OPTIM)
/*
 * Fast reduction modulo the primes used by the NIST curves.
 *
 * These functions are critical for speed, but not needed for correct
 * operations. So, we make the choice to heavily rely on the internals of our
 * bignum library, which creates a tight coupling between these functions and
 * our MPI implementation.  However, the coupling between the ECP module and
 * MPI remains loose, since these functions can be deactivated at will.
 */

#if defined(POLARSSL_ECP_DP_SECP192R1_ENABLED)
/*
 * Compared to the way things are presented in FIPS 186-3 D.2,
 * we proceed in columns, from right (least significant chunk) to left,
 * adding chunks to N in place, and keeping a carry for the next chunk.
 * This avoids moving things around in memory, and uselessly adding zeros,
 * compared to the more straightforward, line-oriented approach.
 *
 * For this prime we need to handle data in chunks of 64 bits.
 * Since this is always a multiple of our basic t_uint, we can
 * use a t_uint * to designate such a chunk, and small loops to handle it.
 */

/* Add 64-bit chunks (dst += src) and update carry */
static inline void add64( t_uint *dst, t_uint *src, t_uint *carry )
{
    unsigned char i;
    t_uint c = 0;
    for( i = 0; i < 8 / sizeof( t_uint ); i++, dst++, src++ )
    {
        *dst += c;      c  = ( *dst < c );
        *dst += *src;   c += ( *dst < *src );
    }
    *carry += c;
}

/* Add carry to a 64-bit chunk and update carry */
static inline void carry64( t_uint *dst, t_uint *carry )
{
    unsigned char i;
    for( i = 0; i < 8 / sizeof( t_uint ); i++, dst++ )
    {
        *dst += *carry;
        *carry  = ( *dst < *carry );
    }
}

#define WIDTH       8 / sizeof( t_uint )
#define A( i )      N->p + i * WIDTH
#define ADD( i )    add64( p, A( i ), &c )
#define NEXT        p += WIDTH; carry64( p, &c )
#define LAST        p += WIDTH; *p = c; while( ++p < end ) *p = 0

/*
 * Fast quasi-reduction modulo p192 (FIPS 186-3 D.2.1)
 */
static int ecp_mod_p192( mpi *N )
{
    int ret;
    t_uint c = 0;
    t_uint *p, *end;

    /* Make sure we have enough blocks so that A(5) is legal */
    MPI_CHK( mpi_grow( N, 6 * WIDTH ) );

    p = N->p;
    end = p + N->n;

    ADD( 3 ); ADD( 5 );             NEXT; // A0 += A3 + A5
    ADD( 3 ); ADD( 4 ); ADD( 5 );   NEXT; // A1 += A3 + A4 + A5
    ADD( 4 ); ADD( 5 );             LAST; // A2 += A4 + A5

cleanup:
    return( ret );
}

#undef WIDTH
#undef A
#undef ADD
#undef NEXT
#undef LAST
#endif /* POLARSSL_ECP_DP_SECP192R1_ENABLED */

#if defined(POLARSSL_ECP_DP_SECP224R1_ENABLED) ||   \
    defined(POLARSSL_ECP_DP_SECP256R1_ENABLED) ||   \
    defined(POLARSSL_ECP_DP_SECP384R1_ENABLED)
/*
 * The reader is advised to first understand ecp_mod_p192() since the same
 * general structure is used here, but with additional complications:
 * (1) chunks of 32 bits, and (2) subtractions.
 */

/*
 * For these primes, we need to handle data in chunks of 32 bits.
 * This makes it more complicated if we use 64 bits limbs in MPI,
 * which prevents us from using a uniform access method as for p192.
 *
 * So, we define a mini abstraction layer to access 32 bit chunks,
 * load them in 'cur' for work, and store them back from 'cur' when done.
 *
 * While at it, also define the size of N in terms of 32-bit chunks.
 */
#define LOAD32      cur = A( i );

#if defined(POLARSSL_HAVE_INT8)     /* 8 bit */

#define MAX32       N->n / 4
#define A( j )      (uint32_t)( N->p[4*j+0]       ) |  \
                              ( N->p[4*j+1] << 8  ) |  \
                              ( N->p[4*j+2] << 16 ) |  \
                              ( N->p[4*j+3] << 24 )
#define STORE32     N->p[4*i+0] = (t_uint)( cur       );   \
                    N->p[4*i+1] = (t_uint)( cur >> 8  );   \
                    N->p[4*i+2] = (t_uint)( cur >> 16 );   \
                    N->p[4*i+3] = (t_uint)( cur >> 24 );

#elif defined(POLARSSL_HAVE_INT16)  /* 16 bit */

#define MAX32       N->n / 2
#define A( j )      (uint32_t)( N->p[2*j] ) | ( N->p[2*j+1] << 16 )
#define STORE32     N->p[2*i+0] = (t_uint)( cur       );  \
                    N->p[2*i+1] = (t_uint)( cur >> 16 );

#elif defined(POLARSSL_HAVE_INT32)  /* 32 bit */

#define MAX32       N->n
#define A( j )      N->p[j]
#define STORE32     N->p[i] = cur;

#else                               /* 64-bit */

#define MAX32       N->n * 2
#define A( j ) j % 2 ? (uint32_t)( N->p[j/2] >> 32 ) : (uint32_t)( N->p[j/2] )
#define STORE32                                   \
    if( i % 2 ) {                                 \
        N->p[i/2] &= 0x00000000FFFFFFFF;          \
        N->p[i/2] |= ((t_uint) cur) << 32;        \
    } else {                                      \
        N->p[i/2] &= 0xFFFFFFFF00000000;          \
        N->p[i/2] |= (t_uint) cur;                \
    }

#endif /* sizeof( t_uint ) */

/*
 * Helpers for addition and subtraction of chunks, with signed carry.
 */
static inline void add32( uint32_t *dst, uint32_t src, signed char *carry )
{
    *dst += src;
    *carry += ( *dst < src );
}

static inline void sub32( uint32_t *dst, uint32_t src, signed char *carry )
{
    *carry -= ( *dst < src );
    *dst -= src;
}

#define ADD( j )    add32( &cur, A( j ), &c );
#define SUB( j )    sub32( &cur, A( j ), &c );

/*
 * Helpers for the main 'loop'
 * (see fix_negative for the motivation of C)
 */
#define INIT( b )                                           \
    int ret;                                                \
    signed char c = 0, cc;                                  \
    uint32_t cur;                                           \
    size_t i = 0, bits = b;                                 \
    mpi C;                                                  \
    t_uint Cp[ b / 8 / sizeof( t_uint) + 1 ];               \
                                                            \
    C.s = 1;                                                \
    C.n = b / 8 / sizeof( t_uint) + 1;                      \
    C.p = Cp;                                               \
    memset( Cp, 0, C.n * sizeof( t_uint ) );                \
                                                            \
    MPI_CHK( mpi_grow( N, b * 2 / 8 / sizeof( t_uint ) ) ); \
    LOAD32;

#define NEXT                    \
    STORE32; i++; LOAD32;       \
    cc = c; c = 0;              \
    if( cc < 0 )                \
        sub32( &cur, -cc, &c ); \
    else                        \
        add32( &cur, cc, &c );  \

#define LAST                                    \
    STORE32; i++;                               \
    cur = c > 0 ? c : 0; STORE32;               \
    cur = 0; while( ++i < MAX32 ) { STORE32; }  \
    if( c < 0 ) fix_negative( N, c, &C, bits );

/*
 * If the result is negative, we get it in the form
 * c * 2^(bits + 32) + N, with c negative and N positive shorter than 'bits'
 */
static inline int fix_negative( mpi *N, signed char c, mpi *C, size_t bits )
{
    int ret;

    /* C = - c * 2^(bits + 32) */
#if !defined(POLARSSL_HAVE_INT64)
    ((void) bits);
#else
    if( bits == 224 )
        C->p[ C->n - 1 ] = ((t_uint) -c) << 32;
    else
#endif
        C->p[ C->n - 1 ] = (t_uint) -c;

    /* N = - ( C - N ) */
    MPI_CHK( mpi_sub_abs( N, C, N ) );
    N->s = -1;

cleanup:

    return( ret );
}

#if defined(POLARSSL_ECP_DP_SECP224R1_ENABLED)
/*
 * Fast quasi-reduction modulo p224 (FIPS 186-3 D.2.2)
 */
static int ecp_mod_p224( mpi *N )
{
    INIT( 224 );

    SUB(  7 ); SUB( 11 );               NEXT; // A0 += -A7 - A11
    SUB(  8 ); SUB( 12 );               NEXT; // A1 += -A8 - A12
    SUB(  9 ); SUB( 13 );               NEXT; // A2 += -A9 - A13
    SUB( 10 ); ADD(  7 ); ADD( 11 );    NEXT; // A3 += -A10 + A7 + A11
    SUB( 11 ); ADD(  8 ); ADD( 12 );    NEXT; // A4 += -A11 + A8 + A12
    SUB( 12 ); ADD(  9 ); ADD( 13 );    NEXT; // A5 += -A12 + A9 + A13
    SUB( 13 ); ADD( 10 );               LAST; // A6 += -A13 + A10

cleanup:
    return( ret );
}
#endif /* POLARSSL_ECP_DP_SECP224R1_ENABLED */

#if defined(POLARSSL_ECP_DP_SECP256R1_ENABLED)
/*
 * Fast quasi-reduction modulo p256 (FIPS 186-3 D.2.3)
 */
static int ecp_mod_p256( mpi *N )
{
    INIT( 256 );

    ADD(  8 ); ADD(  9 );
    SUB( 11 ); SUB( 12 ); SUB( 13 ); SUB( 14 );             NEXT; // A0

    ADD(  9 ); ADD( 10 );
    SUB( 12 ); SUB( 13 ); SUB( 14 ); SUB( 15 );             NEXT; // A1

    ADD( 10 ); ADD( 11 );
    SUB( 13 ); SUB( 14 ); SUB( 15 );                        NEXT; // A2

    ADD( 11 ); ADD( 11 ); ADD( 12 ); ADD( 12 ); ADD( 13 );
    SUB( 15 ); SUB(  8 ); SUB(  9 );                        NEXT; // A3

    ADD( 12 ); ADD( 12 ); ADD( 13 ); ADD( 13 ); ADD( 14 );
    SUB(  9 ); SUB( 10 );                                   NEXT; // A4

    ADD( 13 ); ADD( 13 ); ADD( 14 ); ADD( 14 ); ADD( 15 );
    SUB( 10 ); SUB( 11 );                                   NEXT; // A5

    ADD( 14 ); ADD( 14 ); ADD( 15 ); ADD( 15 ); ADD( 14 ); ADD( 13 );
    SUB(  8 ); SUB(  9 );                                   NEXT; // A6

    ADD( 15 ); ADD( 15 ); ADD( 15 ); ADD( 8 );
    SUB( 10 ); SUB( 11 ); SUB( 12 ); SUB( 13 );             LAST; // A7

cleanup:
    return( ret );
}
#endif /* POLARSSL_ECP_DP_SECP256R1_ENABLED */

#if defined(POLARSSL_ECP_DP_SECP384R1_ENABLED)
/*
 * Fast quasi-reduction modulo p384 (FIPS 186-3 D.2.4)
 */
static int ecp_mod_p384( mpi *N )
{
    INIT( 384 );

    ADD( 12 ); ADD( 21 ); ADD( 20 );
    SUB( 23 );                                              NEXT; // A0

    ADD( 13 ); ADD( 22 ); ADD( 23 );
    SUB( 12 ); SUB( 20 );                                   NEXT; // A2

    ADD( 14 ); ADD( 23 );
    SUB( 13 ); SUB( 21 );                                   NEXT; // A2

    ADD( 15 ); ADD( 12 ); ADD( 20 ); ADD( 21 );
    SUB( 14 ); SUB( 22 ); SUB( 23 );                        NEXT; // A3

    ADD( 21 ); ADD( 21 ); ADD( 16 ); ADD( 13 ); ADD( 12 ); ADD( 20 ); ADD( 22 );
    SUB( 15 ); SUB( 23 ); SUB( 23 );                        NEXT; // A4

    ADD( 22 ); ADD( 22 ); ADD( 17 ); ADD( 14 ); ADD( 13 ); ADD( 21 ); ADD( 23 );
    SUB( 16 );                                              NEXT; // A5

    ADD( 23 ); ADD( 23 ); ADD( 18 ); ADD( 15 ); ADD( 14 ); ADD( 22 );
    SUB( 17 );                                              NEXT; // A6

    ADD( 19 ); ADD( 16 ); ADD( 15 ); ADD( 23 );
    SUB( 18 );                                              NEXT; // A7

    ADD( 20 ); ADD( 17 ); ADD( 16 );
    SUB( 19 );                                              NEXT; // A8

    ADD( 21 ); ADD( 18 ); ADD( 17 );
    SUB( 20 );                                              NEXT; // A9

    ADD( 22 ); ADD( 19 ); ADD( 18 );
    SUB( 21 );                                              NEXT; // A10

    ADD( 23 ); ADD( 20 ); ADD( 19 );
    SUB( 22 );                                              LAST; // A11

cleanup:
    return( ret );
}
#endif /* POLARSSL_ECP_DP_SECP384R1_ENABLED */

#undef A
#undef LOAD32
#undef STORE32
#undef MAX32
#undef INIT
#undef NEXT
#undef LAST

#endif /* POLARSSL_ECP_DP_SECP224R1_ENABLED ||
          POLARSSL_ECP_DP_SECP256R1_ENABLED ||
          POLARSSL_ECP_DP_SECP384R1_ENABLED */

#if defined(POLARSSL_ECP_DP_SECP521R1_ENABLED)
/*
 * Here we have an actual Mersenne prime, so things are more straightforward.
 * However, chunks are aligned on a 'weird' boundary (521 bits).
 */

/* Size of p521 in terms of t_uint */
#define P521_WIDTH      ( 521 / 8 / sizeof( t_uint ) + 1 )

/* Bits to keep in the most significant t_uint */
#if defined(POLARSSL_HAVE_INT8)
#define P521_MASK       0x01
#else
#define P521_MASK       0x01FF
#endif

/*
 * Fast quasi-reduction modulo p521 (FIPS 186-3 D.2.5)
 * Write N as A1 + 2^521 A0, return A0 + A1
 */
static int ecp_mod_p521( mpi *N )
{
    int ret;
    size_t i;
    mpi M;
    t_uint Mp[P521_WIDTH + 1];
    /* Worst case for the size of M is when t_uint is 16 bits:
     * we need to hold bits 513 to 1056, which is 34 limbs, that is
     * P521_WIDTH + 1. Otherwise P521_WIDTH is enough. */

    if( N->n < P521_WIDTH )
        return( 0 );

    /* M = A1 */
    M.s = 1;
    M.n = N->n - ( P521_WIDTH - 1 );
    if( M.n > P521_WIDTH + 1 )
        M.n = P521_WIDTH + 1;
    M.p = Mp;
    memcpy( Mp, N->p + P521_WIDTH - 1, M.n * sizeof( t_uint ) );
    MPI_CHK( mpi_shift_r( &M, 521 % ( 8 * sizeof( t_uint ) ) ) );

    /* N = A0 */
    N->p[P521_WIDTH - 1] &= P521_MASK;
    for( i = P521_WIDTH; i < N->n; i++ )
        N->p[i] = 0;

    /* N = A0 + A1 */
    MPI_CHK( mpi_add_abs( N, N, &M ) );

cleanup:
    return( ret );
}

#undef P521_WIDTH
#undef P521_MASK
#endif /* POLARSSL_ECP_DP_SECP521R1_ENABLED */

#endif /* POLARSSL_ECP_NIST_OPTIM */

#endif