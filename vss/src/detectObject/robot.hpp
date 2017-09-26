#pragma once
#include <string>

using namespace std;

class robot{
public:
    robot(void);
    ~robot(void);
    int getxpos();
    void setxpos(int x);
    int getypos();
    void setypos(int y);

private:
    int xpos,ypos;
    string type;
};
