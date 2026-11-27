using System;
using UnityEngine;

public abstract class BaseMission
{
    public event Action<MissionState> OnMissionStateChanged;

    public MissionState State { get; private set; } = MissionState.None;
    protected PlayerCharacter MissionPlayer { get; private set; }

    public void Initialize()
    {
        InitData();
        SetState(MissionState.Ready);
    }

    protected virtual void InitData() { }
    protected virtual void OnReady() { }
    protected virtual void OnInProgress() { }
    protected virtual void OnSucceeded() { }
    protected virtual void OnFailed() { }

    public virtual void AgreeMission(PlayerCharacter player)
    {
        MissionPlayer = player;
        SetState(MissionState.InProgress);
    }

    public virtual void DisagreeMission()
    {
    }

    public virtual void SuccessMission()
    {
        SetState(MissionState.Succeeded);
    }

    public virtual void FailedMission()
    {
        SetState(MissionState.Failed);
    }

    public virtual float GetMinigameRemainingTime()
    {
        return -1f;
    }

    public virtual float GetMinigameTotalDuration()
    {
        return 0f;
    }

    private bool SetState(MissionState newState)
    {
        if(State == newState)
        {
            Debug.Log($"## BaseMission ## SetState ## Warning : Already In State ## Mission : {GetType().Name} ## State : {newState}");
            return false;
        }

        State = newState;

        Debug.Log($"## BaseMission ## SetState ## Mission : {GetType().Name} ## State : {State}");

        switch (newState)
        {
            case MissionState.Ready:
                {
                    OnReady();
                }
                break;
            case MissionState.InProgress:
                {
                    OnInProgress();
                }
                break;
            case MissionState.Succeeded:
                {
                    OnSucceeded();
                }
                break;
            case MissionState.Failed:
                {
                    OnFailed();
                }
                break;
        }

        OnMissionStateChanged?.Invoke(newState);

        return true;
    }
}// end class
