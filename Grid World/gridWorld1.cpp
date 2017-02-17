#include <iostream>
#include "states.h"
//#include "utilityUpdates.h"

#define gamma 10

map<string, state*> stateCoords;
vector<state*> listOfStates;

using namespace std;
class state;
int main(){
  int numberOfStates = 12;

  state s0(0,0,-0.04,1,0);
  state s1(1,0,-0.04,1,0);
  state s2(2,0,-0.04,1,0);
  state s3(3,0,-0.04,1,0);

  state s4(0,1,-0.04,1,0);
  state s5(1,1,0,2,0);
  state s6(2,1,-0.04,1,0);
  state s7(3,1,-1,3,-1);

  state s8(0,2,-0.04,1,0);
  state s9(1,2,-0.04,1,0);
  state s10(2,2,-0.04,1,0);
  state s11(3,2,1,3,1);

  //Loop through all states and update their utilities
  int loopCount, maxEpochs = 50;
  vector<state*>::iterator iterStates;
  map<state*, float>::iterator iterStateUtils;
  for (loopCount = 0; loopCount < maxEpochs; loopCount++){
    for (iterStates = listOfStates.begin(); iterStates != listOfStates.end(); iterStates++){
      //if((*iterStates)->getCoords() == "2,2"){
      if((*iterStates)->getStateCat() == 1){
        (*iterStates)->updateUtility();
        cout << "Updated for: " << *iterStates << " Util: " << (*iterStates)->getUtility() << endl;
      }
    }
    //Update the utilites for all states for this iteration
    for (iterStateUtils = newUtilitiesMap.begin(); iterStateUtils != newUtilitiesMap.end(); iterStateUtils++){
      (iterStateUtils->first)->setNewUtility(newUtilitiesMap[iterStateUtils->first]);
      cout << "New utility for: " << (iterStateUtils->first) << "  " << (iterStateUtils->first)->getUtility() << endl;
    }
  }
  //s0.updateUtility();



  //cout << (*s).getReward() << endl;
  return 0;
}
