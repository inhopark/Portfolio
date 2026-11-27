using UnityEngine;

public class MissionAct1Chapter1 : DefenseMinigameMission
{
    protected override void OnReady()
    {
        base.OnReady();
        Debug.Log("## MissionAct1Chapter1 ## Ready state entered");
    }

    protected override void OnInProgress()
    {
        base.OnInProgress();
        Debug.Log("## MissionAct1Chapter1 ## InProgress state entered");
    }

    protected override void OnSucceeded()
    {
        base.OnSucceeded();
        Debug.Log("## MissionAct1Chapter1 ## Succeeded state entered");
    }

    protected override void OnFailed()
    {
        base.OnFailed();
        Debug.Log("## MissionAct1Chapter1 ## Failed state entered");
    }
}// end class
