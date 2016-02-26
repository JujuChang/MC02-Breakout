#pragma once

#ifndef __gamename__GameOverScene__
#define __gamename__GameOverScene__

#include "cocos2d.h"
using namespace std;


class GameOverScene : public cocos2d::Scene
{
public:
	bool init();
	CREATE_FUNC(GameOverScene);

	
	void showMessage(string, bool);
};

#endif