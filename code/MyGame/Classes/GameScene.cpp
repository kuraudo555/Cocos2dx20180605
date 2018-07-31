#include "GameScene.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

cocos2d::Scene* GameLayer::createScene() {
	auto scene = Scene::create();
	auto layer = GameLayer::create();
	scene->addChild(layer);
	return scene;
}

bool GameLayer::init() {
	if (!Layer::init()) {
		return false;
	}
    this->setTouchEnabled(true);
    Size winSize = Director::getInstance()->getWinSize();
    char buff[15];
    int id = getRand(1, 5);
    sprintf(buff, "img_bg_%d.jpg", id);
    auto bg1 = Sprite::create(buff);
    //bg1坐标在中间
    bg1->setPosition(Point(winSize.width / 2, winSize.height / 2));
    bg1->setScale(1.1);
    bg1->setTag(10);
    this->addChild(bg1);
    auto bg2 = Sprite::create(buff);
    //第二张背景图在第一张上面，坐标在屏幕外
    bg2->setPosition(Point(winSize.width / 2, winSize.height / 2 + bg1->getContentSize().height * 1.1));
    bg2->setScale(1.1);
    bg2->setTag(11);
    this->addChild(bg2);
    this->schedule(schedule_selector(GameLayer::update));
    
    //添加背景音乐
    auto audioengin = SimpleAudioEngine::getInstance();
    char mbuff[15];
    sprintf(mbuff, "Music/bg_0%d.mp3",id);
    audioengin->preloadBackgroundMusic(mbuff);
    audioengin->playBackgroundMusic(mbuff,true);
    audioengin->preloadEffect("Music/fireEffect.mp3");
    audioengin->preloadEffect("Music/explodeEffect.mp3");
    
	return true;
}
int GameLayer::getRand(int start, int end) {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	unsigned long int rand_seed = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	srand(rand_seed);
	float i = CCRANDOM_0_1() * (end - start + 1) + start;
	return (int) i;
}

void GameLayer::onEnterTransitionDidFinish() {
	planeLayer = PlaneLayer::create();
	this->addChild(planeLayer);
	bulletSprite = BulletSprite::create();
	this->addChild(bulletSprite);
	enemyLayer = EnemyLayer::create();
	this->addChild(enemyLayer);

	this->schedule(schedule_selector(GameLayer::gameUpdate));

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();
	auto closeItem = MenuItemImage::create("CloseNormal.png",
			"CloseSelected.png",
			CC_CALLBACK_1(GameLayer::menuCloseCallback, this));
	closeItem->setPosition(
			Point(
					origin.x + visibleSize.width
							- closeItem->getContentSize().width / 2,
					origin.y + closeItem->getContentSize().height / 2));
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu, 1);
}

void GameLayer::menuCloseCallback(Ref* pSender) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
	return;
#endif
	Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
bool GameLayer::bulletCollisionEnemy(Sprite* pBullet) {
	for (auto& eEnemy : enemyLayer->vecEnemy) {
		EnemySprite* pEnemySprite = (EnemySprite*) eEnemy;
		if (pBullet->getBoundingBox().intersectsRect(
				pEnemySprite->getBoundingBox())) {
			if (1 == pEnemySprite->getLife()) {
				pEnemySprite->loseLife();
				enemyLayer->blowupEnemy(pEnemySprite);
			} else {
				pEnemySprite->loseLife();
			}
			bulletSprite->removeBullet(pBullet);
			return true;
		}
	}
	return false;
}

void GameLayer::gameUpdate(float dt) {
	bool bMoveButt = false;
	for (auto& eButtle : bulletSprite->vecBullet) {
		Sprite* pBullet = (Sprite*) eButtle;
		bMoveButt = bulletCollisionEnemy(pBullet);
		if (bMoveButt) {
			return;
		}
	}
	enemyCollisionPlane();
}

bool GameLayer::enemyCollisionPlane() {
	Sprite* pPlane = (Sprite*) planeLayer->getChildByTag(AIRPLANE);
	for (auto& eEnemy : enemyLayer->vecEnemy) {
		EnemySprite* pEnemySprite = (EnemySprite*) eEnemy;
		if (pPlane->boundingBox().intersectsRect(pEnemySprite->getBoundingBox())
				&& pEnemySprite->getLife() > 0) {
//            this->unscheduleAllSelectors();
			//	this->bulletLayer->StopBulletShoot();
			//	this->planeLayer->blowUp();
			return true;
		}
	}

	return false;
}

void GameLayer::update(float t){
    auto bg1 = this->getChildByTag(10);
    auto bg2 = this->getChildByTag(11);
    auto x1 = bg1->getPositionY();
    auto x2 = bg2->getPositionY();
    float speed = 0.8;
    x1 -= speed;
    x2 -= speed;
    Size size = bg1->getContentSize() * 1.1;
    if (x1 < -size.height / 2) {
        x2 = size.height / 2;
        x1 = size.height / 2 + size.height;
    }
    if (x2 < -size.height / 2) {
        x1 = size.height / 2;
        x2 = size.height / 2 + size.height;
    }
    bg1->setPositionY(x1);
    bg2->setPositionY(x2);
}















