using System.Net;

namespace Luminance.Services.Interfaces
{
    public interface IBaseRestClient
    {
        void SetBaseAddress(IPAddress ip);
    }
}
