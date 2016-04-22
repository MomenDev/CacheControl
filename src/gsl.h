/**
 * Copyright(c) 2015 AvaiLink Technologies - All Rights Reserved
 * 
 * @File types.h
 *
 * @Brif 
 *
 * @Version 1.0
 *
 * @Date 2016Äê4ÔÂ19ÈÕ
 *
 * @Author chenj
 *
 */

#ifndef GSL_H_
#define GSL_H_
#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<time.h>
#include<string.h>

typedef unsigned long long 	UINT64;
typedef unsigned int   		UINT32;
typedef unsigned short 		UINT16;
typedef unsigned char 		UINT8;

typedef long long 			INT64;
typedef int   				INT32;
typedef short 				INT16;
typedef signed char			INT8;
#define FALSE 0
#define TRUE 1
typedef char Boolean;
#undef NULL
#define NULL (void *)0

#define diag_printf printf
#define GL_MemAlloc malloc
#define GL_MemFreeValidAddr free

#endif /* GSL_H_ */
