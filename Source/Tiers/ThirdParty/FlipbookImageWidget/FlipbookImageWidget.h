#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/StreamableManager.h"
#include "FlipbookImageWidget.generated.h"

class SImage;
class UPaperFlipbook;

UCLASS(MinimalAPI)
class UFlipbookImageWidget : public UWidget, public FTickableGameObject
{
	GENERATED_UCLASS_BODY()
	
	DECLARE_DYNAMIC_DELEGATE_RetVal(FLinearColor, FGetLinearColor);

	virtual void SynchronizeProperties() override;

	virtual void ReleaseSlateResources(const bool bReleaseChildren) override;

	virtual void Tick(const float DeltaTime) override;

	virtual bool IsTickableWhenPaused() const override;

	virtual bool IsTickableInEditor() const override;

	virtual TStatId GetStatId() const override;

	virtual UWorld* GetTickableGameObjectWorld() const override;

	UFUNCTION(BlueprintCallable, Category=Appearance)
	void SetFlipbook(UPaperFlipbook* Value);

	UFUNCTION(BlueprintCallable, Category=Appearance)
	const UPaperFlipbook* GetFlipbook() const;

	UFUNCTION(BlueprintCallable, Category=Appearance)
	void SetColorAndOpacity(const FLinearColor& Value);

	UFUNCTION(BlueprintCallable, Category=Appearance)
	const FLinearColor& GetColorAndOpacity() const;

	UFUNCTION(BlueprintCallable, Category=Appearance)
	void SetOpacity(const float Value);
	
	UFUNCTION(BlueprintCallable, Category=Appearance)
	bool GetFlipForRightToLeftFlowDirection() const;

	UFUNCTION(BlueprintCallable, Category=Appearance)
	void SetFlipForRightToLeftFlowDirection(const bool& Value);

#if WITH_EDITOR
	
	virtual const FText GetPaletteCategory() override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
#endif // WITH_EDITOR

#if WITH_ACCESSIBILITY
	
	virtual TSharedPtr<SWidget> GetAccessibleWidget() const override;
	
#endif // WITH_ACCESSIBILITY
	
protected:
	TSharedPtr<FStreamableHandle> StreamingHandle;
	FSoftObjectPath StreamingObjectPath;

	virtual TSharedRef<SWidget> RebuildWidget() override;

	void RequestAsyncLoad(const TSoftObjectPtr<>& SoftObject, TFunction<void()>&& Callback);
	
	virtual void RequestAsyncLoad(const TSoftObjectPtr<>& SoftObject, FStreamableDelegate Delegate);

	virtual void CancelFlipbookImageStreaming();

	virtual void OnFlipbookImageStreamingStarted(TSoftObjectPtr<> SoftObject);
	
	virtual void OnFlipbookImageStreamingComplete(TSoftObjectPtr<> SoftObject);

	PROPERTY_BINDING_IMPLEMENTATION(FSlateColor, ColorAndOpacity);
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Getter, Setter, BlueprintSetter=SetFlipBook, FieldNotify,
		Category=Appearance, meta=(AllowPrivateAccess=true))
	UPaperFlipbook* Flipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Getter=GetFlipForRightToLeftFlowDirection,
		Setter=SetFlipForRightToLeftFlowDirection, Category=Localization, meta=(AllowPrivateAccess=true))
	bool bFlipForRightToLeftFlowDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Getter, Setter, Category=Appearance,
		meta=(sRGB=true, AllowPrivateAccess=true))
	FLinearColor ColorAndOpacity;

	UPROPERTY()
	FGetLinearColor ColorAndOpacityDelegate;
	
	UPROPERTY()
	FSlateBrush CurrentBrush;
	FSlateNoResource SlateNoResource;
	
	TSharedPtr<SImage> Image;
	
	uint32 FrameCounter = INDEX_NONE;

	uint32 TotalFrames;
	uint32 CurrentFrame;
	uint32 LastFrame;

	double Threshold;
	double Elapsed;

	FORCEINLINE void SynchronizeFlipbookProperties() noexcept;

	FORCEINLINE const FSlateBrush* GetBrushAtFrame(const uint32 Frame);
};
