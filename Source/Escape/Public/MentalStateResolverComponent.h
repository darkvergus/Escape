#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "GASAbilitySystemComponent.h"
#include "MentalStateResolverComponent.generated.h"

UCLASS(ClassGroup=(Logic), meta=(BlueprintSpawnableComponent))
class ESCAPE_API UMentalStateResolverComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMentalStateResolverComponent();

    UPROPERTY(EditDefaultsOnly, Category="Resolver")
    UDataTable* MentalStateTable = nullptr;

    void Initialise(UAbilitySystemComponent* ASC);

    UFUNCTION(BlueprintCallable)
    void Recompute();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
    UPROPERTY() UAbilitySystemComponent* CachedASC;
    FGameplayTag CurrentMental;
};
