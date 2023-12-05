using System.Net;

namespace Luminance.Client.Services.Interfaces
{
    public interface IUdpService
    {
        IPAddress GetLuminanceControllerIp(IPAddress broadcastAddress);
    }
}
