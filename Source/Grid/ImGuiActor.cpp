// Fill out your copyright notice in the Description page of Project Settings.


#include "ImGuiActor.h"
#include <imgui.h>

// Sets default values
AImGuiActor::AImGuiActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AImGuiActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AImGuiActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ImGui::ShowDemoWindow();
	// ImGui::ShowMetricsWindow();

	// WARNING: Actor's only have labels in Development builds.
	// If needed in a production build, each ImgGuiActor will need
	// a different unique identifier for their ImgGui windows.
	// const FString Label = GetActorLabel();

	static ImVec4 Color;


	ImGui::Begin("Settings ");
	const char* Items[] = {"Classic", "Light", "Dark"};
	static int Item_Current = 2;
	if (ImGui::Combo("Editor Theme", &Item_Current, Items, IM_ARRAYSIZE(Items)))
	{
		switch (Item_Current)
		{
		case 0:
			ImGui::StyleColorsClassic();
			break;
		case 1:
			ImGui::StyleColorsLight();
			break;
		case 2:
			ImGui::StyleColorsDark();
			break;
		default:
			break;
		}
	}

	ImGui::Separator();
	ImGui::Text("Unreal Engine Version: 5.03");

	static bool bIsDrawWireframe = true;
	static bool isDrawAABB = true;
	static bool isWireframe = true;
	static bool isTree = true;
	static bool isNPC = true;

	if (ImGui::Checkbox("Draw Wireframe", &bIsDrawWireframe))
	{
	}

	if (ImGui::Checkbox("Draw AABB", &isDrawAABB))
	{
	}

	if (ImGui::Checkbox("Model Wireframe", &isWireframe))
	{
	}
	ImGui::Separator();

	ImGui::Checkbox("Tree", &isTree);
	ImGui::Checkbox("NPC", &isNPC);
	ImGui::ColorEdit4("Draw Color", reinterpret_cast<float*>(&Color));

	ImGui::Separator();
	ImGui::Text("Camera Mode");
	static int modeCamera = 0;
	if (ImGui::RadioButton("First Person", &modeCamera, 0))
	{
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Third Person", &modeCamera, 1))
	{
	}
	ImGui::Separator();

	ImGui::Text("Vitual Reality");


	ImGui::SameLine(240);

	ImGui::End();


	// Update this actor's scale if the Scale variable has changed
	// by way of the ImGui slider.
	if (Scale != ScaleOld)
	{
		ScaleOld = Scale;
		SetActorScale3D(FVector3d{Scale});
	}
}
