#include "CBPlayerController.h"

#include "UI/CBChatInput.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ChatBaseballGame.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Game/CBGameModeBase.h"
#include "CBPlayerState.h"
#include "Net/UnrealNetwork.h"

ACBPlayerController::ACBPlayerController()
{
	bReplicates = true;
}

void ACBPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalController())
	{
		return;
	}

	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);

	if (IsValid(ChatInputWidgetClass))
	{
		ChatInputWidgetInstance = CreateWidget<UCBChatInput>(this, ChatInputWidgetClass);
		if (IsValid(ChatInputWidgetInstance))
		{
			ChatInputWidgetInstance->AddToViewport();
		}
	}

	if (IsValid(NotificationTextWidgetClass))
	{
		NotificationTextWidgetInstance = CreateWidget<UUserWidget>(this, NotificationTextWidgetClass);
		if (IsValid(NotificationTextWidgetInstance))
		{
			NotificationTextWidgetInstance->AddToViewport();
		}
	}
}

void ACBPlayerController::SetChatMessage(const FString& InChatMessage)
{
	ChatMessage = InChatMessage;

	if (IsLocalController())
	{
		ACBPlayerState* CBPS = GetPlayerState<ACBPlayerState>();
		if (IsValid(CBPS))
		{
			FString CombinedMessage = CBPS->GetPlayerInfoString() + TEXT(": ") + InChatMessage;


			ServerRPCPrintChatMessage(CombinedMessage);
		}
	}
}

void ACBPlayerController::PrintChatMessage(const FString& InChatMessage)
{
	ChatBaseballGameFunctionbrary::CustomPrint(this, InChatMessage, 10.f);
}

void ACBPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, NotificationText);
}

void ACBPlayerController::ServerRPCPrintChatMessage_Implementation(const FString& InChatMessage)
{
	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (IsValid(GM))
	{
		ACBGameModeBase* CBGM = Cast<ACBGameModeBase>(GM);
		if (IsValid(CBGM))
		{
			CBGM->PrintChatMessage(this, InChatMessage);
		}
	}
}

void ACBPlayerController::ClientRPCPrintChatMessage_Implementation(const FString& InChatMessage)
{
	PrintChatMessage(InChatMessage);	
}
