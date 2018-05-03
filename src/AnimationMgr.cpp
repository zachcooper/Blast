/*
 * AnimationMgr.cpp
 *
 *  Created on: Apr 24, 2018
 *      Author: zachcooper
 */

#include <AnimationMgr.h>
#include <cstdlib>
#include <ctime>

AnimationMgr::AnimationMgr(Engine *engine): Mgr(engine) {
    mWalkSpeed = 70.0;

}

AnimationMgr::~AnimationMgr(){

}

void AnimationMgr::Init(){
	srand(time(NULL));
    mWalkList.push_back(Ogre::Vector3(550.0, 0, 50.0));
    mWalkList.push_back(Ogre::Vector3(-100.0, 0, -200.0));
    mWalkList.push_back(Ogre::Vector3(0, 0, 25.0));
}

void AnimationMgr::LoadLevel(){

}

bool AnimationMgr::nextLocation(void){
	for (unsigned int i = 0; i < engine->entityMgr->entities.size(); i ++){
		if (engine->entityMgr->entities[i]->meshfilename == "robot.mesh"){
			float rand1  = rand()%2000 - 200;
			float rand2 = 0.0;
			float rand3 = rand()%2000 - 200;
			engine->entityMgr->entities[i]->mDestination = Ogre::Vector3(rand1, rand2, rand3);
		}
	}

	for (unsigned int i = 0; i < engine->entityMgr->entities.size(); i ++){
		if (engine->entityMgr->entities[i]->meshfilename == "robot.mesh"){
			engine->entityMgr->entities[i]->mDirection = engine->entityMgr->entities[i]->mDestination - engine->entityMgr->entities[i]->sceneNode->getPosition();
			engine->entityMgr->entities[i]->mDistance = engine->entityMgr->entities[i]->mDirection.normalise();
		}
	}
    return true;
}

void AnimationMgr::createFrameListener(void){

}

void AnimationMgr::Tick(float dt){

	for (unsigned int i = 0; i < engine->entityMgr->entities.size(); i ++){
		if (engine->entityMgr->entities[i]->meshfilename == "robot.mesh"){

			engine->entityMgr->entities[i]->mAnimation = engine->entityMgr->entities[i]->ogreEntity->getAnimationState("Walk");
			engine->entityMgr->entities[i]->mAnimation->setLoop(true);
			engine->entityMgr->entities[i]->mAnimation->setEnabled(true);
			engine->entityMgr->entities[i]->mAnimation->addTime(dt);

			Ogre::Real move = mWalkSpeed * dt;
			engine->entityMgr->entities[i]->mDistance -= move;

			if (engine->entityMgr->entities[i]->mDistance <= 0.0f){
				engine->entityMgr->entities[i]->sceneNode->setPosition(engine->entityMgr->entities[i]->mDestination);
				engine->entityMgr->entities[i]->mDirection = Ogre::Vector3::ZERO;
				// Set animation based on if the robot has another point to walk to.
				if (!nextLocation()){
					// Set Idle animation
					engine->entityMgr->entities[i]->mAnimation = engine->entityMgr->entities[i]->ogreEntity->getAnimationState("Idle");
					engine->entityMgr->entities[i]->mAnimation->setLoop(true);
					engine->entityMgr->entities[i]->mAnimation->setEnabled(true);
					engine->entityMgr->entities[i]->mAnimation->addTime(dt);
				}else{
					// Rotation Code will go here later
					Ogre::Vector3 src = engine->entityMgr->entities[i]->sceneNode->getOrientation() * Ogre::Vector3::UNIT_X;

					if ((1.0f + src.dotProduct(engine->entityMgr->entities[i]->mDirection)) < 0.0001f) {
						engine->entityMgr->entities[i]->sceneNode->yaw(Ogre::Degree(180));

					}else{
						Ogre::Quaternion quat = src.getRotationTo(engine->entityMgr->entities[i]->mDirection);
						engine->entityMgr->entities[i]->sceneNode->rotate(quat);

					} // else
				}//else
			}else{
				engine->entityMgr->entities[i]->sceneNode->translate(engine->entityMgr->entities[i]->mDirection * move);
				engine->entityMgr->entities[i]->position += engine->entityMgr->entities[i]->mDirection * move;
			}
		}
	}
}

void AnimationMgr::Stop(){

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

bool AnimationMgr::frameStarted(const Ogre::FrameEvent& evt){

	return true;
}

bool AnimationMgr::frameEnded(const Ogre::FrameEvent& evt){

	return true;
}


