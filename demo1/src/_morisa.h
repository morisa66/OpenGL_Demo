#ifndef ___MORISA_H__
#define ___MORISA_H__

#define USE_MORISA_NAMESPACE 1

#if USE_MORISA_NAMESPACE
#define MORISA_NAMESPACE_BEGIN namespace morisa{
#define MORISA_NAMESPACE_END }
#else
#define MORISA_NAMESPACE_BEGIN 
#define MORISA_NAMESPACE_END 
#endif


#define IS_USE_LOG 1

#if IS_USE_LOG
#define USE_LOG
#endif

#define IS_USE_CONCURRENCY 1

#if IS_USE_CONCURRENCY
#define USE_CONCURRENCY
#endif


#endif