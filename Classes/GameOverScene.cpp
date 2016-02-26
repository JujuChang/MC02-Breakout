#include "GameOverScene.h"
#include "HelloWorldScene.h"
USING_NS_CC;
using namespace std;

bool GameOverScene::init()
{
	return true;
	//if (Scene::init())
	//{
	//	this->_layer = GameOverLayer::create();
	//	this->_layer->retain();
	//	this->addChild(_layer);

	//	return true;
	//}
	//else
	//{
	//	return false;
	//}
}

void GameOverScene::showMessage(string message, bool win){
	auto winSize = Director::getInstance()->getWinSize();
	auto _label = LabelTTF::create(message, "fonts/Middle Ages.ttf", 50);
	_label->retain();
	_label->setColor(Color3B::WHITE);
	
	_label->setPosition(Point(winSize.width / 2, winSize.height / 2));
		
	LabelTTF* _label2 = LabelTTF::create(message , "fonts/Middle Ages.ttf", 51);
	_label2->retain();
	_label2->setColor(Color3B::BLACK);
	_label2->setPosition(_label->getPosition());

	Sprite * sprite;
	if (!win)
		sprite = Sprite::create("deadknight.jpg");
	else
		sprite = Sprite::create("knights.jpg");
	sprite->setScaleY((winSize.height / sprite->getContentSize().height));
	sprite->setScaleX((winSize.width / sprite->getContentSize().width));
	sprite->setAnchorPoint(Vec2(0, 0));
	this->addChild(sprite, 0);
	this->addChild(_label2, 1);
	this->addChild(_label, 2);
}

//GameOverScene::~GameOverScene()
//{
//	if (_layer)
//	{
//		_layer->release();
//		_layer = NULL;
//	}
//}
//
//bool GameOverLayer::init()
//{
//	if (LayerColor::initWithColor(Color4B(255, 255, 255, 255)))
//	{
//
//
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}

//void GameOverLayer::gameOverDone()
//{
//	Director::getInstance()->replaceScene( HelloWorld::createScene() );
//}

//GameOverLayer::~GameOverLayer()
//{
//	if (_label)
//	{
//		_label->release();
//		_label = NULL;
//	}
//}
