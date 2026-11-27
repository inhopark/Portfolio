using System;
using UnityEngine;
using UnityEngine.UI;

public class MissionResultWidget : BaseWidget
{
    public Text titleText;
    public Button confirmButton;

    public event Action OnConfirmed;

    private void Awake()
    {
        confirmButton.onClick.AddListener(() => OnConfirmed?.Invoke());
    }

    public void SetResultState(MissionState state)
    {
        titleText.text = state == MissionState.Succeeded ? MissionText.MissionSuccess : MissionText.MissionFailed;
    }
}// end class
