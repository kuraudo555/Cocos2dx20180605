#include "BulletSprite.h"
#include "PlaneLayer.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;


BulletSprite::BulletSprite() {
}
BulletSprite::~BulletSprite() {
}
bool BulletSprite::init() {
    if (!Layer::init()) {
		return false;
	}
	this->schedule(schedule_selector(BulletSprite::ShootBullet),0.1f);
	return true;
}
void BulletSprite::ShootBullet(float dt) {
	Size winSize = Director::getInstance()->getWinSize();
	auto PlanePos =
			PlaneLayer::sharedPlane->getChildByTag(AIRPLANE)->getPosition();
	auto spritebullet1 = Sprite::createWithSpriteFrameName("bullet_1.png");
    auto spritebullet2 = Sprite::createWithSpriteFrameName("bullet_1.png");
//	(SpriteFrameCache::getInstance()->getSpriteFrameByName("bullet_1.png"));
	this->addChild(spritebullet1);
    this->addChild(spritebullet2);
	auto self = f_createAnimate(2, 10);
	spritebullet1->runAction(RepeatForever::create(Animate::create(self)));
	Point bulletPos1 =
			(Point(PlanePos.x - 5,
					PlanePos.y
							+ PlaneLayer::sharedPlane->getChildByTag(AIRPLANE)->getContentSize().height
									/ 2 + 15));
    Point bulletPos2 = (Point(PlanePos.x + 5,PlanePos.y + PlaneLayer::sharedPlane->getChildByTag(AIRPLANE)->getContentSize().height / 2 + 15));
    
	spritebullet1->setPosition(bulletPos1);
	spritebullet1->setScale(0.4f);
    spritebullet2->setPosition(bulletPos2);
    spritebullet2->setScale(0.4f);
	vecBullet.pushBack(spritebullet1);
    vecBullet.pushBack(spritebullet2);
	float flyLen = winSize.height - PlanePos.y;
	float flyVelocity = 320 / 1;
    float realFlyDuration = flyLen / flyVelocity;

	auto actionMove1 = MoveTo::create(realFlyDuration,
			Point(bulletPos1.x, winSize.height));
	auto actionDone1 = CallFuncN::create(
			CC_CALLBACK_1(BulletSprite::removeBullet, this));
    
    auto actionMove2 = MoveTo::create(realFlyDuration, Point(bulletPos2.x,winSize.height));
    auto actionDone2 = CallFuncN::create(CC_CALLBACK_1(BulletSprite::removeBullet, this));

	Sequence* sequence1 = Sequence::create(actionMove1, actionDone1, NULL);
    Sequence* sequence2 = Sequence::create(actionMove2, actionDone2, NULL);
	spritebullet1->runAction(sequence1);
    spritebullet2->runAction(sequence2);

}

void BulletSprite::removeBullet(Node* pNode) {
	if (NULL == pNode) {
		return;
	}
	Sprite* bullet = (Sprite*) pNode;
	this->removeChild(bullet, true);
	vecBullet.eraseObject(bullet);
}

Animation* BulletSprite::f_createAnimate(int count, int fps) {
	char buff[16];
	Animation *panimation = Animation::create();
	panimation->setDelayPerUnit(1.0 / fps);
	for (int id = 1; id <= count; id++) {
		sprintf(buff, "bullet_%d.png", id);
		panimation->addSpriteFrame(
				SpriteFrameCache::getInstance()->getSpriteFrameByName(buff));
	}
    SimpleAudioEngine::getInstance()->playEffect("Music/fireEffect.mp3");
	return panimation;
}

