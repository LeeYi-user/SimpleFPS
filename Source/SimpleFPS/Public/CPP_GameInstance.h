// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
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
	virtual void Init() override;
};
