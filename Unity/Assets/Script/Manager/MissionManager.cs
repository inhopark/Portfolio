using System;
using UnityEngine;

/// Facade/Mediator: the single hub NPCs and widgets talk to. Widgets and missions never
/// reference each other directly - command flow goes down (this calls widget/mission methods),
/// event flow goes up (mission/widget events, consumed here).
public class MissionManager : MonoBehaviour, ICoroutineRunner
{
    private static MissionManager _instance = null;

    public static MissionManager Instance
    {
        get
        {
            if(_instance == null)
            {
                GameObject managerObject = new GameObject("MissionManager");
                _instance = managerObject.AddComponent<MissionManager>();
            }
            return _instance;
        }
    }

    public event Action<MissionState> OnAnyMissionStateChanged;

    public BaseMission CurrentMission { get; private set; }
    public NPC CurrentMissionNPC { get; private set; }

    private readonly MissionFactory _missionFactory = new MissionFactory();

    private MainController _mainController = null;
    private MainMissionWidget _mainMissionWidget = null;
    private MissionResultWidget _missionResultWidget = null;

    private void Awake()
    {
        if(_instance != null && _instance != this)
        {
            Destroy(gameObject);
            return;
        }

        _instance = this;
        DontDestroyOnLoad(gameObject);

        _missionFactory.Initialize();
    }

    private void OnDestroy()
    {
        if(_instance == this)
        {
            _instance = null;
        }
    }

    public static bool IsInstanceValid()
    {
        return _instance != null;
    }

    public void Initialize()
    {
        RegisterMainMissionWidget(UIManager.Instance.GetUI<MainMissionWidget>(EUIType.MainMission));
        RegisterMissionResultWidget(UIManager.Instance.GetUI<MissionResultWidget>(EUIType.MissionResult));

        Debug.Log("## MissionManager ## Initialize ## Completed");
    }

    public void RegisterMainController(MainController controller)
    {
        if(controller == null)
        {
            Debug.LogError("## MissionManager ## RegisterMainController ## Error : Controller Is Null");
            return;
        }

        _mainController = controller;
    }

    public void RegisterMainMissionWidget(MainMissionWidget widget)
    {
        if(widget == null)
        {
            Debug.LogError("## MissionManager ## RegisterMainMissionWidget ## Error : Widget Is Null");
            return;
        }

        if(_mainMissionWidget != null)
        {
            _mainMissionWidget.OnMissionButtonAction -= HandleMissionButtonAction;
        }

        _mainMissionWidget = widget;
        _mainMissionWidget.OnMissionButtonAction += HandleMissionButtonAction;
    }

    public void RegisterMissionResultWidget(MissionResultWidget widget)
    {
        if(widget == null)
        {
            Debug.LogError("## MissionManager ## RegisterMissionResultWidget ## Error : Widget Is Null");
            return;
        }

        if(_missionResultWidget != null)
        {
            _missionResultWidget.OnConfirmed -= HandleMissionResultConfirmed;
        }

        _missionResultWidget = widget;
        _missionResultWidget.OnConfirmed += HandleMissionResultConfirmed;
    }

    public bool IsMissionRequest()
    {
        return CurrentMission == null;
    }

    public void ShowMainMissionWidget(MissionUnique missionUnique, NPC npc)
    {
        if(CurrentMission != null)
        {
            return;
        }

        if(_mainMissionWidget == null)
        {
            Debug.LogError($"## MissionManager ## ShowMainMissionWidget ## Error : MainMissionWidget Not Registered ## MissionUnique : {missionUnique}");
            return;
        }

        CurrentMissionNPC = npc;

        SetUICursorMode(true);

        _mainMissionWidget.Show();

        RequestMission(missionUnique);
    }

    public void HideMainMissionWidget()
    {
        if(_mainMissionWidget != null)
        {
            _mainMissionWidget.Hide();
        }
        SetUICursorMode(false);
    }

    public void HandleMissionButtonAction(MissionButtonAction action)
    {
        if(CurrentMission == null || CurrentMission.State != MissionState.Ready)
        {
            return;
        }

        if(action == MissionButtonAction.Left)
        {
            PlayerCharacter player = GetPlayerCharacter();
            if(player == null)
            {
                Debug.LogError("## MissionManager ## HandleMissionButtonAction ## Error : Player Not Found");
                return;
            }

            if(CurrentMissionNPC != null)
            {
                CurrentMissionNPC.Hide();
            }
            player.SetPlayMode(PlayMode.Defense);
            CurrentMission.AgreeMission(player);
            SetDefenseHUDVisible(true);
            HideMainMissionWidget();
        }
        else
        {
            CurrentMission.DisagreeMission();
            ClearCurrentMission();
        }
    }

    public void HandlePlayerDefeated()
    {
        if(CurrentMission != null)
        {
            CurrentMission.FailedMission();
        }
    }

    public void HandleMissionResultConfirmed()
    {
        _missionResultWidget.Hide();

        ClearCurrentMission();
    }

    public void SetUICursorMode(bool uiMode)
    {
        Cursor.lockState = uiMode == true ? CursorLockMode.None : CursorLockMode.Locked;
        Cursor.visible = uiMode;

        PlayerCharacter player = GetPlayerCharacter();
        if(player != null)
        {
            player.SetMovementLocked(uiMode);
        }
    }

    public float GetMissionRemainingTime()
    {
        if(CurrentMission == null)
        {
            return -1f;
        }

        return CurrentMission.GetMinigameRemainingTime();
    }

    public float GetMissionTotalDuration()
    {
        if(CurrentMission == null)
        {
            return 0f;
        }

        return CurrentMission.GetMinigameTotalDuration();
    }

    private void RequestMission(MissionUnique missionUnique)
    {
        if(CurrentMission != null)
        {
            return;
        }

        CurrentMission = _missionFactory.CreateMission(missionUnique);
        if(CurrentMission == null)
        {
            return;
        }

        CurrentMission.OnMissionStateChanged += HandleMissionStateChanged;
        CurrentMission.Initialize();
    }

    private void HandleMissionStateChanged(MissionState newState)
    {
        if(newState == MissionState.Ready || newState == MissionState.InProgress)
        {
            _mainMissionWidget.SetWidgetState(newState);
        }
        else if(newState == MissionState.Succeeded || newState == MissionState.Failed)
        {
            SetDefenseHUDVisible(false);
            ShowMissionResultWidget(newState);
        }

        OnAnyMissionStateChanged?.Invoke(newState);
    }

    private void ShowMissionResultWidget(MissionState resultState)
    {
        SetUICursorMode(true);
        _missionResultWidget.SetResultState(resultState);
        _missionResultWidget.Show();
    }

    private void ClearCurrentMission()
    {
        if(CurrentMission != null)
        {
            CurrentMission.OnMissionStateChanged -= HandleMissionStateChanged;
            CurrentMission = null;
        }

        CharManager.Instance.ResetPlayer();

        if(CurrentMissionNPC != null)
        {
            CurrentMissionNPC.Show();
        }
        CurrentMissionNPC = null;

        HideMainMissionWidget();
    }

    private void SetDefenseHUDVisible(bool visible)
    {
        if(_mainController != null)
        {
            _mainController.SetDefenseHUDVisible(visible);
        }
    }

    private PlayerCharacter GetPlayerCharacter()
    {
        return CharManager.Instance.Player;
    }
}// end class
