#pragma once
#include <memory>

#include "Actions/Basic/IAction.h"
#include "Attributes/Basic/IAttribute.h"
#include "Goals/Basic/Goal.h"

#include "Navigation/Navigator.h"
#include "Tools/Catalogue.h"
#include <ActionPerformer.h>




//A base class for access to all data catalogues
class DataBase
{
public:
    DataBase();
    bool RegisterAttribute(const std::string& name, IAttribute* attributePtr);
    bool RegisterGoal(const std::string& name, Goal* goalPtr);
    bool RegisterAction(const std::string& name, IAction* actionPtr);
    
    size_t GetNumAttributes() const;
    size_t GetAttributeId(const std::string& name) const;

    Catalogue<std::shared_ptr<IAttribute>>   AttributeCatalogue;
    Catalogue<std::shared_ptr<IAction>>      ActionCatalogue;
    Catalogue<std::shared_ptr<Goal>>         GoalCatalogue;

    std::vector<UActionPerformer*> ActionPerformers;

    Navigator Navigator;
};

