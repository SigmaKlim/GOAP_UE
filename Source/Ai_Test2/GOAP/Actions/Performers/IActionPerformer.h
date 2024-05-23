#pragma once
class IActionPerformer
{
public:
    virtual ~IActionPerformer() = default;
    //Do the logic and return true if action is complete
    virtual bool PerformAction() = 0;
};