#pragma once
#include <memory>

#include "Actions/Basic/IAction.h"
#include "Attributes/Basic/IAttribute.h"
#include "Goals/Basic/Goal.h"

#include "Navigation/Navigator.h"
#include "Tools/Catalogue.h"




//A base class for access to all data catalogues
class DataBase
{
public:
    DataBase();

    size_t GetNumAttributes() const;
    size_t GetAttributeId(const std::string& name) const;

    Catalogue<std::shared_ptr<IAttribute>>   AttributeCatalogue;
    Catalogue<std::shared_ptr<IAction>>      ActionCatalogue;
    Catalogue<std::shared_ptr<Goal>>         GoalCatalogue;

    Navigator Navigator;
};

