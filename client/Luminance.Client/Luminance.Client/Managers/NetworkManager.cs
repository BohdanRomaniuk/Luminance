using Luminance.Client.Managers.Interfaces;

namespace Luminance.Client.Managers
{
    public partial class NetworkManager : INetworkManager
    {
        public string GetApplicationIp() => GetPlatformIpAddress();

        protected partial string GetPlatformIpAddress();
    }
}
