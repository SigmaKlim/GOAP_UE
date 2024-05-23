#include "DataBase.h"

#include "GoapController.h"

DataBase::DataBase()
{
    UGoapController::DataPtr	= this;
    IAttribute::DataPtr 		= this;
    IAction::DataPtr  			= this;
    Goal::DataPtr				= this;
    ConditionSet::DataPtr 		= this;
}
bool DataBase::RegisterAttribute(const std::string& name, IAttribute* attributePtr)
{
    return AttributeCatalogue.AddItem(name, std::shared_ptr<IAttribute>(attributePtr));
}

bool DataBase::RegisterGoal(const std::string& name, Goal* goalPtr)
{
    return GoalCatalogue.AddItem(name, std::shared_ptr<Goal>(goalPtr));
}

bool DataBase::RegisterAction(const std::string& name, IAction* actionPtr)
{
    return ActionCatalogue.AddItem(name, std::shared_ptr<IAction>(actionPtr));
}
size_t DataBase::GetNumAttributes() const
{
    return AttributeCatalogue.Size();
}

size_t DataBase::GetAttributeId(const std::string& name) const
{
    if (AttributeCatalogue.GetId(name) == nullptr)
        return SIZE_MAX;
    return *AttributeCatalogue.GetId(name);
}
