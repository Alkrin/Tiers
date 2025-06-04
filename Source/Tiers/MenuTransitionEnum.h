// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
 
#include "CoreMinimal.h"
#include "MenuTransitionEnum.generated.h"
 
/**
 * List of supported transition animations for the MenuWrapperWidget.
 */
UENUM(BlueprintType)
enum class EMenuTransitionEnum : uint8
{
  None UMETA(DisplayName = "None"),
  Dialog UMETA(DisplayName = "Dialog"),
  FadeAndScale UMETA(DisplayName = "Fade And Scale"),
  RotateAndSlide UMETA(DisplayName = "Rotate And Slide"),
};