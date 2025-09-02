#pragma once

//#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CBPawn.generated.h"

UCLASS()
class CHATBASEBALLGAME_API ACBPawn : public APawn
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;
};
