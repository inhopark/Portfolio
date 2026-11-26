using System;
using System.Collections.Generic;
using UnityEngine;

[CreateAssetMenu(fileName = "UIData", menuName = "MissionSystem/UIData")]
public class UIData : ScriptableObject
{
    [Serializable]
    public class UIEntry
    {
        public EUIType Type = EUIType.MainMission;
        public GameObject Prefab = null;
    }

    [SerializeField] private List<UIEntry> _uiList = new List<UIEntry>();

    public IReadOnlyList<UIEntry> UIList
    {
        get
        {
            return _uiList;
        }
    }
}// end class
