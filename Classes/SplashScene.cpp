#include "SplashScene.h"
#include "MainMenuScene.h"
#include "Definitions.h"
//��Ϸ��ʼ������ҳ�棬ͣ��2����Զ���ת��������
USING_NS_CC;

Scene* SplashScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SplashScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SplashScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//��ʱ������ͣ��2����Զ���ת��������
    this->scheduleOnce( schedule_selector( SplashScene::GoToMainMenuScene ), DISPLAY_TIME_SPLASH_SCENE );

    auto backgroundSprite = Sprite::create( "splash.jpg" );
	Size size = Director::getInstance()->getWinSize();
	backgroundSprite->setPosition(Vec2(size.width / 2, size.height / 2));
	float winw = size.width; //��ȡ��Ļ���
	float winh = size.height;//��ȡ��Ļ�߶�

	float spx = backgroundSprite->getTextureRect().getMaxX();
	float spy = backgroundSprite->getTextureRect().getMaxY();

	backgroundSprite->setScaleX(winw / spx); //���þ��������ű���
	backgroundSprite->setScaleY(winh / spy);

    //backgroundSprite->setPosition( Point( visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y) );
    
    this->addChild( backgroundSprite );
    
    return true;
}
//��ת��������
void SplashScene::GoToMainMenuScene( float dt )
{
    auto scene = MainMenuScene::createScene();
    
    Director::getInstance( )->replaceScene( TransitionFade::create( TRANSITION_TIME, scene ) );
}



