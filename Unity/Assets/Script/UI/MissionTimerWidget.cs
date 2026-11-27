using UnityEngine;
using UnityEngine.UI;

public class MissionTimerWidget : BaseWidget
{
    public Slider timerBar;
    public Text timerText;

    private void Update()
    {
        float remaining = MissionManager.Instance.GetMissionRemainingTime();
        if (remaining < 0f)
        {
            return;
        }

        float total = MissionManager.Instance.GetMissionTotalDuration();
        if (timerBar != null && total > 0f)
        {
            timerBar.value = remaining / total;
        }

        timerText.text = Mathf.CeilToInt(remaining).ToString();
    }
}// end class
