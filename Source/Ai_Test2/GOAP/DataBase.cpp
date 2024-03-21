#include "DataBase.h"

#include "GController.h"

DataBase::DataBase()
{
    GController::DataPtr		= this;
    IAttribute::DataPtr 		= this;
    IAction::DataPtr  			= this;
    Goal::DataPtr				= this;
    ConditionSet::DataPtr 		= this;
    Debugger::DataPtr			= this;
}

size_t DataBase::GetNumAttributes() const
{
    return AttributeCatalogue.Size();
}

size_t DataBase::GetAttributeId(const std::string& name) const
{
    return *AttributeCatalogue.GetId(name);
}
