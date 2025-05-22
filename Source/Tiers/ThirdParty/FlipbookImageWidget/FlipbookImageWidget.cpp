#include "FlipbookImageWidget.h"

#include "GenericPlatform/GenericPlatformMath.h"
#include "Slate/SlateTextureAtlasInterface.h"
#include "PaperFlipbook.h"
#include "PaperSprite.h"
#include "Engine/AssetManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FlipbookImageWidget)

#define LOCTEXT_NAMESPACE "Paper2D"

UFlipbookImageWidget::UFlipbookImageWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), Flipbook(nullptr), bFlipForRightToLeftFlowDirection(false),
	  ColorAndOpacity(FLinearColor::White), TotalFrames(1), CurrentFrame(0), LastFrame(0), Threshold(0), Elapsed(0)
{
}

TSharedRef<SWidget> UFlipbookImageWidget::RebuildWidget()
{
	SynchronizeFlipbookProperties();

	Image = SNew(SImage).FlipForRightToLeftFlowDirection(bFlipForRightToLeftFlowDirection);
	return Image.ToSharedRef();
}

void UFlipbookImageWidget::RequestAsyncLoad(const TSoftObjectPtr<>& SoftObject, TFunction<void()>&& Callback)
{
	RequestAsyncLoad(SoftObject, FStreamableDelegate::CreateLambda(MoveTemp(Callback)));
}

void UFlipbookImageWidget::RequestAsyncLoad(const TSoftObjectPtr<>& SoftObject, FStreamableDelegate Delegate)
{
	CancelFlipbookImageStreaming();

	if(const UObject* Object = SoftObject.Get(); Object)
	{
		// no streaming was needed, complete immediately
		const auto _ = Delegate.ExecuteIfBound();
		return;
	}

	OnFlipbookImageStreamingStarted(SoftObject);

	TWeakObjectPtr<UFlipbookImageWidget> WeakThis(this);
	StreamingObjectPath = SoftObject.ToSoftObjectPath();
	StreamingHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(
		StreamingObjectPath, [&WeakThis, &Delegate, &SoftObject]
		{
			if(UFlipbookImageWidget* This = WeakThis.Get())
			{
				// if the object paths don't match, then this delegate was interrupted,
				// but had already been queued for a callback so ignore everything and abort.
				if(This->StreamingObjectPath != SoftObject.ToSoftObjectPath())
				{
					// abort
					return;
				}
				
				const auto _ = Delegate.ExecuteIfBound();
				
				This->OnFlipbookImageStreamingComplete(SoftObject);
			}
		}, FStreamableManager::AsyncLoadHighPriority);	
}

void UFlipbookImageWidget::CancelFlipbookImageStreaming()
{
	if(StreamingHandle.IsValid())
	{
		StreamingHandle->CancelHandle();
		StreamingHandle.Reset();
	}

	StreamingObjectPath.Reset();
}

void UFlipbookImageWidget::OnFlipbookImageStreamingStarted(TSoftObjectPtr<> SoftObject)
{
	// no op
}

void UFlipbookImageWidget::OnFlipbookImageStreamingComplete(TSoftObjectPtr<> SoftObject)
{
	// no op
}

void UFlipbookImageWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	SynchronizeFlipbookProperties();

	const auto ColorAndOpacityBinding = PROPERTY_BINDING(FSlateColor, ColorAndOpacity);
	if (Image.IsValid())
	{
		Image->SetImage(GetBrushAtFrame(0));
		Image->InvalidateImage();
		Image->SetColorAndOpacity(ColorAndOpacityBinding);
	}
}

void UFlipbookImageWidget::ReleaseSlateResources(const bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	Image.Reset();
}

void UFlipbookImageWidget::Tick(const float DeltaTime)
{
	if (FrameCounter == GFrameCounter || !Image.IsValid())
	{
		return;
	}

	auto Frames = 0.0;

	Elapsed += DeltaTime;
	Elapsed = Threshold * FMath::Modf(Elapsed / Threshold, &Frames);
	CurrentFrame += Frames;
	CurrentFrame %= TotalFrames;

	if (CurrentFrame != LastFrame)
	{
		Image->SetImage(GetBrushAtFrame(CurrentFrame));
		Image->InvalidateImage();
		LastFrame = CurrentFrame;
	}

	FrameCounter = GFrameCounter;
}

bool UFlipbookImageWidget::IsTickableWhenPaused() const
{
	return false;
}

bool UFlipbookImageWidget::IsTickableInEditor() const
{
	return true;
}

TStatId UFlipbookImageWidget::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UFlipbookImageWidget, STATGROUP_Tickables)
}

UWorld* UFlipbookImageWidget::GetTickableGameObjectWorld() const
{
	return GetWorld();
}

void UFlipbookImageWidget::SetFlipbook(UPaperFlipbook* Value)
{
	if (Flipbook != Value)
	{
		Flipbook = Value;
		BroadcastFieldValueChanged(FFieldNotificationClassDescriptor::Flipbook);

		if (Image.IsValid())
		{
			Image->InvalidateImage();
		}
	}
}

const UPaperFlipbook* UFlipbookImageWidget::GetFlipbook() const
{
	return Flipbook;
}

void UFlipbookImageWidget::SetColorAndOpacity(const FLinearColor& Value)
{
	ColorAndOpacity = Value;
	if (Image.IsValid())
	{
		Image->SetColorAndOpacity(ColorAndOpacity);
	}
}

const FLinearColor& UFlipbookImageWidget::GetColorAndOpacity() const
{
	return ColorAndOpacity;
}

void UFlipbookImageWidget::SetOpacity(const float Value)
{
	ColorAndOpacity.A = Value;
	if(Image.IsValid())
	{
		Image->SetColorAndOpacity(ColorAndOpacity);
	}
}

bool UFlipbookImageWidget::GetFlipForRightToLeftFlowDirection() const
{
	return bFlipForRightToLeftFlowDirection;
}

void UFlipbookImageWidget::SetFlipForRightToLeftFlowDirection(const bool& Value)
{
	bFlipForRightToLeftFlowDirection = Value;
}

FORCEINLINE const FSlateBrush* UFlipbookImageWidget::GetBrushAtFrame(const uint32 Frame)
{
	if (!Flipbook)
	{
		return &SlateNoResource;
	}

	if (Frame <= TotalFrames)
	{
		if (const auto Sprite = Flipbook->GetSpriteAtFrame(Frame); Sprite)
		{
			const FSlateAtlasData SpriteAtlasData = Sprite->GetSlateAtlasData();
			const FVector2D SpriteSize = SpriteAtlasData.GetSourceDimensions();

			CurrentBrush.SetResourceObject(Sprite);
			CurrentBrush.ImageSize = FVector2D(SpriteSize.X, SpriteSize.Y);

			return &CurrentBrush;
		}
	}

	return &SlateNoResource;
}

FORCEINLINE void UFlipbookImageWidget::SynchronizeFlipbookProperties() noexcept
{
	if (Flipbook)
	{
		TotalFrames = Flipbook->GetNumFrames();
		Threshold = 1.0 / Flipbook->GetFramesPerSecond();
	}

	if (TotalFrames == 0)
	{
		TotalFrames = 1;
	}
}

#if WITH_EDITOR

const FText UFlipbookImageWidget::GetPaletteCategory()
{
	return LOCTEXT("FlipbookImage", "Paper 2D");
}

void UFlipbookImageWidget::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property)
	{
		const static auto FlipBookPropertyName = GET_MEMBER_NAME_CHECKED(UFlipbookImageWidget, Flipbook);
		if (const auto PropertyName = PropertyChangedEvent.Property->GetFName(); PropertyName == FlipBookPropertyName)
		{
			SynchronizeFlipbookProperties();
		}
	}
}

#endif // WITH_EDITOR

#if WITH_ACCESSIBILITY

TSharedPtr<SWidget> UFlipbookImageWidget::GetAccessibleWidget() const
{
	return Image;
}

#endif // WITH_ACCESSIBILITY

#undef LOCTEXT_NAMESPACE
