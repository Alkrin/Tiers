// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuManagerSubsystem.h"

#include "MenuWrapperWidget.h"
#include "TimerManager.h"

void UMenuManagerSubsystem::SetMenuWrapperClass(TSubclassOf<UUserWidget> WidgetClass)
{
  if (IsValid(WidgetClass))
  {
    MenuWrapperClass = WidgetClass;
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
      Wrapper->SetContentClass(MenuClass);
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
    GetWorld()->GetTimerManager().SetTimer(ExitTimer, ExitingMenu, &UMenuWrapperWidget::RemoveFromParent, 1.5f);
    

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
    // TODO: Remove the menu from the stack, change its transition to the passed override, then run the exit animation.
  }
}

void UMenuManagerSubsystem::ClearAllMenus(bool ShouldAnimate)
{
  if (ShouldAnimate)
  {
    // TODO: Run exit animations.
  }

  // TODO:
}