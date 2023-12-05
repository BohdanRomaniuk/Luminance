using Newtonsoft.Json;

namespace Luminance.Client.Models.Json
{
    public class ApplicationInfo
    {
        [JsonProperty("name")]
        public string? Name { get; set; }

        [JsonProperty("version")]
        public string? Version { get; set; }

        public override string ToString() => $"{Name} {Version}";
    }
}
