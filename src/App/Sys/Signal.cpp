//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include "Sys/Signal.hpp"
using namespace Sys;


int SignalEvent::set_handler(const SignalHandler & handler) const {
	if( handler.sigaction()->sa_flags & (1<<SA_SIGINFO) ){
		return ::sigaction(signo_, handler.sigaction(), 0);
	} else {
		_sig_func_ptr ptr = handler.sigaction()->sa_handler;
		::signal(signo_, ptr);
		return 0;
	}
}
