#include "EntityManager.h"
using namespace Simplex;
EntityManager* EntityManager::myInstance = nullptr;

MyEntity * Simplex::EntityManager::getEntity(String id)
{
	for (uint i = 0; i < this->entityList.size(); i++) {
		if (this->entityList[i]->GetUniqueID == id) {
			return this->entityList[i];
		}
	}
	return nullptr;
}

RigidBody * Simplex::EntityManager::getRigidBody(String id)
{
	for (uint i = 0; i < this->entityList.size(); i++) {
		if (this->entityList[i]->GetUniqueID == id) {
			return this->entityList[i]->GetRigidBody();
		}
	}
	return nullptr;
}

Model * Simplex::EntityManager::getModel(String id)
{
	for (uint i = 0; i < this->entityList.size(); i++) {
		if (this->entityList[i]->GetUniqueID == id) {
			return this->entityList[i]->GetModel();
		}
	}
	return nullptr;
}

void Simplex::EntityManager::removeEntity(String id)
{
	MyEntity* entity = this->getEntity(id);
	std::vector<MyEntity*>::iterator itr = this->entityList.begin();
	while (itr != this->entityList.end())
	{
		if (*itr == entity) {
			this->entityList.erase(itr);
		}
		++itr;
	}
}

int Simplex::EntityManager::getCount()
{
	return this->entityList.size();
}

void Simplex::EntityManager::addEntity(MyEntity* entity)
{
	this->entityList.push_back(entity);
}

void Simplex::EntityManager::addToRenderList(String id, bool visible)
{
	MyEntity* entity = this->getEntity(id);
	entity->AddToRenderList(visible);
}

EntityManager & Simplex::EntityManager::operator=(EntityManager const & other)
{
	return *this;
}

Simplex::EntityManager::~EntityManager(void)
{
	for (uint i = 0; i < this->entityList.size(); i++)
	{
		MyEntity* entity = this->entityList[i];
		delete(entity);
	}
	this->entityList.clear();
}
