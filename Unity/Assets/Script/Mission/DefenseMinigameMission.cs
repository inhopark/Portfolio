using UnityEngine;

public abstract class DefenseMinigameMission : BaseMission
{
    private DefenseMinigameController _defenseMinigame;

    protected override void InitData()
    {
        base.InitData();
        _defenseMinigame = new DefenseMinigameController();
        _defenseMinigame.OnSurvived += HandleMinigameSurvived;
    }

    protected override void OnInProgress()
    {
        base.OnInProgress();
        if(_defenseMinigame == null)
        {
            Debug.LogError($"## DefenseMinigameMission ## OnInProgress ## Error : DefenseMinigame Not Initialized ## Mission : {GetType().Name}");
            return;
        }

        _defenseMinigame.Start(MissionPlayer, MissionManager.Instance);
    }

    protected override void OnSucceeded()
    {
        base.OnSucceeded();
        if(_defenseMinigame != null)
        {
            _defenseMinigame.Stop();
        }
    }

    protected override void OnFailed()
    {
        base.OnFailed();
        if(_defenseMinigame != null)
        {
            _defenseMinigame.Stop();
        }
    }

    public override float GetMinigameRemainingTime()
    {
        if(_defenseMinigame == null)
        {
            return -1f;
        }

        return _defenseMinigame.GetRemainingTime();
    }

    public override float GetMinigameTotalDuration()
    {
        if(_defenseMinigame == null)
        {
            return 0f;
        }

        return _defenseMinigame.SurvivalDuration;
    }

    public DefenseMinigameController GetDefenseMinigameController()
    {
        return _defenseMinigame;
    }

    private void HandleMinigameSurvived()
    {
        SuccessMission();
    }
}// end class
