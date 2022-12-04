using Luminance.Helpers;
using Luminance.Services.Interfaces;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace Luminance.Services
{
    public class UdpService : IUdpService
    {
        public IPAddress GetLuminanceControllerIp(IPAddress broadcastAddress)
        {
            var result = new IPEndPoint(IPAddress.Any, 0);
            try
            {
                var requestData = Encoding.ASCII.GetBytes(Configuration.UdpSecretKey);
                var requestAddress = new IPEndPoint(broadcastAddress, Configuration.UdpPort);
                using var udpClient = new UdpClient();
                udpClient.Client.ReceiveTimeout = Configuration.UdpTimeOut;
                udpClient.EnableBroadcast = true;
                udpClient.Send(requestData, requestData.Length, requestAddress);

                var response = udpClient.Receive(ref result);
                udpClient.Close();

                return response?.Length == 4 ? new IPAddress(response) : result.Address;
            }
            catch (Exception ex)
            {
                ex.Report();

                return result.Address;
            }
        }
    }
}
