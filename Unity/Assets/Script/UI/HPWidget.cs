using UnityEngine;
using UnityEngine.UI;

public class HPWidget : BaseWidget
{
    public Slider hpBar;
    public Text hpText;

    [SerializeField] private float barInterpSpeed = 2f;

    private float _targetPercent = 1f;

    private PlayerCharacter _boundPlayer = null;

    private void Update()
    {
        hpBar.value = Mathf.Lerp(hpBar.value, _targetPercent, 1f - Mathf.Exp(-barInterpSpeed * Time.deltaTime));
    }

    private void OnDestroy()
    {
        UnbindPlayer();
    }

    protected override void OnShow()
    {
        base.OnShow();
        BindPlayer();
    }

    protected override void OnHide()
    {
        base.OnHide();
        UnbindPlayer();
    }

    public void SetHP(float current, float max)
    {
        _targetPercent = max > 0f ? current / max : 0f;
        hpText.text = $"{Mathf.RoundToInt(current)} / {Mathf.RoundToInt(max)}";
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
