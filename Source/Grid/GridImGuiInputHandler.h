// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ImGuiInputHandler.h"
#include "GridImGuiInputHandler.generated.h"

/**
 * 
 */
UCLASS()
class GRID_API UGridImGuiInputHandler : public UImGuiInputHandler
{
	GENERATED_BODY()
public:
	virtual FReply OnKeyChar(const FCharacterEvent& CharacterEvent) override;
	virtual FReply OnKeyDown(const FKeyEvent& KeyEvent) override;
	virtual FReply OnKeyUp(const FKeyEvent& KeyEvent) override;
	virtual void OnKeyboardInputEnabled() override;
	virtual void OnKeyboardInputDisabled() override;
	virtual void OnGamepadInputEnabled() override;
	virtual void OnGamepadInputDisabled() override;
	virtual void OnMouseInputEnabled() override;
	virtual void OnMouseInputDisabled() override;
};
