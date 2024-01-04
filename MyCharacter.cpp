// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	if(TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);

		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}
    

	//카메라 붐 초기화
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));

	//클래스의 루트 컴포넌트에 카메라 붐 어태치먼트 설정
	CameraBoom->SetupAttachment(RootComponent);

	//PawnControlRotation 사용 부울을 true로 설정
	CameraBoom->bUsePawnControlRotation = true;

	//FollowCamera 초기화
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));

	//카메라 붐에 FollowCamera 어태치먼트 설정
	FollowCamera->SetupAttachment(CameraBoom);

    FollowCamera->bUsePawnControlRotation = false;

    bUseControllerRotationYaw = true;

    JumpMaxCount = 2;

    gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
    gunMeshComp->SetupAttachment(GetMesh());

    ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));

    if (TempGunMesh.Succeeded())
    {
        gunMeshComp->SetSkeletalMesh(TempGunMesh.Object);
        gunMeshComp->SetRelativeLocation(FVector(-14, 52, 120));
    }
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

    PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);
    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    
}

void AMyCharacter::MoveForward(float AxisValue)
{
    if ((Controller != NULL) && (AxisValue != 0.0f))
    {
        //앞쪽 찾기
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        //앞쪽 벡터 구하기
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, AxisValue);
    }

}

void AMyCharacter::MoveRight(float AxisValue)
{
    if ((Controller != NULL) && (AxisValue != 0.0f))
    {
        //오른쪽 방향 찾기
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        //오른쪽 벡터 구하기
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        //해당 방향으로 이동 추가
        AddMovementInput(Direction, AxisValue);
    }
}