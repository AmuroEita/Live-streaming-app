#pragma once
#include <atlimage.h>
#include "stringHelper.h"
#include "PathHelper.h"
#include "fileHelper.h"

#include "../../../baseclass/MD5.h"

#include "IMDataDef.h"

#define MAKEINT64(a, b)	\
	((((__int64)(a) & 0xFFFFFFFF)) | (((__int64)(b) << 32) & 0xFFFFFFFF00000000))
#define LOINT64(i64)	\
	((int)(i64))	
#define HIINT64(i64)	\
	((int)((__int64)(i64) >> 32))

