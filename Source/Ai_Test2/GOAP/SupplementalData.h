#pragma once

//Extra parameters passed to user overloads
struct SupplementalData
{
    //Extra parameters, that can be tuned according to design needs.
    
    //std::vector<float> position; //Agent's position on the map. By default must be set to its initial position.
    
    //Service value, that must be set to their default values (check comments) for planning to succeed. Thread carefully
    
    int initNode = -1;                      //The value must be equal to 'atNode' value from init state. 
    int futureGoToDestinationNode = -1;     //Default value is 'atNode' goal value if set, otherwise -1. Check SupplementalData.h for details.
                                            //As we build our plan from future to the past, the GoTo action, chosen at a particular
                                            //plan constructing step, has no info on the id of the departure node (it is yet to be evaluated further in
                                            //the process). In order to provide at least some cost measure, at first we assume that we departure from initial node (O).
                                            //If the plan does not include more than 1 GoTo action (GoTo A1), our assumption was right. Otherwise we have GoTo A1 and
                                            //GoTo A2 and our route is O->A2->A1. So, we must compensate total plan cost by adding cost(O->A2) + cost(A1->A2) - cost(O->A1).
                                            //For that purpose we need info on whether in future the agent will have to go at some node, which is stored in this variable.

    
    //int minimalNumHKits = 0;//Default value is 'hKitsLeft' goal value if set, otherwise 0. Check ActionData.h for details.
                            //Actions like 'Use health kit' require that agent possesses at least 1 health kit. However, if we stack these actions together
                            //in a single plan, minimal required number of health kits increases. Furthermore, we can only know the actual number of health
                            //kits at the moment when we achieve a state when all other conditions were satisfied -> we come to initial state. That means we
                            //can't assign an effect like 'make number of health kits less by one', because we simply do not know how many health kits the agent
                            //had before he used one. That is due to our future->past plan construction process. So, instead of decreasing the actual number
                            //of health kits within effects, we increase their required number within conditions. For that we need a value showing current
                            //minimal required value for the number of health kits. As it is more of a auxiliary value, which does not reflect any real world
                            //parameter of the agent, it was moved from world state to action data.
};
