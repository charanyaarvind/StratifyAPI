//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include "Draw/MPoint.hpp"
using namespace Draw;


MPoint::MPoint(const mg_t & a){
	d.x = 0;
	d.y = 0;
	rotate(a.rotation);
	shift(a.shift);
}

