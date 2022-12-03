using Luminance.Helpers;
using Luminance.Managers;
using Luminance.Managers.Interfaces;
using Luminance.Services;
using Luminance.Services.Interfaces;
using Luminance.ViewModels.Base;
using System.Net;
using System.Windows.Input;

namespace Luminance.ViewModels
{
    public sealed class SettingsViewModel : BaseViewModel
    {
        private readonly INetworkManager _networkManager;
        private readonly IUdpService _udpService;

        private ICommand _searchCommand;

        public string AppIpAddress { get; set; }
        public string SubnetMask { get; set; }
        public bool IsTurnedOn { get; set; }
        public int MaxBrightness => 255;
        public int Brightness { get; set; }
        public int BrightnessPercents => Brightness * 100 / MaxBrightness;
        public List<string> Addreses { get; set; }
        public string SelectedAddress { get; set; }

        public ICommand SearchCommand => _searchCommand ??= new Command(FindLocalAddress);


        public SettingsViewModel()
        {
            _networkManager = new NetworkManager();
            _udpService = new UdpService();

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
            var broadcast = ipv4.GetBroadcastAddress(mask);
            var controller = _udpService.GetLuminanceControllerIp(broadcast);
            Addreses = new List<string>() { controller.ToString() };
            SelectedAddress = Addreses.FirstOrDefault();


            //var ping = new Ping();
            //var baseAddress = "192.168.1.";
            //for (int i = 0; i < 254; ++i)
            //{
            //    var url = $"{baseAddress}{i}";
            //    if (IsActiveHost(url))
            //    {
            //        Addreses.Add(url);
            //    }
            //}

            //SelectedAddress = Addreses.FirstOrDefault();

            //bool IsActiveHost(string url)
            //    => ping.Send(url, 20).Status == IPStatus.Success;
        }
    }
}
