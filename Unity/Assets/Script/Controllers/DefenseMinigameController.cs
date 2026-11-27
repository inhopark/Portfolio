using System;
using System.Collections;
using UnityEngine;

public class DefenseMinigameController
{
    public event Action OnSurvived;

    public float SurvivalDuration { get; private set; } = 30f;

    private MonsterSpawner _spawner = null;
    private ICoroutineRunner _runner = null;
    private Coroutine _survivalRoutine = null;
    private float _remainingTime = 0f;
    private bool _isRunning = false;

    public void SetSurvivalDurationOverride(float seconds)
    {
        SurvivalDuration = seconds;

        Debug.Log($"## DefenseMinigameController ## SetSurvivalDurationOverride ## SurvivalDuration : {SurvivalDuration}");
    }

    public void Start(PlayerCharacter player, ICoroutineRunner runner)
    {
        _runner = runner;
        _remainingTime = SurvivalDuration;
        _isRunning = true;

        _spawner = CreateMonsterSpawner();
        _spawner.StartSpawning(player);

        IEnumerator survivalTimerRoutine = SurvivalTimerRoutine();
        _survivalRoutine = runner.StartCoroutine(survivalTimerRoutine);
    }

    public void Stop()
    {
        _isRunning = false;

        if(_survivalRoutine != null && _runner != null)
        {
            _runner.StopCoroutine(_survivalRoutine);
            _survivalRoutine = null;
        }

        if(_spawner != null)
        {
            _spawner.StopSpawning();
            UnityEngine.Object.Destroy(_spawner.gameObject);
            _spawner = null;
        }
    }

    public float GetRemainingTime()
    {
        return _isRunning == true ? _remainingTime : -1f;
    }

    private static MonsterSpawner CreateMonsterSpawner()
    {
        GameObject monsterSpawnerObject = new GameObject("MonsterSpawner");
        return monsterSpawnerObject.AddComponent<MonsterSpawner>();
    }

    private IEnumerator SurvivalTimerRoutine()
    {
        while (_remainingTime > 0f)
        {
            yield return null;
            _remainingTime -= Time.deltaTime;
        }

        _remainingTime = 0f;
        _isRunning = false;
        OnSurvived?.Invoke();
    }
}// end class
