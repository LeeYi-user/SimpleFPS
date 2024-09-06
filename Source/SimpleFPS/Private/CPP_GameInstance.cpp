// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_GameInstance.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"

UCPP_GameInstance::UCPP_GameInstance()
{

}

void UCPP_GameInstance::Init()
{
	if (IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
	{
		SessionInterface = Subsystem->GetSessionInterface();

		if (SessionInterface.IsValid())
		{
			// Bind Delegates Here
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UCPP_GameInstance::OnCreateSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UCPP_GameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UCPP_GameInstance::OnJoinSessionComplete);
		}
	}
}

void UCPP_GameInstance::OnCreateSessionComplete(FName ServerName, bool Succeeded)
{
	UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete, Succeeded: %d"), Succeeded);
	if (Succeeded)
	{
		GetWorld()->ServerTravel("/Game/FirstPerson/Maps/FirstPersonMap?listen");
	}
}

void UCPP_GameInstance::OnFindSessionComplete(bool Succeeded)
{
	UE_LOG(LogTemp, Warning, TEXT("OnFindSessionComplete, Succeeded: %d"), Succeeded);
	if (Succeeded)
	{
		TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;

		UE_LOG(LogTemp, Warning, TEXT("SearchResults, Server Count: %d"), SearchResults.Num());

		if (SearchResults.Num())
		{
			UE_LOG(LogTemp, Warning, TEXT("Joining Server"));
			SessionInterface->JoinSession(0, "My Session", SearchResults[0]);
		}
	}
}

void UCPP_GameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	UE_LOG(LogTemp, Warning, TEXT("OnJoinSessionComplete, SessionName: %s"), *SessionName.ToString());
	if (APlayerController* PController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FString JoinAddress = "";
		SessionInterface->GetResolvedConnectString(SessionName, JoinAddress);
		if (JoinAddress != "")
		{
			PController->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
		}
	}
}

FString UCPP_GameInstance::CreateServer()
{
	UE_LOG(LogTemp, Warning, TEXT("CreateServer"));

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.NumPublicConnections = 5;
	SessionSettings.bUseLobbiesIfAvailable = true;

	FString code = GenerateRandomString();
	SessionSettings.Set(FName("Code"), code, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	SessionInterface->CreateSession(0, FName("My Session"), SessionSettings);

	return code;
}

void UCPP_GameInstance::JoinServer(FString code)
{
	UE_LOG(LogTemp, Warning, TEXT("JoinServer"));

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	SessionSearch->MaxSearchResults = 150000;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	SessionSearch->QuerySettings.Set(FName("Code"), code, EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

FString UCPP_GameInstance::GenerateRandomString()
{
	FString RandomString;
	const int32 StringLength = 6;

	for (int32 i = 0; i < StringLength; i++)
	{
		RandomString.AppendChar(GetRandomCharacter());
	}

	return RandomString;
}

TCHAR UCPP_GameInstance::GetRandomCharacter()
{
	// Define character pool (A-Z, 0-9)
	const FString CharPool = TEXT("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

	// Generate a random index to select a character from the pool
	int32 Index = FMath::RandRange(0, CharPool.Len() - 1);

	return CharPool[Index];
}
