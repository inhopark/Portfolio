using System;
using UnityEngine;
using UnityEngine.UI;

public class MainMissionWidget : BaseWidget
{
    public Text titleText;
    public Button leftButton;
    public Text leftButtonText;
    public Button rightButton;
    public Text rightButtonText;

    public event Action<MissionButtonAction> OnMissionButtonAction;

    private void Awake()
    {
        leftButton.onClick.AddListener(() => OnMissionButtonAction?.Invoke(MissionButtonAction.Left));
        rightButton.onClick.AddListener(() => OnMissionButtonAction?.Invoke(MissionButtonAction.Right));
    }

    public void SetWidgetState(MissionState state)
    {
        leftButton.gameObject.SetActive(true);
        rightButton.gameObject.SetActive(true);

        switch (state)
        {
            case MissionState.Ready:
                {
                    titleText.text = MissionText.WaitForMission;
                    leftButtonText.text = MissionText.AgreeButton;
                    rightButtonText.text = MissionText.DisagreeButton;
                }
                break;
            case MissionState.InProgress:
                {
                    titleText.text = MissionText.MissionInProgress;
                    leftButtonText.text = MissionText.SuccessButton;
                    rightButtonText.text = MissionText.FailButton;
                }
                break;
        }
    }
}// end class
