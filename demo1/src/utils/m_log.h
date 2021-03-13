#ifndef __M_LOG_H__
#define __M_LOG_H__


#include <fstream>
#include <time.h> 

#include "../_morisa.h"

#ifdef USE_CONCURRENCY
#include <mutex>
using std::mutex;
using std::lock_guard;
mutex mtx;
#endif 

MORISA_NAMESPACE_BEGIN

bool m_log(const char* info, const char* path = "log.txt")
{
	using std::endl;
#ifdef USE_CONCURRENCY
	lock_guard<mutex> m_mtx(mtx);
#endif 
	std::ofstream log(path, std::ios::app);
	if (!log.is_open()) return false;
	time_t cur;
	time(&cur);
	log << endl << ctime(&cur) << info << endl;
	log.close();
	return true;
}

MORISA_NAMESPACE_END

#endif

