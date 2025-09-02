#pragma once

//#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CBGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class CHATBASEBALLGAME_API ACBGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCBroadcastLoginMessage(const FString& InName = FString(TEXT("XXXXXXX")));
};
