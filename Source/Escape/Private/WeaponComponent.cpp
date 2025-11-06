// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponComponent.h"
#include "WeaponBase.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

UWeaponComponent::UWeaponComponent()
{
   PrimaryComponentTick.bCanEverTick = false;

   
}

void UWeaponComponent::BeginPlay()
{
    Super::BeginPlay();


    if (DefaultWeaponClass)
    {
        EquipWeapon(DefaultWeaponClass);
    }
}

void UWeaponComponent::EquipWeapon(TSubclassOf<AWeaponBase> NewWeaponClass)
{
    if (!NewWeaponClass) return;

    AActor* Owner = GetOwner();
    if (!Owner) return;

    ACharacter* CharacterOwner = Cast<ACharacter>(Owner);
    if (!CharacterOwner) return;

    // Destroy old weapon if exists
    if (CurrentWeapon)
    {
        CurrentWeapon->Destroy();
        CurrentWeapon = nullptr;
    }

    // Spawn new weapon
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = Owner;
    SpawnParams.Instigator = CharacterOwner;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AWeaponBase* NewWeapon = GetWorld()->SpawnActor<AWeaponBase>(NewWeaponClass, SpawnParams);
    if (!NewWeapon)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to spawn weapon!"));
        return;
    }

    // Attach to character’s skeletal mesh
    USkeletalMeshComponent* Mesh = CharacterOwner->GetMesh();
    if (Mesh)
    {
        NewWeapon->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
        OnWeaponEquipped.Broadcast(CurrentWeapon);
    }

    CurrentWeapon = NewWeapon;
    CurrentWeapon->SetWeaponOwner(GetOwner());

    UE_LOG(LogTemp, Log, TEXT("Equipped weapon: %s"), *NewWeapon->GetName());
}

void UWeaponComponent::UnequipWeapon()
{
    if (CurrentWeapon)
    {
        CurrentWeapon->Destroy();
        CurrentWeapon = nullptr;
    }
}


#if WITH_EDITOR
void UWeaponComponent::UpdatePreviewWeapon()
{
    // Cleanup any old preview
    if (IsValid(PreviewWeaponActor)) {
        PreviewWeaponActor->Destroy();
        PreviewWeaponActor = nullptr;
    }

    if (!PreviewWeaponClass) return;

    // Make sure we have a valid world and owner
    if (!GetWorld() || !GetOwner()) return;



    UE_LOG(LogTemp, Error, TEXT("PREVIEW"));
    
    

    // Spawn preview actor
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = GetOwner();
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnParams.ObjectFlags = RF_Transactional | RF_TextExportTransient; // <- keeps it alive for editor preview
    SpawnParams.bTemporaryEditorActor = true; // this helps Unreal know it’s an editor-only actor

    PreviewWeaponActor = GetWorld()->SpawnActor<AWeaponBase>(PreviewWeaponClass, SpawnParams);

    if (PreviewWeaponActor)
    {
        // Hide from play but visible in editor
        PreviewWeaponActor->SetActorEnableCollision(false);
        PreviewWeaponActor->SetActorHiddenInGame(false);
        PreviewWeaponActor->SetFlags(RF_Transactional);

        ACharacter* CharOwner = Cast<ACharacter>(GetOwner());
        if (CharOwner && CharOwner->GetMesh())
        {
            PreviewWeaponActor->AttachToComponent(
                this,
                FAttachmentTransformRules::SnapToTargetIncludingScale                
            );
        }
    }
}
#endif


#if WITH_EDITOR
void UWeaponComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    // Recreate preview if class changed
    const FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
    if (PropertyName == GET_MEMBER_NAME_CHECKED(UWeaponComponent, PreviewWeaponClass))
    {

        UpdatePreviewWeapon(); // regenerate preview
    }

}
#endif

#if WITH_EDITOR
void UWeaponComponent::OnUnregister()
{

   /* if (PreviewWeaponActor)
    {
        UE_LOG(LogTemp, Error, TEXT("Unregister"));
        PreviewWeaponActor->Destroy();
        PreviewWeaponActor = nullptr;
    }*/

    Super::OnUnregister();

    
}
#endif