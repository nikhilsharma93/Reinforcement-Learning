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
private:
  int xCord;
  int yCord;
  float reward;
  int stateCat;
  float utility;
};

float getRewardTemp(int, int, int, int, int, int);
string IntToString (int);
extern map<state*, float> newUtilitiesMap;
