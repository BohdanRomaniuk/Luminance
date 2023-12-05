using System.Net;

namespace Luminance.Client.Services.Interfaces
{
    public interface IBaseRestClient
    {
        void SetBaseAddress(IPAddress ip);
    }
}
