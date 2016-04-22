/**
 * Copyright(c) 2015 AvaiLink Technologies - All Rights Reserved
 * 
 * @File cache_control.h
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

#ifndef _COMMON_CACHE_CONTROL_H_
#define _COMMON_CACHE_CONTROL_H_
#include "gsl.h"

#define INVALID_CACHE_VAL (-1)

typedef struct _cache_buf_ CacheBuf_t;

typedef long long CacheVal_t;

typedef void (*FunClearCache)(CacheBuf_t* pstCacheBuf, CacheVal_t val);

CacheBuf_t* NewCacheCtrler1(INT32 dCacheCapacity);

CacheBuf_t* NewCacheCtrler(INT32 dCacheCapacity, FunClearCache pfClearCacheCallback);

void DeleteCacheCtrler(CacheBuf_t* pstCacheBuf);

void InsertValToCache(CacheBuf_t* pstCacheBuf, CacheVal_t val);

void InsertValsToCache(CacheBuf_t* pstCacheBuf, INT32 dValSize, CacheVal_t* val);

void ClearCacheBuf(CacheBuf_t* pstCacheBuf);

#endif /* _COMMON_CACHE_CONTROL_H_ */
