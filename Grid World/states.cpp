#include <iostream>
#include "states.h"
using namespace std;

extern map<string, state*> stateCoords;
extern vector<state*> listOfStates;
extern map<state*, string> optimalPolicy;
extern const float gammaValue;
map<state*, float> newUtilitiesMap;

state::state(int xCord, int yCord, float reward, int stateCat, float utility){
  this->xCord = xCord;
  this->yCord = yCord;
  this->reward = reward;
  this->stateCat = stateCat;
  this->utility = utility;
  //map<int(*) [2], float> stateUt = this->stateUtils;
  //int s[2] = {xCord, yCord};
  string cordString = IntToString(xCord) + "," + IntToString(yCord);

  stateCoords.insert(std::pair<string, state*>(cordString, this));
  listOfStates.push_back(this);
  //cout << "this: " << this << "   " << (stateCoords.find(cordString)->second)->getUtility() <<  endl;
}

int state::getStateCat(){
  return this->stateCat;
}

float state::getReward(){
  return this->reward;
}

float state::getUtility(){
  return this->utility;
}

void state::setNewUtility(float newUtility){
  this->utility = newUtility;
}

string state::getCoords(){
  string cordString = IntToString(this->xCord) + "," + IntToString(this->yCord);
  return cordString;
}

string IntToString (int a)
{
    ostringstream temp;
    temp<<a;
    return temp.str();
}

void getNeighboringStates(int currentX, int currentY, int* cordLeftX, int* cordLeftY, int* cordRightX, int* cordRightY, int* cordUpX, int* cordUpY, int* cordDownX, int* cordDownY){
  *cordLeftX = max(currentX - 1, 0);
  *cordRightX = min(currentX + 1, 3);
  *cordUpX = *cordDownX = currentX;
  *cordLeftY = *cordRightY = currentY;
  *cordUpY = min(currentY + 1, 2);
  *cordDownY = max(currentY - 1, 0);
  //cout << "neighbors: " << *cordLeftX << *cordLeftY << *cordRightX << *cordRightY << *cordUpX<< *cordUpY<< *cordDownX<< *cordDownY <<endl;
  if (*cordLeftX == 1 && *cordLeftY == 1){
    *cordLeftX = 2;
    *cordLeftY = 1;
  }
  if (*cordRightX == 1 && *cordRightY == 1){
    *cordRightX = 0;
    *cordRightY = 1;
  }
  if (*cordUpX == 1 && *cordUpY == 1){
    *cordUpX = 1;
    *cordUpY = 0;
  }
  if (*cordDownX == 1 && *cordDownY == 1){
    *cordDownX = 1;
    *cordDownY = 2;
  }
}

void state::updateUtility(){
  float reward = (*this).getReward();
  int currentX = this->xCord;
  int currentY = this->yCord;
  int cordLeftX, cordLeftY, cordRightX, cordRightY, cordUpX, cordUpY, cordDownX, cordDownY;
  getNeighboringStates(currentX, currentY, &cordLeftX, &cordLeftY, &cordRightX, &cordRightY, &cordUpX, &cordUpY, &cordDownX, &cordDownY);
  /*
  int cordLeftX, cordLeftY, cordRightX, cordRightY, cordUpX, cordUpY, cordDownX, cordDownY;
  cordLeftX = max(currentX - 1, 0);
  cordRightX = min(currentX + 1, 3);
  cordUpX = cordDownX = currentX;
  cordLeftY = cordRightY = currentY;
  cordUpY = min(currentY + 1, 2);
  cordDownY = max(currentY - 1, 0);
  //cout << "neighbors: " << cordLeftX << cordLeftY << cordRightX << cordRightY << cordUpX<< cordUpY<< cordDownX<< cordDownY <<endl;
  if (cordLeftX == 1 && cordLeftY == 1){
    cordLeftX = 2;
    cordLeftY = 1;
  }
  if (cordRightX == 1 && cordRightY == 1){
    cordRightX = 0;
    cordRightY = 1;
  }
  if (cordUpX == 1 && cordUpY == 1){
    cordUpX = 1;
    cordUpY = 0;
  }
  if (cordDownX == 1 && cordDownY == 1){
    cordDownX = 1;
    cordDownY = 2;
  }
  */
  float rewardTempUp = getRewardTemp(cordLeftX, cordLeftY, cordRightX, cordRightY, cordUpX, cordUpY);
  float rewardTempLeft = getRewardTemp(cordDownX, cordDownY, cordUpX, cordUpY, cordLeftX, cordLeftY);
  float rewardTempRight = getRewardTemp(cordUpX, cordUpY, cordDownX, cordDownY, cordRightX, cordRightY);
  float rewardTempDown = getRewardTemp(cordRightX, cordRightY, cordLeftX, cordLeftY, cordDownX, cordDownY);
  float maxReward = max(max(rewardTempUp, rewardTempLeft), max(rewardTempRight, rewardTempDown));
  float discountedReward = reward + gammaValue*maxReward;
  //cout << "discountedReward is: " << discountedReward << endl;
  newUtilitiesMap.insert(std::pair<state*, float>(this, discountedReward));
  newUtilitiesMap.find(this)->second = discountedReward;
}

float getRewardTemp(int cordLeftX, int cordLeftY, int cordRightX, int cordRightY, int cordUpX, int cordUpY){
  string cordStringLeft = IntToString(cordLeftX) + "," + IntToString(cordLeftY);
  float utiliyLeft = 0.1 * (stateCoords.find(cordStringLeft)->second)->getUtility();
  //cout << "Left: " << cordStringLeft << "  " << utiliyLeft << endl;

  string cordStringRight = IntToString(cordRightX) + "," + IntToString(cordRightY);
  float utiliyRight = 0.1 * (stateCoords.find(cordStringRight)->second)->getUtility();
  //cout << "Right: " << cordStringRight << "  " << utiliyRight << endl;

  string cordStringUp = IntToString(cordUpX) + "," + IntToString(cordUpY);
  float utiliyUp = 0.8 * (stateCoords.find(cordStringUp)->second)->getUtility();
  //cout << "Up: " << cordStringUp << "  " << utiliyUp << endl;
  //cout << "\n\n" ;
  return (utiliyLeft + utiliyRight + utiliyUp);
}

void state::setOptimalPolicy(){
  int currentX = this->xCord;
  int currentY = this->yCord;
  int cordLeftX, cordLeftY, cordRightX, cordRightY, cordUpX, cordUpY, cordDownX, cordDownY;
  getNeighboringStates(currentX, currentY, &cordLeftX, &cordLeftY, &cordRightX, &cordRightY, &cordUpX, &cordUpY, &cordDownX, &cordDownY);
  float rewardTempUp = getRewardTemp(cordLeftX, cordLeftY, cordRightX, cordRightY, cordUpX, cordUpY);
  float rewardTempLeft = getRewardTemp(cordDownX, cordDownY, cordUpX, cordUpY, cordLeftX, cordLeftY);
  float rewardTempRight = getRewardTemp(cordUpX, cordUpY, cordDownX, cordDownY, cordRightX, cordRightY);
  float rewardTempDown = getRewardTemp(cordRightX, cordRightY, cordLeftX, cordLeftY, cordDownX, cordDownY);
  float maxReward = max(max(rewardTempUp, rewardTempLeft), max(rewardTempRight, rewardTempDown));
  string optimalAction;
  if (maxReward == rewardTempUp) optimalAction = "Up";
  else if (maxReward == rewardTempLeft) optimalAction = "Left";
  else if (maxReward == rewardTempRight) optimalAction = "Right";
  else optimalAction = "Down";
  optimalPolicy.insert(std::pair<state*, string>(this, optimalAction));
}
