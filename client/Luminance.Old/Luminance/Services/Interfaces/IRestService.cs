using Luminance.Models.Json;

namespace Luminance.Services.Interfaces
{
    public interface IRestService : IBaseRestClient
    {
        Task<ApplicationInfo> GetApplicationInfoAsync();
        Task<int[]> StartMappingAsync();
        Task<ApiResponse> GetFrameAsync(int id);
        Task<ApiResponse> SetBrightnessAsync(int brightness);
    }
}
