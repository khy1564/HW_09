#pragma once

//#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CBPlayerController.generated.h"

class UCBChatInput;
class UUserWidget;

/**
 * 
 */
UCLASS()
class CHATBASEBALLGAME_API ACBPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACBPlayerController();

	virtual void BeginPlay() override;

	void SetChatMessage(const FString& InChatMessage);

	void PrintChatMessage(const FString& InChatMessage);

	UFUNCTION(Client, Reliable)
	void ClientRPCPrintChatMessage(const FString& InChatMessage);

	UFUNCTION(Server, Reliable)
	void ServerRPCPrintChatMessage(const FString& InChatMessage);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCBChatInput> ChatInputWidgetClass;

	UPROPERTY()
	TObjectPtr<UCBChatInput> ChatInputWidgetInstance;

	FString ChatMessage;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> NotificationTextWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> NotificationTextWidgetInstance;

public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	FText NotificationText;
};
