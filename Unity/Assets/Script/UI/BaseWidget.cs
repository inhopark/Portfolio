using UnityEngine;

public abstract class BaseWidget : MonoBehaviour
{
    [SerializeField] private EWidgetVisibilityMode _visibilityMode = EWidgetVisibilityMode.Active;

    private CanvasGroup _canvasGroup = null;

    public void Show()
    {
        SetVisible(true);
        OnShow();
    }

    public void Hide()
    {
        OnHide();
        SetVisible(false);
    }

    protected virtual void OnShow()
    {
    }

    protected virtual void OnHide()
    {
    }

    private void SetVisible(bool isVisible)
    {
        if(_visibilityMode == EWidgetVisibilityMode.CanvasGroup)
        {
            if(_canvasGroup == null)
            {
                _canvasGroup = GetComponent<CanvasGroup>();
            }

            if(_canvasGroup == null)
            {
                Debug.LogError($"## BaseWidget ## SetVisible ## Error : CanvasGroup Not Found ## Widget : {name}");
                return;
            }

            _canvasGroup.alpha = isVisible == true ? 1f : 0f;
            _canvasGroup.blocksRaycasts = false;
            _canvasGroup.interactable = false;
            return;
        }

        gameObject.SetActive(isVisible);
    }
}// end class
