// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "CPP_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEFPS_API UCPP_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UCPP_GameInstance();

protected:
	IOnlineSessionPtr SessionInterface;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	virtual void Init() override;

	virtual void OnCreateSessionComplete(FName ServerName, bool Succeeded);
	virtual void OnFindSessionComplete(bool Succeeded);
	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	UFUNCTION(BlueprintCallable)
	FString CreateServer();

	UFUNCTION(BlueprintCallable)
	void JoinServer(FString code);

public:
	// Function to generate a random 6-character string
	FString GenerateRandomString();

private:
	// Helper function to generate a random character
	TCHAR GetRandomCharacter();
};
