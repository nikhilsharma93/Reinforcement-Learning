/*
  gridWorld.cpp : Main source file
  @author Nikhil Sharma
*/

#include <iostream>
#include "states.h"
#include <cmath>
#include<opencv2/highgui/highgui.hpp>
#include "openCVOutput.h"
#include "gridInitialization.h"
#include "zenityInterface.h"
#include <cstdlib>

using namespace cv;
//#include "utilityUpdates.h"

#define maxEpochs 500
#define maxErrorAllowedPercent 0.01
extern const float gammaValue = 1.0;
//const float gammaValue = 1.0;

map<string, state*> stateCoordsAccessible;
map<string, state*> stateCoordsBlocked;

vector<state*> listOfStates;
map<state*, string> optimalPolicy;
int statesHeight;
int statesWidth;

using namespace std;
class state;
int main(){

  cout << "\n--------------------------------------------------------------------\n";
  cout << "--------------------------------------------------------------------\n";
  cout << "------Reinforcement Learning in a grid world via Value Iteration------\n";
  cout << "--------------------------------------------------------------------\n";
  cout << "--------------------------------------------------------------------\n";


  bool printPolicies = false;
  string outputTemp = GetStdoutFromCommand("zenity --forms --title=\"Grid World\" --text=\"Enter the parameters\" \
     --add-entry=\"Horizontal Dimesion\" \
     --add-entry=\"Vertical Dimension\"\
     --add-entry=\"Reward for non-terminal states\" \
     --add-entry=\"Reward for positive terminal state\" \
     --add-entry=\"Reward for negative terminal state\" \
     --add-entry=\"Initial Utility of non-terminal states\" ");

  string delimiter = "|";
  size_t pos = 0;

  pos = outputTemp.find(delimiter);
  statesWidth = atoi(outputTemp.substr(0, pos).c_str());
  outputTemp.erase(0, pos + delimiter.length());

  pos = outputTemp.find(delimiter);
  statesHeight = atoi(outputTemp.substr(0, pos).c_str());
  outputTemp.erase(0, pos + delimiter.length());

  pos = outputTemp.find(delimiter);
  float rewardNonTerminal = atof(outputTemp.substr(0, pos).c_str());
  outputTemp.erase(0, pos + delimiter.length());

  pos = outputTemp.find(delimiter);
  float rewardPosTerminal = atof(outputTemp.substr(0, pos).c_str());
  outputTemp.erase(0, pos + delimiter.length());

  pos = outputTemp.find(delimiter);
  float rewardNegTerminal = atof(outputTemp.substr(0, pos).c_str());
  outputTemp.erase(0, pos + delimiter.length());

  pos = outputTemp.find(delimiter);
  float initialUtilityNotTerminal = atof(outputTemp.substr(0, pos).c_str());
  outputTemp.erase(0, pos + delimiter.length());

  string blockedPresent = GetStdoutFromCommand("zenity  --title  \"Blocked States\" --entry --text \"Are there any unaccessible states? Enter \'yes\' or \'no\'\"");


  initializeGrid(statesHeight, statesWidth, rewardNonTerminal, rewardPosTerminal, rewardNegTerminal, initialUtilityNotTerminal, blockedPresent);


/*
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

*/
  cout << "\n--------------------------------------------------------------------\n";
  cout << "--------------------------------------------------------------------\n";
  cout << "----------------------Updating utilities----------------------------\n";
  cout << "--------------------------------------------------------------------\n";
  cout << "--------------------------------------------------------------------\n";
  float maxError;
  if (gammaValue == 0) maxError = 0;
  else maxError = maxErrorAllowedPercent*(1 - gammaValue)/ gammaValue;
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
        //cout << "Updated for: " << *iterStates << " Util: " << (*iterStates)->getUtility() << endl;
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
      //cout << "New utility for: " << (iterStateUtils->first) << "  " << (iterStateUtils->first)->getUtility() << endl;
    }
    loopCount++;
  }

  cout << "Performed " << loopCount << " iterations, and ended with delta " << delta << endl;
  cout << "\nwhere delta is the maximum of differences between successive utilities of states\n\n";
  //s0.updateUtility();
  for (iterStates = listOfStates.begin(); iterStates != listOfStates.end(); iterStates++){
    if((*iterStates)->getStateCat() == 1){
      (*iterStates)->setOptimalPolicy();
    }
  }

  if (printPolicies == true){
    map<state*, string>::iterator iterPolicy;
    for (iterPolicy = optimalPolicy.begin(); iterPolicy != optimalPolicy.end(); iterPolicy++){
        cout << "Policy for " << (iterPolicy->first)->getCoords() << " is:  " << (iterPolicy->second) << endl;
    }
  }

  showOutput(optimalPolicy, statesHeight, statesWidth);

  return 0;
}
