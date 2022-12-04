using Luminance.Models.Json;

namespace Luminance.Services.Interfaces
{
    public interface IRestService : IBaseRestClient
    {
        Task<ApplicationInfo> GetApplicationInfo();
    }
}
