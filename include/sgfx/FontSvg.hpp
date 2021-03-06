/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#ifndef SGFX_FONTSVG_HPP_
#define SGFX_FONTSVG_HPP_

#include "SvgFont.hpp"

namespace sgfx {

#if !defined __link
[[deprecated("Use SvgFont")]]
#endif
typedef SvgFont FontSvg;

}

#endif /* SGFX_FONTSVG_HPP_ */
