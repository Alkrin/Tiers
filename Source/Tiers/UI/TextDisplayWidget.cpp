// Fill out your copyright notice in the Description page of Project Settings.


#include "TextDisplayWidget.h"

#include "Components\Border.h"
#include "Components\TextBlock.h"

void UTextDisplayWidget::SynchronizeProperties()
{
  Super::SynchronizeProperties();

  TextBlock->SetJustification(TextAlignment);
  FSlateFontInfo Font = TextBlock->GetFont();
  Font.Size = FontSize;
  TextBlock->SetFont(Font);
  TextBlock->SetText(Text);

  UpdateColors();
}

void UTextDisplayWidget::SetIsError(const bool NewIsError)
{
  if (bIsError != NewIsError)
  {
    bIsError = NewIsError;

    UpdateColors();
  }
}

void UTextDisplayWidget::SetText(const FText NewText)
{
  if (Text.ToString() != NewText.ToString())
  {
    Text = NewText;
    TextBlock->SetText(Text);
  }
}

void UTextDisplayWidget::UpdateColors()
{
  RootBorder->SetBrushColor(bIsError ? RootColorError : RootColorNormal);
  BackgroundBorder->SetBrushColor(bIsError ? BackgroundColorError : BackgroundColorNormal);
  TextBlock->SetColorAndOpacity(bIsError ? TextColorError : TextColorNormal);
}