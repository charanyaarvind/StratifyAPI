/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef DRAW_TEXTBOX_HPP_
#define DRAW_TEXTBOX_HPP_

#include "Drawing.hpp"
#include "TextAttr.hpp"
#include "../var/Token.hpp"

namespace draw {

/*! \brief Text Box
 * \details This class is a scrolling text box that can be used to show
 * long text messages.
 *
 */
class TextBox : public TextAttr, public Drawing {
public:
	TextBox();
	TextBox(const char * text);

	inline sg_size_t scroll() const { return m_scroll; }
	inline sg_size_t scroll_max() const { return m_scroll_max; }

	int count_lines(sg_size_t w);
	static int count_lines(const char * str, sg_size_t font, bool bold, sg_size_t w);

	inline void inc_scroll(){ m_scroll++; }
	inline void dec_scroll(){ if( m_scroll ){ m_scroll--; } }

	virtual void draw_to_scale(const DrawingScaledAttr & attr);


private:


	static void build_line(u32 & i, var::String & line, var::Token & tokens, int & build_len, sg_size_t w);

	sg_size_t m_scroll;
	sg_size_t m_scroll_max;
};

};

#endif /* DRAW_TEXTBOX_HPP_ */
