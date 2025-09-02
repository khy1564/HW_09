#pragma once

#include "CoreMinimal.h"

class ChatBaseballGameFunctionbrary
{
public:
	static void CustomPrint(const AActor* InWorldContextActor, const FString& InString, float InTimeToDisplay = 1.f, FColor InColor = FColor::Cyan)
	{
		if (IsValid(GEngine) && IsValid(InWorldContextActor))
		{
			if (InWorldContextActor->GetNetMode() == NM_Client || InWorldContextActor->GetNetMode() == NM_ListenServer)
			{
				GEngine->AddOnScreenDebugMessage(-1, InTimeToDisplay, InColor, InString);
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("%s"), *InString);
			}
		}
	}

	static FString GetNetMode(const AActor* InWorldContextActor)
	{
		FString NetModeString = TEXT("None");

		if (IsValid(InWorldContextActor))
		{
			ENetMode NetMode = InWorldContextActor->GetNetMode();
			if (NetMode == NM_Client)
			{
				NetModeString = TEXT("Client");
			}
			else
			{
				if (NetMode == NM_Standalone)
				{
					NetModeString = TEXT("StandAlone");
				}
				else
				{
					NetModeString = TEXT("Server");
				}
			}
		}

		return NetModeString;
	}

	static FString GetRole(const AActor* InActor)
	{
		FString Role = TEXT("None");

		if (IsValid(InActor))
		{
			FString LocalRole = UEnum::GetValueAsString(TEXT("Engine.ENetRole"), InActor->GetLocalRole());
			FString RemoteRole = UEnum::GetValueAsString(TEXT("Engine.ENetRole"), InActor->GetRemoteRole());

			Role = FString::Printf(TEXT("%s / %s"), *LocalRole, *RemoteRole);
		}

		return Role;
	}
};