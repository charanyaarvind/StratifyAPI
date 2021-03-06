/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef CHRONO_CLOCK_HPP_
#define CHRONO_CLOCK_HPP_

#include "../api/ChronoObject.hpp"
#include "ClockTime.hpp"

namespace chrono {

/*! \brief Clock Class
 * \details The clock class is used for reading
 * the system CPU clocks.
 *
 * It reads out a chrono::ClockTime object which is
 * a 64-bit value with seconds and nanoseconds based
 * on struct timeval.
 *
 */
class Clock : public api::ChronoInfoObject {
public:

    enum {
        REALTIME /*! Realtime clock ID used with get_time() and get_resolution() */ = CLOCK_REALTIME
    };

    /*! \details Assigns the value of CLOCK_REALTIME to this object */
    static ClockTime get_time(int clock_id = REALTIME);

    /*! \details Gets the resolution of the specified clock. */
    static ClockTime get_resolution(int clock_id = REALTIME);
};

}

#endif // CLOCK_HPP
