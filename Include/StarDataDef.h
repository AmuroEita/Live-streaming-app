#ifndef __STAR_DATA_DEF_H__
#define __STAR_DATA_DEF_H__
#pragma once

#include <list>
#include <atlstr.h>


enum EStarType
{
    EST_PERSON = 0,
    EST_GROUP,
    //    EST_CLASS,
	EST_B2S,
};

struct StarInfo {
    StarInfo() : id(0), count(0) {};

    ULONGLONG id;
    CString name;
    UINT32 count;
};

struct GroupStarInfo
{
    StarInfo info;
    std::list<StarInfo> subs;
};

struct IStarData
{
    virtual void GetAllStarData(std::list<StarInfo>& groupDatas, std::list<StarInfo>& userDatas) const = 0;
    virtual CString FormatJsonString() const = 0;
};

#endif