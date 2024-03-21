#pragma once
#include <iostream>
#include <string>

#include "IActionPerformer.h"

class APTest : public IActionPerformer
{
public:
  APTest(const std::string& name) : _name(name) {}
  bool PerformAction() override
  {
      std::cout << "Action " << _name << " completed!\n";
      return true;
  }
private:
  std::string _name;
};