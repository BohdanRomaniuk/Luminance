using Android.Locations;
using Luminance.Helpers;
using Luminance.Managers;
using Luminance.ViewModels.Base;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.NetworkInformation;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using static Android.Provider.ContactsContract.CommonDataKinds;

namespace Luminance.ViewModels
{
    public sealed class SettingsViewModel : BaseViewModel
    {
        private NetworkManager _networkManager;

        public string AppIpAddress { get; set; }
        public string SubnetMask { get; set; }
        public bool IsTurnedOn { get; set; }
        public int MaxBrightness => 255;
        public int Brightness { get; set; }
        public int BrightnessPercents => Brightness * 100 / MaxBrightness;
        public List<string> Addreses { get; set; }
        public string SelectedAddress { get; set; }


        public SettingsViewModel()
        {
            _networkManager = new NetworkManager();

            IsTurnedOn = true;
            SubnetMask = "255.255.255.0";
            Brightness = 255;
            FindLocalAddress();
        }


        private void FindLocalAddress()
        {
            AppIpAddress = _networkManager.GetApplicationIp();
            var ipv4 = IPAddress.Parse(AppIpAddress);
            var mask = IPAddress.Parse(SubnetMask);
            var networkAddress = ipv4.GetBroadcastAddress(mask);

            try
            {
                int PORT = 8888;
                UdpClient udpClient = new UdpClient();
                udpClient.Client.Bind(new IPEndPoint(IPAddress.Any, PORT));

                var from = new IPEndPoint(0, 0);
                var task = Task.Run(() =>
                {
                    while (true)
                    {
                        var recvBuffer = udpClient.Receive(ref from);
                        Console.WriteLine(Encoding.UTF8.GetString(recvBuffer));
                    }
                });

                var data = Encoding.UTF8.GetBytes("LUMINANCE");
                udpClient.Send(data, data.Length, mask.ToString(), PORT);
                var recvBuffer = udpClient.Receive(ref from);
                //task.Wait();
            }
            catch (Exception ex)
            {

            }


            Addreses = new List<string>();
            var ping = new Ping();
            var baseAddress = "192.168.1.";
            for (int i = 0; i < 254; ++i)
            {
                var url = $"{baseAddress}{i}";
                if (IsActiveHost(url))
                {
                    Addreses.Add(url);
                }
            }

            SelectedAddress = Addreses.FirstOrDefault();

            bool IsActiveHost(string url)
                => ping.Send(url, 20).Status == IPStatus.Success;
        }
    }
}
