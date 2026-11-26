using System.Collections.Generic;
using UnityEngine;

public class UIManager : MonoBehaviour
{
    private static UIManager _instance = null;

    public static UIManager Instance
    {
        get
        {
            if(_instance == null)
            {
                GameObject managerObject = new GameObject("UIManager");
                _instance = managerObject.AddComponent<UIManager>();
            }
            return _instance;
        }
    }

    private const string UIDataPath = "Data/UIData";

    private UIData _uiData = null;
    private Transform _widgetRoot = null;

    private readonly Dictionary<EUIType, GameObject> _uiMap = new Dictionary<EUIType, GameObject>();

    private void Awake()
    {
        if(_instance != null && _instance != this)
        {
            Destroy(gameObject);
            return;
        }

        _instance = this;
        DontDestroyOnLoad(gameObject);
    }

    private void OnDestroy()
    {
        if(_instance == this)
        {
            _instance = null;
        }
    }

    public void Initialize()
    {
        ClearUI();
        CreateWidgetRoot();

        _uiData = Resources.Load<UIData>(UIDataPath);
        if(_uiData == null)
        {
            Debug.LogError($"## UIManager ## Initialize ## Error : UIData Load Failed ## Path : {UIDataPath}");
            return;
        }

        foreach (UIData.UIEntry entry in _uiData.UIList)
        {
            AddUI(entry);
        }

        Debug.Log($"## UIManager ## Initialize ## UICount : {_uiMap.Count}");
    }

    public T GetUI<T>(EUIType type) where T : Component
    {
        GameObject uiObject = null;
        if(_uiMap.TryGetValue(type, out uiObject) == false)
        {
            Debug.LogError($"## UIManager ## GetUI ## Error : UI Not Found ## Type : {type}");
            return null;
        }

        T component = uiObject.GetComponentInChildren<T>(true);
        if(component == null)
        {
            Debug.LogError($"## UIManager ## GetUI ## Error : Component Not Found ## Type : {type} ## Component : {typeof(T).Name}");
        }
        return component;
    }

    private void CreateWidgetRoot()
    {
        if(_widgetRoot != null)
        {
            return;
        }

        GameObject rootObject = new GameObject("WidgetRoot");
        rootObject.transform.SetParent(transform, false);
        _widgetRoot = rootObject.transform;
    }

    private void AddUI(UIData.UIEntry entry)
    {
        if(entry == null || entry.Prefab == null)
        {
            Debug.LogError("## UIManager ## AddUI ## Error : Prefab Is Null");
            return;
        }

        if(_uiMap.ContainsKey(entry.Type) == true)
        {
            Debug.LogError($"## UIManager ## AddUI ## Error : Duplicate UI ## Type : {entry.Type}");
            return;
        }

        GameObject uiObject = Instantiate(entry.Prefab, _widgetRoot, false);
        _uiMap.Add(entry.Type, uiObject);

        BaseWidget widget = uiObject.GetComponentInChildren<BaseWidget>(true);
        if(widget == null)
        {
            Debug.LogError($"## UIManager ## AddUI ## Error : BaseWidget Not Found ## Type : {entry.Type}");
            return;
        }

        widget.Hide();
    }

    private void ClearUI()
    {
        foreach (GameObject uiObject in _uiMap.Values)
        {
            if(uiObject != null)
            {
                Destroy(uiObject);
            }
        }
        _uiMap.Clear();
    }
}// end class
