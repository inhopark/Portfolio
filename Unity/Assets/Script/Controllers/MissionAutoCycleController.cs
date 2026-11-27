using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MissionAutoCycleController
{
    private const float AutoApproachMoveSpeed = 6.5f;
    private const float AutoDodgeMoveSpeed = 5f;
    private const float DodgeSwitchDistance = 2.5f;
    private const float NPCArriveDistance = 1.5f;
    private const float AutoAgreeDelay = 1.5f;
    private const float AutoConfirmDelay = 1.5f;
    private const float AutoCycleSurvivalDuration = 5f;

    private ICoroutineRunner _runner = null;
    private PlayerCharacter _player = null;
    private NPC _targetNPC = null;

    private bool _isEnabled = false;

    private Coroutine _approachRoutine = null;
    private Coroutine _dodgeRoutine = null;
    private Coroutine _actionDelayRoutine = null;

    public void Initialize(ICoroutineRunner runner, PlayerCharacter player)
    {
        _runner = runner;
        _player = player;
    }

    public void Deinitialize()
    {
        StopAutoCycle();
    }

    public void ToggleAutoCycleMode()
    {
        if(_isEnabled == true)
        {
            StopAutoCycle();
            return;
        }

        StartAutoCycle();
    }

    private void StartAutoCycle()
    {
        if(_player == null)
        {
            Debug.LogError("## MissionAutoCycleController ## StartAutoCycle ## Error : Player Character Not Found");
            return;
        }

        _targetNPC = FindNearestNPC();
        if(_targetNPC == null)
        {
            Debug.LogError("## MissionAutoCycleController ## StartAutoCycle ## Error : NPC Not Found In Scene");
            return;
        }

        SetAutoCycleEnabled(true);
        MissionManager.Instance.OnAnyMissionStateChanged += HandleMissionStateChanged;
        StartApproachingTargetNPC();
    }

    private void StopAutoCycle()
    {
        StopApproachingTargetNPC();
        StopDodging();
        StopActionDelay();

        _targetNPC = null;

        if(_isEnabled == false)
        {
            return;
        }

        if(MissionManager.IsInstanceValid() == true)
        {
            MissionManager.Instance.OnAnyMissionStateChanged -= HandleMissionStateChanged;
        }
        SetAutoCycleEnabled(false);
    }

    private void SetAutoCycleEnabled(bool isEnabled)
    {
        _isEnabled = isEnabled;

        if(_player != null)
        {
            _player.SetAutoControlActive(isEnabled);
        }

        Debug.Log($"## MissionAutoCycleController ## SetAutoCycleEnabled ## IsEnabled : {isEnabled}");
    }

    private void StartApproachingTargetNPC()
    {
        StopApproachingTargetNPC();
        _approachRoutine = _runner.StartCoroutine(ApproachTargetNPCRoutine());
    }

    private void StopApproachingTargetNPC()
    {
        StopRoutine(ref _approachRoutine);
    }

    private void StartDodging()
    {
        StopDodging();
        _dodgeRoutine = _runner.StartCoroutine(DodgeRoutine());
    }

    private void StopDodging()
    {
        StopRoutine(ref _dodgeRoutine);
    }

    private void StartActionDelay(float delay, Action action)
    {
        StopActionDelay();
        _actionDelayRoutine = _runner.StartCoroutine(ActionDelayRoutine(delay, action));
    }

    private void StopActionDelay()
    {
        StopRoutine(ref _actionDelayRoutine);
    }

    private IEnumerator ApproachTargetNPCRoutine()
    {
        while (_targetNPC != null && Vector3.Distance(_player.transform.position, _targetNPC.transform.position) > NPCArriveDistance)
        {
            Vector3 direction = _targetNPC.transform.position - _player.transform.position;
            direction.y = 0f;
            if(direction.sqrMagnitude > 0.0001f)
            {
                _player.AutoMove(direction.normalized * AutoApproachMoveSpeed * Time.deltaTime);
            }
            yield return null;
        }

        _player.AutoMove(Vector3.zero);
        _approachRoutine = null;
    }

    private IEnumerator DodgeRoutine()
    {
        float direction = 1f;
        float traveled = 0f;

        while (true)
        {
            float step = AutoDodgeMoveSpeed * Time.deltaTime;
            _player.AutoMove(Vector3.right * direction * step);
            traveled += step;

            if(traveled >= DodgeSwitchDistance)
            {
                traveled = 0f;
                direction *= -1f;
            }

            yield return null;
        }
    }

    private IEnumerator ActionDelayRoutine(float delay, Action action)
    {
        yield return new WaitForSeconds(delay);

        _actionDelayRoutine = null;
        action?.Invoke();
    }

    private void HandleMissionStateChanged(MissionState newState)
    {
        if(_isEnabled == false)
        {
            return;
        }

        switch (newState)
        {
            case MissionState.Ready:
                {
                    StopApproachingTargetNPC();
                    _player.AutoMove(Vector3.zero);
                    StartActionDelay(AutoAgreeDelay, TriggerAutoAgree);
                }
                break;
            case MissionState.InProgress:
                {
                    StartDodging();
                }
                break;
            case MissionState.Succeeded:
            case MissionState.Failed:
                {
                    StopDodging();
                    _player.AutoMove(Vector3.zero);
                    StartActionDelay(AutoConfirmDelay, TriggerAutoConfirm);
                }
                break;
        }
    }

    private void TriggerAutoAgree()
    {
        DefenseMinigameMission defenseMission = MissionManager.Instance.CurrentMission as DefenseMinigameMission;
        if(defenseMission != null)
        {
            defenseMission.GetDefenseMinigameController().SetSurvivalDurationOverride(AutoCycleSurvivalDuration);
        }

        MissionManager.Instance.HandleMissionButtonAction(MissionButtonAction.Left);
    }

    private void TriggerAutoConfirm()
    {
        MissionManager.Instance.HandleMissionResultConfirmed();

        StopAutoCycle();
    }

    private NPC FindNearestNPC()
    {
        IReadOnlyList<NPC> npcs = CharManager.Instance.NPCList;
        NPC nearest = null;
        float bestDistance = float.MaxValue;

        foreach (NPC npc in npcs)
        {
            if(npc == null)
            {
                continue;
            }

            float distance = Vector3.Distance(_player.transform.position, npc.transform.position);
            if(distance < bestDistance)
            {
                bestDistance = distance;
                nearest = npc;
            }
        }

        return nearest;
    }

    private void StopRoutine(ref Coroutine routine)
    {
        if(routine == null)
        {
            return;
        }

        if(_runner != null)
        {
            _runner.StopCoroutine(routine);
        }
        routine = null;
    }
}// end class
