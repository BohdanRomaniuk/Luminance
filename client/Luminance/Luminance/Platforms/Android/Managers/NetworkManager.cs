using Java.Net;
using System.Text.RegularExpressions;

namespace Luminance.Managers
{
    public partial class NetworkManager
    {
        protected partial string GetPlatformIpAddress()
        {
            var result = "0.0.0.0";
            var interfaces = NetworkInterface.NetworkInterfaces;
            while (interfaces.HasMoreElements)
            {
                var inter = (NetworkInterface)interfaces.NextElement();
                var addreses = inter.InetAddresses;
                while (addreses.HasMoreElements)
                {
                    var inetAddress = (InetAddress)addreses.NextElement();
                    if (!inetAddress.IsLoopbackAddress && result == "0.0.0.0" && Regex.Match(inetAddress.HostAddress, "\\b(?:\\d{1,3}\\.){3}\\d{1,3}\\b").Success)
                    {
                        result = inetAddress.HostAddress;
                        break;
                    }
                }

                if (result != "0.0.0.0")
                {
                    break;
                }
            }

            return result;
        }
    }
}
