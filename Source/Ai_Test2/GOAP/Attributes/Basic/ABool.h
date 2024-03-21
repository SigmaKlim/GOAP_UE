#pragma once
#include "AEnum.h"
#include <vector>
#include <string>

enum EAVBool
{
    eFalse,
    eTrue
};

const std::vector<std::string> B_ENUMERATORS = {
        "FALSE",
        "TRUE"};

class ABool : public AEnum
{
public:
    ABool() : AEnum(B_ENUMERATORS) {}
};