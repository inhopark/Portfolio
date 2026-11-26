using UnityEngine;
using UnityEngine.UI;

/// Pure display: subscribes to the player's OnHPChanged event, smooths the bar toward the target.
public class HPWidget : BaseWidget
{
    public Slider hpBar;
    public Text hpText;
    public CanvasGroup canvasGroup;

    [SerializeField] private float barInterpSpeed = 2f;

    private float _targetPercent = 1f;

    private PlayerCharacter _boundPlayer = null;

    public void SetHP(float current, float max)
    {
        _targetPercent = max > 0f ? current / max : 0f;
        hpText.text = $"{Mathf.RoundToInt(current)} / {Mathf.RoundToInt(max)}";
    }

    private void Update()
    {
        hpBar.value = Mathf.Lerp(hpBar.value, _targetPercent, 1f - Mathf.Exp(-barInterpSpeed * Time.deltaTime));
    }

    private void OnDestroy()
    {
        UnbindPlayer();
    }

    public override void Show()
    {
        BindPlayer();
        SetVisible(true);
    }

    public override void Hide()
    {
        UnbindPlayer();
        SetVisible(false);
    }

    private void SetVisible(bool visible)
    {
        canvasGroup.alpha = visible == true ? 1f : 0f;
        canvasGroup.blocksRaycasts = false;
        canvasGroup.interactable = false;
    }

    private void BindPlayer()
    {
        UnbindPlayer();

        PlayerCharacter player = CharManager.Instance.Player;
        if(player == null)
        {
            Debug.LogError("## HPWidget ## BindPlayer ## Error : Player Not Found");
            return;
        }

        _boundPlayer = player;
        _boundPlayer.OnHPChanged += SetHP;
        SetHP(_boundPlayer.CurrentHP, _boundPlayer.MaxHP);

        Debug.Log($"## HPWidget ## BindPlayer ## Player : {_boundPlayer.name}");
    }

    private void UnbindPlayer()
    {
        if(_boundPlayer == null)
        {
            return;
        }

        _boundPlayer.OnHPChanged -= SetHP;
        _boundPlayer = null;
    }
}// end class
