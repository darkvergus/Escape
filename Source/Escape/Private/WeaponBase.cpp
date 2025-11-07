#include "WeaponBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"



AWeaponBase::AWeaponBase()
{
    PrimaryActorTick.bCanEverTick = false;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(Root);

    HitCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitCollision"));
    HitCollision->SetupAttachment(Mesh);
    HitCollision->SetCollisionObjectType(ECC_GameTraceChannel1); 
    HitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    HitCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
    HitCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AWeaponBase::BeginPlay()
{
    Super::BeginPlay();
    if (UPrimitiveComponent* Collision = Cast<UPrimitiveComponent>(HitCollision))
    {
        Collision->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnHitOverlap);
    }


    IgnorePlayerCollisions();



}

void AWeaponBase::IgnorePlayerCollisions() {

    if (AActor* MyOwner = GetOwner())
    {
        if (Mesh)
        {
            Mesh->IgnoreActorWhenMoving(MyOwner, true);
        }

        if (HitCollision)
        {
            HitCollision->IgnoreActorWhenMoving(MyOwner, true);
            HitCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
        }
    }
    if (WeaponOwner)
    {
        if (Mesh)
        {
            Mesh->IgnoreActorWhenMoving(WeaponOwner, true);
        }

        if (HitCollision)
        {
            HitCollision->IgnoreActorWhenMoving(WeaponOwner, true);
            HitCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
        }
    }







}


void AWeaponBase::StartHitDetection()
{
    if (!HitCollision) return;

    bIsDetectingHits = true;
    HitCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    AlreadyHitActors.Empty();
    bIsDetectingHits = true;

    UE_LOG(LogTemp, Log, TEXT("[%s] Hit detection started"), *GetName());
}

void AWeaponBase::StopHitDetection()
{
    if (!HitCollision) return;

    bIsDetectingHits = false;
    HitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    UE_LOG(LogTemp, Log, TEXT("[%s] Hit detection stopped"), *GetName());
}

void AWeaponBase::OnHitOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{

    UE_LOG(LogTemp, Display, TEXT("AAAAAAAAAAAAAAAAA"));
    if (!bIsDetectingHits || !OtherActor || AlreadyHitActors.Contains(OtherActor))
        return;

    AlreadyHitActors.Add(OtherActor);
    UE_LOG(LogTemp, Log, TEXT("[%s] Hit actor: %s"), *GetName(), *OtherActor->GetName());

    OnWeaponHit.Broadcast(SweepResult);
}

void AWeaponBase::ResetHitList()
{
    AlreadyHitActors.Empty();
}