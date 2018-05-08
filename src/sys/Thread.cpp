//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#if !defined __link

#include <errno.h>
#include "sys/Thread.hpp"
#include "chrono.hpp"
using namespace sys;

Thread::Thread(int stack_size, bool detached) {
	init(stack_size, detached);
}

Thread::~Thread(){
    //what if thread is still running
	if( id() != ID_ERROR ){
		pthread_attr_destroy(&m_pthread_attr);
	}
}

int Thread::init(int stack_size, bool detached){
    set_id_error();
    if( set_error_number_if_error(pthread_attr_init(&m_pthread_attr)) < 0 ){
		return -1;
	}

    if( set_error_number_if_error(pthread_attr_setstacksize(&m_pthread_attr, stack_size)) < 0 ){
		return -1;
	}

	if( detached == true ){
        if( set_error_number_if_error(pthread_attr_setdetachstate(&m_pthread_attr, PTHREAD_CREATE_DETACHED)) < 0 ){
			return -1;
		}
	} else {
        if( set_error_number_if_error(pthread_attr_setdetachstate(&m_pthread_attr, PTHREAD_CREATE_JOINABLE)) < 0 ){
			return -1;
		}
	}

	set_id_default();
	return 0;
}

int Thread::set_stacksize(int size){
    return set_error_number_if_error(pthread_attr_setstacksize(&m_pthread_attr, size));
}

int Thread::get_stacksize() const {
    u32 stacksize = 0;
    set_error_number_if_error( pthread_attr_getstacksize(&m_pthread_attr, (size_t*)&stacksize));
	return stacksize;
}

int Thread::get_detachstate() const {
    int value = 0;
    set_error_number_if_error(pthread_attr_getdetachstate(&m_pthread_attr, &value));
	return value;
}

int Thread::set_detachstate(int value){
    return set_error_number_if_error(pthread_attr_setdetachstate(&m_pthread_attr, value));
}

int Thread::set_priority(int prio, enum Sched::policy policy){
	struct sched_param param;
	if( (int)m_id == -1 ){
		return -1;
	}
	param.sched_priority = prio;

    return set_error_number_if_error(pthread_setschedparam(m_id, policy, &param));
}

void Thread::yield(){
	sched_yield();
}

int Thread::get_priority() const {
	struct sched_param param;
	int policy;
    if( set_error_number_if_error(pthread_getschedparam(m_id, &policy, &param)) < 0 ){
		return -1;
	}
    return param.sched_priority;
}

int Thread::get_policy() const {
	struct sched_param param;
	int policy;
    if( set_error_number_if_error(pthread_getschedparam(m_id, &policy, &param)) < 0 ){
		return -1;
	}
	return policy;
}

int Thread::create(void * (*func)(void*), void * args, int prio, enum Sched::policy policy){
	if( (int)m_id != -1 ){
        set_error_number(EBUSY);
		return -1;
	}

	//First create the thread
    if( set_error_number_if_error(pthread_create(&m_id, &m_pthread_attr, func, args)) < 0 ){
		return -1;
	}

	//now set the priority
	if( set_priority(prio, policy) < 0 ){
		return -1;
	}

	return 0;
}

bool Thread::is_running() const {
	//check to see if the thread is running
	if( pthread_kill(m_id, 0) == 0 ){
		return true;
	}
	return false;
}

int Thread::wait(void**ret, int interval){

	void * dummy;

	if( id() < 0 ){
		return -1;
	}

	//if thread is joinable, then join it
	if( is_joinable() ){
		if( ret != 0 ){
			join(ret);
		} else {
			join(&dummy);
		}
	} else {
		//just keep sampling until the thread completes
		while( is_running() ){
            Timer::wait_milliseconds(interval);
		}
	}


	return 0;
}

void Thread::reset(){
	u32 stacksize;

	bool detached = !is_joinable();
	stacksize = get_stacksize();

    set_error_number_if_error(pthread_attr_destroy(&m_pthread_attr));

	init(stacksize, detached);
}

int Thread::join(int ident, void ** value_ptr){
	void * tmp_ptr;
	void ** ptr;
	if( value_ptr == 0 ){
		ptr = &tmp_ptr;
	} else {
		ptr = value_ptr;
	}
    return pthread_join(ident, ptr);
}

bool Thread::is_joinable() const{
	int detach_state = get_detachstate();
	return detach_state == JOINABLE;
}

int Thread::join(void ** value_ptr) const {
	void * tmp_ptr;
	void ** ptr;
	if( value_ptr == 0 ){
		ptr = &tmp_ptr;
	} else {
		ptr = value_ptr;
	}
    return set_error_number_if_error(pthread_join(id(), ptr));
}

#endif

