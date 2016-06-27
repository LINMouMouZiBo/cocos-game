#include "GameOverScene.h"
#include "GameScene.h"
#include "MainMenuScene.h"
#include "Definitions.h"

USING_NS_CC;

unsigned int score;
int money;
Scene* GameOverScene::createScene( unsigned int tempScore, int moneyCount)
{
    score = tempScore;
	money = moneyCount;
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameOverScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameOverScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto backgroundSprite = Sprite::create("bg.jpg");
	backgroundSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	backgroundSprite->setPosition(ccp(size.width / 2, size.height / 2));
	float winw = size.width; //��ȡ��Ļ���
	float winh = size.height;//��ȡ��Ļ�߶�

	float spx = backgroundSprite->getTextureRect().getMaxX();
	float spy = backgroundSprite->getTextureRect().getMaxY();

	backgroundSprite->setScaleX(winw / spx); //���þ��������ű���
	backgroundSprite->setScaleY(winh / spy);
    
    this->addChild( backgroundSprite );

	//���¿�ʼ��ť
    auto retryItem = MenuItemImage::create( "Retry Button.png", "Retry Button Clicked.png", CC_CALLBACK_1( GameOverScene::GoToGameScene, this ) );
    retryItem->setPosition( Point( visibleSize.width / 2 + origin.x, visibleSize.height / 4 * 3 ) );

	//�ص������水ť
    auto mainMenuItem = MenuItemImage::create( "Menu Button.png", "Menu Button Clicked.png", CC_CALLBACK_1( GameOverScene::GoToMainMenuScene, this ) );
    mainMenuItem->setPosition( Point( visibleSize.width / 2 + origin.x, visibleSize.height / 4 ) );
    
    auto menu = Menu::create( retryItem, mainMenuItem, NULL );
    menu->setPosition( Point::ZERO );
    
    this->addChild( menu );

	//���ε÷��Լ��񵽵�Ǯ�����ȱ��ؼ�¼�󣬾͸��±��ؼ�¼
    UserDefault *def = UserDefault::getInstance( );
    
    auto highScore = def->getIntegerForKey( "HIGHSCORE FLAPPY Score", 0 );
    
    if ( score > highScore )
    {
        highScore = score;
        
        def->setIntegerForKey( "HIGHSCORE FLAPPY Score", highScore );
    }

	auto highMoneyCount = def->getIntegerForKey("HIGHSCORE FLAPPY Money", 0);

	if (money > highMoneyCount) {
		highMoneyCount = money;

		def->setIntegerForKey("HIGHSCORE FLAPPY Money", highMoneyCount);
	}
    
    def->flush( );
    //���ε÷�
    __String *tempScore = __String::createWithFormat( "score: %i", score );
    
    auto currentScore = LabelTTF::create( tempScore->getCString( ), "fonts/Marker Felt.ttf", visibleSize.height * SCORE_FONT_SIZE );
    currentScore->setPosition( Point( visibleSize.width * 0.25 + origin.x, visibleSize.height / 2 + origin.y + 40) );
    
    this->addChild( currentScore );
	//��ʷ��ߵ÷�
    __String *tempHighScore = __String::createWithFormat( "%i", highScore );
    
    auto highScoreLabel = LabelTTF::create( tempHighScore->getCString( ), "fonts/Marker Felt.ttf", visibleSize.height * SCORE_FONT_SIZE );
    
    highScoreLabel->setColor( Color3B::YELLOW );
    highScoreLabel->setPosition( Point( visibleSize.width * 0.75 + origin.x, visibleSize.height / 2 + origin.y + 40) );

    this->addChild( highScoreLabel );
	
	//���ν��
	__String *tempMoney = __String::createWithFormat("money: %i", money);

	auto currentMoney = LabelTTF::create(tempMoney->getCString(), "fonts/Marker Felt.ttf", visibleSize.height * SCORE_FONT_SIZE);
	currentMoney->setPosition(Point(visibleSize.width * 0.25 + origin.x, visibleSize.height / 2 + origin.y - 40));

	this->addChild(currentMoney);
	//��ʷ��߽��
	__String *tempHighMoney = __String::createWithFormat("%i", highMoneyCount);

	auto highMoneyLabel = LabelTTF::create(tempHighMoney->getCString(), "fonts/Marker Felt.ttf", visibleSize.height * SCORE_FONT_SIZE);

	highMoneyLabel->setColor(Color3B::YELLOW);
	highMoneyLabel->setPosition(Point(visibleSize.width * 0.75 + origin.x, visibleSize.height / 2 + origin.y - 40));

	this->addChild(highMoneyLabel);
    
    return true;
}

//�ص�������
void GameOverScene::GoToMainMenuScene( cocos2d::Ref *sender )
{
    auto scene = MainMenuScene::createScene( );
    
    Director::getInstance( )->replaceScene( TransitionFade::create( TRANSITION_TIME, scene ) );
}

//���¿�ʼ��Ϸ����
void GameOverScene::GoToGameScene( cocos2d::Ref *sender )
{
    auto scene = GameScene::createScene( );
    
    Director::getInstance( )->replaceScene( TransitionFade::create( TRANSITION_TIME, scene ) );
}







