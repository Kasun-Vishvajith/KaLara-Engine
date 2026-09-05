using System.Runtime.InteropServices;

namespace KaLara.Engine.Native;

internal static unsafe class InternalCalls
{
    [DllImport("__Internal", EntryPoint = "KaLara_Log_Info")]
    internal static extern void KaLara_Log_Info(string message);

    [DllImport("__Internal", EntryPoint = "KaLara_Log_Warn")]
    internal static extern void KaLara_Log_Warn(string message);

    [DllImport("__Internal", EntryPoint = "KaLara_Log_Error")]
    internal static extern void KaLara_Log_Error(string message);

    [DllImport("__Internal", EntryPoint = "KaLara_Entity_Create")]
    internal static extern ulong KaLara_Entity_Create(string name);

    [DllImport("__Internal", EntryPoint = "KaLara_Entity_Destroy")]
    internal static extern void KaLara_Entity_Destroy(ulong entityId);

    [DllImport("__Internal", EntryPoint = "KaLara_Transform_GetPosition")]
    internal static extern void KaLara_Transform_GetPosition(ulong entityId, float* outX, float* outY, float* outZ);

    [DllImport("__Internal", EntryPoint = "KaLara_Transform_SetPosition")]
    internal static extern void KaLara_Transform_SetPosition(ulong entityId, float x, float y, float z);

    [DllImport("__Internal", EntryPoint = "KaLara_Input_IsKeyPressed")]
    internal static extern bool KaLara_Input_IsKeyPressed(ushort keyCode);
}
