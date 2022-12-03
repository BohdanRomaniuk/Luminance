using System.Net;

namespace Luminance.Services.Interfaces
{
    public interface IUdpService
    {
        IPAddress GetLuminanceControllerIp(IPAddress broadcastAddress);
    }
}
