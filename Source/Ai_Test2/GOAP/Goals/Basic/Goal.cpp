#include "Goal.h"
#include "../../DataBase.h"

const DataBase* Goal::DataPtr;
Goal::Goal() : _conditions(DataPtr->GetNumAttributes())
{

}