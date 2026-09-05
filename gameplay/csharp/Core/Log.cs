using KaLara.Engine.Native;

namespace KaLara.Engine;

public static class Log
{
    public static void Info(string message) => InternalCalls.KaLara_Log_Info(message);
    public static void Warn(string message) => InternalCalls.KaLara_Log_Warn(message);
    public static void Error(string message) => InternalCalls.KaLara_Log_Error(message);
}
