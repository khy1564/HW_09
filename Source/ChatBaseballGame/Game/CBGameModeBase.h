#pragma once

//#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CBGameModeBase.generated.h"

class ACBPlayerController;

/**
 * 
 */
UCLASS()
class CHATBASEBALLGAME_API ACBGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	virtual void OnPostLogin(AController* NewPlayer) override;

	FString GenerateBaseballNumber();

	bool IsGuessNumberString(const FString& InNumberString);

	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);

	virtual void BeginPlay() override;

	void PrintChatMessage(ACBPlayerController* InChattingPlayerController, const FString& InChatMessage);

	void IncreaseGuessCount(ACBPlayerController* InChattingPlayerController);

	void ResetGame();

	void JudgeGame(ACBPlayerController* InChattingPlayerController, int InStrikeCount);

protected:
	FString BaseballNumberString;

	TArray<TObjectPtr<ACBPlayerController>> AllPlayerControllers;

};
