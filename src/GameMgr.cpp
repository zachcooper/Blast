/*
 * GameMgr.cpp
 *
 *  Created on: Mar 4, 2018
 *      Author: sushil
 */
#include <Engine.h>

#include <EntityMgr.h>
#include <GameMgr.h>
#include <GfxMgr.h>

#include <iostream>
#include <Types381.h>

#include <OgreOverlay.h>
#include <OgreSceneNode.h>
#include <OgreMeshManager.h>
#include <OgreManualObject.h>
#include <OgreStaticGeometry.h>

GameMgr::GameMgr(Engine *engine): Mgr(engine) {
	cameraNode = 0;
}

GameMgr::~GameMgr() {
	// TODO Auto-generated destructor stub
}

void GameMgr::Init(){

}

void GameMgr::LoadLevel(){
    //CEGUI::OgreRenderer* mRenderer;

	  engine->gfxMgr->mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

	  engine->gfxMgr->mCamera->lookAt(Ogre::Vector3(0, 0, 0));
	  Ogre::Light* light = engine->gfxMgr->mSceneMgr->createLight("MainLight");
	  light->setPosition(20.0, 80.0, 50.0);

	  // A node to attach the camera to so we can move the camera node instead of the camera.
	  cameraNode = engine->gfxMgr->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	  cameraNode->setPosition(0, 200, 500);
	  cameraNode->attachObject(engine->gfxMgr->mCamera);

	  MakeGround();
	  MakeSky();
	  MakeEntities();
	  createGrassMesh();
}

void GameMgr::MakeEntities(){
	Ogre::Vector3 pos = Ogre::Vector3(-1000, 0, 0);
	Ogre::Vector3 position = Ogre::Vector3(0, 0, 0);

	for (int i = 0; i < 10; i++){
		engine->entityMgr->CreateEntityOfTypeAtPosition(RobotType, position);
		position.x += 100;
	}

	pos.x += 500;
	engine->entityMgr->CreateEntityOfTypeAtPosition(CarrierType, pos);

	engine->entityMgr->SelectNextEntity(true); //sets selection
}

void GameMgr::MakeGround(){

	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);

	  Ogre::MeshManager::getSingleton().createPlane(
	    "ground",
	    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	    plane,
	    15000, 15000, 20, 20,
	    true,
	    1, 5, 5,
	    Ogre::Vector3::UNIT_Z);

	  Ogre::Entity* groundEntity = engine->gfxMgr->mSceneMgr->createEntity("ground");
	  engine->gfxMgr->mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);
	  groundEntity->setCastShadows(false);
	  //groundEntity->setMaterialName("Ocean2_HLSL_GLSL");
	  //groundEntity->setMaterialName("OceanHLSL_GLSL");
	  groundEntity->setMaterialName("Examples/GrassFloor");
	  //groundEntity->setMaterialName("NavyCg");

	  myPlane = plane;
}

void GameMgr::MakeSky(){
	engine->gfxMgr->mSceneMgr->setSkyDome(true, "Examples/CloudySky");
}

void GameMgr::createGrassMesh()
{
	const float width = 25;
	const float height = 30;
	Ogre::Vector3 vec(width / 2, 0, 0);
	Ogre::ManualObject obj("GrassObject");

	Ogre::Quaternion quat;
	quat.FromAngleAxis(Ogre::Degree(60), Ogre::Vector3::UNIT_Y);

	obj.begin("Examples/GrassBlades", Ogre::RenderOperation::OT_TRIANGLE_LIST);

	for (int i = 0; i < 3; ++i)
	{
		obj.position(-vec.x, height, -vec.z);
		obj.textureCoord(0, 0);
		obj.position(vec.x, height, vec.z);
		obj.textureCoord(1, 0);
		obj.position(-vec.x, 0, -vec.z);
		obj.textureCoord(0, 1);
		obj.position(vec.x, 0, vec.z);
		obj.textureCoord(1, 1);

		int offset = 4 * i;
		obj.triangle(offset, offset + 3, offset + 1);
		obj.triangle(offset, offset + 2, offset + 3);

		vec = quat * vec;
	}

	obj.end();
	obj.convertToMesh("GrassBladesMesh");

	Ogre::Entity* grass = engine->gfxMgr->mSceneMgr->createEntity("GrassBladesMesh");
	Ogre::StaticGeometry* sg = engine->gfxMgr->mSceneMgr->createStaticGeometry("GrassArea");

	const int size = 15000;
	const int amount = 200;

	sg->setRegionDimensions(Ogre::Vector3(size, size, size));
	sg->setOrigin(Ogre::Vector3(-size/2, 0, -size/2));

	for (int x = -size/2; x < size/2; x += (size / amount))
	{
	  for (int z = -size/2; z < size/2; z += (size / amount))
	  {
		Ogre::Real r = size / (float)amount / 2;
		Ogre::Vector3 pos(
	x + Ogre::Math::RangeRandom(-r, r),
	0,
	z + Ogre::Math::RangeRandom(-r, r));

		Ogre::Vector3 scale(1, Ogre::Math::RangeRandom(0.9, 1.1), 1);

		Ogre::Quaternion orientation;
		orientation.FromAngleAxis(
	Ogre::Degree(Ogre::Math::RangeRandom(0, 359)),
	Ogre::Vector3::UNIT_Y);

		sg->addEntity(grass, pos, orientation, scale);
	  }
	}
	sg->build();
}
