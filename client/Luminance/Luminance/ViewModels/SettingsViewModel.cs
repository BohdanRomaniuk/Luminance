using Android.Locations;
using Luminance.ViewModels.Base;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.NetworkInformation;
using System.Text;
using System.Threading.Tasks;
using static Android.Provider.ContactsContract.CommonDataKinds;

namespace Luminance.ViewModels
{
    public sealed class SettingsViewModel : BaseViewModel
    {
        public bool IsTurnedOn { get; set; }
        public int MaxBrightness => 255;
        public int Brightness { get; set; }
        public int BrightnessPercents => Brightness * 100 / MaxBrightness;
        public List<string> Addreses { get; set; }
        public string SelectedAddress { get; set; }
        public string SubnetMask { get; set; }

        public SettingsViewModel()
        {
            IsTurnedOn = true;
            SubnetMask = "255.255.255.0";
            Brightness = 255;
            FindLocalAddress();
        }


        private void FindLocalAddress()
        {
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
