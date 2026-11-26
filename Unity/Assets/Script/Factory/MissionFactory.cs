using System;
using System.Collections.Generic;
using UnityEngine;

public class MissionFactory
{
    private readonly Dictionary<MissionUnique, Type> _missionTypes = new Dictionary<MissionUnique, Type>();

    public void Initialize()
    {
        RegisterMission<MissionAct1Chapter1>(MissionUnique.Act1Chapter1);
        RegisterMission<MissionAct1Chapter2>(MissionUnique.Act1Chapter2);
    }

    public BaseMission CreateMission(MissionUnique missionUnique)
    {
        if(_missionTypes.TryGetValue(missionUnique, out Type missionType) == false)
        {
            Debug.LogError($"## MissionFactory ## CreateMission ## Error : Mission Not Registered ## MissionUnique : {missionUnique}");
            return null;
        }

        return (BaseMission)Activator.CreateInstance(missionType);
    }

    private void RegisterMission<T>(MissionUnique missionUnique) where T : BaseMission, new()
    {
        if(_missionTypes.ContainsKey(missionUnique) == true)
        {
            Debug.LogError($"## MissionFactory ## RegisterMission ## Error : Duplicate Mission ## MissionUnique : {missionUnique}");
            return;
        }

        _missionTypes.Add(missionUnique, typeof(T));
    }
}// end class
