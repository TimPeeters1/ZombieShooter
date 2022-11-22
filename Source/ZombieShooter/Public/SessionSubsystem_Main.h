#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "SessionSubsystem_Main.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FZSOnCreateSessionComplete, bool, Succes);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FZSOnUpdateSessionComplete, bool, Succes);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FZSOnStartSessionComplete, bool, Succes);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FZSOnEndSessionComplete, bool, Succes);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FZSOnDestroySessionComplete, bool, Successful);
DECLARE_MULTICAST_DELEGATE_TwoParams(FZSOnFindSessionsComplete, const TArray<FOnlineSessionSearchResult>& SessionResults, bool Succes);
DECLARE_MULTICAST_DELEGATE_OneParam(FZSOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type Result);

/**
 * Container class for all functions related to finding network sessions (matches) for players to join and create.
 * Code based on: https://cedric-neukirchen.net/2021/06/27/ue4-multiplayer-sessions-in-c/
 */
UCLASS(Blueprintable)
class ZOMBIESHOOTER_API USessionSubsystem_Main : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	USessionSubsystem_Main();

	void CreateSession(int32 NumPublicConnections, bool IsLANMatch);
	void UpdateSession();

	void StartSession();
	void EndSession();
	void DestroySession();

	void FindSessions(int32 MaxSearchResults, bool IsLANQuery);

	void JoinGameSession(const FOnlineSessionSearchResult& SessionResult);

	bool TryTravelToCurrentSession();

	UPROPERTY(BlueprintAssignable)
	FZSOnCreateSessionComplete OnCreateSessionCompleteEvent;
	UPROPERTY(BlueprintAssignable)
	FZSOnUpdateSessionComplete OnUpdateSessionCompleteEvent;

	UPROPERTY(BlueprintAssignable)
	FZSOnStartSessionComplete OnStartSessionCompleteEvent;
	UPROPERTY(BlueprintAssignable)
	FZSOnEndSessionComplete OnEndSessionCompleteEvent;
	UPROPERTY(BlueprintAssignable)
	FZSOnDestroySessionComplete OnDestroySessionCompleteEvent;

	//UPROPERTY()
	FZSOnFindSessionsComplete OnFindSessionsCompleteEvent;

	//UPROPERTY()
	FZSOnJoinSessionComplete OnJoinGameSessionCompleteEvent;
	

protected:
	void OnCreateSessionCompleted(FName SessionName, bool Successful);
	void OnUpdateSessionCompleted(FName SessionName, bool Successful);

	void OnStartSessionCompleted(FName SessionName, bool Successful);
	void OnEndSessionCompleted(FName SessionName, bool Successful);
	void OnDestroySessionCompleted(FName SessionName, bool Successful);

	void OnFindSessionsCompleted(bool Successful);

	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

private:
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;

	FOnUpdateSessionCompleteDelegate UpdateSessionCompleteDelegate;
	FDelegateHandle UpdateSessionCompleteDelegateHandle;

	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;
	FDelegateHandle StartSessionCompleteDelegateHandle;

	FOnEndSessionCompleteDelegate EndSessionCompleteDelegate;
	FDelegateHandle EndSessionCompleteDelegateHandle;

	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FDelegateHandle DestroySessionCompleteDelegateHandle;

	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FDelegateHandle FindSessionsCompleteDelegateHandle;
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;

	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionCompleteDelegateHandle;
};
