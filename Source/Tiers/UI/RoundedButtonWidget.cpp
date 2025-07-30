// Fill out your copyright notice in the Description page of Project Settings.


#include "RoundedButtonWidget.h"

void URoundedButtonWidget::SetIsDisabled(const bool abIsDisabled)
{
  if (bIsDisabled != abIsDisabled)
  {
    bIsDisabled = abIsDisabled;

    BroadcastFieldValueChanged(FFieldNotificationClassDescriptor::bIsDisabled);
  }
}
