using UnityEngine;
using UnityEngine.InputSystem;

/// Owns the 4 UI widget instances and registers them with MissionManager.
public class MainController : MonoBehaviour, ICoroutineRunner
{
    private readonly MissionAutoCycleController _autoCycleController = new MissionAutoCycleController();

    private void Awake()
    {
        InitManager();
    }

    private void Start()
    {
        MissionManager.Instance.RegisterMainController(this);

        _autoCycleController.Initialize(this, CharManager.Instance.Player);
    }

    private void Update()
    {
        if(Keyboard.current != null && Keyboard.current.f8Key.wasPressedThisFrame == true)
        {
            ToggleAutoCycleMode();
        }
    }

    private void OnDestroy()
    {
        _autoCycleController.Deinitialize();
    }

    public void SetDefenseHUDVisible(bool visible)
    {
        HPWidget hpWidget = UIManager.Instance.GetUI<HPWidget>(EUIType.HP);
        MissionTimerWidget missionTimerWidget = UIManager.Instance.GetUI<MissionTimerWidget>(EUIType.MissionTimer);
        if(visible == true)
        {
            hpWidget.Show();
            missionTimerWidget.Show();
        }
        else
        {
            hpWidget.Hide();
            missionTimerWidget.Hide();
        }

        Debug.Log($"## MainController ## SetDefenseHUDVisible ## Visible : {visible}");
    }

    public void ToggleAutoCycleMode()
    {
        _autoCycleController.ToggleAutoCycleMode();
    }

    private void InitManager()
    {
        UIManager.Instance.Initialize();
        CharManager.Instance.Initialize();
        MissionManager.Instance.Initialize();
    }
}// end class
