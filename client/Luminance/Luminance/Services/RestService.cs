using Luminance.Models.Json;
using Luminance.Services.Interfaces;

namespace Luminance.Services
{
    public class RestService : BaseRestService, IRestService
    {
        public Task<ApplicationInfo> GetApplicationInfo()
            => GetAsync<ApplicationInfo>("getAppInfo");

        public Task<int[]> StartMappingAsync()
            => GetAsync<int[]>("startMapping");

        public Task<ApiResponse> GetFrameAsync(int id)
            => GetAsync<ApiResponse>("getFrame", $"id={id}");
    }
}
