/**
 * Copyright(c) 2015 AvaiLink Technologies - All Rights Reserved
 * 
 * @File cache_control.c
 *
 * @Brif 
 *
 * @Version 1.0
 *
 * @Date 2016-4-19
 *
 * @Author chenjian
 *
 */
#include "cache_control.h"

struct _cache_buf_
{
	FunClearCache pfClearCacheCallback;
	INT32 dCacheCapacity;
	CacheVal_t* pCachedVal;
};

static void _DefaultClearCacheCallback(CacheBuf_t* pstCacheBuf, CacheVal_t val)
{
//	diag_printf("_DefaultClearCacheCallback\n");
}

CacheBuf_t* NewCacheCtrler(INT32 dCacheCapacity, FunClearCache pfClearCacheCallback)
{
	if(dCacheCapacity <= 0) return NULL;
	CacheBuf_t* pstCacheBuf = (CacheBuf_t*)GL_MemAlloc(sizeof(CacheBuf_t));
	memset(pstCacheBuf, 0, sizeof(CacheBuf_t));
	if(!pstCacheBuf) return NULL;
	pstCacheBuf->dCacheCapacity = dCacheCapacity;
	if(pfClearCacheCallback)
		pstCacheBuf->pfClearCacheCallback = pfClearCacheCallback;
	else
		pstCacheBuf->pfClearCacheCallback = _DefaultClearCacheCallback;
	pstCacheBuf->pCachedVal = (CacheVal_t*)GL_MemAlloc(sizeof(CacheVal_t) * dCacheCapacity);
	memset(pstCacheBuf->pCachedVal, INVALID_CACHE_VAL, sizeof(CacheVal_t) * dCacheCapacity);
	if(!pstCacheBuf->pCachedVal)  goto Exit;
	return pstCacheBuf;
Exit:
	diag_printf("NewCacheCtrler exit.\n");
	DeleteCacheCtrler(pstCacheBuf);
	return NULL;
}

CacheBuf_t* NewCacheCtrler1(INT32 dCacheCapacity)
{
	return NewCacheCtrler(dCacheCapacity, _DefaultClearCacheCallback);
}

void DeleteCacheCtrler(CacheBuf_t* pstCacheBuf)
{
	if(!pstCacheBuf) return;
	GL_MemFreeValidAddr(pstCacheBuf->pCachedVal);
	GL_MemFreeValidAddr(pstCacheBuf);
}

void InsertValToCache(CacheBuf_t* pstCacheBuf, CacheVal_t val)
{
	CacheVal_t dVals[1] = {val};
	InsertValsToCache(pstCacheBuf, 1, dVals);
}

void InsertValsToCache(CacheBuf_t* pstCacheBuf, INT32 dValSize, CacheVal_t* val)
{
	INT32 i, j;
	INT32 dCachedIndex = 0;
	if(!pstCacheBuf) return;
	if(!val) return;
	if(pstCacheBuf->dCacheCapacity <= 0) return;
	CacheBuf_t* pstTmpCacheBuf = NewCacheCtrler1(pstCacheBuf->dCacheCapacity);
	if(!pstTmpCacheBuf) { return; }
	if(dValSize > pstCacheBuf->dCacheCapacity) {
		dValSize = pstCacheBuf->dCacheCapacity;
	}
	for (i = 0; i < dValSize; ++i) {
//		diag_printf("dCachedIndex:%d, val:%d\n",dCachedIndex, val[i]);
		pstTmpCacheBuf->pCachedVal[dCachedIndex++] = val[i];
	}
	FunClearCache pfClearCacheCallback = pstCacheBuf->pfClearCacheCallback;
	for (i = 0; i < pstCacheBuf->dCacheCapacity; ++i) {
		if(dCachedIndex >= pstTmpCacheBuf->dCacheCapacity) { break; }
		Boolean bSame = FALSE;
		for (j = 0; j < dValSize; ++j) {
			if(pstCacheBuf->pCachedVal[i] == val[j]){
				bSame = TRUE;
				break;
			}
		}
		if(bSame == TRUE) continue;
//		diag_printf("dCachedIndex:%d, val:%d\n",dCachedIndex, pstCacheBuf->pCachedVal[i]);
		pstTmpCacheBuf->pCachedVal[dCachedIndex++] = pstCacheBuf->pCachedVal[i];
	}
	for (i = 0; i < pstCacheBuf->dCacheCapacity; ++i) {
		Boolean bCleared = TRUE;
		if(pstCacheBuf->pCachedVal[i] == INVALID_CACHE_VAL) continue;
		for (j = 0; j < pstTmpCacheBuf->dCacheCapacity; ++j) {
			if(pstCacheBuf->pCachedVal[i] == pstTmpCacheBuf->pCachedVal[j]){
				bCleared = FALSE;
				break;
			}
		}
		if(bCleared == FALSE) continue;
		pfClearCacheCallback(pstCacheBuf, pstCacheBuf->pCachedVal[i]);
	}
	memcpy(pstCacheBuf->pCachedVal, pstTmpCacheBuf->pCachedVal,
			sizeof(CacheVal_t) * pstCacheBuf->dCacheCapacity);
	DeleteCacheCtrler(pstTmpCacheBuf);
}

void ClearCacheBuf(CacheBuf_t* pstCacheBuf)
{
	if(!pstCacheBuf) return;
	INT32 i;
	if(!pstCacheBuf->pCachedVal) return;
	if(!pstCacheBuf->pfClearCacheCallback) return;
	for (i = 0; i < pstCacheBuf->dCacheCapacity; ++i) {
		pstCacheBuf->pfClearCacheCallback(pstCacheBuf, pstCacheBuf->pCachedVal[i]);
	}
	memset(pstCacheBuf->pCachedVal, INVALID_CACHE_VAL, sizeof(CacheVal_t) * pstCacheBuf->dCacheCapacity);
}
