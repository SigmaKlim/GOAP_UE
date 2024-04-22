#pragma once
class ACharacter;
//Extra parameters passed to user overloads. All int members can potentially be moved to attribute array.
struct SupplementalData
{
    //Extra parameters, that can be tuned according to design needs.
    
    
    //Service value, that must be set to their default values (check comments) for planning to succeed. Tread carefully
    
    int initNode = -1;                      //The value must -1. 
    int futureGoToDestinationNode = -1;     //Default value is 'atNode' goal value if set, otherwise -1. Check SupplementalData.h for details.
                                            //As we build our plan from future to the past, the GoTo action, chosen at a particular
                                            //plan constructing step, has no info on the id of the departure node (it is yet to be evaluated further in
                                            //the process). In order to provide at least some cost measure, at first we assume that we departure from initial node (O).
                                            //If the plan does not include more than 1 GoTo action (GoTo A1), our assumption was right. Otherwise we have GoTo A1 and
                                            //GoTo A2 and our route is O->A2->A1. So, we must compensate total plan cost by adding cost(O->A2) + cost(A1->A2) - cost(O->A1).
                                            //For that purpose we need info on whether in future the agent will have to go at some node, which is stored in this variable.
    ACharacter* agentPtr;
};
