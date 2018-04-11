#ifndef __ENTITYMANAGER_H_
#define __ENTITYMANAGER_H_

#include "MyEntity.h"

namespace Simplex
{
	class EntityManager
	{
		static EntityManager* myInstance;
		std::vector<MyEntity*> entityList;

	public:
		MyEntity* getEntity(String id);
		RigidBody* getRigidBody(String id);
		Model* getModel(String id);
		void removeEntity(String id);

		int getCount();

		void addEntity(MyEntity* entity);
		void addToRenderList(String id, bool visible);

	private:
		EntityManager(void);
		EntityManager(EntityManager const& other);

		EntityManager& operator=(EntityManager const& other);
		~EntityManager(void);
	};

}

#endif