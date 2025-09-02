#pragma once

//#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CBPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class CHATBASEBALLGAME_API ACBPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ACBPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	FString GetPlayerInfoString();

public:
	UPROPERTY(Replicated)
	FString PlayerName;

	UPROPERTY(Replicated)
	int32 CurrentGuessCount;

	UPROPERTY(Replicated)
	int32 MaxGuessCount;
};
