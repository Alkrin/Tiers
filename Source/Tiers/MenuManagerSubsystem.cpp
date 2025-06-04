// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuManagerSubsystem.h"

#include "MenuWrapperWidget.h"
#include "TimerManager.h"
#include "InfoDialogInterface.h"

void UMenuManagerSubsystem::ConnectBlueprints(
  TSubclassOf<UUserWidget> aMenuWrapperClass,
  TSubclassOf<UUserWidget> aDialogWrapperClass,
  TSubclassOf<UUserWidget> aInfoDialogClass)
{
  if (IsValid(aMenuWrapperClass))
  {
    MenuWrapperClass = aMenuWrapperClass;
  }

  if (IsValid(aDialogWrapperClass))
  {
    DialogWrapperClass = aDialogWrapperClass;
  }

  if (IsValid(aInfoDialogClass))
  {
    InfoDialogClass = aInfoDialogClass;
  }
}

void UMenuManagerSubsystem::PushMenuWidget(TSubclassOf<UUserWidget> MenuClass, EMenuTransitionEnum Transition)
{
  if (IsValid(MenuClass))
  {
    // If there is already a menu on the stack, run its exit animation.
    // The menu stays on the stack so we can return to it later.
    if (MenuStack.Num() > 0)
    {
      MenuStack.Last()->PerformTransitionAnimation(false);
    }

    UMenuWrapperWidget* Wrapper = Cast<UMenuWrapperWidget>(CreateWidget(GetWorld(), MenuWrapperClass));
    if (IsValid(Wrapper))
    {
      // Add the new menu to the stack, add it to the viewport, and run its entrance animation.
      Wrapper->GenerateContent(MenuClass);
      Wrapper->SetTransition(Transition);
      MenuStack.Push(Wrapper);
      Wrapper->AddToViewport();
      Wrapper->PerformTransitionAnimation(true);
    }
  }
}

void UMenuManagerSubsystem::PopMenuWidget(bool ShouldAnimate)
{
  if (MenuStack.Num() > 0)
  {
    UMenuWrapperWidget* ExitingMenu = MenuStack.Pop();
    if (ShouldAnimate)
    {
      ExitingMenu->PerformTransitionAnimation(false);
    }

    // Animations take 1.5 seconds.  After that, fully remove the ExitingMenu.
    FTimerHandle ExitTimer;
    GetWorld()->GetTimerManager().SetTimer(ExitTimer, ExitingMenu, &UMenuWrapperWidget::RemoveFromParent, ShouldAnimate ? 1.5f : 0.f);
    

    // If there is a menu under this one, animate it back in.
    if (MenuStack.Num() > 0)
    {
      MenuStack.Last()->PerformTransitionAnimation(true);
    }
  }
}

void UMenuManagerSubsystem::PopMenuWidgetWithTransition(EMenuTransitionEnum Transition)
{
  if (MenuStack.Num() > 0)
  {
    MenuStack.Last()->SetTransition(Transition);
    PopMenuWidget(true);
  }
}

void UMenuManagerSubsystem::ClearAllMenus(bool ShouldAnimate)
{
  while (MenuStack.Num() > 0)
  {
    PopMenuWidget(ShouldAnimate);
  }
}


void UMenuManagerSubsystem::PushDialogWidget(TSubclassOf<UUserWidget> DialogClass)
{
  if (IsValid(DialogClass))
  {
    UMenuWrapperWidget* Wrapper = Cast<UMenuWrapperWidget>(CreateWidget(GetWorld(), DialogWrapperClass));
    if (IsValid(Wrapper))
    {
      // Add the new dialog to the stack, add it to the viewport, and run its entrance animation.
      Wrapper->GenerateContent(DialogClass);
      Wrapper->SetTransition(EMenuTransitionEnum::Dialog);
      DialogStack.Push(Wrapper);
      Wrapper->AddToViewport();
      Wrapper->PerformTransitionAnimation(true);
    }
  }
}

void UMenuManagerSubsystem::PushInfoDialog(const FText Title, const FText Message, bool bIsError)
{
  if (IsValid(InfoDialogClass))
  {
    UMenuWrapperWidget* Wrapper = Cast<UMenuWrapperWidget>(CreateWidget(GetWorld(), DialogWrapperClass));
    if (IsValid(Wrapper))
    {
      // Add the new dialog to the stack, add it to the viewport, and run its entrance animation.
      UUserWidget* Content = Wrapper->GenerateContent(InfoDialogClass);
      if (Content->Implements<UInfoDialog>())
      {
        IInfoDialog::Execute_SetDisplayData(Content, bIsError, Title, Message);
        Wrapper->SetTransition(EMenuTransitionEnum::Dialog);
        DialogStack.Push(Wrapper);
        Wrapper->AddToViewport();
        Wrapper->PerformTransitionAnimation(true);
      } else {
        UE_LOG(LogTemp, Error, TEXT("Invalid InfoDialogClass registered with MenuManagerSubsystem.  Does not implement the UInfoDialog interface."));
      }
    }
  }
}

void UMenuManagerSubsystem::PopDialogWidget(bool ShouldAnimate)
{
  if (DialogStack.Num() > 0)
  {
    UMenuWrapperWidget* ExitingDialog = DialogStack.Pop();
    if (ShouldAnimate)
    {
      ExitingDialog->PerformTransitionAnimation(false);
    }

    // Animation takes 0.5 seconds.  After that, fully remove the ExitingDialog.
    FTimerHandle ExitTimer;
    GetWorld()->GetTimerManager().SetTimer(ExitTimer, ExitingDialog, &UMenuWrapperWidget::RemoveFromParent, ShouldAnimate ? 0.5f : 0.f);
  }
}

void UMenuManagerSubsystem::ClearAllDialogs(bool ShouldAnimate)
{
  while (DialogStack.Num() > 0)
  {
    PopMenuWidget(ShouldAnimate);
  }
}
