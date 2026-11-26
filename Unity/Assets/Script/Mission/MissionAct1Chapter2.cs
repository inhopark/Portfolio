using UnityEngine;

/// No unique content vs Chapter1 by design (matches the UE5 reference exactly) - log tag only.
public class MissionAct1Chapter2 : DefenseMinigameMission
{
    protected override void OnReady()
    {
        base.OnReady();
        Debug.Log("## MissionAct1Chapter2 ## Ready state entered");
    }

    protected override void OnInProgress()
    {
        base.OnInProgress();
        Debug.Log("## MissionAct1Chapter2 ## InProgress state entered");
    }

    protected override void OnSucceeded()
    {
        base.OnSucceeded();
        Debug.Log("## MissionAct1Chapter2 ## Succeeded state entered");
    }

    protected override void OnFailed()
    {
        base.OnFailed();
        Debug.Log("## MissionAct1Chapter2 ## Failed state entered");
    }
}
