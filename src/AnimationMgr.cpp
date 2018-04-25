/*
 * AnimationMgr.cpp
 *
 *  Created on: Apr 24, 2018
 *      Author: zachcooper
 */

#include <AnimationMgr.h>

AnimationMgr::AnimationMgr(Engine *engine): Mgr(engine) {
	//mWalkSpeed = 5.0;
	mAnimation = 0;
	dirVec = Ogre::Vector3::ZERO;
}

AnimationMgr::~AnimationMgr(){

}

void AnimationMgr::Init(){
}

void AnimationMgr::LoadLevel(){

}

void AnimationMgr::Tick(float dt){

	if (engine->inputMgr->mKeyboard->isKeyDown(OIS::KC_UP)){
		for (unsigned int i = 0; i < engine->entityMgr->entities.size(); i ++){
			if (engine->entityMgr->entities[i]->meshfilename == "robot.mesh"){
				mAnimation = engine->entityMgr->entities[i]->ogreEntity->getAnimationState("Walk");
				mAnimation->setLoop(true);
				mAnimation->setEnabled(true);
				mAnimation->addTime(dt);

				engine->entityMgr->entities[i]->position.x += .2;

				dirVec = engine->entityMgr->entities[i]->position + engine->entityMgr->entities[i]->mWalkSpeed;
				engine->entityMgr->entities[i]->sceneNode->translate(dirVec * dt);
			}
		}
	}

	if (engine->inputMgr->mKeyboard->isKeyDown(OIS::KC_RIGHT)){
		for (unsigned int i = 0; i < engine->entityMgr->entities.size(); i ++){
			if (engine->entityMgr->entities[i]->meshfilename == "robot.mesh"){
				mAnimation = engine->entityMgr->entities[i]->ogreEntity->getAnimationState("Walk");
				mAnimation->setLoop(true);
				mAnimation->setEnabled(true);

				engine->entityMgr->entities[i]->position.z += .2;

				dirVec = engine->entityMgr->entities[i]->position + engine->entityMgr->entities[i]->mWalkSpeed;
				engine->entityMgr->entities[i]->sceneNode->translate(dirVec * dt);

				mAnimation->addTime(dt);
			}
		}
	}

	if (engine->inputMgr->mKeyboard->isKeyDown(OIS::KC_LEFT)){
		for (unsigned int i = 0; i < engine->entityMgr->entities.size(); i ++){
			if (engine->entityMgr->entities[i]->meshfilename == "robot.mesh"){
				mAnimation = engine->entityMgr->entities[i]->ogreEntity->getAnimationState("Walk");
				mAnimation->setLoop(true);
				mAnimation->setEnabled(true);

				engine->entityMgr->entities[i]->position.z -= .2;

				dirVec = engine->entityMgr->entities[i]->position + engine->entityMgr->entities[i]->mWalkSpeed;
				engine->entityMgr->entities[i]->sceneNode->translate(dirVec * dt);

				mAnimation->addTime(dt);
			}
		}
	}

	if (engine->inputMgr->mKeyboard->isKeyDown(OIS::KC_DOWN)){
		for (unsigned int i = 0; i < engine->entityMgr->entities.size(); i ++){
			if (engine->entityMgr->entities[i]->meshfilename == "robot.mesh"){
				mAnimation = engine->entityMgr->entities[i]->ogreEntity->getAnimationState("Walk");
				mAnimation->setLoop(true);
				mAnimation->setEnabled(true);

				engine->entityMgr->entities[i]->position.x -= .2;

				dirVec = engine->entityMgr->entities[i]->position + engine->entityMgr->entities[i]->mWalkSpeed;
				engine->entityMgr->entities[i]->sceneNode->translate(dirVec * dt);

				mAnimation->addTime(dt);
			}
		}
	}
}

void AnimationMgr::Stop(){

}

void AnimationMgr::createFrameListener(void){

}

bool AnimationMgr::frameRenderingQueued(const Ogre::FrameEvent &evt){

	return true;
}

bool AnimationMgr::keyPressed(const OIS::KeyEvent& ke){

	return true;
}

bool AnimationMgr::keyReleased(const OIS::KeyEvent& ke){
	  switch (ke.key)
	  {
	  case OIS::KC_1:
			for (unsigned int i = 0; i < engine->entityMgr->entities.size(); i ++){
				if (engine->entityMgr->entities[i]->meshfilename == "robot.mesh"){

				}
			}
	    break;

	  default:
		  break;
	  }
	return true;
}

bool AnimationMgr::nextLocation(void){

	return true;
}

bool AnimationMgr::frameStarted(const Ogre::FrameEvent& evt){

	return true;
}

bool AnimationMgr::frameEnded(const Ogre::FrameEvent& evt){

	return true;
}


