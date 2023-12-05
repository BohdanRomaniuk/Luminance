using Newtonsoft.Json;

namespace Luminance.Client.Models.Json
{
    public class ApiResponse
    {
        [JsonProperty("success")]
        public bool IsSuccess { get; set; }
    }
}
