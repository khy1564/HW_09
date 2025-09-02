#include "CBPawn.h"

#include "ChatBaseballGame.h"

void ACBPawn::BeginPlay()
{
	Super::BeginPlay();
	
	FString NetRole = ChatBaseballGameFunctionbrary::GetRole(this);
	FString Combined = FString::Printf(TEXT("CBPawn::BeginPlay() %s [%s]"), *ChatBaseballGameFunctionbrary::GetNetMode(this), *NetRole);
	ChatBaseballGameFunctionbrary::CustomPrint(this, Combined, 10.f);
}

void ACBPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	FString NetRole = ChatBaseballGameFunctionbrary::GetRole(this);
	FString Combined = FString::Printf(TEXT("CBPawn::PossessedBy() %s [%s]"), *ChatBaseballGameFunctionbrary::GetNetMode(this), *NetRole);
	ChatBaseballGameFunctionbrary::CustomPrint(this, Combined, 10.f);
}


