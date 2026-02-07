#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include "BobAndSpin.h"
#include "Camera.h"
#include "GameObject.h"
#include "MouseLook.h"
#include "Movement.h"
#include "Scene.h"
#include "SceneBuilder.h"
#include "Transform.h"
#include "VoxelObject.h"
namespace SceneBuilder
{
	Scene* TestScene()
	{
		Scene* scene = new Scene();

		GameObject* gameObject = scene->InstantiateGameObject();

		Transform* transform = scene->InstantiateComponent<Transform>();
		gameObject->AddComponent(transform);
		transform->SetLocalPosition(glm::vec3(0, 0, 1), true);
		transform->Enable();

		Camera* camera = scene->InstantiateComponent<Camera>();
		gameObject->AddComponent(camera);
		camera->SetFovDegrees(90);
		camera->Enable();

		MouseLook* mouseLook = scene->InstantiateComponent<MouseLook>();
		gameObject->AddComponent(mouseLook);
		mouseLook->Enable();

		Movement* movement = scene->InstantiateComponent<Movement>();
		gameObject->AddComponent(movement);
		movement->Enable();


		gameObject->Enable();


		gameObject = scene->InstantiateGameObject();

		transform = scene->InstantiateComponent<Transform>();
		Transform* spinnyTransform = transform;
		gameObject->AddComponent(transform);
		transform->SetLocalPosition(glm::vec3(0, 0, 0), true);
		transform->Enable();

		VoxelObject* voxelObject = scene->InstantiateComponent<VoxelObject>();
		gameObject->AddComponent(voxelObject);
		voxelObject->Enable();

		BobAndSpin* bobAndSpin = scene->InstantiateComponent<BobAndSpin>();
		gameObject->AddComponent(bobAndSpin);
		bobAndSpin->Enable();

		gameObject->Enable();


		BVH* bvh = scene->GetBVH();
		GLuint model = bvh->RegisterModelData(glm::ivec3(1, 2, 3));
		bvh->SetOffset(model, glm::vec3(-0.5f, -1.0f, -1.5f));

		voxelObject->SetModelIndex(model);


		gameObject = scene->InstantiateGameObject();

		transform = scene->InstantiateComponent<Transform>();
		gameObject->AddComponent(transform);
		transform->SetLocalPosition(glm::vec3(32, 0, 0), true);
		transform->SetParent(spinnyTransform);
		transform->Enable();

		voxelObject = scene->InstantiateComponent<VoxelObject>();
		gameObject->AddComponent(voxelObject);
		voxelObject->Enable();

		gameObject->Enable();

		model = bvh->RegisterModelData(glm::ivec3(1, 1, 1));
		bvh->SetOffset(model, glm::vec3(-0.5f, -0.5f, -0.5f));

		voxelObject->SetModelIndex(model);

		//for (long long i = 0; i < 1000; i++)
		//{
		//	gameObject = scene->InstantiateGameObject();
		//
		//	transform = scene->InstantiateComponent<Transform>();
		//	gameObject->AddComponent(transform);
		//	transform->SetLocalPosition(glm::ballRand(64.0f), true);
		//	//transform->SetParent(spinnyTransform);
		//	transform->Enable();
		//
		//	voxelObject = scene->InstantiateComponent<VoxelObject>();
		//	gameObject->AddComponent(voxelObject);
		//	voxelObject->Enable();
		//
		//	mouseLook = scene->InstantiateComponent<MouseLook>();
		//	gameObject->AddComponent(mouseLook);
		//	mouseLook->Enable();
		//
		//	gameObject->Enable();
		//
		//	voxelObject->SetModelIndex(model);
		//}

		size_t i = 0;
		for (int x = -1024; x < 1024; x += 256)
		{
			for (int y = -2048; y < -128; y += 256)
			{
				for (int z = -1024; z < 1024; z += 256)
				{
					model = bvh->RegisterModelData(glm::ivec3(128));
					bvh->SetOffset(model, glm::vec3(-128 / 2.0f));

					gameObject = scene->InstantiateGameObject();

					transform = scene->InstantiateComponent<Transform>();
					gameObject->AddComponent(transform);
					transform->SetLocalPosition(glm::vec3(x, y, z), true);
					//transform->SetParent(spinnyTransform);
					transform->Enable();

					voxelObject = scene->InstantiateComponent<VoxelObject>();
					gameObject->AddComponent(voxelObject);
					voxelObject->Enable();

					//mouseLook = scene->InstantiateComponent<MouseLook>();
					//gameObject->AddComponent(mouseLook);
					//mouseLook->Enable();

					gameObject->Enable();

					voxelObject->SetModelIndex(model);
					i++;
				}
			}
		}
		Logger::Log(std::to_string(i));

		//long long chunksize = 128;
		//long long worldsize = 2048;
		//long long objectcount = worldsize / chunksize;
		//objectcount *= objectcount * objectcount;
		//
		//for (long long i = 0; i < objectcount; i++)
		//{
		//	model = bvh->RegisterModelData(glm::ivec3(chunksize));
		//	bvh->SetOffset(model, glm::vec3(-chunksize / 2.0f));
		//
		//	gameObject = scene->InstantiateGameObject();
		//
		//	transform = scene->InstantiateComponent<Transform>();
		//	gameObject->AddComponent(transform);
		//	transform->SetLocalPosition(glm::linearRand(glm::vec3(-worldsize / 2.0f, -256.0f, -worldsize / 2.0f), glm::vec3(worldsize / 2.0f, 256.0f, worldsize / 2.0f)), true);
		//	//transform->SetParent(spinnyTransform);
		//	transform->Enable();
		//
		//	voxelObject = scene->InstantiateComponent<VoxelObject>();
		//	gameObject->AddComponent(voxelObject);
		//	voxelObject->Enable();
		//
		//	//mouseLook = scene->InstantiateComponent<MouseLook>();
		//	//gameObject->AddComponent(mouseLook);
		//	//mouseLook->Enable();
		//
		//	gameObject->Enable();
		//
		//	voxelObject->SetModelIndex(model);
		//}



		return scene;
	}
}