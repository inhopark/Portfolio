using UnityEngine;

/// Template Method: owns and drives a DefenseMinigameController through the mission's own state hooks.
public abstract class DefenseMinigameMission : BaseMission
{
    private DefenseMinigameController _defenseMinigame;

    public DefenseMinigameController GetDefenseMinigameController()
    {
        return _defenseMinigame;
    }

    protected override void InitData()
    {
        base.InitData();
        _defenseMinigame = new DefenseMinigameController();
        _defenseMinigame.OnSurvived += HandleMinigameSurvived;
    }

    protected override void OnInProgress()
    {
        base.OnInProgress();
        if (_defenseMinigame == null)
        {
            Debug.LogError($"## DefenseMinigameMission ## OnInProgress ## Error : DefenseMinigame Not Initialized ## Mission : {GetType().Name}");
            return;
        }

        _defenseMinigame.Start(MissionPlayer, MissionManager.Instance);
    }

    protected override void OnSucceeded()
    {
        base.OnSucceeded();
        _defenseMinigame?.Stop();
    }

    protected override void OnFailed()
    {
        base.OnFailed();
        _defenseMinigame?.Stop();
    }

    private void HandleMinigameSurvived()
    {
        SuccessMission();
    }

    public override float GetMinigameRemainingTime()
    {
        return _defenseMinigame?.GetRemainingTime() ?? -1f;
    }

    public override float GetMinigameTotalDuration()
    {
        return _defenseMinigame?.SurvivalDuration ?? 0f;
    }
}
