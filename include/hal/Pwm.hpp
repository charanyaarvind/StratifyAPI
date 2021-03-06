/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#ifndef PWM_HPP_
#define PWM_HPP_

#include <sos/dev/pwm.h>
#include "PinAssignment.hpp"
#include "Periph.hpp"

namespace hal {

/*! \brief PWM Attributes Class
 * \details This class is for containing ADC attributes.
 */
class PwmAttr : public PinAssignmentPeriphAttr<pwm_attr_t, pwm_pin_assignment_t> {
public:
    //period, channel

    PwmAttr(){
        m_attr.channel = mcu_channel(-1, -1);
    }

    /*! \details Accesses the PWM period. */
    u32 period() const { return m_attr.period; }

    /*! \details Sets the PWM period. */
    void set_period(u32 period){ m_attr.period = period; }

    /*! details Access the attibutes channel. */
    mcu_channel_t channel() const { return m_attr.channel; }

    /*! \details Sets the PWM attributes channel. */
    void set_channel(u32 loc, u32 value){
        m_attr.channel = mcu_channel(loc, value);
    }

    /*! \details Sets the PWM attributes channel. */
    void set_channel(const mcu_channel_t & channel){
        m_attr.channel = channel;
    }

};


/*! \brief PWM Pin Assignment
 * \details This class allows simple manipulation of the pwm_pin_assignment_t.
 */
class PwmPinAssignment : public PinAssignment<pwm_pin_assignment_t>{};

/*! \brief PWM Class
 * \details This class implements PWM outputs.
 *
 * Here is an example of how to use the Pwm class
 *
 * \code
 * #include <sapi/hal.hpp>
 *
 * Pwm pwm(1); //use PWM port 1
 * PwmPinAssignment pin_assignment;
 * pin_assignment->channel[0] = mcu_pin(1,20); //Use Pin 1.20
 * pwm.init(Pwm::SET_TIMER | Pwm::IS_ENABLED, 24000000, 1000, pin_assignment);
 * pwm.set_channel(2, 500);
 *
 * \endcode
 *
 *
 */
class Pwm : public Periph<pwm_info_t, pwm_attr_t, 'p'> {
public:
	Pwm(port_t port);

	enum {
        FLAG_SET_TIMER = PWM_FLAG_SET_TIMER,
        FLAG_IS_ACTIVE_HIGH = PWM_FLAG_IS_ACTIVE_HIGH,
        FLAG_IS_ACTIVE_LOW = PWM_FLAG_IS_ACTIVE_LOW,
        FLAG_SET_CHANNELS = PWM_FLAG_SET_CHANNELS,
        FLAG_CLEAR_CHANNELS = PWM_FLAG_CLEAR_CHANNELS,
        FLAG_IS_ENABLED = PWM_FLAG_IS_ENABLED,

        SET_TIMER /*! See \ref PWM_FLAG_SET_TIMER */ = PWM_FLAG_SET_TIMER,
        IS_ACTIVE_HIGH /*! See \ref PWM_FLAG_IS_ACTIVE_HIGH */ = PWM_FLAG_IS_ACTIVE_HIGH,
        IS_ACTIVE_LOW /*! See \ref PWM_FLAG_IS_ACTIVE_LOW */ = PWM_FLAG_IS_ACTIVE_LOW,
        SET_CHANNELS /*! See \ref PWM_FLAG_SET_CHANNELS */ = PWM_FLAG_SET_CHANNELS,
        CLEAR_CHANNELS /*! See \ref PWM_FLAG_CLEAR_CHANNELS */ = PWM_FLAG_CLEAR_CHANNELS,
        IS_ENABLED /*! See \ref PWM_FLAG_IS_ENABLED */ = PWM_FLAG_IS_ENABLED
	};


    /*! \details Sets the channel to the specified value.
     *
     * @param loc The channel location (number)
     * @param value The value to assign to the channel
     * @return Zero on success
     */
    int set_channel(u32 loc, u32 value) const {
		return Periph::set_channel(loc, value, I_PWM_SETCHANNEL);
	}

    /*! \details Sets the channel using the channel reference.
     *
     * @param channel The channel location and value
     * @return Zero on success
     */
	int set_channel(const mcu_channel_t & channel) const {
		return Periph::set_channel(channel, I_PWM_SETCHANNEL);
	}

    /*! \details Gets the channel value.
     *
     * @param loc Channel location
     * @return The value of the channel or (u32)-1 for an error
     */
    int get_channel(u32 loc){
        return Periph::get_channel(loc, I_PWM_GETCHANNEL);
    }

    /*! \details Enables the PWM timer.
     *
     * @return Zero on success
     *
     * This method can be used with disable() to start and stop
     * the PWM timer while preserving the attributes.
     *
     *
     *
     */
	int enable() const {
		return ioctl(I_PWM_ENABLE);
	}

    /*! \details Disables the PWM timer.
     *
     * \sa enable()
     *
     */
	int disable() const {
		return ioctl(I_PWM_DISABLE);
	}


    /*! \details Sets the PWM attributes.
     *
     * @param o_flags PWM flags to apply (for example, \ref SET_TIMER)
     * @param freq The PWM timer frequency
     * @param period The PWM timer period
     * @param pin_assignment A pointer to the pin assignment (leave blank to use BSP pin assignment)
     * @return Zero on success or less than zero for an error
     *
     */
	int set_attr(u32 o_flags, u32 freq, u32 period, const pwm_pin_assignment_t * pin_assignment = 0) const {
		pwm_attr_t attr;
		attr.o_flags = o_flags;
		if( pin_assignment != 0 ){
			memcpy(&attr.pin_assignment, pin_assignment, sizeof(pwm_pin_assignment_t));
		} else {
			memset(&attr.pin_assignment, 0xff, sizeof(pwm_pin_assignment_t));
		}
		attr.freq = freq;
		attr.period = period;
		return set_attr(attr);
	}

    /*! \details Sets the channel configuration.
     *
     * @param pin_assignment A pointer to the pin assignment
     * @return Zero on success
     *
     * This method is equivalent to set_attr() with o_flags set to PWM::FLAG_SET_CHANNELS.
     */
	int set_channels(const pwm_pin_assignment_t * pin_assignment){
		return set_attr(FLAG_SET_CHANNELS, 0, 0, pin_assignment);
	}

    /*! \details Opens the PWM and sets the attributes.
     *
     * This method takes the same arguments as set_attr().
     *
     */
	int init(u32 o_flags, u32 freq, u32 period, const pwm_pin_assignment_t * pin_assignment = 0){

		if( open() < 0 ){
			return -1;
		}

		return set_attr(o_flags, freq, period, pin_assignment);
	}

	using Periph::init;
	using Periph::set_attr;
    using Periph::get_channel;

#ifdef __MCU_ONLY__
	//using Pblock::write;
	//int write(const void * buf, int nbyte);
	int close();
#endif


private:

};

}

#endif /* PWM_HPP_ */
