#include <iostream>
#include "states.h"
#include <cmath>
//#include "utilityUpdates.h"

#define maxEpochs 50
#define maxErrorAllowedPercent 0.01
extern const float gammaValue = 1;
//const float gammaValue = 1.0;

map<string, state*> stateCoords;
vector<state*> listOfStates;
map<state*, string> optimalPolicy;

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

  float maxError = maxErrorAllowedPercent*(1 - gammaValue)/ gammaValue;
  //Loop through all states and update their utilities
  int loopCount = 0;
  float delta = maxError + 1;
  vector<state*>::iterator iterStates;
  map<state*, float>::iterator iterStateUtils;
  while (loopCount < maxEpochs && delta > maxError){
    delta = 0;
    for (iterStates = listOfStates.begin(); iterStates != listOfStates.end(); iterStates++){
      if((*iterStates)->getStateCat() == 1){
        (*iterStates)->updateUtility();
        cout << "Updated for: " << *iterStates << " Util: " << (*iterStates)->getUtility() << endl;
      }
    }
    //Update the utilites for all states for this iteration
    for (iterStateUtils = newUtilitiesMap.begin(); iterStateUtils != newUtilitiesMap.end(); iterStateUtils++){
      float oldUtility = (iterStateUtils->first)->getUtility();
      float newUtility = newUtilitiesMap[iterStateUtils->first];
      float diffInUntility = std::abs((oldUtility - newUtility));
      if (diffInUntility < 1e-7) diffInUntility = 0;
      if (diffInUntility > delta){
        delta = diffInUntility;
      }
      (iterStateUtils->first)->setNewUtility(newUtility);
      cout << "New utility for: " << (iterStateUtils->first) << "  " << (iterStateUtils->first)->getUtility() << endl;
    }
    loopCount++;
  }

  cout << "Ended with: " << loopCount << " " << delta << endl;
  //s0.updateUtility();
  for (iterStates = listOfStates.begin(); iterStates != listOfStates.end(); iterStates++){
    if((*iterStates)->getStateCat() == 1){
      (*iterStates)->setOptimalPolicy();
    }
  }

  map<state*, string>::iterator iterPolicy;
  for (iterPolicy = optimalPolicy.begin(); iterPolicy != optimalPolicy.end(); iterPolicy++){
      cout << "Policy for " << (iterPolicy->first) << " is:  " << (iterPolicy->second) << endl;
  }



  //cout << (*s).getReward() << endl;
  return 0;
}
