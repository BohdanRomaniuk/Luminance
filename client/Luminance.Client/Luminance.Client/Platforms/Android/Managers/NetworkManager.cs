using Java.Net;
using System.Text.RegularExpressions;

namespace Luminance.Client.Managers
{
    public partial class NetworkManager
    {
        private Regex _ipAddressRegex = new Regex("\\b(?:\\d{1,3}\\.){3}\\d{1,3}\\b", RegexOptions.Compiled);

        protected partial string GetPlatformIpAddress()
        {
            var result = "0.0.0.0";
            var interfaces = NetworkInterface.NetworkInterfaces;
            while (interfaces?.HasMoreElements == true)
            {
                if (interfaces.NextElement() is not NetworkInterface inter)
                {
                    continue;
                }

                var addreses = inter.InetAddresses;
                while (addreses?.HasMoreElements == true)
                {
                    if (addreses.NextElement() is not InetAddress inetAddress)
                    {
                        continue;
                    }

                    if (inetAddress.IsLoopbackAddress 
                        && result == "0.0.0.0" 
                        && !string.IsNullOrWhiteSpace(inetAddress.HostAddress) 
                        && _ipAddressRegex.Match(inetAddress.HostAddress).Success)
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
