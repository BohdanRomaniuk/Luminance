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
        private readonly IRestService _restService;

        private int _brightness;

        private ICommand _searchCommand;

        public string AppIpAddress { get; set; }
        public string SubnetMask { get; set; }
        public bool IsTurnedOn { get; set; }
        public int MaxBrightness => 255;
        public int Brightness
        {
            get => _brightness;
            set
            {
                _brightness = value;
                Task.Run(async () => await ChangeBrightness(value));
            }
        }
        public int BrightnessPercents => Brightness * 100 / MaxBrightness;
        public List<string> Addreses { get; set; }
        public string SelectedAddress { get; set; }
        public string AppInfo { get; set; }

        public ICommand SearchCommand => _searchCommand ??= new Command(async () => await FindLocalAddress());


        public SettingsViewModel()
        {
            _networkManager = new NetworkManager();
            _udpService = new UdpService();
            _restService = new RestService();

            IsTurnedOn = true;
            SubnetMask = "255.255.255.0";
            //Brightness = 255;
            GetApplicationAddress();
            Task.Run(FindLocalAddress);
        }

        private void GetApplicationAddress()
        {
            AppIpAddress = _networkManager.GetApplicationIp();
        }

        private async Task FindLocalAddress()
        {
            try
            {
                AppInfo = "Пошук";
                
                var ipv4 = IPAddress.Parse(AppIpAddress);
                var mask = IPAddress.Parse(SubnetMask);
                var broadcast = ipv4.GetBroadcastAddress(mask);
                var controller = _udpService.GetLuminanceControllerIp(broadcast);
                Addreses = new List<string>() { controller.ToString() };
                SelectedAddress = Addreses.FirstOrDefault();
                if (controller != IPAddress.Any)
                {
                    _restService.SetBaseAddress(controller);
                    var appInfo = await _restService.GetApplicationInfoAsync();
                    AppInfo = appInfo?.ToString();
                }
                else
                {
                    AppInfo = "Не знайдено";
                }
            }
            catch (Exception ex)
            {
                ex.Report();
            }
        }

        private async Task ChangeBrightness(int value)
        {
            var result = await _restService.SetBrightnessAsync(value);
        }
    }
}
