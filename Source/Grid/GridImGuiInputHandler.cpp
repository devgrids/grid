// Fill out your copyright notice in the Description page of Project Settings.


#include "GridImGuiInputHandler.h"

FReply UGridImGuiInputHandler::OnKeyChar(const FCharacterEvent& CharacterEvent)
{
	return Super::OnKeyChar(CharacterEvent);
}

FReply UGridImGuiInputHandler::OnKeyDown(const FKeyEvent& KeyEvent)
{
	return Super::OnKeyDown(KeyEvent);
}

FReply UGridImGuiInputHandler::OnKeyUp(const FKeyEvent& KeyEvent)
{
	return Super::OnKeyUp(KeyEvent);
}

void UGridImGuiInputHandler::OnKeyboardInputEnabled()
{
	Super::OnKeyboardInputEnabled();
	// UE_LOG(LogTemp, Warning, TEXT("Some warning message"));
	// OnKeyboardInputDisabled();
}

void UGridImGuiInputHandler::OnKeyboardInputDisabled()
{
	Super::OnKeyboardInputDisabled();
}

void UGridImGuiInputHandler::OnGamepadInputEnabled()
{
	Super::OnGamepadInputEnabled();
}

void UGridImGuiInputHandler::OnGamepadInputDisabled()
{
	Super::OnGamepadInputDisabled();
}

void UGridImGuiInputHandler::OnMouseInputEnabled()
{
	Super::OnMouseInputEnabled();
}

void UGridImGuiInputHandler::OnMouseInputDisabled()
{
	Super::OnMouseInputDisabled();
}
