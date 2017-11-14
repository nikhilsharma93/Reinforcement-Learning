/*
  states.cpp: Implementation of states.h
  @author Nikhil Sharma
*/

#include <iostream>
#include "states.h"
using namespace std;

extern map<string, state*> stateCoordsAccessible;
extern map<string, state*> stateCoordsBlocked;
extern vector<state*> listOfStates;
extern map<state*, string> optimalPolicy;
extern const float gammaValue;
extern int statesHeight;
extern int statesWidth;
extern float probCorrect; //Probability of executing the direction chosen
extern float probLeft; //Probability of going left of the direction chosen
extern float probRight; //Probability of going right of the direction chosen
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
  if (stateCat == 1 || stateCat == 3)
    stateCoordsAccessible.insert(std::pair<string, state*>(cordString, this));
  else  if (stateCat == 2)
      stateCoordsBlocked.insert(std::pair<string, state*>(cordString, this));
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

int state::getXCord(){
  return this->xCord;
}

int state::getYCord(){
  return this->yCord;
}

string IntToString (int a)
{
    ostringstream temp;
    temp<<a;
    return temp.str();
}



void getNeighboringStates(int currentX, int currentY, int* cordLeftX, int* cordLeftY, int* cordRightX, int* cordRightY, int* cordUpX, int* cordUpY, int* cordDownX, int* cordDownY){
  *cordLeftX = max(currentX - 1, 0);
  *cordRightX = min(currentX + 1, statesWidth - 1);
  *cordUpX = *cordDownX = currentX;
  *cordLeftY = *cordRightY = currentY;
  *cordUpY = min(currentY + 1, statesHeight - 1);
  *cordDownY = max(currentY - 1, 0);
  //cout << "starting check----------------------------------------------------\n";
  //Check if any of the neighbors are blocked, and hence not accessible
  map<string, state*>::iterator iterBlocked;
  //Left
  string cordCheckString = IntToString(*cordLeftX) + "," + IntToString(*cordLeftY);
  iterBlocked = stateCoordsBlocked.find(cordCheckString);
  if (iterBlocked != stateCoordsBlocked.end()){
    (*cordLeftX)++;
  }
  //Right
  cordCheckString = IntToString(*cordRightX) + "," + IntToString(*cordRightY);
  iterBlocked = stateCoordsBlocked.find(cordCheckString);
  if (iterBlocked != stateCoordsBlocked.end()){
    (*cordRightX)--;
  }
  //Up
  cordCheckString = IntToString(*cordUpX) + "," + IntToString(*cordUpY);

  iterBlocked = stateCoordsBlocked.find(cordCheckString);
  if (iterBlocked != stateCoordsBlocked.end()){
    (*cordUpY)--;
  }
  //Down
  cordCheckString = IntToString(*cordDownX) + "," + IntToString(*cordDownY);
  iterBlocked = stateCoordsBlocked.find(cordCheckString);
  if (iterBlocked != stateCoordsBlocked.end()){
    (*cordDownY)++;
  }
  //cout << "ending check========================================================\n";
}

void state::updateUtility(){
  float reward = (*this).getReward();
  int currentX = this->xCord;
  int currentY = this->yCord;
  int cordLeftX, cordLeftY, cordRightX, cordRightY, cordUpX, cordUpY, cordDownX, cordDownY;
  getNeighboringStates(currentX, currentY, &cordLeftX, &cordLeftY, &cordRightX, &cordRightY, &cordUpX, &cordUpY, &cordDownX, &cordDownY);
  //cout << "Values: " << currentX << " " << currentY << " " << cordLeftX << " " << cordLeftY << " " << cordRightX << " " << cordRightY << " " << cordUpY << " " << cordUpY << " " << cordDownX << " " << cordDownY << endl;
  float rewardTempUp = getRewardTemp(cordLeftX, cordLeftY, cordRightX, cordRightY, cordUpX, cordUpY);
  float rewardTempLeft = getRewardTemp(cordDownX, cordDownY, cordUpX, cordUpY, cordLeftX, cordLeftY);
  float rewardTempRight = getRewardTemp(cordUpX, cordUpY, cordDownX, cordDownY, cordRightX, cordRightY);
  float rewardTempDown = getRewardTemp(cordRightX, cordRightY, cordLeftX, cordLeftY, cordDownX, cordDownY);
  //cout << "Reward for up, left, right, down: " << rewardTempUp << "  " << rewardTempLeft << "  " << rewardTempRight << "  " << rewardTempDown << endl;
  float maxReward = max(max(rewardTempUp, rewardTempLeft), max(rewardTempRight, rewardTempDown));
  float discountedReward = reward + gammaValue*maxReward;
  newUtilitiesMap.insert(std::pair<state*, float>(this, discountedReward));
  newUtilitiesMap.find(this)->second = discountedReward;
}

float getRewardTemp(int cordLeftX, int cordLeftY, int cordRightX, int cordRightY, int cordUpX, int cordUpY){
  string cordStringLeft = IntToString(cordLeftX) + "," + IntToString(cordLeftY);
  float utiliyLeft = probLeft * (stateCoordsAccessible.find(cordStringLeft)->second)->getUtility();

  string cordStringRight = IntToString(cordRightX) + "," + IntToString(cordRightY);
  float utiliyRight = probRight * (stateCoordsAccessible.find(cordStringRight)->second)->getUtility();

  string cordStringUp = IntToString(cordUpX) + "," + IntToString(cordUpY);
  float utiliyUp = probCorrect * (stateCoordsAccessible.find(cordStringUp)->second)->getUtility();
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
  cout << "Optimal policy values" << currentX << "," << currentY << " are: " << rewardTempUp << " " << rewardTempLeft << " " << rewardTempRight << " " << rewardTempDown << endl;
  string optimalAction;
  if (maxReward == rewardTempRight) optimalAction = "Right";
  else if (maxReward == rewardTempLeft) optimalAction = "Left";
  else if (maxReward == rewardTempUp) optimalAction = "Up";
  else optimalAction = "Down";
  optimalPolicy.insert(std::pair<state*, string>(this, optimalAction));
}
