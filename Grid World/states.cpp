#include <iostream>
#include "states.h"
using namespace std;

extern map<string, state*> stateCoords;
extern vector<state*> listOfStates;
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



void state::updateUtility(){
  float reward = (*this).getReward();
  int currentX = this->xCord;
  int currentY = this->yCord;
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
  float rewardTempUp = getRewardTemp(cordLeftX, cordLeftY, cordRightX, cordRightY, cordUpX, cordUpY);
  float rewardTempLeft = getRewardTemp(cordDownX, cordDownY, cordUpX, cordUpY, cordLeftX, cordLeftY);
  float rewardTempRight = getRewardTemp(cordUpX, cordUpY, cordDownX, cordDownY, cordRightX, cordRightY);
  float rewardTempDown = getRewardTemp(cordRightX, cordRightY, cordLeftX, cordLeftY, cordDownX, cordDownY);
  float maxReward = max(max(rewardTempUp, rewardTempLeft), max(rewardTempRight, rewardTempDown));
  float discountedReward = reward + 1.0*maxReward;
  //cout << "discountedReward is: " << discountedReward << endl;
  newUtilitiesMap.insert(std::pair<state*, float>(this, discountedReward));
  newUtilitiesMap.find(this)->second = discountedReward;

  //return (reward + maxReward);


  //cout << "done" << endl;
  /*
  int reward = a->getReward();
  map<vector<int>, float> neighbors;
  neighbors = a->getNeighboringStates(a);
  map<vector<int>, float>::iterator iterStates;
  vector<int> coords;
  float tempProb;
  for (iterStates = neighbors.begin(); iterStates != neighbors.end(); iterStates++){
    coords = iterStates->first;
    tempProb = iterStates->second;
    int a1 = coords[0];
    cout << a1 << endl;
    cout << "te: " << tempProb << endl;
    cout << "uuu: " << a->getUtility(0,1) << endl;
    */
    //cout << "Util: " << (this->stateCoords.find("01")->second) << endl;
    //cout << "Util1: " << (stateCoords.find("0,0")->second)->getUtility() << endl;
    //cout << "Util2: " << (stateCoords.find("0,1")->second)->getUtility() << endl;
    //return 1;
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
