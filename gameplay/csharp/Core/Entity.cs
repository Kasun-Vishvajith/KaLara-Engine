using KaLara.Engine.Native;

namespace KaLara.Engine;

public class Entity
{
    public ulong Id { get; protected set; }

    public Entity()
    {
        Id = 0;
    }

    public Entity(ulong id)
    {
        Id = id;
    }

    public Vector3 Position
    {
        get
        {
            float x = 0, y = 0, z = 0;
            unsafe
            {
                InternalCalls.KaLara_Transform_GetPosition(Id, &x, &y, &z);
            }
            return new Vector3(x, y, z);
        }
        set
        {
            InternalCalls.KaLara_Transform_SetPosition(Id, value.X, value.Y, value.Z);
        }
    }
}
