#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageTypes.h"
#include "WeaponBase.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponHitSignature, const FHitResult&, HitResult);

UCLASS()
class ESCAPE_API AWeaponBase : public AActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


    /** Root and Mesh */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    USceneComponent* Root;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Mesh")
    UStaticMeshComponent* Mesh;

    /** Collision volume for melee hit detection */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = "Weapon|HitDetection")
    UShapeComponent* HitCollision;

    /** Whether weapon is currently detecting hits */
    bool bIsDetectingHits = false;

    /** Actors hit this swing, to prevent multiple hits per swing */
    UPROPERTY()
    TArray<AActor*> AlreadyHitActors;

    /** Base weapon stats */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stats")
    float BaseDamage = 20.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stats")
    float Agility = 1.0f; // could affect attack speed

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stats")
    float Range = 150.f;

    /** Gameplay Tag to identify the weapon type or attack category */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|GAS")
    FGameplayTag WeaponTag;

public:
    // Sets default values for this actor's properties
    AWeaponBase();


    /** Starts detecting hits */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    virtual void StartHitDetection();

    /** Stops detecting hits */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    virtual void StopHitDetection();

    /** Called when overlap happens during hit detection */
    UFUNCTION()
    virtual void OnHitOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    /** Resets actors hit for next attack */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void ResetHitList();

    /** Broadcasts hit to whoever is listening (AttackComponent, GAS ability, etc.) */
    UPROPERTY(BlueprintAssignable, Category = "Weapon|Events")
    FOnWeaponHitSignature OnWeaponHit;

};
