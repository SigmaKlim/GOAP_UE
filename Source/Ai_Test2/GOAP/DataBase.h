#pragma once
#include <memory>

#include "Navigation/Navigator.h"
#include "Tools/Catalogue.h"


class GController;
class IAction;
class Goal;
class IAttribute;

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

