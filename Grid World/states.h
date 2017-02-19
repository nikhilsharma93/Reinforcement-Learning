/*
  states.h: Header file for class state and the revlevant functions
  @author Nikhil Sharma
*/

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>

using namespace std;

struct coords{
  int x;
  int y;
};

class state{
public:
  state(int, int, float, int, float);
  int getStateCat();
  float getReward();
  float getUtility();
  void updateUtility();
  void setNewUtility(float);
  string getCoords();
  int getXCord();
  int getYCord();
  void setOptimalPolicy();
private:
  int xCord;
  int yCord;
  float reward;
  int stateCat;
  float utility;
};

void getNeighboringStates(int, int, int*, int*, int*, int*, int*, int*, int*, int*);
float getRewardTemp(int, int, int, int, int, int);
string IntToString (int);
extern map<state*, float> newUtilitiesMap;
