using Luminance.Client.Models.Json;
using Luminance.Client.Services.Interfaces;
using Newtonsoft.Json.Linq;

namespace Luminance.Client.Services
{
    public class RestService : BaseRestService, IRestService
    {
        public Task<ApplicationInfo> GetApplicationInfoAsync()
            => GetAsync<ApplicationInfo>("getAppInfo");

        public Task<int[]> StartMappingAsync()
            => GetAsync<int[]>("startMapping");

        public Task<ApiResponse> GetFrameAsync(int id)
            => GetAsync<ApiResponse>("getFrame", $"id={id}");

        public Task<ApiResponse> SetBrightnessAsync(int brightness)
            => PostAsync<ApiResponse>("setBrightness", JObject.FromObject(new { brightness }));
    }
}
