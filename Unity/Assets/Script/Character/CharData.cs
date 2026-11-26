using System;
using System.Collections.Generic;
using UnityEngine;

[CreateAssetMenu(fileName = "CharData", menuName = "MissionSystem/CharData")]
public class CharData : ScriptableObject
{
    [Serializable]
    public class CharEntry
    {
        public GameObject Prefab = null;
        public Vector3 Position = Vector3.zero;
        public Vector3 Rotation = Vector3.zero;
    }

    [SerializeField] private CharEntry _player = new CharEntry();
    [SerializeField] private List<CharEntry> _npcList = new List<CharEntry>();

    public CharEntry Player
    {
        get
        {
            return _player;
        }
    }

    public IReadOnlyList<CharEntry> NPCList
    {
        get
        {
            return _npcList;
        }
    }
}// end class
