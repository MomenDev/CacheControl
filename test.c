/**
 * Copyright(c) 2015 AvaiLink Technologies - All Rights Reserved
 * 
 * @File cache_control_test.c
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
#include <assert.h>
#include "src/cache_control.h"

struct _cache_buf_
{
	FunClearCache pfClearCacheCallback;
	INT32 dCacheCapacity;
	CacheVal_t* pCachedVal;
};

typedef struct _test_data_
{
	int dAge;
	char* pName;
}TestData_t;

#define test(fn) \
  puts("...test "#fn); \
  _test_##fn();
/*
void PrintCache(CacheBuf_t* pstCacheBuf)
{
	if(!pstCacheBuf) return;
	INT32 i;
	for (i = 0; i < pstCacheBuf->dCacheCapacity; ++i) {
		diag_printf("[%d]%lld,", i, pstCacheBuf->pCachedVal[i]);
	}
	diag_printf("\n");
}
*/

void _test_NewCacheCtrler1()
{
	INT32 dCacheCapacity = 10;
	CacheBuf_t* pstCacheBuf = NewCacheCtrler1(dCacheCapacity);
	assert(pstCacheBuf != NULL);
	assert(pstCacheBuf->dCacheCapacity == dCacheCapacity);
	assert(pstCacheBuf->pCachedVal[0] == INVALID_CACHE_VAL);
	assert(pstCacheBuf->pCachedVal[dCacheCapacity - 1] == INVALID_CACHE_VAL);
	DeleteCacheCtrler(pstCacheBuf);

	pstCacheBuf = NewCacheCtrler1(0);
	assert(pstCacheBuf == NULL);
	pstCacheBuf = NewCacheCtrler1(-1);
	assert(pstCacheBuf == NULL);
}

void _test_InsertValToCache()
{
	int dCacheCapacity = 3;
	CacheBuf_t* pstCacheBuf = NewCacheCtrler1(dCacheCapacity);
	InsertValToCache(pstCacheBuf, 0);
	InsertValToCache(pstCacheBuf, 1);
	InsertValToCache(pstCacheBuf, 2);

	assert(pstCacheBuf->pCachedVal[0] == 2);
	assert(pstCacheBuf->pCachedVal[1] == 1);
	assert(pstCacheBuf->pCachedVal[2] == 0);
	InsertValToCache(pstCacheBuf, 1);
	assert(pstCacheBuf->pCachedVal[0] == 1);
	assert(pstCacheBuf->pCachedVal[1] == 2);
	assert(pstCacheBuf->pCachedVal[2] == 0);
	InsertValToCache(pstCacheBuf, 4);
	assert(pstCacheBuf->pCachedVal[0] == 4);
	assert(pstCacheBuf->pCachedVal[1] == 1);
	assert(pstCacheBuf->pCachedVal[2] == 2);
	InsertValToCache(pstCacheBuf, INVALID_CACHE_VAL);
	assert(pstCacheBuf->pCachedVal[0] == INVALID_CACHE_VAL);
	assert(pstCacheBuf->pCachedVal[1] == 4);
	assert(pstCacheBuf->pCachedVal[2] == 1);
	DeleteCacheCtrler(pstCacheBuf);

	InsertValToCache(NULL, 10);
}

void _test_InsertValsToCache()
{

	int dCacheCapacity = 5;
	CacheBuf_t* pstCacheBuf = NewCacheCtrler1(dCacheCapacity);
	CacheVal_t vals[] = {0, 1, 2, 3, 4};
	InsertValsToCache(pstCacheBuf, 5, vals);
	assert(pstCacheBuf->pCachedVal[0] == 0);
	assert(pstCacheBuf->pCachedVal[1] == 1);
	assert(pstCacheBuf->pCachedVal[2] == 2);
	assert(pstCacheBuf->pCachedVal[3] == 3);
	assert(pstCacheBuf->pCachedVal[4] == 4);

	CacheVal_t vals1[] = {0,1,2};
	InsertValsToCache(pstCacheBuf, sizeof(vals1) / sizeof(vals1[0]), vals1);
	assert(pstCacheBuf->pCachedVal[0] == 0);
	assert(pstCacheBuf->pCachedVal[1] == 1);
	assert(pstCacheBuf->pCachedVal[2] == 2);
	assert(pstCacheBuf->pCachedVal[3] == 3);
	assert(pstCacheBuf->pCachedVal[4] == 4);

	CacheVal_t vals2[] = {1,2};
	InsertValsToCache(pstCacheBuf, sizeof(vals2) / sizeof(vals2[0]), vals2);
	assert(pstCacheBuf->pCachedVal[0] == 1);
	assert(pstCacheBuf->pCachedVal[1] == 2);
	assert(pstCacheBuf->pCachedVal[2] == 0);
	assert(pstCacheBuf->pCachedVal[3] == 3);
	assert(pstCacheBuf->pCachedVal[4] == 4);
	CacheVal_t vals3[] = {4,3};
	InsertValsToCache(pstCacheBuf, sizeof(vals3) / sizeof(vals3[0]), vals3);
	assert(pstCacheBuf->pCachedVal[0] == 4);
	assert(pstCacheBuf->pCachedVal[1] == 3);
	assert(pstCacheBuf->pCachedVal[2] == 1);
	assert(pstCacheBuf->pCachedVal[3] == 2);
	assert(pstCacheBuf->pCachedVal[4] == 0);

	CacheVal_t vals4[] = {0,1,2,3,4,5};
	InsertValsToCache(pstCacheBuf, sizeof(vals4) / sizeof(vals4[0]), vals4);
	assert(pstCacheBuf->pCachedVal[0] == 0);
	assert(pstCacheBuf->pCachedVal[1] == 1);
	assert(pstCacheBuf->pCachedVal[2] == 2);
	assert(pstCacheBuf->pCachedVal[3] == 3);
	assert(pstCacheBuf->pCachedVal[4] == 4);
	InsertValsToCache(pstCacheBuf, sizeof(vals4) / sizeof(vals4[0]), NULL);
	DeleteCacheCtrler(pstCacheBuf);

	InsertValsToCache(NULL, sizeof(vals4) / sizeof(vals4[0]), vals4);
}

static void _ClearCacheBufCB(CacheBuf_t* pstCacheBuf, CacheVal_t val)
{
	TestData_t pTetsDataArr[] = {
		{0, "chenjian0"},
		{1, "chenjian1"},
		{2, "chenjian2"}
	};

	static int dIndex = 0;
	TestData_t* pTestData = (TestData_t*)val;
	diag_printf("Clear val(CacheCapacity:%d, Age:%d,name:%s)\n",pstCacheBuf->dCacheCapacity, pTestData->dAge, pTestData->pName);
	assert(pTestData->dAge == pTetsDataArr[dIndex].dAge);
	assert(strcmp(pTestData->pName, pTetsDataArr[dIndex].pName) == 0);
	//free(pTestData->pName);
	dIndex++;
}

void _test_ClearCacheBuf()
{
	int dCacheCapacity = 3;
	CacheBuf_t* pstCacheBuf = NewCacheCtrler(dCacheCapacity, _ClearCacheBufCB);
	TestData_t pTetsData[] = {
			{0, "chenjian0"},
			{1, "chenjian1"},
			{2, "chenjian2"}
	};
	CacheVal_t vals[] = {(CacheVal_t)&pTetsData[0], (CacheVal_t)&pTetsData[1], (CacheVal_t)&pTetsData[2]};
	InsertValsToCache(pstCacheBuf, 3, vals);
	assert(pstCacheBuf->pCachedVal[0] == (CacheVal_t)&pTetsData[0]);
	assert(pstCacheBuf->pCachedVal[1] == (CacheVal_t)&pTetsData[1]);
	assert(pstCacheBuf->pCachedVal[2] == (CacheVal_t)&pTetsData[2]);
	ClearCacheBuf(pstCacheBuf);
	assert(pstCacheBuf->pCachedVal[0] == INVALID_CACHE_VAL);
	assert(pstCacheBuf->pCachedVal[1] == INVALID_CACHE_VAL);
	assert(pstCacheBuf->pCachedVal[2] == INVALID_CACHE_VAL);
	DeleteCacheCtrler(pstCacheBuf);
}

static void _CacheBufCallback(CacheBuf_t* pstCacheBuf, CacheVal_t val)
{
	TestData_t* pTestData = (TestData_t*)val;
	diag_printf("Clear val(CacheCapacity:%d, Age:%d,name:%s)\n",pstCacheBuf->dCacheCapacity, pTestData->dAge, pTestData->pName);
	assert(pTestData->dAge == 0);
	assert(strcmp(pTestData->pName, "chenjian0") == 0);
	//free(pTestData->pName);
}

void _test_CacheBufCallback()
{
	int dCacheCapacity = 3;
	CacheBuf_t* pstCacheBuf = NewCacheCtrler(dCacheCapacity, _CacheBufCallback);
	assert(pstCacheBuf != NULL);
	TestData_t pTetsData[] = {
			{0, "chenjian0"},
			{1, "chenjian1"},
			{2, "chenjian2"}
	};
	CacheVal_t vals[] = {(CacheVal_t)&pTetsData[0], (CacheVal_t)&pTetsData[1], (CacheVal_t)&pTetsData[2]};
	InsertValsToCache(pstCacheBuf, 3, vals);
	assert(pstCacheBuf->pCachedVal[0] == (CacheVal_t)&pTetsData[0]);
	assert(pstCacheBuf->pCachedVal[1] == (CacheVal_t)&pTetsData[1]);
	assert(pstCacheBuf->pCachedVal[2] == (CacheVal_t)&pTetsData[2]);

	CacheVal_t vals1[] = {(CacheVal_t)&pTetsData[1], (CacheVal_t)&pTetsData[2]};
	InsertValsToCache(pstCacheBuf, 2, vals1);
	assert(pstCacheBuf->pCachedVal[0] == (CacheVal_t)&pTetsData[1]);
	assert(pstCacheBuf->pCachedVal[1] == (CacheVal_t)&pTetsData[2]);
	assert(pstCacheBuf->pCachedVal[2] == (CacheVal_t)&pTetsData[0]);

	TestData_t pTetsData2[] = {
			{3, "chenjian3"},
	};
	CacheVal_t vals2[] = {(CacheVal_t)&pTetsData2[0]};
	InsertValsToCache(pstCacheBuf, 1, vals2);
	assert(pstCacheBuf->pCachedVal[0] == (CacheVal_t)&pTetsData2[0]);
	assert(pstCacheBuf->pCachedVal[1] == (CacheVal_t)&pTetsData[1]);
	assert(pstCacheBuf->pCachedVal[2] == (CacheVal_t)&pTetsData[2]);
	DeleteCacheCtrler(pstCacheBuf);
}

void TestCacheControlExe()
{
	test(NewCacheCtrler1);
	test(InsertValToCache);
	test(InsertValsToCache);
	test(ClearCacheBuf);
	test(CacheBufCallback);
}

int main(void){
	TestCacheControlExe();
	return 0;
}
