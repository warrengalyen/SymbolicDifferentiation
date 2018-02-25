#pragma once

#include "MathStack.h"

// supported functions for differentiation
#define d(u,stack)			Differentiate(u,stack)
#define d_sin(u,stack)		d(u,stack)+"*cos("+u+")"							// du*cos(u)
#define d_cos(u,stack)		d(u,stack)+"*-sin("+u+")"							// du*(-sin(u))
#define d_tan(u,stack)		d(u,stack)+"*sec("+u+")^2"							// du*sec(u)^2
#define d_sec(u,stack)		d(u,stack)+"*sec("+u+")*tan("+u+")"					// du*sec(u)*tan(u)
#define d_cosec(u,stack)	"(-"+d(u,stack)+")*cosec("+u+")*cot("+u+")"			// -du*cosec(u)*cot(u)
#define d_cot(u,stack)		d(u,stack)+"*-cosec("+u+")^2"						// du*(-cosec(u)^2)
#define d_sinh(u,stack)		d(u,stack)+"*cosh("+u+")"							// du*cosh(u)
#define d_cosh(u,stack)		d(u,stack)+"*sinh("+u+")"							// du*(sinh(u))
#define d_tanh(u,stack)		d(u,stack)+"*sech("+u+")^2"							// du*sech(u)^2
#define d_sech(u,stack)		d(u,stack)+"*sech("+u+")*tanh("+u+")"				// du*sech(u)*tanh(u)
#define d_cosech(u,stack)	"(-"+d(u,stack)+")*cosech("+u+")*coth("+u+")"		// -du*cosech(u)*coth(u)
#define d_coth(u,stack)		d(u,stack)+"*-cosech("+u+")^2"						// -du*cosech(u)^2
#define d_asin(u,stack)		d(u,stack)+"/sqrt(1-("+u+")^2)"						// du/sqrt(1-u^2)
#define d_acos(u,stack)		"(-"+d(u,stack)+")/sqrt(1-("+u+")^2)"				// -du/sqrt(1-u^2)
#define d_atan(u,stack)		d(u,stack)+"/(1+("+u+")^2)"							// du/(1+u^2)
#define d_asec(u,stack)		d(u,stack)+"/(abs("+u+")*sqrt(("+u+")^2-1))"		// du/(|u|*sqrt(u^2-1))
#define d_acosec(u,stack)	"(-"+d(u,stack)+")/(abs("+u+")*sqrt(("+u+")^2-1))"	// -du/(|u|*sqrt(u^2-1))
#define d_acot(u,stack)		"(-"+d(u,stack)+")/(1+("+u+")^2)"					// -du/(1+u^2)
#define d_asinh(u,stack)	d(u,stack)+"/sqrt(("+u+")^2+1)"						// du/sqrt(u^2+1)
#define d_acosh(u,stack)	d(u,stack)+")/sqrt(("+u+")^2-1)"					// du/sqrt(u^2-1)
#define d_atanh(u,stack)	d(u,stack)+"/(1-("+u+")^2)"							// du/(1-u^2)
#define d_asech(u,stack)	"(-"+d(u,stack)+")/(("+u+")*sqrt(1-("+u+")^2))"		// -du/(u*sqrt(1-u^2))
#define d_acosech(u,stack)	"(-"+d(u,stack)+")/(("+u+")*sqrt(1+("+u+")^2))"		// -du/(u*sqrt(1+u^2))
#define d_acoth(u,stack)	d(u,stack)+"/(1-("+u+")^2)"							// du/(1-u^2)
#define d_sqrt(u,stack)		d(u,stack)+"/(2*sqrt("+u+"))"						// du/(2*sqrt(u))
#define d_log10(u,stack)	d(u,stack)+"/(("+u+")*log(10))"						// du/(u*log(10))
#define d_log(u,stack)		d(u,stack)+"/("+u+")"								// du/u
#define d_ln(u,stack)		d_log(u,stack)
#define d_sign(u,stack)		"0"
#define d_abs(u,stack)		d(u,stack)+"*("+u+")/abs("+u+")"					// du*u/abs(u)

CString Differentiate(LPCSTR lpcstrInput, CString& sStack, bool bOptimize = false);

