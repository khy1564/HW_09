#include "CBGameStateBase.h"

#include "Kismet/GameplayStatics.h"
#include "Player/CBPlayerController.h"

void ACBGameStateBase::MulticastRPCBroadcastLoginMessage_Implementation(const FString& InName)
{
	if (!HasAuthority())
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC))
		{
			ACBPlayerController* CBPC = Cast<ACBPlayerController>(PC);
			if (IsValid(CBPC))
			{
				FString NotificationString = InName + TEXT(" has joined the game.");
				CBPC->PrintChatMessage(NotificationString);
			}
		}
	}
}
