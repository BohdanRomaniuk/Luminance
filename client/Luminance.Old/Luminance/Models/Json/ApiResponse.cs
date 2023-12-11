using Newtonsoft.Json;

namespace Luminance.Models.Json
{
    public class ApiResponse
    {
        [JsonProperty("success")]
        public bool IsSuccess { get; set; }
    }
}
