/*
 * UiMgr.cpp
 *
 *  Created on: Apr 12, 2017
 *      Author: chad
 */

#include <UiMgr.h>
#include <Engine.h>
#include <GfxMgr.h>
#include <InputMgr.h>
#include <EntityMgr.h>
#include <Types381.h>
#include <GameMgr.h>


UiMgr::UiMgr(Engine* eng): Mgr(eng){
	// Initialize the OverlaySystem (changed for Ogre 1.9)
	    mOverlaySystem = new Ogre::OverlaySystem();
	    engine->gfxMgr->mSceneMgr->addRenderQueueListener(mOverlaySystem);

	    //Ogre::WindowEventUtilities::addWindowEventListener(engine->gfxMgr->ogreRenderWindow, this);
	    health = 1.00;
	    enemyHealth = 1.00;
	    score = 0;
}

UiMgr::~UiMgr(){ // before gfxMgr destructor

}

void UiMgr::Init(){
	//init sdktrays

    mInputContext.mKeyboard = engine->inputMgr->mKeyboard;
    mInputContext.mMouse = engine->inputMgr->mMouse;
    mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", engine->gfxMgr->mWindow, mInputContext, this);
    //mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    //mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
    //mTrayMgr->hideCursor();
}

void UiMgr::stop(){

}

void UiMgr::Splash()
{
	mTrayMgr->showLoadingBar();
}

void UiMgr::Hide()
{
	mTrayMgr->hideLoadingBar();
}

void UiMgr::LoadLevel(){

	Splash();
	//Hide();
	mTrayMgr->showBackdrop("myUI");
	healthButton = mTrayMgr->createButton(OgreBites::TL_LEFT, "HealthButton", "Health Drain");
	enemyHealthButton = mTrayMgr->createButton(OgreBites::TL_RIGHT, "EnemyHealthButton", "Enemy Drain");

	//OgreBites::ProgressBar * pbar;
	pbar = mTrayMgr->createProgressBar(OgreBites::TL_BOTTOMLEFT,"HealthBar", "Health", 300, 200);
	pbar->setProgress(health);

	//OgreBites::ProgressBar * ebar;
	ebar = mTrayMgr->createProgressBar(OgreBites::TL_BOTTOMRIGHT,"EnemyHealthBar", "Enemy Health", 350, 200);
	ebar->setProgress(enemyHealth);

	std::stringstream ss2;
	ss2 << score;
	std::string str2 = ss2.str();
	scoreLabel = mTrayMgr->createLabel(OgreBites::TL_TOPRIGHT, "Score", str2, 150.0);



}

void UiMgr::Tick(float dt){
	mTrayMgr->refreshCursor();

	Ogre::Vector3 diff;

	for (unsigned int i = 0; i < engine->entityMgr->entities.size(); i++)
	{
		diff = engine->gameMgr->cameraNode->getPosition() - engine->entityMgr->entities[i]->position;
		if(engine->entityMgr->entities[i] != engine->entityMgr->shipEntity)
		{
			if (diff.length() < 5.0)
			{
				health -= 0.1;
				pbar->setProgress(health);
			}
		}
	}
	if(engine->inputMgr->mKeyboard->isKeyDown(OIS::KC_RETURN))
	{
		mTrayMgr->hideLoadingBar();
	}

	std::stringstream ss2;
	ss2 << score;
	std::string str2 = ss2.str();
	scoreLabel->setCaption(str2);
}


void UiMgr::windowResized(Ogre::RenderWindow* rw){
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState &ms = engine->inputMgr->mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}

void UiMgr::windowClosed(Ogre::RenderWindow* rw){

}

bool UiMgr::keyPressed(const OIS::KeyEvent &arg) {
	std::cout << "Key Pressed: " << arg.key << std::endl;
	return true;
}
bool UiMgr::keyReleased(const OIS::KeyEvent &arg){
	return true;
}
bool UiMgr::mouseMoved(const OIS::MouseEvent &arg){
    if (mTrayMgr->injectMouseMove(arg)) return true;
	return false;
}
bool UiMgr::mousePressed(const OIS::MouseEvent &me, OIS::MouseButtonID mid) {
	std::cout << "mouse clicked" << std::endl;
	if (mTrayMgr->injectMouseDown(me, mid)) return true;
	return false;
}
bool UiMgr::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id){
    if (mTrayMgr->injectMouseUp(arg, id)) return true;
    /* normal mouse processing here... */
	return false;
}

void UiMgr::buttonHit(OgreBites::Button *b){

    if(b->getName()=="HealthButton")
    {
    	health = health - 0.1;
    	pbar->setProgress(health);
    }

    if(b->getName()=="EnemyHealthButton")
    {
    	enemyHealth = enemyHealth - 0.1;
    	ebar->setProgress(enemyHealth);
    }

}


