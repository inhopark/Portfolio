using System.Collections.Generic;
using UnityEngine;

public class CharManager : MonoBehaviour
{
    private static CharManager _instance = null;

    public static CharManager Instance
    {
        get
        {
            if(_instance == null)
            {
                GameObject managerObject = new GameObject("CharManager");
                _instance = managerObject.AddComponent<CharManager>();
            }
            return _instance;
        }
    }

    private const string CharDataPath = "Data/CharData";

    private CharData _charData = null;
    private Transform _charRoot = null;

    public PlayerCharacter Player { get; private set; }

    private readonly List<NPC> _npcList = new List<NPC>();

    public IReadOnlyList<NPC> NPCList
    {
        get
        {
            return _npcList;
        }
    }

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
        ClearChar();
        CreateCharRoot();

        _charData = Resources.Load<CharData>(CharDataPath);
        if(_charData == null)
        {
            Debug.LogError($"## CharManager ## Initialize ## Error : CharData Load Failed ## Path : {CharDataPath}");
            return;
        }

        AddPlayer(_charData.Player);

        foreach (CharData.CharEntry entry in _charData.NPCList)
        {
            AddNPC(entry);
        }

        Debug.Log($"## CharManager ## Initialize ## Player : {(Player != null ? Player.name : "None")} ## NPCCount : {_npcList.Count}");
    }

    private void CreateCharRoot()
    {
        if(_charRoot != null)
        {
            return;
        }

        GameObject rootObject = new GameObject("CharRoot");
        rootObject.transform.SetParent(transform, false);
        _charRoot = rootObject.transform;
    }

    private void AddPlayer(CharData.CharEntry entry)
    {
        if(entry == null || entry.Prefab == null)
        {
            Debug.LogError("## CharManager ## AddPlayer ## Error : Prefab Is Null");
            return;
        }

        GameObject playerObject = Instantiate(entry.Prefab, entry.Position, Quaternion.Euler(entry.Rotation), _charRoot);
        Player = playerObject.GetComponent<PlayerCharacter>();
        if(Player == null)
        {
            Debug.LogError($"## CharManager ## AddPlayer ## Error : PlayerCharacter Not Found ## Prefab : {entry.Prefab.name}");
            return;
        }

        Player.SetFollowCamera(Camera.main);
    }

    private void AddNPC(CharData.CharEntry entry)
    {
        if(entry == null || entry.Prefab == null)
        {
            Debug.LogError("## CharManager ## AddNPC ## Error : Prefab Is Null");
            return;
        }

        GameObject npcObject = Instantiate(entry.Prefab, entry.Position, Quaternion.Euler(entry.Rotation), _charRoot);
        NPC npc = npcObject.GetComponent<NPC>();
        if(npc == null)
        {
            Debug.LogError($"## CharManager ## AddNPC ## Error : NPC Not Found ## Prefab : {entry.Prefab.name}");
            Destroy(npcObject);
            return;
        }

        _npcList.Add(npc);
    }

    private void ClearChar()
    {
        if(Player != null)
        {
            Destroy(Player.gameObject);
        }
        Player = null;

        foreach (NPC npc in _npcList)
        {
            if(npc != null)
            {
                Destroy(npc.gameObject);
            }
        }
        _npcList.Clear();
    }
}// end class
