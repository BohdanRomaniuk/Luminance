using System.Net;

namespace Luminance.Client.Helpers
{
    public static class IpAdressHelper
    {
        public static IPAddress GetBroadcastAddress(this IPAddress address, IPAddress subnetMask)
        {
            var ip = address.GetAddressBytes();
            var mask = subnetMask.GetAddressBytes();

            if (ip.Length != mask.Length)
            {
                return new IPAddress(0);
            }

            var result = new byte[ip.Length];
            for (int i = 0; i < result.Length; ++i)
            {
                result[i] = (byte)(ip[i] | (mask[i] ^ 255));
            }

            return new IPAddress(result);
        }

        public static IPAddress GetNetworkBaseAddress(this IPAddress address, IPAddress subnetMask)
        {
            var ip = address.GetAddressBytes();
            var mask = subnetMask.GetAddressBytes();

            if (ip.Length != mask.Length)
            {
                return new IPAddress(0);
            }

            var result = new byte[ip.Length];
            for (int i = 0; i < result.Length; ++i)
            {
                result[i] = (byte)(ip[i] & mask[i]);
            }

            return new IPAddress(result);
        }
    }
}
