#pragma once
#include <iostream>
#include <string>

#include "IActionPerformer.h"
#include <Internationalization/Text.h>

class APTest : public IActionPerformer
{
public:
  APTest(const std::string& name) : _name(name) {}
  bool PerformAction() override
  {
 
      std::string s = "Action " + _name + " completed!";
      GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString(s.c_str()));
      return true;
  }
private:
  std::string _name;
};