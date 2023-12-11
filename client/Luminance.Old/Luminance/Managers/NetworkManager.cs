using Luminance.Managers.Interfaces;

namespace Luminance.Managers
{
    public partial class NetworkManager : INetworkManager
    {
        public string GetApplicationIp() => GetPlatformIpAddress();

        protected partial string GetPlatformIpAddress();
    }
}
