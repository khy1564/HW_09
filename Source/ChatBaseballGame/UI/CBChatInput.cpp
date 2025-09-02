#include "CBChatInput.h"

#include "Components/EditableTextBox.h"
#include "Player/CBPlayerController.h"

void UCBChatInput::NativeConstruct()
{
	Super::NativeConstruct();

	if (!ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted))
	{
		ChatInput->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UCBChatInput::NativeDestruct()
{
	Super::NativeDestruct();

	if (ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted))
	{
		ChatInput->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UCBChatInput::OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		APlayerController* OwningPlayerController = GetOwningPlayer();
		if (IsValid(OwningPlayerController))
		{
			ACBPlayerController* OwningCBPlayerController = Cast<ACBPlayerController>(OwningPlayerController);
			if (IsValid(OwningCBPlayerController))
			{
				OwningCBPlayerController->SetChatMessage(Text.ToString());

				ChatInput->SetText(FText());
			}
		}
	}
}
