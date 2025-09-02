#include "CBGameModeBase.h"

#include "CBGameStateBase.h"
#include "Player/CBPlayerController.h"
#include "EngineUtils.h"
#include "Player/CBPlayerState.h"

void ACBGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	ACBPlayerController* CBPlayerController = Cast<ACBPlayerController>(NewPlayer);
	if (IsValid(CBPlayerController))
	{
		CBPlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server."));

		AllPlayerControllers.Add(CBPlayerController);

		ACBPlayerState* CBPS = CBPlayerController->GetPlayerState<ACBPlayerState>();
		if (IsValid(CBPS))
		{
			CBPS->PlayerName = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
		}

		ACBGameStateBase* CBGameStateBase = GetGameState<ACBGameStateBase>();
		if (IsValid(CBGameStateBase))
		{
			CBGameStateBase->MulticastRPCBroadcastLoginMessage(CBPS->PlayerName);
		}
	}
}

FString ACBGameModeBase::GenerateBaseballNumber()
{
	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; ++i)
	{
		Numbers.Add(i);
	}
	FMath::RandInit(FDateTime::Now().GetTicks());
	Numbers = Numbers.FilterByPredicate([](int32 Num) {return Num > 0;});

	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		Result.Append(FString::FromInt(Numbers[Index]));
		Numbers.Remove(Index);
	}


	return Result;
}

bool ACBGameModeBase::IsGuessNumberString(const FString& InNumberString)
{
	bool bCanPlay = false;

	do {

		if (InNumberString.Len() != 3)
		{
			break;
		}

		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : InNumberString)
		{
			if (!FChar::IsDigit(C) || C == '0')
			{
				bIsUnique = false;
				break;
			}

			UniqueDigits.Add(C);
		}

		if (!bIsUnique)
		{
			break;
		}

		bCanPlay = true;

	} while (false);

	return bCanPlay;
}

FString ACBGameModeBase::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;

	for (int32 i = 0; i < 3; ++i)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i])
		{
			StrikeCount++;
		}
		else
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (InSecretNumberString.Contains(PlayerGuessChar))
			{
				BallCount++;
			}
		}
	}

	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS %dB"), StrikeCount, BallCount);
}

void ACBGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	BaseballNumberString = GenerateBaseballNumber();
}

void ACBGameModeBase::PrintChatMessage(ACBPlayerController* InChattingPlayerController, const FString& InChatMessage)
{
	//FString ChatMessage = InChatMessage;
	int Index = InChatMessage.Len() - 3;
	FString GuessNumberString = InChatMessage.RightChop(Index);
	if (IsGuessNumberString(GuessNumberString))
	{
		FString JudgeResultString = JudgeResult(BaseballNumberString, GuessNumberString);
		
		IncreaseGuessCount(InChattingPlayerController);

		for (TActorIterator<ACBPlayerController> It(GetWorld()); It; ++It)
		{
			ACBPlayerController* CBPlayerController = *It;
			if (IsValid(CBPlayerController))
			{
				FString CombinedMessage = InChatMessage + TEXT(" -> ") + JudgeResultString;
				CBPlayerController->ClientRPCPrintChatMessage(CombinedMessage);
			
				int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));
				JudgeGame(InChattingPlayerController, StrikeCount);
			}
		}
	}
	else
	{
		for (TActorIterator<ACBPlayerController> It(GetWorld()); It; ++It)
		{
			ACBPlayerController* CBPlayerController = *It;
			if (IsValid(CBPlayerController))
			{
				CBPlayerController->ClientRPCPrintChatMessage(InChatMessage);
			}
		}
	}
}

void ACBGameModeBase::IncreaseGuessCount(ACBPlayerController* InChattingPlayerController)
{
	ACBPlayerState* CBPS = InChattingPlayerController->GetPlayerState<ACBPlayerState>();
	if (IsValid(CBPS))
	{
		CBPS->CurrentGuessCount++;
	}
}

void ACBGameModeBase::ResetGame()
{
	BaseballNumberString = GenerateBaseballNumber();

	for (const auto& CBPlayerController : AllPlayerControllers)
	{
		ACBPlayerState* CBPS = CBPlayerController->GetPlayerState<ACBPlayerState>();
		if (IsValid(CBPS))
		{
			CBPS->CurrentGuessCount = 0;
		}
	}
}

void ACBGameModeBase::JudgeGame(ACBPlayerController* InChattingPlayerController, int InStrikeCount)
{
	if (InStrikeCount == 3)
	{
		ACBPlayerState* CBPS = InChattingPlayerController->GetPlayerState<ACBPlayerState>();
		for (const auto& CBPlayerController : AllPlayerControllers)
		{
			if (IsValid(CBPS))
			{
				FString CombinedMessage = CBPS->PlayerName + TEXT(" has won the game.");
				CBPlayerController->NotificationText = FText::FromString(CombinedMessage);

				ResetGame();
			}
		}
	}
	else
	{
		bool bIsDraw = true;
		for (const auto& CBPlayerController : AllPlayerControllers)
		{
			ACBPlayerState* CBPS = CBPlayerController->GetPlayerState<ACBPlayerState>();
			if (IsValid(CBPS))
			{
				if (CBPS->CurrentGuessCount < CBPS->MaxGuessCount)
				{
					bIsDraw = false;
					break;
				}
			}
		}

		if (bIsDraw)
		{
			for (const auto& CBPlayerController : AllPlayerControllers)
			{
				CBPlayerController->NotificationText = FText::FromString(TEXT("Draw..."));

				ResetGame();
			}
		}
	}
}
